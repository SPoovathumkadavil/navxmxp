/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/* Scott Libert, 8/24/2014 - modified version of libmaple file. */

/**
 *  @file ext_interrupts.h
 *
 *  @brief Wiring-like external interrupt prototypes and types.
 */

#ifndef _EXT_INTERRUPTS_H_
#define _EXT_INTERRUPTS_H_

#include <stdint.h>

#ifdef __cplusplus
#define _EXTERN_ATTRIB extern "C"
#else
#define _EXTERN_ATTRIB
#endif

/**
 * The kind of transition on an external pin which should trigger an
 * interrupt.
 */
typedef enum ExtIntTriggerMode {
    RISING, /**< To trigger an interrupt when the pin transitions LOW
               to HIGH */
    FALLING, /**< To trigger an interrupt when the pin transitions
                HIGH to LOW */
    CHANGE /**< To trigger an interrupt when the pin transitions from
              LOW to HIGH or HIGH to LOW (i.e., when the pin
              changes). */
} ExtIntTriggerMode;

/**
 *  @brief Registers an interrupt handler on a pin.
 *
 *  The interrupt will be triggered on a given transition on the pin,
 *  as specified by the mode parameter.  The handler runs in interrupt
 *  context.  The new handler will replace whatever handler is
 *  currently registered for the pin, if any.
 *
 *  @param pin STM32 EXTI GPIO Pin (GPIO_PIN_0 through GPIO_PIN_15)
 *  @param handler Function to run upon external interrupt trigger.
 *  The handler should take no arguments, and have void return type.
 *  @param mode Type of transition to trigger on, e.g. falling, rising, etc.
 *
 *  @sideeffect Registers a handler
 *  @see detachInterrupt()
 */
<<<<<<< HEAD
void attachInterrupt(uint16_t pin, void (*func)(unsigned char), ExtIntTriggerMode mode);
=======
_EXTERN_ATTRIB void attachInterrupt(uint16_t pin, void (*func)(uint8_t), ExtIntTriggerMode mode);
>>>>>>> navX-PI-Dev

/**
 * @brief Disable any registered external interrupt.
 * @param pin STM32 EXTI GPIO Pin (GPIO_PIN_0 through GPIO_PIN_15)
 * @sideeffect unregisters external interrupt handler
 * @see attachInterrupt()
 */
_EXTERN_ATTRIB void detachInterrupt(uint16_t pin);

/**
 * Re-enable interrupts.
 *
 * Call this after noInterrupts() to re-enable interrupt handling,
 * after you have finished with a timing-critical section of code.
 *
 * @see noInterrupts()
 */
static inline void interrupts() {
	asm volatile("cpsie i");
}

/**
 * Disable interrupts.
 *
 * After calling this function, all user-programmable interrupts will
 * be disabled.  You can call this function before a timing-critical
 * section of code, then call interrupts() to re-enable interrupt
 * handling.
 *
 * @see interrupts()
 */
static inline void noInterrupts() {
	asm volatile("cpsid i");
}

#endif

