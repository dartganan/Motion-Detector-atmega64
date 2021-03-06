;<$DS_HEADER>
;+--------------------------------------------------------------------------------+              
;|Jo�o D�Artagnan A. Oliveira Programmer and Author; 				  |
;|										  |
;| All rights reserved.								  |
;|										  |
;|   Redistribution and use in source and binary forms, with or without		  |
;|  modification, are permitted provided that the following conditions are met:	  |
;|										  |
;|   * Redistributions of source code must retain the above copyright		  |
;|     notice, this list of conditions and the following disclaimer.		  |
;|										  |
;|   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  |
;|   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    |
;|   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   |
;|   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE     |
;|   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR	  |
;|   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF	  |
;|   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS	  |
;|   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN	  |
;|   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)	  |
;|   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   |
;|   POSSIBILITY OF SUCH DAMAGE.						  |
;|										  |
;|   Bras�lia, Brazil November 30,2015 	06:28 am				  |
;+--------------------------------------------------------------------------------+
;</$DS_HEADER>
;+--------------------------------------------------------------+
;|								|
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA				|
;| DATA:22/03/2001						|
;>--------------------------------------------------------------<
;| A T 2 4 C 6 4 / 3 2  S E R I A L  E E P R O M 		|
;| A C C E S S  R O U T I N E S					|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _AT24C64_BYTE_WRITE	WRITE A BYTE INTO EEPROM						|
;|			SAMPLE USING BYTE_WRITE TO WRITE A BYTE INTO ADDRESS 1000 DATA 33	|
;|			AT CHIP ADDRESS 5							|
;|												|
;|			LDI	ACC,33			;DATA TO BE WRITE			|
;|			LDI	ACCH,5			;CHIP ADDRESS NUMBER 5			|
;|			LDI	ACCT,LOW(1000)		;ACCT:ACCTH=ADDRESS=1000		|
;|			LDI	ACCTH,HIGH(1000)						|
;|			RCALL	_AT24C64_BYTE_WRITE 	;WRITE DATA				|
;|----------------------------------------------------------------------------------------------|
;| _AT24C64_BYTE_READ	READ A BYTE FROM EEPROM							|
;|			SAMPLE USING BYTE_READ TO READ A BYTE INTO ADDRESS 1000			|
;|			AT CHIP ADDRESS 5							|
;|												|
;|			LDI	ACCH,5			;CHIP ADDRESS NUMBER 5			|
;|			LDI	ACCT,LOW(1000)		;ACCT:ACCTH=ADDRESS=1000		|
;|			LDI	ACCTH,HIGH(1000)						|
;|			RCALL	_AT24C64_BYTE_READ 	;AFTER THIS POINT ACC=DATA		|
;|----------------------------------------------------------------------------------------------|
#ifndef __AT24C64__
	
	#define __at24c64__
	
	.message "+---------+
	.message "| AT24C64 |
	.message "+---------+
	
	
	.EQU	_AT24C64_DEVICE_NUMBER	= 0XA
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   _AT24C64_BYTE_WRITE	  |
	;|  				  |
	;| 			          |
	;|   Input  : Acc   Data to write |
	;|\	      AccH  Device Address|
	;| |	      AccT  Address low	  |
	;|/	      AccTH Address high  |
	;|   Output : None	          |
	;|   Destroy: None	          |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	;		/
	;		| Device address format
	;		|	+---+---+---+---+---+---+---+---+
	;		|	| 1 | 0 | 1 | 0 | a2| a1| a0|r/w|
	;		|	+---+---+---+---+---+---+---+---+
	;		|
	;		| a0..a2 = Device address
	;		| r/w	 = 0 (write) 1 (read)
	;		|			 write
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	|s|1|0|1|0|a|a|a|0|A|*|*|*|a|a|a|a|a|A|a|a|a|a|a|a|a|a|A|d|d|d|d|d|d|d|d|A|s| 
	;		|	|t| | | | |2|1|0| |C| | | |1|1|1|9|8|C|7|6|5|4|3|2|1|0|C|7|6|5|4|3|2|1|0|C|t| 
	;		|	|t| | | | | | | | |K| | | |2|1|0| | |K| | | | | | | | |K| | | | | | | | |K|p| 
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	  \- Device Addr-/ 0 \- Addr high -/ 0 \- Addr low --/ 0 \--- Data ----/ 0
	;		|
	;		|stt = start,a=address 0..8191,d=data 0..255, stp=stop
	;		|write time at 4Mhz ~490uS
	;		\_______________________________________________________________________________________
	_AT24C64_BYTE_WRITE:
		push	AccH
		push	Acc
		rcall	_I2C_START				;Start I2C protocol
		lsl	AccH					;Place address in corretly position
		andi	AccH,0b00001110				;Set WRITE
		ori	AccH,(_AT24C64_DEVICE_NUMBER<<4)	;Add Device Number
		mov	Acc,AccH
		rcall	_I2C_BYTE_OUT				;Send Device Address
		rcall	_I2C_ACK_IN				;Read ACK
		mov	Acc,AccTH				;Send Address high
		rcall	_I2C_BYTE_OUT
		rcall	_I2C_ACK_IN				;Read ACK
		mov	Acc,AccT				;Send Address low
		rcall	_I2C_BYTE_OUT
		rcall	_I2C_ACK_IN				;Read ACK
		pop	Acc
		push	Acc
		rcall	_I2C_BYTE_OUT
		rcall	_I2C_ACK_IN				;Read ACK
		rcall	_I2C_STOP				;Stop I2C protocol
		ldi	Acc,low(10)				;Wait a hundredth a second
		ldi	AccH,high(10)
		rcall	_DELAY_MILESECONDS
		pop	Acc
		pop	AccH
		ret
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   _AT24C64_BYTE_READ		  |
	;|  				  |
	;| 			          |
	;|\  Input  : AccH  Device Address|
	;| |          AccT  Address low	  |
	;|/	      AccTH Address high  |
	;|   Output : Acc   Data read     |
	;|   Destroy: None	          |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	;		/
	;		|			 write					 		 read
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	|s|1|0|1|0|a|a|a|0|A|*|*|*|a|a|a|a|a|A|a|a|a|a|a|a|a|a|A|s|1|0|1|0|a|a|a|1|A|d|d|d|d|d|d|d|d|A|s| 
	;		|	|t| | | | |2|1|0| |C| | | |1|1|1|9|8|C|7|6|5|4|3|2|1|0|C|t| | | | |2|1|0| |C|7|6|5|4|3|2|1|0|C|t| 
	;		|	|t| | | | | | | | |K| | | |2|1|0| | |K| | | | | | | | |K|t| | | | | | | | |K| | | | | | | | |K|p| 
	;		|	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	;		|	  \- Device Addr-/ 0 \- Addr high -/ 0 \-- Addr low -/ 0  \- Device Addr-/ 0 \---- Data ---/ 1
	;		|
	;		|stt = start,a=address 0..8191,d=data 0..255, stp=stop
	;		|read time at 4Mhz ~610uS
	;		\_______________________________________________________________________________________
	_AT24C64_BYTE_READ:
		push	AccH
		rcall	_I2C_START				;Start I2C protocol
		lsl	AccH					;Place address in corretly position
		andi	AccH,0b00001110				;Set WRITE
		ori	AccH,(_AT24C64_DEVICE_NUMBER<<4)	;Add Device Number
		push	AccH					;Save device address
		mov	Acc,AccH
		rcall	_I2C_BYTE_OUT				;Send Device Address
		rcall	_I2C_ACK_IN				;Read ACK
		mov	Acc,AccTH				;Send Address high
		rcall	_I2C_BYTE_OUT
		rcall	_I2C_ACK_IN				;Read ACK
		mov	Acc,AccT				;Send Address low
		rcall	_I2C_BYTE_OUT
		rcall	_I2C_ACK_IN				;Read ACK
		rcall	_I2C_START				;Start I2C protocol AGAIN
		pop	Acc					;Restore Device Address
		ori	Acc,0b00000001				;Set Read now
		rcall	_I2C_BYTE_OUT				;Send Device Adress Again
		rcall	_I2C_ACK_IN				;Read Ack
		rcall	_I2C_BYTE_IN				;Read Byte
		mov	AccH,Acc				;AccH=Byte read
		rcall	_I2C_ACK_IN				;Read ACK
		rcall	_I2C_STOP				;Stop I2C protocol
		mov	Acc,AccH
		pop	AccH
		ret
#endif
.exit

