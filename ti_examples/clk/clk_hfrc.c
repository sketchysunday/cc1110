
/***********************************************************************************
  Filename:     clk_hfrc.c

  Description:  Set system clock source to High Speed RC oscillator (HS RCOSC)
                and set system clock speed = 3.25 Mhz.

***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include <hal_types.h>
#include <hal_defs.h>
#include <hal_cc8051.h>
#include <ioCCxx10_bitdef.h>

#if (chip == 2510)
#include <ioCC2510.h>
#endif
#if (chip == 1110)
#include <ioCC1110.h>
#endif
#if (chip == 2511)
#include <ioCC2511.h>
#endif
#if (chip == 1111)
#include <ioCC1111.h>
#endif

/***********************************************************************************
* CONSTANTS
*/


/***********************************************************************************
* LOCAL VARIABLES
*/


/***********************************************************************************
* LOCAL FUNCTIONS
*/



/***********************************************************************************
* @fn          main
*
* @brief       Set system clock source to HS RCOSC @ 3.25 MHz
*
* @param       void
*
* @return      0
*/

int main(void)
{
    /* Initially, the HS RCOSC (HFRC) is not calibrated.
       To do so, the system clock source must be changed to the HS XOSC.
       Once the system clock source is HS XOSC then the chip will start
       calibrating both the HS RCOSC and low power LS RCOSC. The HS RCOSC
       calibration takes about 65 us, while the LS RCOSC calibration takes
       about 2 ms.

       The following is not possible before calibration is completed:
        - Changing system clock source back to HS RCOSC.
        - Entering PM {1-3}.
        - Power down HS RCOSC (SLEEP.OSC_PD = 1).
       Hence, it will take about 2.065 ms before the system clock source can
       be changed back to HS RCOSC.

       If the HS RCOSC is powered down after calibration, then the calibration
       value is stored and used when the HS RCOSC is powered up again. */


    // This code section changes the system clock source to the HS XOSC:

    /////////////////////////////////////////////////////////////////////////
    ///////////////////////// Code section begin ////////////////////////////

    // Power up unused oscillator (HS XOSC).
    SLEEP &= ~SLEEP_OSC_PD;

    // Wait until the HS XOSC is stable.
    while( !(SLEEP & SLEEP_XOSC_S) );

    // Change system clock source to HS XOSC.
    CLKCON &= ~CLKCON_OSC;

    // Wait until system clock source has changed to HS XOSC (CLKCON.OSC = 0).
    while ( CLKCON & CLKCON_OSC ) ;

    ///////////////////////// Code section end //////////////////////////////
    /////////////////////////////////////////////////////////////////////////



    // This code section changes the system clock source back to the HS RCOSC:

    /////////////////////////////////////////////////////////////////////////
    ///////////////////////// Code section begin ////////////////////////////

    // Note that at this point in time the HS RCOSC is already powered up
    // and stable, so it is not necessary to execute that instruction here.

    // Change system clock source to HS RCOSC and set clock speed = 3.25 MHz.
    CLKCON = (CLKCON & ~CLKCON_CLKSPD) | CLKCON_OSC | CLKSPD_DIV_8;

    // Wait until the system clock source has indeed changed to HS RCOSC.
    // Once [CLKCON.OSC = 1] this now confirms all of the following statements:
    //  - Initial calibration of the HS RCOSC and LS RCOSC has completed.
    //  - The desired system clock source is stable (SLEEP.HFRC_STB = 1).
    //  - The desired system clock source change has taken effect.
    while ( !(CLKCON & CLKCON_OSC) ) ;

    // Power down the HS XOSC, since it is not beeing used.
    // Note that the HS XOSC should not be powered down before the applied
    // system clock source is stable (SLEEP.HFRC_STB = 1).
    SLEEP |= SLEEP_OSC_PD;

    ///////////////////////// Code section end //////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    /* Continue... */

    return 0;
}


/***********************************************************************************
  Copyright 2008 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
***********************************************************************************/
