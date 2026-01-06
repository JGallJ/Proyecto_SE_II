/*
 * buttons.c
 *
 *  Created on: 4 ene. 2026
 *      Author: Jogue
 */

#include <stdint.h>
#include <stdbool.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Swi.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"


#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

#include "computos.h"

/******************************************************************************/
/*                        Function prototypes                                 */
/******************************************************************************/

Void myIsr_buttons(UArg arg);

Void Swi_func_button_4(UArg arg0, UArg arg1);
Void Swi_func_button_0(UArg arg0, UArg arg1);

/******************************************************************************/
/*                        Global variables                                    */
/******************************************************************************/

/* Primera componente es para button_4 (izq), segunda para button_0 (der) */
static bool buttons_pressed[2] = { false, false };

Hwi_Params hwiParams_buttons;
Swi_Params swiParams_button_4;
Swi_Params swiParams_button_0;

Hwi_Handle myHwi_buttons;
Swi_Handle mySwi_button_4;
Swi_Handle mySwi_button_0;

/******************************************************************************/
/*                        Functions code                                      */
/******************************************************************************/

void initButtons(void) {

    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Enable clock for Port F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); //Wait until ready

    // Neccessary for PF0 (boton der)
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  // Unlock Port F
    GPIO_PORTF_CR_R |= GPIO_PIN_0;

    /***************************************************************************/
    /*****************************Cosas de leds*********************************/

    //Configure leds as output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    /***************************************************************************/

    // Configure PF4 and PF0 as input with internal pull-up
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Tercero parámetro es la amplitud de salida, cuarto es weak pull-up

    //Configure GPIO interrupt
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0);

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);

    // Hwi for buttons
    Hwi_Params_init(&hwiParams_buttons);
    hwiParams_buttons.priority = 1;

    myHwi_buttons = Hwi_create(INT_GPIOF, myIsr_buttons, &hwiParams_buttons, NULL);
    if (myHwi_buttons == NULL) printf("myHwi_buttons create failed");

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);

    // Swi for button 4
    Swi_Params_init(&swiParams_button_4);
    swiParams_button_4.priority = 1 ;
    mySwi_button_4 = Swi_create(Swi_func_button_4, &swiParams_button_4, NULL);
    if (mySwi_button_4 == NULL) printf("mySwi_button_4 create failed");

    // Swi for button 0
    Swi_Params_init(&swiParams_button_0);
    swiParams_button_0.priority = 1 ;
    mySwi_button_0 = Swi_create(Swi_func_button_0, &swiParams_button_0, NULL);
    if (mySwi_button_0 == NULL) printf("mySwi_button_0 create failed");

    // Comprobamos que se termina la inicialización
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    CS(2000);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
}

// importante mantener esta interrupción lo más simple posible
Void myIsr_buttons(UArg arg) {

    uint32_t status;
    status = GPIOIntStatus(GPIO_PORTF_BASE, true);

    // Limpiar interrupciones
    GPIOIntClear(GPIO_PORTF_BASE, status);

    if (status & GPIO_PIN_4) {
        Swi_post(mySwi_button_4);
    }

    if (status & GPIO_PIN_0) {
        Swi_post(mySwi_button_0);
    }
}

Void Swi_func_button_4(UArg arg0, UArg arg1) {
    // Gestion rebotes
    //CS(200);

    buttons_pressed[0] = !buttons_pressed[0];
    // Encendemos un led para comprobar que funciona todo bien
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, buttons_pressed[0] ? GPIO_PIN_1 : 0);
}

Void Swi_func_button_0(UArg arg0, UArg arg1) {
    // Gestion rebotes
    //CS(200);

    buttons_pressed[1] = !buttons_pressed[1];
    // Encendemos un led para comprobar que funciona todo bien
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, buttons_pressed[1] ? GPIO_PIN_2 : 0);
}

