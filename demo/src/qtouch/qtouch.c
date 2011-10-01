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

/* Atmel UI buttons definitions */
#include "sam-gui/wgt/frontends/frontend_pushbuttons.h"
#include "sam-gui/wgt/core/wgt_core_message.h"
#include "sam-gui/common/sam_gui_errors.h"

/* Atmel Freetos wrapper */
#include "qtouch.h"
#include "touch_api.h"
#include <board.h>
#include <wdt/wdt.h>
#include <pmc/pmc.h>
#include <pio/pio.h>
#include <cmsis/core_cm3.h>

#include <usart/uart_console.h>
#include <pwmc/pwmc.h>
#include <utility/trace.h>
#include <utility/led.h>
#include <stdio.h>
#include "qtouch/compiler.h"

#define __delay_cycles(n)     arm_delay_cycles(n)
#define arm_delay_cycles(n) {\
    unsigned long int temp; \
    temp = n;               \
    do {                    \
        nop();              \
        temp--;             \
    } while(temp); }

/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/
#define GET_SENSOR_STATE(SENSOR_NUMBER) (qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8)))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]


/*----------------------------------------------------------------------------
                                extern variables
----------------------------------------------------------------------------*/
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;

/* measurement data */
extern qt_touch_lib_measure_data_t qt_measure_data;

/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);

/*----------------------------------------------------------------------------
  static variables
----------------------------------------------------------------------------*/
/* flag set by timer ISR when it's time to measure touch */
static volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
static volatile uint16_t current_time_ms_touch = 0u;

/**
 * \brief qt_set_parameters
 * This will fill the default threshold values in the configuration data
 * structure. But User can change the values of these parameters
*/
static void qt_set_parameters( void )
{
    /* This will be modified by the user to different values */
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
}

/**
 * \brief config_sensors_at91
 * Configuration of the slider and the touch of the QTouch
*/
static void config_sensors_at91(void)
{

#if defined (_AT913SREVABETA1_)
    static const Pin pinsQTouch[] = {__SLIDER_PIO_SNS, __SLIDER_PIO_SNSK};
    PIO_Configure(pinsQTouch, PIO_LISTSIZE(pinsQTouch));
#endif

#if defined (_AT913SREVABETA2_)
    static const Pin pinsQTouch[] = {__KEYS_PIO_SNS, __KEYS_PIO_SNSK};
    PIO_Configure(pinsQTouch, PIO_LISTSIZE(pinsQTouch));
#endif

#if defined (_AT913SREVB_)
    static const Pin pinsQTouch[] = {__SLIDER_PIO_SNS, __SLIDER_PIO_SNSK,\
                                     __KEYS_PIO_SNS, __KEYS_PIO_SNSK};
    PIO_Configure(pinsQTouch, PIO_LISTSIZE(pinsQTouch));
#endif

#if defined (_AT913SREVABETA1_)
    // Slider: PA0, PA1, PA2, PA3, PA4, PA5
    qt_enable_slider( CHANNEL_0, CHANNEL_2, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );
#endif

#if  defined (_AT913SREVB_)
    // Slider: PA0, PA1, PA2, PA3, PA4, PA5
    qt_enable_slider( CHANNEL_0, CHANNEL_2, NO_AKS_GROUP, 16u, HYST_6_25, RES_8_BIT, 0u  );

    // Valid: SNS = PC22, SNSK = PC23
    qt_enable_key( CHANNEL_27, AKS_GROUP_1, 50u, HYST_6_25 );

    // Up: SNS = PC24, SNSK = PC25
    qt_enable_key( CHANNEL_28, AKS_GROUP_1, 50u, HYST_6_25 );

    // Down: SNS = PC26, SNSK = PC27
    qt_enable_key( CHANNEL_29, AKS_GROUP_1, 50u, HYST_6_25 );

    // Left: SNS = PC28, SNSK = PC29
    qt_enable_key( CHANNEL_30, AKS_GROUP_1, 50u, HYST_6_25 );

    // Right: SNS = PC30, SNSK = PC31
    qt_enable_key( CHANNEL_31, AKS_GROUP_1, 50u, HYST_6_25 );
#endif


#if defined (_AT913SREVABETA2_)
    // Valid: SNS = PC22, SNSK = PC23
    qt_enable_key( CHANNEL_11, AKS_GROUP_1, 10u, HYST_6_25 );

    // Up: SNS = PC24, SNSK = PC25
    qt_enable_key( CHANNEL_12, AKS_GROUP_1, 10u, HYST_6_25 );

    // Down: SNS = PC26, SNSK = PC27
    qt_enable_key( CHANNEL_13, AKS_GROUP_1, 10u, HYST_6_25 );

    // Left: SNS = PC28, SNSK = PC29
    qt_enable_key( CHANNEL_14, AKS_GROUP_1, 10u, HYST_6_25 );

    // Right: SNS = PC30, SNSK = PC31
    qt_enable_key( CHANNEL_15, AKS_GROUP_1, 10u, HYST_6_25 );
#endif

}

