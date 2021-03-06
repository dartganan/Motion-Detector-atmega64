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
;+--------------------------------------------------------------+---+
;+------------------------------------------------------+
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA			|
;| DATA: 10/04/2000					|
;>------------------------------------------------------<
;| S R A M  I N I T I A L I Z A T I O N			|
;+------------------------------------------------------+
#ifndef __SRAM_INITIALIZATION__
	
	#define __SRAM_INITIALIZATION__
	
	.message "+---------------------+"
	.message "|                     |"
	.message "| SRAM INITIALIZATION |"
	.message "|                     |"
	.message "+---------------------+"
	;+----------------------
	;| INITIALIZE SRAM
	;+----------------------
	
		ldi	Acc,_SRAM_BOOT_TYPE	;see if need to clear sram
		cpi	Acc,_SRAM_NOT_CLEAR	;not clear SRAM ?
		breq	_VECT_INIT		;yes, branch
		ldi	ZL,low(RAMSTART)			;sram start address
		ldi	ZH,high(RAMSTART)
		ldi	Acc,low(RAMEND-RAMSTART+1)		;num bytes to erase
		ldi	AccH,high(RAMEND-RAMSTART+1)
		clr	AccT
	_SRAM_I00:
		st	Z+,AccT			;clear SRAM location
		subi	Acc,1
		sbci	AccH,0
		brne	_SRAM_I00		;until all sram locations cleaded
	;+----------------------
	;| INITIALIZE VECTORS
	;+----------------------
	_VECT_INIT:
		ldi	ZL,low(_HDC_INT0_VECT)	;Z-->First vector
		ldi	ZH,high(_HDC_INT0_VECT)
		ldi	AccT,low(_HDC_RETURN)
		ldi	AccTH,high(_HDC_RETURN)
		ldi	Acc,(_HDC_END-_HDC_START)/2
	_VECT00:
		st	Z+,AccT			;store return address of each handdle vector
		st	Z+,AccTH
		dec	Acc
		brne	_VECT00
	#ifdef	__MATH_DEFINITIONS__
		;+------------------------------+
		;| INITIALIZA MATH VARIABLES	|
		;+------------------------------+
		_DFINIT:
			push	Acc			;Save useds
			clr	Acc			;set normal float point division 
			sts	_DF_FRAC,Acc
			pop	Acc
	#endif
	nop
#endif
.exit

