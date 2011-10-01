/*! \file *********************************************************************
 *
 * \brief Compiler file for AVR32.
 *
 * This file defines commonly used types and macros.
 *
 * - Compiler:           ARM IAR
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

/*============================ INCLUDES ======================================*/
#include <intrinsics.h>
#include "board.h"

/*============================ MACROS ========================================*/

#define nop() (__no_operation())

/* enable interrupts */
#define sei() //(taskEXIT_CRITICAL())

/* Disable interrupts */
#define cli() //(taskENTER_CRITICAL())

/* Disable IRQ interrupt and save status */
#define sai()
/* Restore interrupt status */
#define rei(X)

#if (QT_DELAY_CYCLES == 2)
    #define delay_cycles(n)
#elif (QT_DELAY_CYCLES == 3)
    #define delay_cycles(n) nop()
#elif (QT_DELAY_CYCLES == 4)
    #define delay_cycles(n) do{   \
        nop();  \
        nop();  \
    } while(0);
#elif (QT_DELAY_CYCLES == 5)
    #define delay_cycles(n) do{   \
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
