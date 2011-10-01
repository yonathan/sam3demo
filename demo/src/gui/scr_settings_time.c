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
#include <stdio.h>
#include "sam-gui/common/sam_gui_colors.h"

/**
 * \addtogroup screens
 * @{
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_settings_time Time settings screen
 * @{
 *
 * \brief This screen allows configuration of system time.
 */

static SWGT_Widget g_btnUpHour ;
static SWGT_Widget g_btnDownHour ;
static SWGT_Widget g_wgt_Hour ;

static SWGT_Widget g_btnUpMin ;
static SWGT_Widget g_btnDownMin ;
static SWGT_Widget g_wgt_Min ;

static SWGT_Widget g_btnUpSec ;
static SWGT_Widget g_btnDownSec ;
static SWGT_Widget g_wgt_Sec ;

static SWGT_Widget g_btnBack ;

//screen	Slide_11				
//bitmap	logo_atmel	4	8	87	40
//bitmap	btn_back	12	252	30	56
//text	Time setup	144	25		
//bitmap	btn_up	43	96	43	31
//bitmap	btn_down	43	164	43	31
//text		43	135		
//bitmap	btn_up	104	96	43	31
//bitmap	btn_down	104	164	43	31
//text		104	135		
//bitmap	btn_up	162	96	43	31
//bitmap	btn_down	162	164	43	31
//text		162	135		
//text	Hour	53	209		
//text	Min	115	209		
//text	Sec	172	209		

static __no_init char gs_szHour[3] ;
static __no_init char gs_szMin[3] ;
static __no_init char gs_szSec[3] ;
static __no_init uint32_t gs_dwHour ;
static __no_init uint32_t gs_dwMin ;
static __no_init uint32_t gs_dwSec ;

static void _ScrSettings_Time_SetHour( void )
{
    // Set Hour text
    snprintf( gs_szHour, sizeof( gs_szHour ), "%02u", gs_dwHour ) ;
    WGT_SetText( &g_wgt_Hour, gs_szHour ) ;
}

static void _ScrSettings_Time_SetMin( void )
{
    // Set Min text
    snprintf( gs_szMin, sizeof( gs_szMin ), "%02u", gs_dwMin ) ;
    WGT_SetText( &g_wgt_Min, gs_szMin ) ;
}

static void _ScrSettings_Time_SetSec( void )
{
    // Set Sec text
    snprintf( gs_szSec, sizeof( gs_szSec ), "%02u", gs_dwSec ) ;
    WGT_SetText( &g_wgt_Sec, gs_szSec ) ;
}

static uint32_t _ScrSettings_Time_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    WGT_CreateWidget( &g_btnUpHour, WGT_TYPE_BUTTON, 43,96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpHour, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpHour ) ;

    WGT_CreateWidget( &g_wgt_Hour, WGT_TYPE_TEXT, 43+12, 135, 22, 15 ) ;
    WGT_SetText( &g_wgt_Hour, "HH" ) ;
    WGT_SetBkgndColor( &g_wgt_Hour, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Hour, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Hour ) ;

    WGT_CreateWidget( &g_btnDownHour, WGT_TYPE_BUTTON, 43, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownHour, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( &ScrSettings_Time, &g_btnDownHour ) ;



    WGT_CreateWidget( &g_btnUpMin, WGT_TYPE_BUTTON, 104,96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpMin, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpMin ) ;

    WGT_CreateWidget( &g_wgt_Min, WGT_TYPE_TEXT, 104+12, 135, 22, 15 ) ;
    WGT_SetText( &g_wgt_Min, "MM" ) ;
    WGT_SetBkgndColor( &g_wgt_Min, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Min, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Min ) ;

    WGT_CreateWidget( &g_btnDownMin, WGT_TYPE_BUTTON, 104, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownMin, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnDownMin ) ;



    WGT_CreateWidget( &g_btnUpSec, WGT_TYPE_BUTTON, 162,96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpSec, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpSec ) ;

    WGT_CreateWidget( &g_wgt_Sec, WGT_TYPE_TEXT, 162+12, 135, 22, 15 ) ;
    WGT_SetText( &g_wgt_Sec, "SS" ) ;
    WGT_SetBkgndColor( &g_wgt_Sec, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Sec, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Sec ) ;

    WGT_CreateWidget( &g_btnDownSec, WGT_TYPE_BUTTON, 162, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownSec, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnDownSec ) ;



    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    WGT_SetBitmap( &g_btnBack, (void*)"/demo/bitmaps/btn_back.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

    gs_dwHour=g_demo_parameters.dwHour ;
    gs_dwMin=g_demo_parameters.dwMin ;
    gs_dwSec=g_demo_parameters.dwSec ;

    // Initialize text widgets with parameters data
    _ScrSettings_Time_SetHour() ;
    _ScrSettings_Time_SetMin() ;
    _ScrSettings_Time_SetSec() ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Time_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nTime setup", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 43, 210, "Hour", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 108, 210, "Min", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 166, 210, "Sec", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Time_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrSettings_Time - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
            /*
             * Handle Back button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnBack )
            {
                g_demo_parameters.dwHour=gs_dwHour ;
                g_demo_parameters.dwMin=gs_dwMin ;
                g_demo_parameters.dwSec=gs_dwSec ;

                Demo_Parameters_CommitChanges() ;

                WGT_SetCurrentScreen( &ScrSettings ) ;
            }

            /*
             * Handle UP/Hour button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpHour )
            {
                if ( gs_dwHour == 23 )
                {
                    gs_dwHour=0 ;
                }
                else
                {
                    gs_dwHour++ ;
                }

                _ScrSettings_Time_SetHour() ;
                WGT_Draw( &g_wgt_Hour, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Hour button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownHour )
            {
                if ( gs_dwHour == 0 )
                {
                    gs_dwHour=23 ;
                }
                else
                {
                    gs_dwHour-- ;
                }

                _ScrSettings_Time_SetHour() ;
                WGT_Draw( &g_wgt_Hour, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Min button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpMin )
            {
                if ( gs_dwMin == 59 )
                {
                    gs_dwMin=0 ;
                }
                else
                {
                    gs_dwMin++ ;
                }

                _ScrSettings_Time_SetMin() ;
                WGT_Draw( &g_wgt_Min, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Min button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownMin )
            {
                if ( gs_dwMin == 0 )
                {
                    gs_dwMin=59 ;
                }
                else
                {
                    gs_dwMin-- ;
                }

                _ScrSettings_Time_SetMin() ;
                WGT_Draw( &g_wgt_Min, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Sec button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpSec )
            {
                if ( gs_dwSec == 59 )
                {
                    gs_dwSec=0 ;
                }
                else
                {
                    gs_dwSec++ ;
                }

                _ScrSettings_Time_SetSec() ;
                WGT_Draw( &g_wgt_Sec, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Sec button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownSec )
            {
                if ( gs_dwSec == 0 )
                {
                    gs_dwSec=59 ;
                }
                else
                {
                    gs_dwSec-- ;
                }

                _ScrSettings_Time_SetSec() ;
                WGT_Draw( &g_wgt_Sec, g_WGT_CoreData.pBE ) ;
            }
        break ;

        case WGT_MSG_TIMER :
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrSettings_Time=
{
    .ProcessMessage=_ScrSettings_Time_ProcessMessage,

    .OnInitialize=_ScrSettings_Time_OnInitialize,
    .OnEraseBackground=_ScrSettings_Time_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
