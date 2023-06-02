#include "IOCX.h"
#include "IMURegisters.h"
#include "navx-mxp_hal.h"
#include "IOCXRegisters.h"
#include "IOCXExRegisters.h"
#include "stm32f4xx_hal.h"
#include "navx-mxp.h"

static IOCX_REGS iocx_regs;
static uint32_t last_loop_timestamp = 0;
static IOCX_EX_REGS iocx_ex_regs;

static uint32_t last_io_watchdog_fed_timestamp = 0;
static IO_WATCHDOG_MODE shadow_io_watchdog_mode = IO_WATCHDOG_DISABLED;
static uint8_t shadow_io_watchdog_command = IO_WATCHDOG_CMD_NONE;
static bool io_watchdog_output_suspended = false;
static bool io_watchdog_commdio_suspended = false;
static bool io_watchdog_hicurrdio_suspended = false;
static bool io_watchdog_flexdio_suspended = false;

_EXTERN_ATTRIB void IOCX_init()
{
	HAL_IOCX_TIMER_Enable_Clocks(0,1);
	HAL_IOCX_TIMER_Enable_Clocks(1,1);
	HAL_IOCX_TIMER_Enable_Clocks(2,1);
	HAL_IOCX_TIMER_Enable_Clocks(3,1);
	HAL_IOCX_TIMER_Enable_Clocks(4,1);
	HAL_IOCX_TIMER_Enable_Clocks(5,1);

	/* Configure Timer Capture Event Interrupt Priorities */
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(0);
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(1);
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(2);
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(3);
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(4);
	HAL_IOCX_TIMER_ConfigureInterruptPriorities(5);

	HAL_IOCX_RPI_GPIO_Driver_Enable(1);
	HAL_IOCX_RPI_COMM_Driver_Enable(1);

	/* Update static capability flags */
	iocx_regs.capability_flags.interrupt_support = 1;
	iocx_regs.capability_flags.unused = 0;
	iocx_ex_regs.capability_flags.countercfg_support = 1;
	iocx_ex_regs.capability_flags.slvmd_cfg_support = 1;
	iocx_ex_regs.capability_flags.stall_support = 1;
	iocx_ex_regs.capability_flags.inputcap_support = 1;
	iocx_ex_regs.capability_flags.tmrcntreset_support = 1;
	iocx_ex_regs.capability_flags.io_watchdog_support = 1;
	iocx_ex_regs.capability_flags.virt_cntr_support = 1;
	iocx_ex_regs.capability_flags.unused = 0;

	/* Configure IO Watchdog defaults */
	iocx_ex_io_watchdog_encode_mode(&iocx_ex_regs.io_watchdog_mode, shadow_io_watchdog_mode);
	iocx_ex_io_watchdog_encode_state(&iocx_ex_regs.io_watchdog_status, IO_WATCHDOG_STATE_NORMAL);
	iocx_ex_regs.io_watchdog_timeout_period_ms = IO_WATCHDOG_TIMEOUT_DEFAULT_PERIOD_MS;
	iocx_ex_regs.io_watchdog_output_cfg.manage_commdio = 1;
	iocx_ex_regs.io_watchdog_output_cfg.manage_hicurrdio = 1;
	iocx_ex_regs.io_watchdog_output_cfg.manage_flexdio = 1;
}

