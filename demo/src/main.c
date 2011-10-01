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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Atmel library includes. */
#include <board.h>
#include "board_memories.h"
#include <cmsis/core_cm3.h>
#include <pio/pio_it.h>
#include <lcd/color.h>
#include <lcd/lcdd.h>
#include <lcd/draw.h>
#include <wdt/wdt.h>
#include <utility/trace.h>
#include <utility/assert.h>

/* Atmel SAM-GUI includes. */
#include "sam-gui/wgt/core/wgt_core.h"
#include "sam-gui/common/sam_gui_errors.h"
#include "sam-gui/porting/sam_gui_porting.h"
#include "disp/disp_backend.h"
#include "memories/media.h"

/* Atmel NAND FAT fs includes. */
#include <memories/MEDNandFlash.h>
#include <memories/nandflash/RawNandFlash.h>
#include <memories/nandflash/TranslatedNandFlash.h>
#include "fatfs_config.h"

/* Bitmaps */
//#include "bitmaps/bitmaps.h"

/* Demo GUI */
#include "gui/screens.h"
#include "demo_parameters.h"

/* Includes for Atmel FTL library */
#include "nand/ftldrv.h"

/* Atmel USB MSD include */
#include "usb/usbmsd.h"

/* Atmel Qtouch library */
#include "qtouch/qtouch.h"

#include "board_dependencies.h"

/**
 * \addtogroup SAM3S_demo AT91SAM3S demo
 * @{
 * \addtogroup main AT91SAM3S Demo main()
 * @{
 */

/**
 * Called if stack overflow during execution
 */
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
//	( void ) pxTask;
//	( void ) pcTaskName;

    printf( "stack overflow %x %s\r\n", pxTask, pcTaskName ) ;
    /* If the parameters have been corrupted then inspect pxCurrentTCB to
       identify which task has overflowed its stack. */
    for( ;; ) ;
}

/**
 * Configure the hardware for the demo.
 */
static void _SetupHardware( void )
{
    uint32_t i = 0;

    /* Initialize default IRQ priority for all Peripheral IRQs */
    for ( i=0 ; i < 35 ; i++ )
    {
        NVIC_SetPriority( (IRQn_Type)i, configMAX_SYSCALL_INTERRUPT_PRIORITY + 16 ) ;
    }

    Demo_Parameters_Initialize() ;

    /* Initialize DISP backends */
    if ( DISP_Initialize() != SAMGUI_E_OK )
    {
        printf( "_SetupHardware - Error initializing DISP\n" ) ;
    }

    /* Initialize NVIC to get accept interrupts from PIO controllers */
    PIO_InitializeInterrupts(configMAX_SYSCALL_INTERRUPT_PRIORITY + 16);

    /* Initialize USB Disk */
    UsbMsdInitialize() ;

    /* Initialize Disk */
    NandFlashInitialize() ;

#if defined DEMO_USE_QTOUCH
    /* Initialize QTouch */
    QTouchInitialize() ;
#endif // defined DEMO_USE_QTOUCH
}

extern void vApplicationIdleHook( void )
{
}

/**
 * Update current time
 */
static void _TimeUpdate( void )
{
    /** Do one second increment */
    g_demo_parameters.dwSec++ ;
    if ( g_demo_parameters.dwSec == 60 )
    {
        g_demo_parameters.dwSec=0 ;
        g_demo_parameters.dwMin++ ;
    }

    if ( g_demo_parameters.dwMin == 60 )
    {
        g_demo_parameters.dwMin=0 ;
        g_demo_parameters.dwHour++ ;
    }

    if ( g_demo_parameters.dwHour == 24 )
    {
        g_demo_parameters.dwHour=0 ;
        g_demo_parameters.dwDay++ ;
    }

    if ( g_demo_parameters.dwDay == 32 )
    {
        g_demo_parameters.dwDay=1 ;
        g_demo_parameters.dwMonth++ ;
    }

    if ( g_demo_parameters.dwMonth == 13 )
    {
        g_demo_parameters.dwMonth=1 ;
        g_demo_parameters.dwYear++ ;
    }
}

/**
 * This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook( void )
{
    /** As we don't use RTC in this application, we have to update current time every second */
    if ( xTaskGetTickCount()%1000 == 0 )
    {
        _TimeUpdate() ;
    }
}

