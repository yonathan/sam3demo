# ----------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support 
# ----------------------------------------------------------------------------
# Copyright (c) 2009, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------
BOARD  = at91sam3s-ek
DEVICE = at91sam3s4

PERL     = perl
DOXYGEN  = doxygen

MAKE_IAR = $(PERL) ../admin/perl/make-iar-project.pl \
    --board $(BOARD) \
    --device $(DEVICE) \
    --config sram \
    --config flash
MAKE_DOC  = $(DOXYGEN) admin/doxygen/Doxyfile	
BUILD_IAR = iarbuild.exe




# ----------------------------------------------------------------------------
# Build all projects	
clean: 
	(cd getting-started/bin; rm -f *)
	(cd rtc; rm -f rtc.eww)



all:  getting-started-example \
#      adc12 \
#      adc12_temp_sensor \
#      cm3-bit-banding \
#	  crccu \
#	  eefc_pgm \
#	  eefc_uniqueid \
#	  pio_alternate_function \
#	  pwm_pdc \
#	  rtc \
#	  rtt \
#	  smc_lcd \
#	  smc_nandflash \
#	  usart_hard_handshaking \
#	  usart_rs485 \
#	  wdg_irq



# ----------------------------------------------------------------------------
# Project:     acc_irq
# Project build scripts for iar and gnu
acc_irq-example: acc_irq
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     adc12
# Project build scripts for iar and gnu
adc12-example: adc12
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     adc12_temp_sensor
# Project build scripts for iar and gnu
adc12_temp_sensor-example: adc12_temp_sensor
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     adc12_threshold_wakeup
# Project build scripts for iar and gnu
adc12_threshold_wakeup-example: adc12_threshold_wakeup
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     cm3-bit-banding
# Project build scripts for iar and gnu
cm3-bit-banding-example: cm3-bit-banding
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     crccu
# Project build scripts for iar and gnu
crccu-example: crccu
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     dac12_sinewave
# Project build scripts for iar and gnu
dac12_sinewave-example: dac12_sinewave
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     demo
# Project build scripts for iar and gnu
demo-example: demo
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     eefc_pgm
# Project build scripts for iar and gnu
eefc_pgm-example: eefc_pgm
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     eefc_uniqueid
# Project build scripts for iar and gnu
eefc_uniqueid-example: eefc_uniqueid
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     pio_alternate_function
# Project build scripts for iar and gnu
pio_alternate_function-example: pio_alternate_function
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     pmc_clock_failure_detec
# Project build scripts for iar and gnu
pmc_clock_failure_detec-example: pmc_clock_failure_detec
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     pwm_pdc
# Project build scripts for iar and gnu
pwm_pdc-example:  pwm_pdc
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     rtc
# Project build scripts for iar and gnu
rtc-example: rtc
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     rtt
# Project build scripts for iar and gnu
rtt-example: rtt
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# # ----------------------------------------------------------------------------
# Project:     smc_lcd
# Project build scripts for iar and gnu
smc_lcd-example: smc_lcd
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     smc_nandflash
# Project build scripts for iar and gnu
smc_nandflash-example: smc_nandflash
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     spi_dataflash
# Project build scripts for iar and gnu
spi_dataflash-example: spi_dataflash
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     usart_hard_handshaking
# Project build scripts for iar and gnu
usart_hard_handshaking-example: usart_hard_handshaking
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     usart_rs485
# Project build scripts for iar and gnu
usart_rs485-example: usart_rs485
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     usb_device_cdc_serial
# Project build scripts for iar and gnu
usb_device_cdc_serial-example: usb_device_cdc_serial
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?) 
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project:     wdg_irq
# Project build scripts for iar and gnu
wdg_irq-example: wdg_irq
#	(cd $?; make) 
	(cd $?; $(MAKE_IAR) --project $?)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_sram)
#	(cd $?; $(BUILD_IAR) ewp/$?.ewp -build at91sam3s4_flash)

# ----------------------------------------------------------------------------
# Project: getting-started

# Project build scripts for iar and gnu
getting-started-example: getting-started
	pwd
	(cd $?; make -f Makefile) 
	'cd $?; $(MAKE_IAR) --project $?'
	$(MAKE_IAR) --project $?
# ----------------------------------------------------------------------------

