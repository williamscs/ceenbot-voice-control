// Auth: Jose Santos
// Desc: Header file for the SPIFLASH subsystem module.  This module contains
//       functions for reading/writing to the on-board SPI FLASH device on the
//       v2.21 '324 platform.

#ifndef __SPIFLASH324V221_H__
#define __SPIFLASH324V221_H__

#include<avr/io.h>

#include "utils324v221.h"
#include "sys324v221.h"
#include "spi324v221.h"

// =============================== defines ================================== //
// ============================ type declarations =========================== //
// ============================== prototypes ================================ //
// Desc: Initializes and opens services for manipulating the on-board SPI 
//       flash.
extern SUBSYS_OPENSTAT SPIFLASH_open( void );
// -------------------------------------------------------------------------- //
// Desc: Function deallocates and releases SPIFLASH resources.
extern void SPIFLASH_close( void );
// ============================ external globals ============================ //

#endif /* __SPIFLASH324V221_H__ */