static void IOCX_OutputSuspend(bool suspend)
{
	if (suspend) {
		if (!io_watchdog_output_suspended) {
			if (iocx_ex_regs.io_watchdog_output_cfg.manage_flexdio) {
				HAL_IOCX_FlexDIO_Suspend(1);
				io_watchdog_flexdio_suspended = true;
			}
			if (HAL_IOCX_RPI_GPIO_Output() && iocx_ex_regs.io_watchdog_output_cfg.manage_hicurrdio) {
				HAL_IOCX_RPI_GPIO_Driver_Enable(0);
				io_watchdog_hicurrdio_suspended = true;
			}
			if (iocx_ex_regs.io_watchdog_output_cfg.manage_commdio) {
				HAL_IOCX_RPI_COMM_Driver_Enable(0);
				io_watchdog_commdio_suspended = true;
			}
			io_watchdog_output_suspended = true;
		}
	} else {
		if (io_watchdog_output_suspended) {
			if (io_watchdog_flexdio_suspended) {
				HAL_IOCX_FlexDIO_Suspend(0);
				io_watchdog_flexdio_suspended = false;
			}
			if (io_watchdog_hicurrdio_suspended) {
				HAL_IOCX_RPI_GPIO_Driver_Enable(1);
				io_watchdog_hicurrdio_suspended = false;
			}
			if (io_watchdog_commdio_suspended) {
				HAL_IOCX_RPI_COMM_Driver_Enable(1);
				io_watchdog_commdio_suspended = false;
			}
			io_watchdog_output_suspended = false;
		}
	}
}

#define NUM_MS_BETWEEN_SUCCESSIVE_LOOPS 20

_EXTERN_ATTRIB void IOCX_loop()
{
	uint32_t curr_loop_timestamp = HAL_GetTick();
	if ( curr_loop_timestamp < last_loop_timestamp) {
		/* Timestamp rollover */
		last_loop_timestamp = curr_loop_timestamp;
	} else {
		if ((curr_loop_timestamp - last_loop_timestamp) >= NUM_MS_BETWEEN_SUCCESSIVE_LOOPS){
			last_loop_timestamp = curr_loop_timestamp;

			/* Update dynamic capability flags */
			iocx_regs.capability_flags.rpi_gpio_out = HAL_IOCX_RPI_GPIO_Output();
		}
	}
	IO_WATCHDOG_MODE wd_mode = iocx_ex_io_watchdog_decode_mode(&iocx_ex_regs.io_watchdog_mode);
	IO_WATCHDOG_STATE wd_state = iocx_ex_io_watchdog_decode_state(&iocx_ex_regs.io_watchdog_status);
	if (wd_mode == IO_WATCHDOG_ENABLED) {
		if (wd_state == IO_WATCHDOG_STATE_NORMAL) {
			if ((shadow_io_watchdog_command == IO_WATCHDOG_CMD_FEED) ||
				(shadow_io_watchdog_mode != IO_WATCHDOG_ENABLED)){
				last_io_watchdog_fed_timestamp = curr_loop_timestamp;
				shadow_io_watchdog_command = IO_WATCHDOG_CMD_NONE;
			} else if (shadow_io_watchdog_command == IO_WATCHDOG_CMD_EXPIRENOW) {
				last_io_watchdog_fed_timestamp = curr_loop_timestamp + iocx_ex_regs.io_watchdog_timeout_period_ms;
				shadow_io_watchdog_command = IO_WATCHDOG_CMD_NONE;
			}
			uint32_t ms_since_last_watchdog_feeding = curr_loop_timestamp - last_io_watchdog_fed_timestamp;
			if (ms_since_last_watchdog_feeding >= iocx_ex_regs.io_watchdog_timeout_period_ms) {
				IOCX_OutputSuspend(true);
				iocx_ex_io_watchdog_encode_state(&iocx_ex_regs.io_watchdog_status, IO_WATCHDOG_STATE_EXPIRED);
			}
		} else /* IO_WATCHDOG_STATE_EXPIRED, OR watchdog re-enabled after previously being expired */ {
			if ((shadow_io_watchdog_command == IO_WATCHDOG_CMD_FEED) ||
				 (shadow_io_watchdog_mode != wd_mode)){
				last_io_watchdog_fed_timestamp = curr_loop_timestamp;
				shadow_io_watchdog_command = IO_WATCHDOG_CMD_NONE;
				IOCX_OutputSuspend(false);
				iocx_ex_io_watchdog_encode_state(&iocx_ex_regs.io_watchdog_status, IO_WATCHDOG_STATE_NORMAL);
			}
		}
	} else {
		if (wd_state == IO_WATCHDOG_STATE_EXPIRED) {
			IOCX_OutputSuspend(false);
			iocx_ex_io_watchdog_encode_state(&iocx_ex_regs.io_watchdog_status, IO_WATCHDOG_STATE_NORMAL);
		}
	}
	shadow_io_watchdog_mode = wd_mode;
}

