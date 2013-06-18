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

#pragma comment(lib, "ftd2xx.lib")

#define ID					(2)
#define LENGTH				(3)
#define INSTRUCTION			(4)
#define PARAMETER			(5)
#define INST_WRITE			(3)
#define INST_READ			(2)

#define LATENCY_TIME		1 //ms
#define IN_TRASFER_SIZE		512 

unsigned char InstructionPacket[160] = {0};
unsigned char StatusPacket[70]={0};

FT_STATUS ft_status;




bool DYNA_initialize(FT_HANDLE ftHandleDYNA)
{
	ft_status = FT_ResetDevice(ftHandleDYNA);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetDataCharacteristics(ftHandleDYNA, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetFlowControl(ftHandleDYNA, FT_FLOW_NONE, (UCHAR)0, (UCHAR)0);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetLatencyTimer(ftHandleDYNA, LATENCY_TIME);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetUSBParameters(ftHandleDYNA, IN_TRASFER_SIZE, 0);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetTimeouts(ftHandleDYNA, 0, 0);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_Purge(ftHandleDYNA, FT_PURGE_RX|FT_PURGE_TX);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

	ft_status = FT_SetBaudRate(ftHandleDYNA, 1000000);
	if( ft_status != FT_OK )
		goto DYNA_init_error;

    return true;

DYNA_init_error:
	std::cout << "\n\nUSB2Dynamixel Initialization Error!";
	FT_Close(ftHandleDYNA);
    return false;
}


int get_lowbyte(int speed)
{
	unsigned short temp = speed & 0xff;
	return (int)temp;
}


int get_highbyte(int speed)
{
	unsigned short temp = speed & 0xff00;
	temp = temp >> 8;
	return (int)temp;
}

int makeword( int lowbyte, int highbyte )
{
	unsigned short word;
	word = highbyte;
	word = word << 8;
	word = word + lowbyte;
	return (int)word;
}

int motor_spin(FT_HANDLE ftHandleDYNA, int motorNum, int speed)
{	
	//make the instruction packet
	InstructionPacket[0] = 0xff;
	InstructionPacket[1] = 0xff;
	InstructionPacket[ID] = (unsigned char)motorNum;
	InstructionPacket[LENGTH] = 5; //=parameter number +2
	InstructionPacket[INSTRUCTION] = INST_WRITE; 
	InstructionPacket[PARAMETER] = (unsigned char) 32; //address for free spin
	InstructionPacket[PARAMETER+1] = (unsigned char)get_lowbyte(speed);
	InstructionPacket[PARAMETER+2] = (unsigned char)get_highbyte(speed);
		
	unsigned char checksum = 0;
	for(unsigned char i=0; i<(InstructionPacket[LENGTH]+1); i++ )
		checksum += InstructionPacket[i+2];
	InstructionPacket[InstructionPacket[LENGTH]+3] = ~checksum;

	//send the packet
	unsigned char *pPacket = InstructionPacket;
	int numPacket = 9; //length plus 4
	DWORD dwNumToWrite = (DWORD)numPacket;
	DWORD dwNumWritten;
	
	ft_status = FT_Write(ftHandleDYNA, (LPVOID)pPacket, dwNumToWrite, &dwNumWritten );
	if( ft_status == FT_IO_ERROR )
	{
		std::cout << "\n\nError sending packet to motor!";
		return 0;
	}
	if(numPacket != dwNumWritten)
	{
		std::cout << "\n\nAll Bytes were not written to device!";
		return 0;
	}

	return 1;
}



int read_encoder(FT_HANDLE ftHandleDYNA, int motorNum)
{
	//make the packet
	InstructionPacket[0] = 0xff;
	InstructionPacket[1] = 0xff;
	InstructionPacket[ID] = (unsigned char)motorNum;
	InstructionPacket[LENGTH] = 4;
	InstructionPacket[INSTRUCTION] = INST_READ;
	InstructionPacket[PARAMETER] = (unsigned char)36;//address for encoder
	InstructionPacket[PARAMETER+1] = 2;

	unsigned char checksum = 0;
	for(unsigned char i=0; i<(InstructionPacket[LENGTH]+1); i++ )
		checksum += InstructionPacket[i+2];
	InstructionPacket[InstructionPacket[LENGTH]+3] = ~checksum;
	
	//send the packet
	unsigned char *pPacket = InstructionPacket;
	int numPacket = 8; //length plus 4
	DWORD dwNumToWrite = (DWORD)numPacket;
	DWORD dwNumWritten;
	
	ft_status = FT_Write(ftHandleDYNA, (LPVOID)pPacket, dwNumToWrite, &dwNumWritten );
	if( ft_status == FT_IO_ERROR )
	{
		std::cout << "\n\nError sending encoder packet to motor!";
		return 0;
	}
	if(numPacket != dwNumWritten)
	{
		std::cout << "\n\nAll Bytes of encoder packet were not written to device!";
		return 0;
	}
	
	//get status packet
	unsigned char *ppPacket = StatusPacket;
	DWORD dwNumToRead;
	DWORD dwNumRead = 0;

	ft_status = FT_GetQueueStatus(ftHandleDYNA, &dwNumToRead );
	if( ft_status != FT_OK )
	{
		std::cout << "\n\nError receiving encoder status packet, Queue not empty!";
		return 0;
	}
	if( dwNumToRead > 0 )
	{
		ft_status = FT_Read(ftHandleDYNA, (LPVOID)ppPacket, dwNumToRead, &dwNumRead ); 
		if( ft_status == FT_IO_ERROR )
		{
			std::cout << "\n\nError reading encoder value!";
			return 0;
		}
	}
	
	return makeword((int)StatusPacket[PARAMETER], (int)StatusPacket[PARAMETER+1]);	
	
}
