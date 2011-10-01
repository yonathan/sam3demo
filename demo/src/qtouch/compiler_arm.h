/*! \file *********************************************************************
 *
 * \brief Compiler file for AVR32.
 *
 * This file defines commonly used types and macros.
 *
 * - Compiler:           ARM
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _COMPILER_H_
#define _COMPILER_H_

//#include "preprocessor.h"

//_____ D E C L A R A T I O N S ____________________________________________

//_____ M A C R O S ________________________________________________________

/*============================ INCLUDES ======================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//#include "touch_io.h"
/*============================ MACROS ========================================*/

#define nop()   __asm__ __volatile__ ( "nop" )


#if defined (AT91SAM7) || defined (AT91SAM9)
// AT91SAM7, SAM9
static inline uint32_t __get_CPSR(void)
{
  uint32_t temp;
  asm volatile ("mrs %0,CPSR":"=r" (temp):) ;
  return temp;
}

static inline void __set_CPSR(uint32_t save_cpsr)
{
  asm volatile (" msr CPSR_cxsf,%0"::"r"(save_cpsr) );
}

/* enable interrupts */
//static inline void enable_interrupt(void){
#define sei() {\
  uint32_t temp;\
  temp = __get_CPSR();\
  __set_CPSR(temp & ~0xC0);\
}

/* Disable interrupts and save CPSR */
//static inline uint32_t disable_interrupt(void){
#define cli() {\
  uint32_t temp;\
  temp = __get_CPSR();\
  __set_CPSR(temp | 0xC0);\
  return temp;\
}
/* Disable IRQ interrupt and save CPSR */
//static inline uint32_t disable_irq(void){
#define sai() {\
  uint32_t temp;\
  temp = __get_CPSR();\
  __set_CPSR(temp | 0x80);\
  return temp;\
}

/* Restore interrupt status */
//static inline void restore_interrupt(uint32_t saved_CPSR){
#define rei(saved_CPSR) {\
  __set_CPSR (saved_CPSR);\
}

#else
// AT91SAM3S/3U, CORTEX M3

#ifndef __IAR__
    // CortexM3
    // GCC
    /* enable interrupts */
    #define sei() //NVIC_EnableIRQ()
    /* Disable interrupts and save CPSR */
    #define cli() //NVIC_DisableIRQ()
    /* Disable IRQ interrupt and save CPSR */
    #define sai() //__get_interrupt_state
    /* Restore interrupt status */
    #define rei(saved) //__set_interrupt_state(saved)
#else
    // IAR
    /* enable interrupts */
    #define sei() __e nable_interrupt
    /* Disable interrupts and save CPSR */
    #define cli() __disable_interrupt
    /* Disable IRQ interrupt and save CPSR */
    #define sai() __get_interrupt_state
    /* Restore interrupt status */
    #define rei(saved) __set_interrupt_state(saved)
#endif
#endif

#if (QT_DELAY_CYCLES == 2)
    #define delay_cycles(n)
#elif (QT_DELAY_CYCLES == 3)
    #define delay_cycles(n)
#elif (QT_DELAY_CYCLES == 4)
    #define delay_cycles(n) nop()
#elif (QT_DELAY_CYCLES == 5)
    #define delay_cycles(n) do{   \
        nop();  \
        nop();  \
    } while(0);
#elif (QT_DELAY_CYCLES == 10)
    #define delay_cycles(n) do{   \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
    } while(0);
#elif (QT_DELAY_CYCLES == 25)
    #define delay_cycles(n) do{   \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
    } while(0);
#elif (QT_DELAY_CYCLES == 50)
#define delay_cycles(n) do{   \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
        nop();  \
    } while(0);
#else
    #error 'DELAY_CYCLES value not supported.'
#endif


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif  // _COMPILER_H_
