#ifndef __CONFIG_H
#define __CONFIG_H

//========================================================================
//                               SCLK Set
//========================================================================
// #define MAIN_Fosc 35000000L // super fast mode
#define MAIN_Fosc 24000000L // fast mode
// #define MAIN_Fosc 20000000L // normal mode
// #define MAIN_Fosc 12000000L // slow mode
// #define MAIN_Fosc 6000000L  // super slow mode

#define STC8G1KxxA
// #define STC8G1KxxT

//========================================================================
//                             Base lib import
//========================================================================

#include "type_def.h"
#include "STC8H.H"
#include <intrins.h>
#include <stdlib.h>
#include <stdio.h>

//========================================================================
//                        function parameter define
//========================================================================
#define Timer0
#define Timer1

#endif
