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

#include "gui/screens.h"
#include "sam-gui/wgt/core/wgt_core.h"
#include "sam-gui/common/sam_gui_errors.h"
#include "sam-gui/porting/sam_gui_porting.h"
#include "demo_parameters.h"
#include "sam-gui/common/sam_gui_colors.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_main Main screen
 * @{
 *
 * \brief This screen is the main screen with access to all functions.
 * Buttons can be browsed using TouchScreen, pushbuttons, potentiometer and QTouch.
 */

static SWGT_Widget g_btnAudio ;
static SWGT_Widget g_btnVideo ;
static SWGT_Widget g_btnSlideShow ;
#if defined DEMO_USE_QTOUCH
static SWGT_Widget g_btnQTouch ;
#endif // defined DEMO_USE_QTOUCH
static SWGT_Widget g_btnLPModes ;
static SWGT_Widget g_btnSettings ;
static SWGT_Widget g_btnInfos ;
static SWGT_Widget g_wgt_Status ;

static char szStatusText[20] ;

static void ScrMain_DrawStatus( void )
{
    snprintf( szStatusText, sizeof( szStatusText ), "%02u/%02u/%02u %02u:%02u:%02u",
    g_demo_parameters.dwMonth, g_demo_parameters.dwDay, g_demo_parameters.dwYear-2000,
    g_demo_parameters.dwHour, g_demo_parameters.dwMin, g_demo_parameters.dwSec ) ;

    WGT_SetText( &g_wgt_Status, szStatusText ) ;
    WGT_Draw( &g_wgt_Status, g_WGT_CoreData.pBE ) ;
}

static uint32_t ScrMain_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    if ( g_demo_parameters.dwBacklight > 2 )
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)g_demo_parameters.dwBacklight, NULL ) ;
    }
    else
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)14, NULL ) ;
    }

    WGT_CreateWidget( &g_btnAudio, WGT_TYPE_BUTTON, 14, 62, 64, 64 ) ;
    WGT_SetBitmap( &g_btnAudio, (void*)"/demo/bitmaps/btn_audio.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnAudio ) ;

    WGT_CreateWidget( &g_btnVideo, WGT_TYPE_BUTTON, 92, 60, 64, 64 ) ;
    WGT_SetBitmap( &g_btnVideo, (void*)"/demo/bitmaps/btn_video.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnVideo ) ;

    WGT_CreateWidget( &g_btnSlideShow, WGT_TYPE_BUTTON, 166, 61, 70, 63 ) ;
    WGT_SetBitmap( &g_btnSlideShow, (void*)"/demo/bitmaps/btn_slideshow.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnSlideShow ) ;

    WGT_CreateWidget( &g_btnSettings, WGT_TYPE_BUTTON, 18, 140, 59, 60 ) ;
    WGT_SetBitmap( &g_btnSettings, (void*)"/demo/bitmaps/btn_settings.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnSettings ) ;

#if defined DEMO_USE_QTOUCH
    WGT_CreateWidget( &g_btnQTouch, WGT_TYPE_BUTTON, 95, 165-26, 69, 69 ) ;
    WGT_SetBitmap( &g_btnQTouch, (void*)"/demo/bitmaps/btn_qtouch.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnQTouch ) ;

    WGT_CreateWidget( &g_btnLPModes, WGT_TYPE_BUTTON, 184, 140, 47, 67 ) ;
    WGT_SetBitmap( &g_btnLPModes, (void*)"/demo/bitmaps/btn_low_power_modes.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnLPModes ) ;

    WGT_CreateWidget( &g_btnInfos, WGT_TYPE_BUTTON, 100, 217, 62, 62 ) ;
    WGT_SetBitmap( &g_btnInfos, (void*)"/demo/bitmaps/btn_infos.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnInfos ) ;
#else
    WGT_CreateWidget( &g_btnLPModes, WGT_TYPE_BUTTON, 98, 140, 47, 67 ) ;
    WGT_SetBitmap( &g_btnLPModes, (void*)"/demo/bitmaps/btn_low_power_modes.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnLPModes ) ;

    WGT_CreateWidget( &g_btnInfos, WGT_TYPE_BUTTON, 166, 145, 62, 62 ) ;
    WGT_SetBitmap( &g_btnInfos, (void*)"/demo/bitmaps/btn_infos.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnInfos ) ;
#endif // defined DEMO_USE_QTOUCH

    WGT_CreateWidget( &g_wgt_Status, WGT_TYPE_TEXT, 0, 300, BOARD_LCD_WIDTH, 20 ) ;
    WGT_SetText( &g_wgt_Status, NULL ) ;
    WGT_SetBkgndColor( &g_wgt_Status, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Status, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Status ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrMain_OnEraseBackground( SWGTScreen* pScreen )
{
     SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nDemo home", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrMain_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    static uint32_t dwTimeUpdate=0 ;
    SWGT_Widget* pWidget ;

//    if ( pMsg->dwID != WGT_MSG_TIMER ) printf( "ScrMain - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :
            ScrMain_DrawStatus() ;

            // Increment timestamp
            dwTimeUpdate++ ;
            if ( dwTimeUpdate == 60000 )
            {
                // Write parameters every 60 seconds
                Demo_Parameters_CommitChanges() ;
                dwTimeUpdate=0 ;
            }
        break ;

        case WGT_MSG_PAINT :
#if defined DEMO_USE_QTOUCH
            WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
#else
            WGT_Screen_SetSelectedWidget( pScreen, &g_btnAudio ) ;
#endif // defined DEMO_USE_QTOUCH

            ScrMain_DrawStatus() ;
        break ;

        case WGT_MSG_KEY_RELEASED : // button released
//        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                // Select next widget
                case WGT_KEY_PB1 :
                case WGT_KEY_K4 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == &g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnVideo ) ;
                            }

                            if ( pWidget == &g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSlideShow ) ;
                            }

                            if ( pWidget == &g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSettings ) ;
                            }

#if defined DEMO_USE_QTOUCH
                            if ( pWidget == &g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
                            }

                            if ( pWidget == &g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                            }
#else
                            if ( pWidget == &g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                            }
#endif // defined DEMO_USE_QTOUCH

                            if ( pWidget == &g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnAudio ) ;
                            }
                        }
                    }
                break ;

#if defined DEMO_USE_QTOUCH
                // Select previous widget
                case WGT_KEY_K2 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == &g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnAudio ) ;
                            }

                            if ( pWidget == &g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnVideo ) ;
                            }

                            if ( pWidget == &g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSlideShow ) ;
                            }

                            if ( pWidget == &g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSettings ) ;
                            }

                            if ( pWidget == &g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
                            }

                            if ( pWidget == &g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                            }
                        }
                    }
                break ;
