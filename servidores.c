/*
 * servidores.c
 *
 *  Created on: 04/01/2026
 *      Author: Jogue
 */


#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/uia/events/UIABenchmark.h>
#include <xdc/runtime/Log.h>

#include "servidores.h"
#include "computos.h"

GateMutexPri_Handle S1;
GateMutexPri_Handle S2;
GateMutexPri_Handle S3;

GateMutexPri_Params prms ;

void Crear_Servidores (void) {

	GateMutexPri_Params_init (&prms) ;
	S1 = GateMutexPri_create (&prms, NULL) ;
	S2 = GateMutexPri_create (&prms, NULL) ;
	S3 = GateMutexPri_create (&prms, NULL) ;

}


// Servidor S1
void S11 (void) {
	IArg key ;

	key = GateMutexPri_enter (S1) ;
	Log_write1(UIABenchmark_start, (xdc_IArg)"S1");
	CS (10) ;
	Log_write1(UIABenchmark_stop, (xdc_IArg)"S1");
	GateMutexPri_leave (S1, key) ;
}

void S12 (void) {
	IArg key ;

	key = GateMutexPri_enter (S1) ;
	Log_write1(UIABenchmark_start, (xdc_IArg)"S1");
	CS (30) ;
	Log_write1(UIABenchmark_stop, (xdc_IArg)"S1");
	GateMutexPri_leave (S1, key) ;
}

// Servidor S2
void S21 (void) {
	IArg key ;

	key = GateMutexPri_enter (S2) ;
	Log_write1(UIABenchmark_start, (xdc_IArg)"S2");
	CS (20) ;
	Log_write1(UIABenchmark_stop, (xdc_IArg)"S2");
	GateMutexPri_leave (S2, key) ;
}

void S22 (void) {
	IArg key ;

	key = GateMutexPri_enter (S2) ;
	Log_write1(UIABenchmark_start, (xdc_IArg)"S2");
	CS (20) ;
	Log_write1(UIABenchmark_stop, (xdc_IArg)"S2");
	GateMutexPri_leave (S2, key) ;
}

void S31(void) {
    IArg key;

    key = GateMutexPri_enter (S3) ;
    Log_write1(UIABenchmark_start, (xdc_IArg)"S3");
    CS(10);
    Log_write1(UIABenchmark_stop, (xdc_IArg)"S3");
    GateMutexPri_leave (S3, key) ;
}

void S32(void) {
    IArg key;

    key = GateMutexPri_enter (S3) ;
    Log_write1(UIABenchmark_start, (xdc_IArg)"S3");
    CS(20);
    Log_write1(UIABenchmark_stop, (xdc_IArg)"S3");
    GateMutexPri_leave (S3, key) ;
}


