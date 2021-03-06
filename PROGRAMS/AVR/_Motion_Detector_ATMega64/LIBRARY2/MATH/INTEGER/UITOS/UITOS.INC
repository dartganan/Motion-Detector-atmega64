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
;+--------------------------------------------------------------+---+-----------+
;|										|
;| DATE:17/12/2001 VERSION 1.0							|
;>------------------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     					|
;| U N S I G N E D  I N T E G E R  T O  S T R I N G  				|
;+------------------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM				|
;+------------------------------------------------------------------------------+
#ifndef	__INTEGER_UITOS__
	#define __INTEGER_UITOS__
	.message "[ (MATH) INTEGER UITOS ]"
;+----------------------------------------------------------------------+
;| _UITOS								|
;| Convert unsigned word to string ASCII 			 	|
;| with left zero removal end 0 terminator				|
;|								  	|
;| Input  : AccH:Acc unsigned word input			  	|
;| Output : Z--> SRAM string output with 0			  	|
;+----------------------------------------------------------------------+
	_UITOS:
		push	Acc					;save useds
		push	AccH
		push	AccT
		push	AccTH
		push	Temp
		ldi	ZL,low(_DF_STR)
		ldi	ZH,high(_DF_STR)
		ldi	AccT,'+'				;assume positive
		st	Z+,AccT
		ldi	Temp,low(10000)				;load constant
		ldi	AccTH,high(10000)
		rcall	_UITOS50				;find 1o digit
		ldi	Temp,low(1000)				;load constant
		ldi	AccTH,high(1000)
		rcall	_UITOS50				;find 2o digit
		ldi	Temp,low(100)				;load constant
		ldi	AccTH,high(100)
		rcall	_UITOS50				;find 3o digit
		ldi	Temp,low(10)				;load constant
		ldi	AccTH,high(10)
		rcall	_UITOS50				;find 4o digit
		ori	Acc,0x30				;convert last digit to ascii
		st	Z+,Acc					;store lst digit
		clr	Acc
		st	Z+,Acc					;end terminated
		ldi	ZL,low(_DF_STR)
		ldi	ZH,high(_DF_STR)
		ldi	Acc,4
	_UITOS20:
		adiw	ZL,1
		ld	Acch,Z
		cpi	Acch,'0'				;zero ?
		brne	_UITOS30				;branch if not
		dec	Acc
		brne	_UITOS20
		adiw	ZL,1					;put into correct position
	_UITOS30:
		pop	Temp
		pop	AccTH
		pop	AccT
		pop	AccH					;restore useds
	_UITOS40:
		pop	Acc
		ret
	;+---------------------
	;| get digit
	;+---------------------
	_UITOS50:
		ldi	AccT,'0'				;load first digit
	_UITOS60:
		sub	Acc,Temp				;subtract by 10 power factor
		sbc	AccH,AccTH
		brcs	_UITOS70				;found ? branch
		inc	AccT					;digit count
		rjmp	_UITOS60
	_UITOS70:
		st	Z+,AccT					;store ASCII Digit
		add	Acc,Temp				;restore original number
		adc	AccH,AccTH
		ret
;+----------------------------------------------------------------------+
;| _UITOSU								|
;| Convert unsigned word to string ASCII 			 	|
;| fixed size 6 using following format +nnnnn/0	 			|
;|								  	|
;| Input  : AccH:Acc unsigned word input			  	|
;| Output : Z--> SRAM string output with 0			  	|
;+----------------------------------------------------------------------+
	_UITOSU:
		push	Acc					;save useds
		rcall	_UITOS					;normal conversion
	_UITOSU_10:
		ld	Acc,Z					;'+'?
		cpi	Acc,'+'
		breq	_UITOSU_20				;yes, branch
		sbiw	ZL,1
		rjmp	_UITOSU_10				;until find '+'
	_UITOSU_20:
		pop	Acc
		ret	
;+----------------------------------------------------------------------+
;| _UITOSUS								|
;| Convert unsigned byte to string ASCII with fixed size 6 chars using 	|
;| following format +nnnnn/0 and fill left zeros with space		|
;|								  	|
;| Input  : Acc unsigned byte input				  	|
;| Output : Z--> SRAM string output with 0			  	|
;+----------------------------------------------------------------------+
	_UITOSUS:
		push	Acc					;save useds
		rcall	_UITOS					;normal conversion
		ldd	Acc,Z+1					;check if next byte is 0 terminator?
		cpi	Acc,0
		brne	_UITOSUS_10				;no, branch
		sbiw	ZL,1					;else Z--
	_UITOSUS_10:
		ld	Acc,Z					;'+'?
		cpi	Acc,'+'
		breq	_UITOSUS_30				;yes, branch
		cpi	Acc,'0'					;zero ?
		brne	_UITOSUS_20
		ldi	Acc,' '					;plase space instead zero
		st	Z,Acc
	_UITOSUS_20:
		sbiw	ZL,1
		rjmp	_UITOSUS_10				;until find '+'
	_UITOSUS_30:
		pop	Acc
		ret	
#endif
.exit