_EXTERN_ATTRIB uint8_t *IOCX_get_reg_addr_and_max_size( uint8_t bank, uint8_t register_offset, uint8_t requested_count, uint16_t* size )
{
	if ( bank == IOCX_REGISTER_BANK) {
	    if ( register_offset >= offsetof(struct IOCX_REGS, end_of_bank) ) {
	        size = 0;
	        return 0;
	    }

	    /* Requested data includes interrupt status; update w/latest value */
	    uint8_t first_offset = register_offset;

	    if((first_offset >= offsetof(struct IOCX_REGS, gpio_intstat)) &&
	       (first_offset <=
	    		   offsetof(struct IOCX_REGS, gpio_intstat) +
	    		   sizeof(iocx_regs.gpio_intstat))) {
	    	iocx_regs.gpio_intstat = HAL_IOCX_GetInterruptStatus();
	    	iocx_regs.gpio_last_int_edge = HAL_IOCX_GetLastInterruptEdges();
	    }

	    if((first_offset >= offsetof(struct IOCX_REGS, timer_counter)) &&
	       (first_offset <=
	    		   offsetof(struct IOCX_REGS, timer_counter) +
	    		   sizeof(iocx_regs.timer_counter))) {
	    	/* Todo:  This can be optimized to only acquire data for requested
	    	 * counters, rather than all counters.
	    	 */
	    	HAL_IOCX_TIMER_Get_Count(0, IOCX_NUM_TIMERS, &iocx_regs.timer_counter[0]);
	   }

	    if((first_offset >= offsetof(struct IOCX_REGS, timer_status)) &&
	       (first_offset <=
	    		   offsetof(struct IOCX_REGS, timer_status) +
	    		   sizeof(iocx_regs.timer_status))) {
	    	/* Todo:  This can be optimized to only acquire data for requested
	    	 * status, rather than all statuses.
	    	 */
	    	HAL_IOCX_TIMER_Get_Status(0, IOCX_NUM_TIMERS, &iocx_regs.timer_status[0]);
	   }

	    if((first_offset >= offsetof(struct IOCX_REGS, timer_chx_ccr)) &&
	       (first_offset <=
	    		   offsetof(struct IOCX_REGS, timer_chx_ccr) +
	    		   sizeof(iocx_regs.timer_chx_ccr))) {
	    	/* Todo:  This can be optimized to only acquire data for requested
	    	 * timer channel ccr values, rather than all.
	    	 */
	    	HAL_IOCX_TIMER_PWM_Get_DutyCycle(0, 0, (IOCX_NUM_TIMERS * IOCX_NUM_CHANNELS_PER_TIMER), &iocx_regs.timer_chx_ccr[0]);
	   }

	    if((first_offset >= offsetof(struct IOCX_REGS, gpio_data)) &&
	       (first_offset <=
	    		   offsetof(struct IOCX_REGS, gpio_data) +
	    		   sizeof(iocx_regs.gpio_data))) {
	    	/* Todo:  This can be optimized to only acquire data for requested
	    	 * gpios, rather than all gpios.
	    	 */
		    HAL_IOCX_GPIO_Get(0, IOCX_NUM_GPIOS, &iocx_regs.gpio_data[0]);
	   }

	    uint8_t *register_base = (uint8_t *)&iocx_regs;
	    *size = offsetof(struct IOCX_REGS, end_of_bank) - register_offset;
	    return register_base + register_offset;
	} else if ( bank == IOCX_EX_REGISTER_BANK) {
	    if ( register_offset >= offsetof(struct IOCX_EX_REGS, end_of_bank) ) {
	        size = 0;
	        return 0;
	    }
	    uint8_t *register_base = (uint8_t *)&iocx_ex_regs;
	    *size = offsetof(struct IOCX_EX_REGS, end_of_bank) - register_offset;
	    return register_base + register_offset;
	}
	return 0;
}

