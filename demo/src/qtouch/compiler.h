/*============================ FILE HEADER ====================================
$Author: rkannan $: 

$Date$: 

$URL$: 
==============================================================================*/

#ifndef COMPILER_H
#define COMPILER_H
/*============================ INCLUDES ======================================*/
#if defined (__AVR32__)
    #include "compiler_avr32_gcc.h"
#elif defined(__ICCAVR__)|| defined (__ICCAVR32__)
    #include "compiler_avr_iar.h"
#elif defined(__GNUC__) && !defined(__AT91__)
    #include "compiler_avr_gcc.h"
#elif defined(__GNUC__) && defined(__AT91__)
    #include "compiler_arm.h"
#elif defined(__ICCARM__) && defined(__AT91__)
    #include "compiler_arm_iar.h"
#else
    #error 'This compiler is not supported at the moment.'
#endif

/*============================ MACROS ========================================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
#endif
/* EOF */
