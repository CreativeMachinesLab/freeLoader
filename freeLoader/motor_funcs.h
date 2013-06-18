#ifndef MOTOR_FUNCS_H
#define MOTOR_FUNCS_H
/**************************************************************************
Motor Functions for using Dynamixel RX-64 with the freeLoader control pgrm
Last update: May 23, 2011

Copyright (c) 2011, Hod Lipson and John Amend
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution.

 * Neither the name of the freeLoader nor the names of its contributors 
   may be used to endorse or promote products derived from this software 
   without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
**************************************************************************/

#include "ftd2xx.h"		//FTDI USB to Serial conversion chip
#include <windows.h>

#include <iostream>		//for I/O
#include <iomanip>		//for I/O manipulation
#include <cstdio>		//more inputs & outputs
#include <fstream>		//for reading/writing to files
#include <string>

#pragma comment(lib, "ftd2xx.lib")

bool DYNA_initialize(FT_HANDLE ftHandleDYNA);

int get_lowbyte(int speed);
int get_highbyte(int speed);

int makeword( int lowbyte, int highbyte );

int motor_spin(FT_HANDLE ftHandleDYNA, int motorNum, int speed);
int read_encoder(FT_HANDLE ftHandleDYNA, int motorNum);



#endif