template<typename T, void (*HAL_FUNC)(uint8_t, T)>
void reg_set_modified(uint8_t first_offset, uint8_t count, T* data) {
	uint8_t first_index_modified = first_offset / sizeof(T);
	uint8_t last_index_modified = ((first_offset + count)-1) / sizeof(T);
	for ( uint8_t index = first_index_modified; index <= last_index_modified; index++ ) {
		HAL_FUNC(index, data[index]);
	}
}

template<typename T, void (*HAL_FUNC)(uint8_t, T *)>
void reg_set_modified_ptr(uint8_t first_offset, uint8_t count, T* data) {
	uint8_t first_index_modified = first_offset / sizeof(T);
	uint8_t last_index_modified = ((first_offset + count)-1) / sizeof(T);
	for ( uint8_t index = first_index_modified; index <= last_index_modified; index++ ) {
		HAL_FUNC(index, &data[index]);
	}
}

template<typename T, void (*HAL_FUNC)(uint8_t, uint8_t, T), int NUM_CHANNELS>
void channel_reg_set_modified(uint8_t first_offset, uint8_t count, T* data) {
	uint8_t first_index_modified = first_offset / (sizeof(T) * NUM_CHANNELS);
	uint8_t first_channel_modified = (first_offset % (sizeof(T) * NUM_CHANNELS)) / sizeof(T);
	uint8_t last_index_modified = ((first_offset + count)-1) / (sizeof(T) * NUM_CHANNELS);
	uint8_t last_channel_modified = ((first_offset + count)-1) % (sizeof(T) * NUM_CHANNELS) / sizeof(T);
	uint8_t index = first_index_modified;
	uint8_t channel = first_channel_modified;
	while ( (index <= last_index_modified) && (channel <= last_channel_modified) ) {
		HAL_FUNC(index, channel, data[(index * NUM_CHANNELS) + channel]);
		channel++;
		if ( channel == NUM_CHANNELS) {
			index++;
			channel = 0;
		}
	}
}

static void int_cfg_modified(uint8_t first_offset, uint8_t count) {
	HAL_IOCX_UpdateInterruptMask(iocx_regs.int_cfg);
}

static void gpio_intstat_modified(uint8_t first_offset, uint8_t count) {
	HAL_IOCX_DeassertInterrupt(iocx_regs.gpio_intstat);
}

static void gpio_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_GPIO_Set_Config>(first_offset, count, iocx_regs.gpio_cfg);
}

static void gpio_data_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_GPIO_Set>(first_offset, count, iocx_regs.gpio_data);
}

static void timer_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_Set_Config>(first_offset, count, iocx_regs.timer_cfg);
}

static void timer_ctl_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified_ptr<uint8_t, HAL_IOCX_TIMER_Set_Control>(first_offset, count, iocx_regs.timer_ctl);
}

static void timer_prescaler_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint16_t, HAL_IOCX_TIMER_Set_Prescaler>(first_offset, count, iocx_regs.timer_prescaler);
#if 0
	uint8_t first_timer_modified = first_offset / sizeof(iocx_regs.timer_prescaler[0]);
	uint8_t last_timer_modified = ((first_offset + count)-1) / sizeof(iocx_regs.timer_prescaler[0]);
	for ( uint8_t timer_index = first_timer_modified; timer_index <= last_timer_modified; timer_index++ ) {
		HAL_IOCX_TIMER_Set_Prescaler(timer_index, iocx_regs.timer_prescaler[timer_index]);
	}
#endif
}

static void timer_aar_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint16_t, HAL_IOCX_TIMER_PWM_Set_FramePeriod>(first_offset, count, iocx_regs.timer_aar);
}

