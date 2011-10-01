/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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
#include <stdio.h>

/* Miscelaneous Include from Atmel */
#include <drivers/utility/assert.h>

/* Include FreeRTOS API */
#include "FreeRTOS.h"
#include "task.h"

/* Include AT91SAM3S-EK definitions and initialization API */
#include <board/board.h>
#include <board/board_memories.h>
#include <pio/pio_it.h>

/* Include USB definitions */
#include <usb/device/massstorage/MSDDriver.h>
#include <nand/MSDLun.h>

/* Include Media definitions */
#include "nand/rtmedia.h"
#include "nand/ftldrv.h"

/** Size of one block in bytes. */
#define BLOCK_SIZE          512

/** Size of the MSD IO buffer in bytes (6K, more the better). */
#define MSD_BUFFER_SIZE     (12*BLOCK_SIZE)

/* MSDLun object */
MSDLun gMsdLun;

/** LUN read/write buffer. */
unsigned char msdBuffer[MSD_BUFFER_SIZE];

/** Total data read/write by MSD */
unsigned int msdReadTotal = 0;
unsigned int msdWriteTotal = 0;
unsigned short msdFullCnt = 0;
unsigned short msdNullCnt = 0;


/*----------------------------------------------------------------------------
 *        VBus monitoring (optional)
 *----------------------------------------------------------------------------*/

/** VBus pin instance. */
const Pin gPinVbus = PIN_USB_VBUS;

/**
 * \brief Configures the VBus Pin
 *
 * To trigger an interrupt when the level on that pin changes.
 */
static void VBus_Configure( void )
{
    TRACE_INFO("VBus configuration\n\r");

    /* Configure PIO */
    PIO_Configure(&gPinVbus, 1);

    /* Enable PIO interrupt */
    PIO_EnableIt(&gPinVbus);

    /* Check current level on VBus */
    if (PIO_Get(&gPinVbus)) {

        /* if VBUS present, force the connect */
        TRACE_INFO("conn\n\r");
        USBD_Connect();
    }
    else {
        USBD_Disconnect();
    }
}

/**
 * Invoked when the MSD finish a READ/WRITE.
 * \param flowDirection 1 - device to host (READ10)
 *                      0 - host to device (WRITE10)
 * \param dataLength Length of data transferred in bytes.
 * \param fifoNullCount Times that FIFO is NULL to wait
 * \param fifoFullCount Times that FIFO is filled to wait
 */
void MSDCallbacks_Data(unsigned char flowDirection,
                       unsigned int  dataLength,
                       unsigned int  fifoNullCount,
                       unsigned int  fifoFullCount)
{
    if (flowDirection) {

        msdReadTotal += dataLength;
    }
    else {

        msdWriteTotal += dataLength;
    }

    msdFullCnt += fifoFullCount;
    msdNullCnt += fifoNullCount;
}

int UsbMsdInitialize( void ) 
{

    return 0;
}


void TaskUsbMsd ( void* pParameter )
{
      /* Initialize LUN */
    RTLUN_Init(&(gMsdLun), &(gRtNandMedias),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0,
             MSDCallbacks_Data);

    /* USB clock source has been initialized in Lowlevel Init */
    
    /* BOT driver initialization */
    MSDDriver_Initialize(&(gMsdLun), 1);

    VBus_Configure();
    
    for ( ; ; ) {
        /* Invoke Mass storage state machine */
        MSDDriver_StateMachine();
        vTaskDelay( 1 ) ;
    }
}


