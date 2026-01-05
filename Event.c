/*
 * Event.h
 *
 *  Created on: 9 oct. 2020
 *      Author: JoseLuis
  *
 *      Practica 2 de Sistemas Empotrados 2
 *      Planificacion DMS
 */

/******************************************************************************/
/*                        Used modules                                        */
/******************************************************************************/

#include <stdbool.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

Clock_Handle EvClock ;
Clock_Params Ev_clockParams;


/******************************************************************************/
/*                        Local functions                                     */
/******************************************************************************/


Void Ev(UArg arg0, UArg arg1) {

    static char SB = 0 ;

    if (SB) {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2,GPIO_PIN_2) ;
        SB = 0 ;
    } else {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2,0) ;
        SB = 1 ;
    }
}

/******************************************************************************/
/*                        Public functions                                    */
/******************************************************************************/

void Init_Events (unsigned int T) {

    Clock_Params_init(&Ev_clockParams);
    Ev_clockParams.period = T/2 ;
    Ev_clockParams.startFlag = TRUE;
    EvClock = Clock_create((Clock_FuncPtr)Ev, 10, &Ev_clockParams, NULL);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
}

void Change_EventsPeriod (unsigned int T) ;

/*
 * It generates a square wave of period T. The rising edges are the events.
 */