static void timer_chx_ccr_modified(uint8_t first_offset, uint8_t count) {
	channel_reg_set_modified<uint16_t, HAL_IOCX_TIMER_PWM_Set_DutyCycle, 2>(first_offset, count, iocx_regs.timer_chx_ccr);
}

WritableRegSet gpio_reg_sets[] =
{
	/* Contiguous registers, increasing order of offset  */
	{ offsetof(struct IOCX_REGS, int_cfg), sizeof(IOCX_REGS::int_cfg), int_cfg_modified },
	{ offsetof(struct IOCX_REGS, gpio_cfg), sizeof(IOCX_REGS::gpio_cfg), gpio_cfg_modified },
	{ offsetof(struct IOCX_REGS, gpio_intstat), sizeof(IOCX_REGS::gpio_intstat), gpio_intstat_modified },
	{ offsetof(struct IOCX_REGS, gpio_data), sizeof(IOCX_REGS::gpio_data), gpio_data_modified },
};

WritableRegSet timer_reg_sets[] =
{
	/* Contiguous registers, increasing order of offset  */
	{ offsetof(struct IOCX_REGS, timer_cfg), sizeof(IOCX_REGS::timer_cfg), timer_cfg_modified },
	{ offsetof(struct IOCX_REGS, timer_ctl), sizeof(IOCX_REGS::timer_ctl), timer_ctl_modified },
	{ offsetof(struct IOCX_REGS, timer_prescaler), sizeof(IOCX_REGS::timer_prescaler), timer_prescaler_modified },
	{ offsetof(struct IOCX_REGS, timer_aar), sizeof(IOCX_REGS::timer_aar), timer_aar_modified },
	{ offsetof(struct IOCX_REGS, timer_chx_ccr), sizeof(IOCX_REGS::timer_chx_ccr), timer_chx_ccr_modified },
};

WritableRegSetGroup iocx_writable_reg_set_groups[] =
{
	{ timer_reg_sets[0].start_offset,
		timer_reg_sets[SIZEOF_STRUCT(timer_reg_sets)-1].start_offset + timer_reg_sets[SIZEOF_STRUCT(timer_reg_sets)-1].num_bytes,
		timer_reg_sets,
		SIZEOF_STRUCT(timer_reg_sets) },
	{ gpio_reg_sets[0].start_offset,
		gpio_reg_sets[SIZEOF_STRUCT(gpio_reg_sets)-1].start_offset + gpio_reg_sets[SIZEOF_STRUCT(gpio_reg_sets)-1].num_bytes,
		gpio_reg_sets,
		SIZEOF_STRUCT(gpio_reg_sets) },
};

static void timer_counter_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_Set_Counter_Cfg>(first_offset, count, iocx_ex_regs.timer_counter_cfg);
}

static void timer_slavemode_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_Set_SlaveMode_Cfg>(first_offset, count, iocx_ex_regs.timer_slavemode_cfg);
}

static void timer_ic_ch_cfg_modified(uint8_t first_offset, uint8_t count) {
	channel_reg_set_modified<uint8_t, HAL_IOCX_TIMER_INPUTCAP_Set_Cfg, 2>(first_offset, count, iocx_ex_regs.timer_ic_ch_cfg);
}

static void timer_ic_ch_cfg2_modified(uint8_t first_offset, uint8_t count) {
	channel_reg_set_modified<uint8_t, HAL_IOCX_TIMER_INPUTCAP_Set_Cfg2, 2>(first_offset, count, iocx_ex_regs.timer_ic_ch_cfg2);
}

static void timer_ic_stall_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_INPUTCAP_Set_StallCfg>(first_offset, count, iocx_ex_regs.timer_ic_stall_cfg);
}

static void timer_counter_reset_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_INPUTCAP_Set_TimerCounterResetCfg>(first_offset, count, iocx_ex_regs.timer_counter_reset_cfg);
}

