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

#include <string.h>

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "semphr.h"

// Atmel library includes.
#include <board.h>
#include <pio/pio_it.h>
#include <utility/trace.h>
#include <utility/assert.h>

/* Atmel USB MSD include */
#include "usb/usbmsd.h"

/* Atmel UI buttons definitions */
#include "sam-gui/wgt/frontends/frontend_pushbuttons.h"
#include "sam-gui/wgt/core/wgt_core_message.h"
#include "sam-gui/common/sam_gui_errors.h"

/*-----------------------------------------------------------*/

// The LCD task uses the sprintf function so requires a little more stack too.
#define mainLCD_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE * 2 )
#define mainFAT_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE * 2 )

/*-----------------------------------------------------------*/

/**
 * \brief VBUS detect IRQ Handler.
 */
#ifdef BOARD_REV_A
void PIOC_Irq23Handler(unsigned char id)
#endif
#ifdef BOARD_REV_B
void PIOC_Irq21Handler(unsigned char id)
#endif
{
    /* Check current level on VBus */
    if (PIO_Get(&gPinVbus)) {

        TRACE_INFO("VBUS conn\n\r");
        USBD_Connect();
    }
    else {

        TRACE_INFO("VBUS discon\n\r");
        USBD_Disconnect();
    }
}

/**
 * \brief Handles Button1 interrupt.
 */

void PIOB_Irq3Handler(unsigned char id)
{
    /* Check current level on VBus */
    if (PIO_Get(&gPinButton1))
        WGT_SendMessageISR(WGT_MSG_KEY_RELEASED, WGT_KEY_PB1, 0);
    else
        WGT_SendMessageISR(WGT_MSG_KEY_PRESSED, WGT_KEY_PB1, 0);
}


/**
 * \brief Handles Button2 interrupt.
 */

void PIOC_Irq12Handler(unsigned char id)
{
    /* Check current level on VBus */
    if (PIO_Get(&gPinButton2))
        WGT_SendMessageISR(WGT_MSG_KEY_RELEASED, WGT_KEY_PB2, 0);
    else
        WGT_SendMessageISR(WGT_MSG_KEY_PRESSED, WGT_KEY_PB2, 0);
}

