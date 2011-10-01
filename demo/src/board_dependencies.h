/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#ifndef BOARD_DEPENDENCIES_H
#define BOARD_DEPENDENCIES_H
/*-----------------------------------------------------------*/

#define TASK_MONITOR_STACK_SIZE            (1024/sizeof(portSTACK_TYPE))
#define TASK_FTL_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#if defined DEMO_USE_QTOUCH
#    define TASK_QTOUCH_STACK_SIZE         (1024/sizeof(portSTACK_TYPE))
#endif // defined DEMO_USE_QTOUCH
#define TASK_USB_STACK_SIZE                (2048/sizeof(portSTACK_TYPE))

#define TASK_MONITOR_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_FTL_STACK_PRIORITY            (tskIDLE_PRIORITY+1)
#if defined DEMO_USE_QTOUCH
#    define TASK_QTOUCH_STACK_PRIORITY     (tskIDLE_PRIORITY+2)
#endif // defined DEMO_USE_QTOUCH
#define TASK_USB_STACK_PRIORITY            (tskIDLE_PRIORITY+3)


/*-----------------------------------------------------------*/

/**
 * \brief VBUS detect IRQ Handler.
 */
#ifdef BOARD_REV_A
extern void PIOC_Irq23Handler(unsigned char id);
#endif
#ifdef BOARD_REV_B
extern void PIOC_Irq21Handler(unsigned char id);
#endif

/**
 * \brief Handles Button1 interrupt.
 */
extern void PIOB_Irq3Handler(unsigned char id);

/**
 * \brief Handles Button2 interrupt.
 */
extern void PIOC_Irq12Handler(unsigned char id);

#endif /* BOARD_DEPENDENCIES_H */