static void io_watchdog_mode_modified(uint8_t first_offset, uint8_t count) {
	/* No-op (value is read directly from register shadow). */
}

static void io_watchdog_output_cfg_modified(uint8_t first_offset, uint8_t count) {
	/* No-op (value is read directly from register shadow). */
}

static void io_watchdog_timeout_period_ms_modified(uint8_t first_offset, uint8_t count) {
	/* No-op (value is read directly from register shadow). */
}

static void io_watchdog_command_modified(uint8_t first_offset, uint8_t count) {
	shadow_io_watchdog_command = iocx_ex_regs.io_watchdog_command;
	iocx_ex_regs.io_watchdog_command = IO_WATCHDOG_CMD_NONE;
}

static void virtual_counter_cfg_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint8_t, HAL_IOCX_TIMER_Set_VirtualCounterConfig>(first_offset, count, iocx_ex_regs.virtual_counter_cfg);
}

static void virtual_counter_parameter1_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint16_t, HAL_IOCX_TIMER_Set_VirtualCounterParameter1>(first_offset, count, iocx_ex_regs.virtual_counter_parameter1);
}

static void virtual_counter_parameter2_modified(uint8_t first_offset, uint8_t count) {
	reg_set_modified<uint16_t, HAL_IOCX_TIMER_Set_VirtualCounterParameter2>(first_offset, count, iocx_ex_regs.virtual_counter_parameter2);
}

WritableRegSet timer_inputcap_reg_sets[] =
{
	/* Contiguous registers, increasing order of offset  */
	{ offsetof(struct IOCX_EX_REGS, timer_counter_cfg), sizeof(IOCX_EX_REGS::timer_counter_cfg), timer_counter_cfg_modified },
	{ offsetof(struct IOCX_EX_REGS, timer_slavemode_cfg), sizeof(IOCX_EX_REGS::timer_slavemode_cfg), timer_slavemode_cfg_modified },
	{ offsetof(struct IOCX_EX_REGS, timer_ic_stall_cfg), sizeof(IOCX_EX_REGS::timer_ic_stall_cfg), timer_ic_stall_cfg_modified },
	{ offsetof(struct IOCX_EX_REGS, timer_ic_ch_cfg), sizeof(IOCX_EX_REGS::timer_ic_ch_cfg), timer_ic_ch_cfg_modified },
	{ offsetof(struct IOCX_EX_REGS, timer_ic_ch_cfg2), sizeof(IOCX_EX_REGS::timer_ic_ch_cfg2), timer_ic_ch_cfg2_modified },
	{ offsetof(struct IOCX_EX_REGS, timer_counter_reset_cfg), sizeof(IOCX_EX_REGS::timer_counter_reset_cfg),timer_counter_reset_cfg_modified },
};

WritableRegSet io_watchdog_reg_sets[] =
{
	/* Contiguous registers, increasing order of offset  */
	{ offsetof(struct IOCX_EX_REGS, io_watchdog_mode), sizeof(IOCX_EX_REGS::io_watchdog_mode), io_watchdog_mode_modified },
	{ offsetof(struct IOCX_EX_REGS, io_watchdog_output_cfg), sizeof(IOCX_EX_REGS::io_watchdog_output_cfg), io_watchdog_output_cfg_modified },
	{ offsetof(struct IOCX_EX_REGS, io_watchdog_timeout_period_ms), sizeof(IOCX_EX_REGS::io_watchdog_timeout_period_ms), io_watchdog_timeout_period_ms_modified },
	{ offsetof(struct IOCX_EX_REGS, io_watchdog_command), sizeof(IOCX_EX_REGS::io_watchdog_command), io_watchdog_command_modified },
};