/**
 * \brief QTouchInitialize
 * QTouchInitialize initialise the QTouch interface
*/
int QTouchInitialize(void)
{
    qt_reset_sensing();

    /* Configure the Sensors as keys or Keys With Rotor/Sliders in this function */
    config_sensors_at91();

    /* initialise touch sensing */
    qt_init_sensing();

    /*  Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user */
    qt_set_parameters( );

    /* Address to pass address of user functions */
    /* This function is called after the library has made capacitive measurements,
    * but before it has processed them. The user can use this hook to apply filter
    * functions to the measured signal values.(Possibly to fix sensor layout faults) */
    qt_filter_callback = 0;

    /* enable interrupts */
    sei();
  return 0;
}


/**
 * \brief TaskQTouch task.
 * This task periodically update the QTouch.
 */
void TaskQTouch ( void* pParameter )
{
    static unsigned old_position=0;
    unsigned char vld_pressed=0;
    unsigned char up_pressed=0;
    unsigned char dwn_pressed=0;
    unsigned char lft_pressed=0;
    unsigned char rgt_pressed=0;

    for ( ; ; ) {

        /* clear flag: it's time to measure touch */
        time_to_measure_touch = 0u;

        /* measure touch sensors */
        qt_measure_sensors( current_time_ms_touch );

        sei();

        /* host application code goes here */

#if defined (_AT913SREVABETA1_)
        // SLIDER: PA0, PA1, PA2, PA3, PA4, PA5: CHANNEL 0
        if( GET_ROTOR_SLIDER_POSITION(0) != old_position ) {
            old_position = GET_ROTOR_SLIDER_POSITION(0);
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_S1, 255-old_position );
        }
#endif
#if defined (_AT913SREVABETA2_)
        if(( GET_SENSOR_STATE(0) != 0 )&&(vld_pressed==0)) {
            vld_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K3, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(0) == 0)&&(vld_pressed==1)) {
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K3, 0 );
              vld_pressed=0;
          }
        }

        if(( GET_SENSOR_STATE(1) != 0 )&&(up_pressed==0)) {
            up_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K1, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(1) == 0)&&(up_pressed==1)) {
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K1, 0 );
            up_pressed=0;
          }
        }

        if(( GET_SENSOR_STATE(2) != 0 )&&(dwn_pressed==0)) {
            dwn_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K5, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(2) == 0)&&(dwn_pressed==1)) {
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K5, 0 );
            dwn_pressed=0;
          }
        }

        if(( GET_SENSOR_STATE(3) != 0 )&&(lft_pressed==0)) {
            lft_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K2, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(3) == 0)&&(lft_pressed==1)) {
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K2, 0 );
            lft_pressed=0;
          }
        }

        if(( GET_SENSOR_STATE(4) != 0 )&&(rgt_pressed==0)) {
            rgt_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K4, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(4) == 0)&&(rgt_pressed==1)) {
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K4, 0 );
            rgt_pressed=0;
          }
        }
#endif
#if defined (_AT913SREVB_)
        // SLIDER: PA0, PA1, PA2, PA3, PA4, PA5: CHANNEL 0
        if( GET_ROTOR_SLIDER_POSITION(0) != old_position ) {
            old_position = GET_ROTOR_SLIDER_POSITION(0);
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_S1, 255-old_position );
        }

      if(( GET_SENSOR_STATE(1) != 0 )&&(vld_pressed==0)) {
            vld_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K3, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(1) == 0)&&(vld_pressed==1)) {
              vld_pressed=0;
              WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K3, 0 );
          }
        }

        if(( GET_SENSOR_STATE(2) != 0 )&&(up_pressed==0)) {
            up_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K1, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(2) == 0)&&(up_pressed==1)) {
            up_pressed=0;
            WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K1, 0 );
          }
        }

      if(( GET_SENSOR_STATE(3) != 0 )&&(dwn_pressed==0)) {
            dwn_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K5, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(3) == 0)&&(dwn_pressed==1)) {
            dwn_pressed=0;
            WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K5, 0 );
          }
        }
        if(( GET_SENSOR_STATE(4) != 0 )&&(lft_pressed==0)) {
            lft_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K2, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(4) == 0)&&(lft_pressed==1)) {
            lft_pressed=0;
            WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K2, 0 );
          }
        }
        if(( GET_SENSOR_STATE(5) != 0 )&&(rgt_pressed==0)) {
            rgt_pressed=1;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K4, 0 );
        }
        else {
          if(( GET_SENSOR_STATE(5) == 0)&&(rgt_pressed==1)) {
            rgt_pressed=0;
            WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K4, 0 );
          }
        }
#endif
        vTaskDelay( 100/portTICK_RATE_MS ) ;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
