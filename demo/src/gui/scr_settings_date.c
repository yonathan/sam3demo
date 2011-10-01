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
#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_settings_date Date settings screen
 * @{
 *
 * \brief This screen allows configuration of system date.
 */

static SWGT_Widget g_btnUpDay ;
static SWGT_Widget g_btnDownDay ;
static SWGT_Widget g_wgt_Day ;

static SWGT_Widget g_btnUpMonth ;
static SWGT_Widget g_btnDownMonth ;
static SWGT_Widget g_wgt_Month ;

static SWGT_Widget g_btnUpYear ;
static SWGT_Widget g_btnDownYear ;
static SWGT_Widget g_wgt_Year ;

static SWGT_Widget g_btnBack ;

//screen	Slide_10				
//bitmap	logo_atmel	4	8	87	40
//bitmap	btn_back	12	252	30	56
//text	Date setup	144	25	

//bitmap	btn_up	43	96	43	31
//bitmap	btn_down	43	164	43	31
//text		43	135		

//bitmap	btn_up	104	96	43	31
//bitmap	btn_down	104	164	43	31
//text		104	135		

//bitmap	btn_up	162	96	43	31
//bitmap	btn_down	162	164	43	31
//text		162	135		

//text	Day	53	209		
//text	Month	115	209		
//text	Year	172	209		

static __no_init char gs_szDay[3] ;
static __no_init char gs_szMonth[3] ;
static __no_init char gs_szYear[3] ;
static __no_init uint32_t gs_dwDay ;
static __no_init uint32_t gs_dwMonth ;
static __no_init uint32_t gs_dwYear ;

static void _ScrSettings_Date_SetDay( void )
{
    // Set Day text
    snprintf( gs_szDay, sizeof( gs_szDay ), "%02u", gs_dwDay ) ;
    WGT_SetText( &g_wgt_Day, gs_szDay ) ;
}

static void _ScrSettings_Date_SetMonth( void )
{
    // Set Month text
    snprintf( gs_szMonth, sizeof( gs_szMonth ), "%02u", gs_dwMonth ) ;
    WGT_SetText( &g_wgt_Month, gs_szMonth ) ;
}

static void _ScrSettings_Date_SetYear( void )
{
    // Set Year text
    snprintf( gs_szYear, sizeof( gs_szYear ), "%02u", gs_dwYear-2000 ) ;
    WGT_SetText( &g_wgt_Year, gs_szYear ) ;
}

static uint32_t _ScrSettings_Date_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    WGT_CreateWidget( &g_btnUpDay, WGT_TYPE_BUTTON, 43,96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpDay, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpDay ) ;

    WGT_CreateWidget( &g_wgt_Day, WGT_TYPE_TEXT, 43+12, 135, 22, 14 ) ;
    WGT_SetText( &g_wgt_Day, "DD" ) ;
    WGT_SetBkgndColor( &g_wgt_Day, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Day, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Day ) ;

    WGT_CreateWidget( &g_btnDownDay, WGT_TYPE_BUTTON, 43, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownDay, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnDownDay ) ;



    WGT_CreateWidget( &g_btnUpMonth, WGT_TYPE_BUTTON, 104,96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpMonth, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpMonth ) ;

    WGT_CreateWidget( &g_wgt_Month, WGT_TYPE_TEXT, 104+12, 135, 22, 14 ) ;
    WGT_SetText( &g_wgt_Month, "MM" ) ;
    WGT_SetBkgndColor( &g_wgt_Month, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Month, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Month ) ;

    WGT_CreateWidget( &g_btnDownMonth, WGT_TYPE_BUTTON, 104, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownMonth, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnDownMonth ) ;



    WGT_CreateWidget( &g_btnUpYear, WGT_TYPE_BUTTON, 162, 96, 43, 31 ) ;
    WGT_SetBitmap( &g_btnUpYear, (void*)"/demo/bitmaps/btn_up.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnUpYear ) ;

    WGT_CreateWidget( &g_wgt_Year, WGT_TYPE_TEXT, 162+12, 135, 22, 14 ) ;
    WGT_SetText( &g_wgt_Year, "YY" ) ;
    WGT_SetBkgndColor( &g_wgt_Year, GUICLR_WHITE ) ;
    WGT_SetTextColor( &g_wgt_Year, GUICLR_ATMEL_BLUE ) ;
    WGT_Screen_AddWidget( pScreen, &g_wgt_Year ) ;

    WGT_CreateWidget( &g_btnDownYear, WGT_TYPE_BUTTON, 162, 164, 43, 31 ) ;
    WGT_SetBitmap( &g_btnDownYear, (void*)"/demo/bitmaps/btn_down.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnDownYear ) ;



    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    WGT_SetBitmap( &g_btnBack, (void*)"/demo/bitmaps/btn_back.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

    gs_dwDay=g_demo_parameters.dwDay ;
    gs_dwMonth=g_demo_parameters.dwMonth ;
    gs_dwYear=g_demo_parameters.dwYear ;

    // Initialize text widgets with parameters data
    _ScrSettings_Date_SetDay() ;
    _ScrSettings_Date_SetMonth() ;
    _ScrSettings_Date_SetYear() ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Date_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nDate setup", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 47, 210, "Day", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 102, 210, "Month", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 166, 210, "Year", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Date_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrSettings_Date - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
            /*
             * Handle Back button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnBack )
            {
                g_demo_parameters.dwDay=gs_dwDay ;
                g_demo_parameters.dwMonth=gs_dwMonth ;
                g_demo_parameters.dwYear=gs_dwYear ;
                Demo_Parameters_CommitChanges() ;

                WGT_SetCurrentScreen( &ScrSettings ) ;
            }

            /*
             * Handle UP/Day button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpDay )
            {
                if ( gs_dwDay == 31 )
                {
                    gs_dwDay=1 ;
                }
                else
                {
                    gs_dwDay++ ;
                }

                _ScrSettings_Date_SetDay() ;
                WGT_Draw( &g_wgt_Day, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Day button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownDay )
            {
                if ( gs_dwDay == 1 )
                {
                    gs_dwDay=31 ;
                }
                else
                {
                    gs_dwDay-- ;
                }

                _ScrSettings_Date_SetDay() ;
                WGT_Draw( &g_wgt_Day, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Month button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpMonth )
            {
                if ( gs_dwMonth == 12 )
                {
                    gs_dwMonth=1 ;
                }
                else
                {
                    gs_dwMonth++ ;
                }

                _ScrSettings_Date_SetMonth() ;
                WGT_Draw( &g_wgt_Month, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Month button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownMonth )
            {
                if ( gs_dwMonth == 1 )
                {
                    gs_dwMonth=12 ;
                }
                else
                {
                    gs_dwMonth-- ;
                }

                _ScrSettings_Date_SetMonth() ;
                WGT_Draw( &g_wgt_Month, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Year button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnUpYear )
            {
                if ( gs_dwYear == 2020 )
                {
                    gs_dwYear=2000 ;
                }
                else
                {
                    gs_dwYear++ ;
                }

                _ScrSettings_Date_SetYear() ;
                WGT_Draw( &g_wgt_Year, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Year button
             */
            if ( pMsg->dwParam1 == (uint32_t)&g_btnDownYear )
            {
                if ( gs_dwYear == 2000 )
                {
                    gs_dwYear=2020 ;
                }
                else
                {
                    gs_dwYear-- ;
                }

                _ScrSettings_Date_SetYear() ;
                WGT_Draw( &g_wgt_Year, g_WGT_CoreData.pBE ) ;
            }
        break ;

        case WGT_MSG_TIMER :
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrSettings_Date=
{
    .ProcessMessage=_ScrSettings_Date_ProcessMessage,

    .OnInitialize=_ScrSettings_Date_OnInitialize,
    .OnEraseBackground=_ScrSettings_Date_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
