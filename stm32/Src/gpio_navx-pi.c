/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio_navx-pi.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

static void MX_CAN_Interrupt_Enable(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = _CAN_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_CAN_INT_GPIO_Port, &GPIO_InitStruct);
}

int MX_CAN_Interrupt_Verify(int fix)
{
	int gpio_group_enabled = 0;
    int gpio_pin_masked_in = 0;
    int gpio_falling_edge_trigger = 0;
    int gpio_floating_input = 1;
    uint32_t temp1, temp2, temp3, temp4;
    temp1 = SYSCFG->EXTICR[1];
    if (( temp1 & 0x0000000F ) == 0x00000001) {
        /* EXTI is enabled for GPIOB Group */
        gpio_group_enabled = 1;
    }
    temp2 = EXTI->IMR;
    if ( temp2 & _CAN_INT_Pin ) {
        /* EXTI is masked in */
        gpio_pin_masked_in = 1;
    }
    temp3 = EXTI->FTSR;
    if ( temp3 & _CAN_INT_Pin ) {
        /* Falling edge triggered */
        gpio_falling_edge_trigger = 1;
    }
    temp4 = GPIOC->PUPDR;
    temp4 >>= (POSITION_VAL(_CAN_INT_Pin) * 2); // 2 bits per pin
    temp4 &= 0x00000003;
    if ( temp4 != GPIO_NOPULL) {
        /* Floating input */
    	gpio_floating_input = 0;
    }
    if ((!gpio_group_enabled) || (!gpio_pin_masked_in) ||
    	(!gpio_falling_edge_trigger) || (!gpio_floating_input)) {
    	if (fix) {
    	  MX_CAN_Interrupt_Enable();
    	}
    	return 1;
    }
    return 0;
}

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
int MX_GPIO_Init_NavX_PI(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = NAVX_2_RPI_SPI_Comm_Ready_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NAVX_2_RPI_SPI_Comm_Ready_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = NAVX_2_RPI_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NAVX_2_RPI_INT2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = NAVX_2_RPI_INT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NAVX_2_RPI_INT3_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = NAVX_2_RPI_INT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NAVX_2_RPI_INT4_GPIO_Port, &GPIO_InitStruct);

  MX_CAN_Interrupt_Enable();

  /*Configure GPIO pins : PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = S2_LED_Pin|S1_LED_Pin|_CAN_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = _I2C_DEV_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(_I2C_DEV_ON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = CAL_LED_Pin|CAN_STANDBY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Configure Ext Power Switch pin */
  GPIO_InitStruct.Pin = EXT_PWR_SWITCH_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(EXT_PWR_SWITCH_ON_GPIO_Port, &GPIO_InitStruct);

  /* Enable GPIO Driver Enable Pins as Outputs */

  GPIO_InitStruct.Pin = _RPI_GPIO_OE1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(_RPI_GPIO_OE1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = _RPI_GPIO_OE2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(_RPI_GPIO_OE2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = COMM_OE2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(COMM_OE2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = EXT_PWR_SWITCH_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(EXT_PWR_SWITCH_ON_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = CAN_OK_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(CAN_OK_LED_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RPI_GPIO_DIR_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RPI_GPIO_DIR_IN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = _IO_POWER_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(_IO_POWER_FAULT_GPIO_Port, &GPIO_InitStruct);

  /* Note this pin is connected to QE3A.  This pin must only ever be
   * an input, and no pull-up should be used, to ensure that the pull-up
   * does not interfere w/usage of the QE3A pin.  The only purpose of
   * this input is to allow a unique EXTI Line input when mapped
   * as an input.
   */
  GPIO_InitStruct.Pin = QE3_A_Second_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(QE3_A_Second_GPIO_Port, &GPIO_InitStruct);

  /*Configure default GPIO pin Output Levels */
  HAL_GPIO_WritePin(GPIOC, S2_LED_Pin|S1_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, _CAN_RESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(_I2C_DEV_ON_GPIO_Port, _I2C_DEV_ON_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, CAL_LED_Pin|CAN_STANDBY_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(NAVX_2_RPI_SPI_Comm_Ready_GPIO_Port, NAVX_2_RPI_SPI_Comm_Ready_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EXT_PWR_SWITCH_ON_GPIO_Port, EXT_PWR_SWITCH_ON_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(_RPI_GPIO_OE1_GPIO_Port, _RPI_GPIO_OE1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(_RPI_GPIO_OE2_GPIO_Port, _RPI_GPIO_OE2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(COMM_OE2_GPIO_Port, COMM_OE2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(NAVX_2_RPI_INT2_GPIO_Port, NAVX_2_RPI_INT2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(NAVX_2_RPI_INT3_GPIO_Port, NAVX_2_RPI_INT3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(NAVX_2_RPI_INT4_GPIO_Port, NAVX_2_RPI_INT4_Pin, GPIO_PIN_SET);

  /* Determine VMX-pi board revision by reading board rev pins after pulling them high */

  GPIO_InitStruct.Pin = VMXPI_BOARDREV_BIT2_Pin |
		  	  	  	    VMXPI_BOARDREV_BIT1_Pin |
		  	  	  	    VMXPI_BOARDREV_BIT0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(VMXPI_BOARDREV_Port, &GPIO_InitStruct);

  HAL_Delay(2);  // Give inputs a moment to stabilize before reading
  int bit0 = (HAL_GPIO_ReadPin(VMXPI_BOARDREV_Port,VMXPI_BOARDREV_BIT0_Pin) == GPIO_PIN_SET);
  int bit1 = (HAL_GPIO_ReadPin(VMXPI_BOARDREV_Port,VMXPI_BOARDREV_BIT1_Pin) == GPIO_PIN_SET);
  int bit2 = (HAL_GPIO_ReadPin(VMXPI_BOARDREV_Port,VMXPI_BOARDREV_BIT2_Pin) == GPIO_PIN_SET);

  int boardrev = (bit2 ? 1 << 2 : 0) +
		  	  	 (bit1 ? 1 << 1 : 0) +
		  	  	 (bit0 ? 1 << 0 : 0);

  // All board revisions later than 5.35 have UART/SPI output signal enable pin (active low)
  // And control of the new enable feature will not cause any problems w5.35.  And board rev
  // 5.38 cannot be reliability detected currently.
  // Therefore, the 5.38 output enable code runs on all boards (5.35 and 5.38)
  GPIO_InitStruct.Pin = _COMM_OE1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(_COMM_OE1_GPIO_Port, &GPIO_InitStruct);

  // Disable outputs by default
  HAL_GPIO_WritePin(_COMM_OE1_GPIO_Port, COMM_OE2_Pin, GPIO_PIN_SET);

  // Disable board revision inputs now that revision check is complete
  HAL_GPIO_DeInit(VMXPI_BOARDREV_Port, VMXPI_BOARDREV_BIT0_Pin);
  HAL_GPIO_DeInit(VMXPI_BOARDREV_Port, VMXPI_BOARDREV_BIT1_Pin);
  HAL_GPIO_DeInit(VMXPI_BOARDREV_Port, VMXPI_BOARDREV_BIT2_Pin);

  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI0_IRQn);
  HAL_NVIC_SetPriority((IRQn_Type)EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI1_IRQn);
  HAL_NVIC_SetPriority((IRQn_Type)EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI2_IRQn);
  HAL_NVIC_SetPriority((IRQn_Type)EXTI2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI3_IRQn);
  HAL_NVIC_SetPriority((IRQn_Type)EXTI3_IRQn, 5, 0);
  /* CAN Interrupt Line */
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI4_IRQn);
  HAL_NVIC_SetPriority((IRQn_Type)EXTI4_IRQn, 2, 0);

  return boardrev;
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