#endif //defined DEMO_USE_QTOUCH

#if defined DEMO_USE_QTOUCH
                // Select up widget
                case WGT_KEY_K1 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == &g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnAudio ) ;
                            }

                            if ( pWidget == &g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnVideo ) ;
                            }

                            if ( pWidget == &g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSlideShow ) ;
                            }

                            if ( pWidget == &g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
                            }
                        }
                    }
                break ;
#endif // defined DEMO_USE_QTOUCH

#if defined DEMO_USE_QTOUCH
                // Select down widget
                case WGT_KEY_K5 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == &g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnSettings ) ;
                            }

                            if ( pWidget == &g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
                            }

                            if ( pWidget == &g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                            }

                            if ( pWidget == &g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                            }

                            if ( pWidget == &g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, &g_btnVideo ) ;
                            }
                        }
                    }
                break ;
#endif // defined DEMO_USE_QTOUCH

                // Enter
                case WGT_KEY_PB2 :
                case WGT_KEY_K3 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget != NULL )
                        {
//                            printf( "ScrMain - Sending selected\r\n" ) ;
                            WGT_PostMessage( WGT_MSG_WIDGET_SELECTED, (uint32_t)pWidget, 0 ) ;
                        }
                    }
                break ;
            }
        break ;

        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_S1 :
                case WGT_KEY_VR1 :
//                    printf( "ScrMain - pot %u %u %u\r\n", pMsg->dwParam2, pMsg->dwParam2/7, ((pMsg->dwParam2*7)/256) ) ;

#if defined DEMO_USE_QTOUCH
                    switch ( ((pMsg->dwParam2*7)/256) )
#else
                    switch ( ((pMsg->dwParam2*6)/256) )
#endif // defined DEMO_USE_QTOUCH
                    {
                        case 0 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnAudio ) ;
                        break ;

                        case 1 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnVideo ) ;
                        break ;

                        case 2 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnSlideShow ) ;
                        break ;

                        case 3 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnSettings ) ;
                        break ;

#if defined DEMO_USE_QTOUCH
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnQTouch ) ;
                        break ;

                        case 5 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                        break ;

                        case 6 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                        break ;
#else
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnLPModes ) ;
                        break ;

                        case 5 :
                            WGT_Screen_SetSelectedWidget( pScreen, &g_btnInfos ) ;
                        break ;
#endif // defined DEMO_USE_QTOUCH
                    }
                break ;
            }
        break ;

        case WGT_MSG_WIDGET_SELECTED :
//            printf( "ScrMain - switching screen\r\n" ) ;
            if ( pMsg->dwParam1 == (uint32_t)&g_btnVideo )
            {
//                WGT_SetCurrentScreen( &ScrCube ) ;
//                WGT_SetCurrentScreen( &ScrStarfield ) ;
                WGT_SetCurrentScreen( &ScrDISPDemo ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)&g_btnSlideShow )
            {
                WGT_SetCurrentScreen( &ScrPPT ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)&g_btnAudio )
            {
                WGT_SetCurrentScreen( &ScrAudio ) ;
            }

#if defined DEMO_USE_QTOUCH
            if ( pMsg->dwParam1 == (uint32_t)&g_btnQTouch )
            {
                WGT_SetCurrentScreen( &ScrQTouch ) ;
            }
#endif // defined DEMO_USE_QTOUCH

            if ( pMsg->dwParam1 == (uint32_t)&g_btnLPModes )
            {
                WGT_SetCurrentScreen( &ScrLPModes ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)&g_btnSettings )
            {
                WGT_SetCurrentScreen( &ScrSettings ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)&g_btnInfos )
            {
                WGT_SetCurrentScreen( &ScrInfos ) ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

SWGTScreen ScrMain=
{
    .ProcessMessage=ScrMain_ProcessMessage,

    .OnInitialize=ScrMain_OnInitialize,
    .OnEraseBackground=ScrMain_OnEraseBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
