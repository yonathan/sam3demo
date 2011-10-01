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


/* Atmel NAND FTL library API. */
#include "rtmedia.h"

/*-----------------------------------------------------------*/
/*                         RtMedia Object                    */
/*-----------------------------------------------------------*/


/**
 * \brief Initialize the RtMedia object.
 * This initialization ONLY concerns the mutex
 * \param  pRtMedia    Pointer to a Media instance
 * \return Operation result code
 */
unsigned char RTMEDIA_Init( RtMedia       *pRtMedia)
{
    /* Initialize driver mutex */
    pRtMedia->mutex = xSemaphoreCreateMutex();
    if (pRtMedia->mutex == NULL) {
        TRACE_ERROR("Unable to allocate a mutex for Nand media\n\r");
        return 1;
    }
    return 0;
}

/**
 * \brief Wrapper for the Media Write function.
 * This wrapper protects from simultanoeous access to the same media
 * \param  pRtMedia    Pointer to a RtMedia instance
 * \param  address  Address at which to write
 * \param  data     Pointer to the data to write
 * \param  length   Size of the data buffer
 * \param  callback Optional pointer to a callback function to invoke when
 *                   the write operation terminates
 * \param  argument Optional argument for the callback function
 * \return Operation result code
 */
unsigned char RTMEDIA_Write( RtMedia       *pRtMedia,
                             unsigned int  address,
                             void          *data,                                      
                             unsigned int  length,
                             MediaCallback callback,
                             void          *argument)
{
  unsigned char result;
  Media *pMedia = &(pRtMedia->media);
  
  /* Take the mutex */
  xSemaphoreTake( pRtMedia->mutex, ( portTickType ) 0 );
  
  /* Invoke the Media Write function */
  result = pMedia->write(pMedia, address, data, length, callback, argument);
 
  /* Release the mutex */ 
  xSemaphoreGive( pRtMedia->mutex );
  
  return result;
}

/**
 * \brief Wrapper for the Media Read function.
 * This wrapper protects from simultanoeous access to the same media
 * \param  pRtMedia    Pointer to a RtMedia instance
 * \param  address  Address at which to write
 * \param  data     Pointer to the data to write
 * \param  length   Size of the data buffer
 * \param  callback Optional pointer to a callback function to invoke when
 *                   the write operation terminates
 * \param  argument Optional argument for the callback function
 * \return Operation result code
 */
unsigned char RTMEDIA_Read( RtMedia       *pRtMedia,
                          unsigned int   address,
                          void           *data,                                     
                          unsigned int   length,
                          MediaCallback  callback,
                          void           *argument)
{
  unsigned char result;
  Media *pMedia = &(pRtMedia->media);
  
  /* Take the mutex */
  xSemaphoreTake( pRtMedia->mutex, ( portTickType ) 0 );
  
  /* Invoke the Media Write function */
  result = pMedia->read(pMedia, address, data, length, callback, argument);
    
  /* Release the mutex */ 
  xSemaphoreGive( pRtMedia->mutex );
  
  return result;
}

/**
 * \brief Wrapper for the Media Flush function.
 * This wrapper protects from simultanoeous access to the same media
 * \param  pRtMedia    Pointer to a RtMedia instance
 * \return Operation result code
 */
unsigned char RTMEDIA_Flush( RtMedia       *pRtMedia)
{
  unsigned char result;
  Media *pMedia = &(pRtMedia->media);
  
  /* Take the mutex */
  xSemaphoreTake( pRtMedia->mutex, ( portTickType ) 0 );
  
  /* Invoke the Media Write function */
  result = pMedia->flush(pMedia);
 
  /* Release the mutex */ 
  xSemaphoreGive( pRtMedia->mutex );
  
  return result;
}
