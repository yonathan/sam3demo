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
#include "sam-gui/common/sam_gui_errors.h"
#include "sam-gui/porting/sam_gui_porting.h"
#include "sam-gui/common/sam_gui_colors.h"
#include "sam-gui/wgt/core/wgt_core.h"
#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_audio Audio screen
 * @{
 *
 * \brief This screens aims at playing a sound sample.
 */

static SWGT_Widget g_bmpUnderConstruction ;
static SWGT_Widget g_btnBack ;

static uint32_t ScrAudio_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    WGT_CreateWidget( &g_bmpUnderConstruction, WGT_TYPE_BITMAP, 87, 81, 70, 84 ) ;
    WGT_SetBitmap( &g_bmpUnderConstruction, (void*)"/demo/bitmaps/bmp_under_construction.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_bmpUnderConstruction ) ;

    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    WGT_SetBitmap( &g_btnBack, (void*)"/demo/bitmaps/btn_back.bmp" ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrAudio_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nAudio", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 12, 200, "Under construction", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrAudio_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrAudio - msg id %x\r\n", pMsg->dwID ) ;

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

SWGTScreen ScrAudio=
{
    .ProcessMessage=ScrAudio_ProcessMessage,

    .OnInitialize=ScrAudio_OnInitialize,
    .OnEraseBackground=ScrAudio_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