WritableRegSet virtual_counter_reg_sets[] =
{
		/* Contiguous registers, increasing order of offset */
		{ offsetof(struct IOCX_EX_REGS, virtual_counter_cfg), sizeof(IOCX_EX_REGS::virtual_counter_cfg), virtual_counter_cfg_modified },
		{ offsetof(struct IOCX_EX_REGS, virtual_counter_parameter1), sizeof(IOCX_EX_REGS::virtual_counter_parameter1), virtual_counter_parameter1_modified },
		{ offsetof(struct IOCX_EX_REGS, virtual_counter_parameter2), sizeof(IOCX_EX_REGS::virtual_counter_parameter2), virtual_counter_parameter2_modified },
};

WritableRegSetGroup iocx_ex_writable_reg_set_groups[] =
{
	{ timer_inputcap_reg_sets[0].start_offset,
			timer_inputcap_reg_sets[SIZEOF_STRUCT(timer_inputcap_reg_sets)-1].start_offset + timer_inputcap_reg_sets[SIZEOF_STRUCT(timer_inputcap_reg_sets)-1].num_bytes,
			timer_inputcap_reg_sets,
		SIZEOF_STRUCT(timer_inputcap_reg_sets) },
	{ io_watchdog_reg_sets[0].start_offset,
			io_watchdog_reg_sets[SIZEOF_STRUCT(io_watchdog_reg_sets)-1].start_offset + io_watchdog_reg_sets[SIZEOF_STRUCT(io_watchdog_reg_sets)-1].num_bytes,
			io_watchdog_reg_sets,
		SIZEOF_STRUCT(io_watchdog_reg_sets) },
	{ virtual_counter_reg_sets[0].start_offset,
			virtual_counter_reg_sets[SIZEOF_STRUCT(virtual_counter_reg_sets)-1].start_offset + virtual_counter_reg_sets[SIZEOF_STRUCT(virtual_counter_reg_sets)-1].num_bytes,
			virtual_counter_reg_sets,
		SIZEOF_STRUCT(virtual_counter_reg_sets) },
};

BankedWritableRegSetGroups iocx_banked_groups[] =
{
	{ IOCX_REGISTER_BANK, iocx_writable_reg_set_groups, SIZEOF_STRUCT(iocx_writable_reg_set_groups) },
	{ IOCX_EX_REGISTER_BANK, iocx_ex_writable_reg_set_groups, SIZEOF_STRUCT(iocx_ex_writable_reg_set_groups) },
};

_EXTERN_ATTRIB void IOCX_banked_writable_reg_update_func(uint8_t bank, uint8_t reg_offset, uint8_t *p_reg, uint8_t count, uint8_t *p_new_values )
{
	for (size_t x = 0; x < SIZEOF_STRUCT(iocx_banked_groups); x++) {
		if (bank == iocx_banked_groups[x].bank) {
			WritableRegSetGroup *p_group = iocx_banked_groups[x].p_group;
			for ( size_t i = 0; i < iocx_banked_groups[x].num_sets_in_group; i++) {
				if ( (reg_offset >= p_group[i].first_offset) &&
					 (reg_offset <= p_group[i].last_offset)) {
					for ( int j = 0; j < p_group[i].set_count; j++) {
						WritableRegSet *p_set = &p_group[i].p_sets[j];
						if ((reg_offset >= p_set->start_offset) &&
							(reg_offset < (p_set->start_offset + p_set->num_bytes))){
							int first_offset = (reg_offset - p_set->start_offset);
							int max_bytes_to_write_in_set = p_set->num_bytes-first_offset;
							int num_bytes_in_set_to_modify = (count < max_bytes_to_write_in_set) ? count : max_bytes_to_write_in_set;
							int num_bytes_in_set_changed = 0;
							while (num_bytes_in_set_changed < num_bytes_in_set_to_modify )  {
								*p_reg++ = *p_new_values++;
								reg_offset++;
								num_bytes_in_set_changed++;
								count--;
							}
							if (num_bytes_in_set_changed > 0){
								/* At least one byte in this set was modified. */
								p_set->changed(first_offset, num_bytes_in_set_changed);
							}
							if (count == 0) {
								break;
							}
						}
					}
				}
			}
		}
	}
}