/**
 * This task, when activated, send every ten seconds on debug UART the whole report of free heap and total tasks status
 */
static void _TaskMonitoring( void *pvParameters )
{
    static char szList[256] ;

    for ( ; ; )
    {
        printf( "--- free heap %u\r\n", xPortGetFreeHeapSize() ) ;
        printf( "--- task ## %u", uxTaskGetNumberOfTasks() ) ;
        vTaskList( (signed char*)szList ) ;
        printf( szList ) ;
//        printf( "\r\n" ) ;
        SAMGUI_TaskDelay( 10000 ) ;
    }
}

/**
 * Main entry point
 * Initialize hardware, SAM-GUI, App GUI, Monitor task, QTouch task, USB MassStorage task
 * Then start scheduler....
 */
extern int main( void )
{
    /** Disable watchdog. */
    WDT_Disable() ;

    /** Output demo infomation. */
    printf( "-- Demo AT91SAM3S4C %s --\n\r", DEMO_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /** Prepare the hardware. */
    _SetupHardware() ;

    /** Initialize and start SAM-GUI. */
    if ( WGT_Initialize() == SAMGUI_E_OK )
    {
#    if 0
        // For tests only
//        WGT_SetCurrentScreen( &ScrIntro ) ;
//        WGT_SetCurrentScreen( &ScrMain ) ;
//        WGT_SetCurrentScreen( &ScrAudio ) ;
//        WGT_SetCurrentScreen( &ScrLPModes ) ;
//        WGT_SetCurrentScreen( &ScrLPModes_Backup ) ;
//        WGT_SetCurrentScreen( &ScrLPModes_Wait ) ;
//        WGT_SetCurrentScreen( &ScrLPModes_Sleep ) ;
//        WGT_SetCurrentScreen( &ScrSettings ) ;
//        WGT_SetCurrentScreen( &ScrSettings_Date ) ;
//        WGT_SetCurrentScreen( &ScrSettings_Time ) ;
//        WGT_SetCurrentScreen( &ScrSettings_Backlight ) ;
//        WGT_SetCurrentScreen( &ScrInfos ) ;
//        WGT_SetCurrentScreen( &ScrPPT ) ;
//        WGT_SetCurrentScreen( &ScrTSDCalibration ) ;
        WGT_SetCurrentScreen( &ScrDISPDemo ) ;
#    else
        /** if no TSD calibration done */
        if ( g_demo_parameters.sCalibration.lSlopeX == 0 )
        {
            /** Set Calibration screen as first screen to be handled. */
            WGT_SetCurrentScreen( &ScrTSDCalibration ) ;
        }
        else
        {
            /** Set Intro as first screen to be handled. */
            WGT_SetCurrentScreen( &ScrIntro ) ;
        }
#    endif

        /** Start task to monitor processor activity */
        if ( xTaskCreate( _TaskMonitoring, "Monitor", TASK_MONITOR_STACK_SIZE, NULL, TASK_MONITOR_STACK_PRIORITY, NULL ) != pdPASS )
        {
            printf( "Failed to create Monitor task\r\n" ) ;
        }

        /** Start the tasks defined within this file/specific to this demo. */
        if ( xTaskCreate( TaskFtl, "FTL", TASK_FTL_STACK_SIZE, NULL, TASK_FTL_STACK_PRIORITY, NULL ) != pdPASS )
        {
            printf( "Failed to create FTL task\r\n" ) ;
        }

#if defined DEMO_USE_QTOUCH
        if ( xTaskCreate( TaskQTouch, "QTOUCH", TASK_QTOUCH_STACK_SIZE, NULL, TASK_QTOUCH_STACK_PRIORITY, NULL ) != pdPASS )
        {
            printf( "Failed to create QTOUCH task\r\n" ) ;
        }
#endif // defined DEMO_USE_QTOUCH

        if ( xTaskCreate( TaskUsbMsd, "USB_MSD", TASK_USB_STACK_SIZE, NULL, TASK_USB_STACK_PRIORITY, NULL ) != pdPASS )
        {
            printf( "Failed to create USB_MSD task\r\n" ) ;
        }

        // Start the scheduler.
        vTaskStartScheduler() ;
    }

    // Will only get here if there was insufficient memory to create the idle task.
    return 0 ;
}

/** @} */
/** @} */
