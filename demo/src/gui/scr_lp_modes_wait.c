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
#include "sam-gui/common/sam_gui_colors.h"
#include <stdio.h>

#include <cmsis/core_cm3.h>
#include <pio/pio.h>
#include <pmc/pmc.h>
#include <usb/device/core/usbd.h>

/**
 * \addtogroup lowpower_modes
 * @{
 * \addtogroup screen_wait_mode Wait Mode screen
 * @{
 */

/** Pins used to wake-up */
static const uint32_t g_waitWakeUpPin = PIN_TCS_IRQ_WUP_ID;

static uint32_t g_PllarValBackup = 0;
static uint32_t g_PllbrValBackup = 0;
static uint32_t g_MckrValBackup  = 0;
static uint32_t g_irq[2];

/**-----------------------------------------------------------------------------
 * Switch master clock to On-Chip Fast RC Oscillator.
 * -----------------------------------------------------------------------------
 */
static void switchMckFastRC(void)
{
    // Enable Fast RC oscillator
    PMC->CKGR_MOR = CKGR_MOR_MOSCSEL | (0x37 << 16) | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCRCEN;
    // Wait the Fast RC to stabilize
    while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

    // Switch to main clock
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES);
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    // Switch from Main Xtal osc to Fast RC
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    // Wait for Main Oscillator Selection Status bit MOSCSELS
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

    // Disable Main XTAL Oscillator
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN;

    // Change frequency on 4MHz RC oscillator
    PMC->CKGR_MOR = (0x37 << 16) | PMC->CKGR_MOR;
    // Wait the Fast RC to stabilize
    while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

    // Stop PLLA
    // STMODE must be set at 2 when the PLLA is OFF
    PMC->CKGR_PLLAR = 0x2 << 14;

    // Stop PLLB
    // STMODE must be set at 2 when the PLLB is OFF
    PMC->CKGR_PLLBR = 0x2 << 14;
}


static void restoreClock(uint32_t PllarVal, uint32_t PllbrVal, uint32_t MckrVal)
{
    // switch to slow clock first
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS) | PMC_MCKR_CSS_SLOW_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES) | PMC_MCKR_PRES_CLK;

    // Restart Main Oscillator
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
    // Switch to moscsel
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

    // Switch to main oscillator
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES) | PMC_MCKR_PRES_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    // Restart PLL A
    if ((PllarVal & CKGR_PLLAR_MULA) != 0) {
        PMC->CKGR_PLLAR = PllarVal;
        while(!(PMC->PMC_SR & PMC_SR_LOCKA));
    }

    // Restart PLL B
    if ((PllbrVal & CKGR_PLLBR_MULB) != 0) {
        PMC->CKGR_PLLBR = PllbrVal;
        while(!(PMC->PMC_SR & PMC_SR_LOCKB));
    }

    // Switch to fast clock
    PMC->PMC_MCKR = (MckrVal & (unsigned int)~PMC_MCKR_CSS) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = MckrVal;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}



static uint32_t ScrLPModes_Wait_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Wait_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;

    // Draw Atmel logo and label
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)"/demo/bitmaps/bmp_logo atmel.bmp" ) ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\nLow-power\n  modes", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw Backup bitmap and Title
    g_WGT_CoreData.pBE->DrawBitmap( 91, 66, 58, 59, (void*)"/demo/bitmaps/bmp_wait_mode.bmp" ) ;
    g_WGT_CoreData.pBE->DrawText( 66, 148, "Wait mode", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 12, 176, "Power consumption:\n       15 uA\n Core at 64 MHz", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw labels
    g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Wait_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    SGUIColor clr ;
    uint32_t i = 0;

//    printf( "ScrLPModes_Wait - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :

            clr.u.dwRGBA = GUICLR_WHITE;
            g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

            clr.u.dwRGBA = GUICLR_ATMEL_BLUE;
            g_WGT_CoreData.pBE->DrawText( 25, 240, "Touch the screen\n   to wake-up", &clr, (void*)&g_Font10x14, 0 ) ;
            // Set wakeup input for fast startup
            PMC->PMC_FSMR &= (unsigned int)~0xFFFF;
            PMC->PMC_FSMR |= g_waitWakeUpPin;

            PMC->PMC_FSMR |= PMC_FSMR_LPM;
            SCB->SCR &= (unsigned int)~(1 << 2);

            // Backup clock settings
            g_PllarValBackup = PMC->CKGR_PLLAR;
            g_PllbrValBackup = PMC->CKGR_PLLBR;
            g_MckrValBackup  = PMC->PMC_MCKR;

            // Disable all ITs
            taskENTER_CRITICAL();
            g_irq[0] = NVIC->ISER[0];
            g_irq[1] = NVIC->ISER[1];
            NVIC->ICER[0] = 0xFFFFFFFF;
            NVIC->ICER[1] = 0xFFFFFFFF;

            // Disable USB
            USBD_Disconnect();
            // Disable Transceiver
            UDP->UDP_TXVC |= UDP_TXVC_TXVDIS;
            // Disable USB Peripheral clock
            PMC_DisablePeripheral(ID_UDP);
            // Disable USB Clock
            REG_PMC_SCDR = PMC_SCER_UDP;

            // Switch to 4MHz RC clock
            switchMckFastRC();

            // Wait the Fast RC to stabilize
            while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

            // Enter Wait Mode
            __WFE();

            // Waiting for MOSCRCEN bit is cleared is strongly recommended
            // to ensure that the core will not execute undesired instructions
            for (i = 0; i < 500; i++)
            {
                __NOP();
            }
            while (!(PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN));

            restoreClock(g_PllarValBackup, g_PllbrValBackup, g_MckrValBackup);

            // Re-enable all ITs
            NVIC->ISER[0] = g_irq[0];
            NVIC->ISER[1] = g_irq[1];
            taskEXIT_CRITICAL();

            // Enable USB Clock
            REG_PMC_SCER = PMC_SCER_UDP;
            // Enable USB Peripheral clock
            PMC_EnablePeripheral(ID_UDP);
            // Enable Transceiver
            UDP->UDP_TXVC &= ~UDP_TXVC_TXVDIS;
            // Disable USB
            USBD_Connect();

            // Reset wakeup inputs
            PMC->PMC_FSMR = 0;
            PMC->PMC_FSPR = 0;

            // Go back to previous screen
            WGT_SetCurrentScreen( &ScrLPModes );
        break;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrLPModes_Wait=
{
    .ProcessMessage=ScrLPModes_Wait_ProcessMessage,

    .OnInitialize=ScrLPModes_Wait_OnInitialize,
    .OnEraseBackground=ScrLPModes_Wait_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
