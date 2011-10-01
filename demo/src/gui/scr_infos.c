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
 * \addtogroup screen_infos Informations screen
 * @{
 *
 * \brief This screen shows informations about this demo application.
 */

/** Demo version string */
static char szVersionText[20] ;


static uint32_t ScrInfos_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    if ( g_demo_parameters.dwBacklight > 2 )
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)g_demo_parameters.dwBacklight, NULL ) ;
    }
    else
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)1, NULL ) ;
    }

//    WGT_CreateWidget( &g_bmpInfo, WGT_TYPE_BITMAP, 161, 82, 49, 49 ) ;
//    WGT_SetText( &g_bmpInfo, "bmp_info" ) ;
//    WGT_SetBitmap( &g_bmpInfo, (void*)aucBmp_info ) ;
//    WGT_SetBkgndColor( &g_bmpInfo, GUICLR( 37, 75, 165 ) ) ;
//    WGT_Screen_AddWidget( pScreen, &g_bmpInfo ) ;

//    WGT_CreateWidget( &g_wgt_Text, WGT_TYPE_TEXT, 15, 159, BOARD_LCD_WIDTH-15, 159+14+2+14 ) ;
//    WGT_SetText( &g_wgt_Text, "AT91SAM3S-EK demo\nversion 1.0" ) ;
//    WGT_SetBkgndColor( &g_wgt_Text, GUICLR_WHITE ) ;
//    WGT_SetTextColor( &g_wgt_Text, GUICLR_ATMEL_BLUE ) ;
//    WGT_Screen_AddWidget( pScreen, &g_wgt_Text ) ;

//    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 30, 56 ) ;
//    WGT_SetText( &g_btnBack, "btn_back" ) ;
////    WGT_SetBitmap( &g_btnBack, (void*)aucBtn_back ) ;
//    WGT_SetBkgndColor( &g_btnBack, GUICLR( 44, 200, 254 ) ) ;
//    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrInfos_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nInformation", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw info bitmap
    g_WGT_CoreData.pBE->DrawBitmap( 95, 82, 49, 49, (void*)"/demo/bitmaps/bmp_info.bmp" ) ;

    // Draw text
    g_WGT_CoreData.pBE->DrawText( 18, 159, "AT91SAM3S-EK Demo", &clr, (void*)&g_Font10x14, 0 ) ;
    snprintf( szVersionText, sizeof( szVersionText ), "    Version %s", DEMO_VERSION);
    g_WGT_CoreData.pBE->DrawText( 0, 191, szVersionText, &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrInfos_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrInfos - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
            // Send message to switch to main screen
            WGT_SetCurrentScreen( &ScrMain ) ;
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrInfos=
{
    .ProcessMessage=ScrInfos_ProcessMessage,

    .OnInitialize=ScrInfos_OnInitialize,
    .OnEraseBackground=ScrInfos_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
