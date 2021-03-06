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
;+--------------------------------------------------------------+---+---+
;|									|
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA					|
;| DATA:13/04/2000							|
;>----------------------------------------------------------------------<
;| E E P R O M  R O U T I N E S						|
;| F O R  A V R  C H I P S  W I T H  2 5 6  B Y T E S  O R  L E S S 	|
;+----------------------------------------------------------------------+
#ifndef __EEPROM_BE256__
	#define __EEPROM_BE256__
	.message "[ (EEPROM) BE256 ]"
	;>------------------------------------------------------<
	;| I M P L E M E N T E D  F U N C T I O N S 		|
	;+----------------------------------------------------------------------------------------------+
	;| _EEPROM_WRITE	WRITE DATA INTO EEPROM							|
	;|			SAMPLE USING EEPROM_WRITE TO WRITE DATA 0X33 INTO ADDRESS 0X14		|
	;|												|
	;|			LDI	ACC,0X33							|
	;|			LDI	ACCH,LOW(0X14)							|
	;|			RCALL	EEPROM_WRITE							|
	;|----------------------------------------------------------------------------------------------|												|
	;| _EEPROM_READ		READ DATA FROM EEPROM							|
	;|			SAMPLE USING EEPROM_READ TO READ A DATA FROM A ADDRESS 0X14		|
	;|												|
	;|			LDI	ACCH,LOW(0X14)							|
	;|			RCALL	EEPROM_READ	;AFTER THIS ACC CONTENTS HAVE A DATA READ	|
	;+----------------------------------------------------------------------------------------------+
	;+--------------------------+
	;| WRITE DATA INTO EEPROM   |
	;|			    |
	;| input: Acc  Data	    |
	;| 	  AccH Address low  |
	;+--------------------------+
	_EEPROM_WRITE:
		sbic	EECR,EEWE			;wait last write operation is done
		rjmp	_EEPROM_WRITE			;wait
		out	EEARL,AccH			;set address Low
		out	EEDR,Acc			;set data
		sbi	EECR,EEMWE  			;request eeprom write
		sbi	EECR,EEWE			;place write command
		ret	
	;+--------------------------+
	;| READ DATA FROM EEPROM    |
	;|			    |
	;| input: AccH Address low  |
	;| output: Acc data	    |
	;+--------------------------+
	_EEPROM_READ:
		sbic	EECR,EEWE			;wait last write operation is done
		rjmp	_EEPROM_READ			;wait
		out	EEARL,AccH			;set address low
		sbi	EECR,EERE			;place read command
		sbi	EECR,EERE			;place read command
		in	Acc,EEDR			;get data
		ret
#endif
.exit

