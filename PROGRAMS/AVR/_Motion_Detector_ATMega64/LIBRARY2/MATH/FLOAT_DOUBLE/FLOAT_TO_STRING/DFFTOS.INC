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
;| DATE:24/05/2001 VERSION 1.0						|
;>--------------------------------------------------------------+-------<
;| D O U B L E   F L O A T  P O I N T  R O U T I N E S	     	|
;| F L O A T  T O  S T R I N G  			 	|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;+---------------------------------------------------+
;| CONVERT DOUBLE FLOAT NUMBER TO STRING NUMBER AT   |
;| SCIENTIFIC NOTATION FORMAT			     |
;|                                                   |
;| Input  : Floating accumulator		     |
;| Output : _DF_STR number in ASCII scientific format|
;| Destroy: Acc,AccH,Flags                           |
;+---------------------------------------------------+
#ifndef __MATH_FLOAT_TO_STRING__
	#define	__MATH_FLOAT_TO_STRING__
	.message "[ (MATH) FLOAT TO STRING ]"
_DFFTOS:
	push	ZL					;save useds
	push	ZH
	push	YL
	push	YH
	push	Acc
	push	AccH
	ldi	ZL,low(_DF_BCD)				;point to bcd number
	ldi	ZH,high(_DF_BCD)
	ldi	YL,low(_DF_STR)				;point to output ASCII string
	ldi	YH,high(_DF_STR)
	ldd	Acc,Z+7					;see mantisa signal
	andi	Acc,0x80
	ldi	AccH,'+'				;assume plus sign
	breq	_DFFTOS00				;positive? branch
	ldi	AccH,'-'				;else turn negative
_DFFTOS00:
	st	Y+,AccH					;store signal
	ldi	AccH,7					;get digits in BCD and store in ASCII
_DFFTOS10:
	ld	Acc,Z+					;get bcd digit
	push	Acc					;save 
	swap	Acc					;high nibble first
	andi	Acc,0x0f				;mask digit
	ori	Acc,0x30				;convert to ascii
	st	Y+,Acc
	cpi	AccH,7					;first digit
	brne	_DFFTOS15
	lds	Acc,_DF_FMASK				;set decimal separator comma or point
	andi	Acc,(1<<2)
	ldi	Acc,','					;assume comma
	breq	_DFFTOS13
	ldi	Acc,'.'					;else point
_DFFTOS13:
	st	Y+,Acc					;set
_DFFTOS15:
	pop	Acc					;restore low nibble
	andi	Acc,0x0f				;mask digit
	ori	Acc,0x30				;convert to ascii
	st	Y+,Acc
	dec	AccH					;next two nibbles
	brne	_DFFTOS10
	ldi	Acc,'E'					;exponent char marc 'E'
	st	Y+,Acc
	ld	Acc,Z					;see exponent signal
	andi	Acc,0x40
	ldi	AccH,'+'				;assume plus sign
	breq	_DFFTOS20				;positive? branch
	ldi	AccH,'-'				;else turn negative
_DFFTOS20:
	st	Y+,AccH					;store exponent signal
	ld	Acc,Z+					;get bcd digit
	push	Acc					;save 
	swap	Acc					;high nibble first
	andi	Acc,0x03				;mask digit
	ori	Acc,0x30				;convert to ascii
	st	Y+,Acc
	pop	Acc					;restore low nibble
	andi	Acc,0x0f				;mask digit
	ori	Acc,0x30				;convert to ascii
	st	Y+,Acc
	clr	Acc					;zero terminate string
	st	Y+,Acc
	pop	AccH					;restore useds
	pop	Acc
	pop	YH
	pop	YL
	pop	ZH
	pop	ZL
	ret
#endif
.exit
	

