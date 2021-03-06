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
;>----------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     				|
;| I N T E G E R  T O  S T R I N G  W I T H  F O R M A T T I N G	|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;+----------------------------------------------------------------------+
#ifndef	__INTEGER_IXTOS__
	#define __INTEGER_IXTOS__
	.message "[ (MATH) INTEGER IXTOS ]"
;+----------------------------------------------------------------------+
;| SET SIZE OF FORMATTED STRING FOR CONVERSION ROUTINES			|
;| Input  : Acc String Formatted Size					|
;| Output : None							|
;| Destroy: None							|
;| Obs: Max size is defined by _DF_STR_SIZE-1 and above value		|
;| 	assume de max size						|
;+----------------------------------------------------------------------+
_I_USE_SIZE:
	cpi	Acc,_DF_STR_BUF_SIZE-1			;>BUFFER_SIZE-1 ?
	brlo	_ISS00					;no, branch
	ldi	Acc,_DF_STR_BUF_SIZE-1			;else set BUFFER_SIZE-1
_ISS00:
	sts	_IS_SIZE,Acc				;store new size 
	ret
;+--------------------------------------------------------------+
;| SET USE PLUS SIGN SEPARATOR FOR POSITIVE NUMBERS		|
;| Input  : Acc =1 use =0 no use				|
;| Output : None						|
;| Destroy: None						|
;+--------------------------------------------------------------+
_I_USE_PLUS:
	push	Acc
	cpi	Acc,_PLUS_SIGN				;use plus sign ?
	brne	_I_USE_P00				;no, branch
	lds	Acc,_IS_MASK				;get mask
	ori	Acc,(1<<_IS_MASK_BIT_SIGNAL)		;use plus sign
_I_USE_P_EXIT:
	sts	_IS_MASK,Acc
	pop	Acc
	ret
_I_USE_P00:
	lds	Acc,_IS_MASK				;get mask	
	andi	Acc,~(1<<_IS_MASK_BIT_SIGNAL)		;no use plus
	rjmp	_I_USE_P_EXIT	
;+--------------------------------------------------------------+
;| SET USE THOUSAND SEPARATOR					|
;| Input  : Acc =1 use =0 no use				|
;| Output : None						|
;| Destroy: None						|
;+--------------------------------------------------------------+
_I_USE_THOUSAND:
	push	Acc
	cpi	Acc,_THOUSAND_SEPARATOR			;use thousand separator ?
	brne	_I_USE_T00				;no, branch
	lds	Acc,_IS_MASK				;get mask
	ori	Acc,(1<<_IS_MASK_BIT_THOUSAND)		;use thousand separator
_I_USE_T_EXIT:
	sts	_IS_MASK,Acc
	pop	Acc
	ret
_I_USE_T00:
	lds	Acc,_IS_MASK				;get mask	
	andi	Acc,~(1<<_IS_MASK_BIT_THOUSAND)		;no use thousand separator
	rjmp	_I_USE_T_EXIT	
;+--------------------------------------------------------------+
;| SET COMMA OR POINT FOR THOUSAND SEPARATOR			|
;| Input  : Acc =1 point =0 comma				|
;| Output : None						|
;| Destroy: None						|
;+--------------------------------------------------------------+
_I_USE_SEP_CHAR:
	push	Acc
	cpi	Acc,_POINT_CHARACTER			;use point as separator ?
	brne	_I_USE_S00				;no, branch
	lds	Acc,_IS_MASK				;get mask
	ori	Acc,(1<<_IS_MASK_BIT_SEPCHAR)		;use point as separator
_I_USE_S_EXIT:
	sts	_IS_MASK,Acc
	pop	Acc
	ret
_I_USE_S00:
	lds	Acc,_IS_MASK				;get mask	
	andi	Acc,~(1<<_IS_MASK_BIT_SEPCHAR)		;use comma as separator
	rjmp	_I_USE_S_EXIT	
;+---------------------------------------------------+
;| CONVERT LONG INTEGER UNSIGNED TO STRING 	     |
;|                                                   |
;| Input  : Acc:AccH:AccT:AccTH unsigned long integer|
;| Output : _DF_STR ASCII string number		     |
;| Destroy: Flags                           	     |
;+---------------------------------------------------+
_ILTOSC:
	push	r0					;save useds
	push	r1
	push	Acc
	push	AccH
	push	AccT
	push	AccTH
	push	XL
	push	XH
	push	YL
	push	YH
	ldi	ZL,LOW(_I_TB_PW10*2)			;Z-->POWER 10 TABLE
	ldi	ZH,HIGH(_I_TB_PW10*2)
	clr	r1					;digit counter
_ILTOSC00:
	lpm
	mov	XL,r0					;Y:X = POWER of 10
	adiw	ZL,1
	lpm
	mov	XH,r0
	adiw	ZL,1
	lpm
	mov	YL,r0
	adiw	ZL,1
	lpm
	mov	YH,r0
	adiw	ZL,1
	clr	r0					;power counter
_ILTOSC10:
	sub	Acc,XL					;N=N-POWER
	sbc	AccH,XH
	sbc	AccT,YL
	sbc	AccTH,YH
	brcs	_ILTOSC20				;<0 ?, yes branch
	inc	r0					;power counter ++
	rjmp	_ILTOSC10
_ILTOSC20:
	push	ZL					;save Z register
	push	ZH
	push	Acc
	ldi	ZL,LOW(_DF_STR+_DF_STR_BUF_SIZE-11)	;Z-->string buffer (space to hold string numbers + 0 terminator)
	ldi	ZH,HIGH(_DF_STR+_DF_STR_BUF_SIZE-11)
	add	ZL,r1
	clr	Acc
	adc	ZH,Acc
	mov	Acc,r0					;Acc=ASCII r0
	ori	Acc,0x30
	st	Z,Acc					;store power
	inc	r1					;point to next digit
	pop	Acc
	pop	ZH
	pop	ZL
	add	Acc,XL					;N=N+POWER restore number
	adc	AccH,XH
	adc	AccT,YL
	adc	AccTH,YH
	cpi	XL,1					;last power ?
	brne	_ILTOSC00				;no, get next power
	ldiw	Z,_DF_STR
	ldi	Acc,_DF_STR_BUF_SIZE-11			;put space in front of number
	ldi	AccH,' '
_ILTOSC30:
	st	Z+,AccH
	dec	Acc
	brne	_ILTOSC30
	clr	Acc					;zero at end of number
	sts	_DF_STR+_DF_STR_BUF_SIZE-1,Acc
	pop	YH
	pop	YL
	pop	XH
	pop	XL
	pop	AccTH
	pop	AccT
	pop	AccH
	pop	Acc
	pop	r1
	pop	r0
	ret	
;+---------------------------------------------------+
;| INSERT THOUSANDS SEPARATOR CHARACTER IF NEED      |
;|                                                   |
;| Input  : None				     |
;| Output : _DF_STR number with thousands separa-    |
;|	               tion character according      |
;|		       _IS_MASK_BIT_THOUSAND	     |
;| Destroy: Flags                           	     |
;+---------------------------------------------------+
_IS_THOUSANDS:
	push	Acc
	lds	Acc,_IS_MASK				;see actual mask
	andi	Acc,(1<<_IS_MASK_BIT_THOUSAND)		;need to put thousand separator ?
	lbreq	_IS_TH20				;no, branch
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-11		;insert separator
	sts	_DF_STR+_DF_STR_BUF_SIZE-14,Acc		;billion
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-10		;million
	sts	_DF_STR+_DF_STR_BUF_SIZE-12,Acc
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-9
	sts	_DF_STR+_DF_STR_BUF_SIZE-11,Acc
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-8
	sts	_DF_STR+_DF_STR_BUF_SIZE-10,Acc
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-7		;thousand
	sts	_DF_STR+_DF_STR_BUF_SIZE-8,Acc
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-6
	sts	_DF_STR+_DF_STR_BUF_SIZE-7,Acc
	lds	Acc,_DF_STR+_DF_STR_BUF_SIZE-5
	sts	_DF_STR+_DF_STR_BUF_SIZE-6,Acc
	lds	Acc,_IS_MASK				;see separator character 
	andi	Acc,(1<<_IS_MASK_BIT_SEPCHAR)
	ldi	Acc,','					;assume separator character (comma)
	breq	_IS_TH10				;comma ? ok branch
	ldi	Acc,'.'					;else separator character (point)
_IS_TH10:
	sts	_DF_STR+_DF_STR_BUF_SIZE-5,Acc
	sts	_DF_STR+_DF_STR_BUF_SIZE-9,Acc
	sts	_DF_STR+_DF_STR_BUF_SIZE-13,Acc
_IS_TH20:
	pop	Acc
	ret
;+-----------------------------------------------------+
;| ZERO LEFT PROCESS				       |
;| Fill with space zero digits on left at the number   |
;| Input  : None				       |
;| Output : _DF_STR number with a zero left process    |
;| Destroy: Flags                           	       |
;+-----------------------------------------------------+
_IS_ZERO_LEFT:
	push	Acc					;save useds
	push	AccH
	push	ZL
	push	ZH
	ldi	ZL,low(_DF_STR)				;Z-->IS_BUFFER
	ldi	ZH,high(_DF_STR)
	ldi	AccH,_DF_STR_BUF_SIZE-2			;maximum digits to verify
_IS_ZERO_L00:
	ld	Acc,Z					;see if zero , comma , point or space
	cpi	Acc,'0'					;is zero ?
	breq	_IS_ZERO_L10				;yes, branch
	cpi	Acc,','					;is comma ?
	breq	_IS_ZERO_L10				;yes, branch
	cpi	Acc,'.'					;is point ?
	breq	_IS_ZERO_L10				;yes, branch
	cpi	Acc,' '					;is space ?
	breq	_IS_ZERO_L10				;yes, branch
	rjmp	_IS_ZERO_DONE				;other character then done
_IS_ZERO_L10:
	ldi	Acc,' '					;put space into IS_BUFFER
	st	Z+,Acc
_IS_ZERO_L20:
	dec	AccH					;maximum digit count--
	brne	_IS_ZERO_L00				;until all done
_IS_ZERO_DONE:
	pop	ZH					;restore useds
	pop	ZL
	pop	AccH
	pop	Acc
	ret
;+--------------------------------------------------------------+
;| FILL * IF _DF_STR > _IS_SIZE		       		|
;| Input  : None				       		|
;| Output : Z --> formatted string entry point to display  	|
;|		  automatic string output size			|
;|	    _DF_STR Filled with * if overflow       		|
;| Destroy: Flags                           	       		|
;| obs: use this function only after IS_ZERO_LEFT      		|	
;+--------------------------------------------------------------+
_IS_OVERFLOW:
	push	Acc					;save useds
	push	AccH
	ldi	ZL,LOW(_DF_STR)				;Z-->_DF_STR
	ldi	ZH,HIGH(_DF_STR)
	ldi	AccH,_DF_STR_BUF_SIZE-2			;AccH=BUFFER SIZE
_IS_OVER00:	
	ld	Acc,Z+					;get character
	cpi	Acc,' '					;space ?
	brne	_IS_OVER10				;no, branch
	dec	AccH					;SIZE--
	rjmp	_IS_OVER00
_IS_OVER10:
	lds	Acc,_IS_SIZE				;see if AccH<IS_SIZE
	cp	AccH,Acc
	brlo	_IS_OVER40				;yes, overflow				
	ldi	AccH,'*'
	tst	Acc					;filled ?
	push	ZL
	push	ZH
	ldi	ZL,low(_DF_STR+_DF_STR_BUF_SIZE-1)	;Z-->End of IS_BUFFER-1
	ldi	ZH,high(_DF_STR+_DF_STR_BUF_SIZE-1)
_IS_OVER20:
	breq	_IS_OVER30
	st	-Z,AccH					;put *	
	dec	Acc
	rjmp	_IS_OVER20
_IS_OVER30:
	pop	ZH	
	pop	ZL
_IS_OVER40:
	sbiw	ZL,1
	pop	AccH
	pop	Acc
	ret
;+------------------------------------------------------+
;| INSERT SIGNAL				        |
;|                                                      |
;| Input  : CY 1=negative 0 positive		        |
;| Output : _DF_STR number with a sign according     	|
;|	    	       _IS_MASK_BIT_SIGNAL	        |
;| Destroy: Flags                           	        |
;| obs: uses ZERO_LEFT function before use this routine |
;+------------------------------------------------------+
_IS_SIGNAL:
	push	Acc					;save useds
	push	AccH
	push	ZL
	push	ZH
	lds	Acc,_IS_MASK				;see positive signal char
	andi	Acc,(1<<_IS_MASK_BIT_SIGNAL)
	ldi	Acc,' '					;assume space a positive number
	breq	_IS_S00					;branch if setted
	ldi	Acc,'+'					;else signal char is PLUS +
_IS_S00:
	brcc	_IS_S10					;positive number branch
	ldi	Acc,'-'					;else put a negative number
_IS_S10:
	ldi	ZL,LOW(_DF_STR)				;Find first no space char
	ldi	ZH,HIGH(_DF_STR)
_IS_S20:
	ld	AccH,Z+					;get character
	cpi	AccH,' '				;space ?
	brne	_IS_S30					;no, branch
	rjmp	_IS_S20					;else get next character
_IS_S30:
	sbiw	ZL,2					;put signal into correctly position
	st	Z,Acc					;put signal
	pop	ZH					;restore useds
	pop	ZL
	pop	AccH
	pop	Acc
	ret
;+--------------------------------------------------------------+
;| IUBTOS - Integer Unsigned Byte to String Automatic Length	|
;| Input  : Acc  Unsigned byte					|
;| Output : Z--> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IUBTOS:
	push	AccH					;save useds
	clr	AccH
	rcall	_ITOS					;integer signed to string
	pop	AccH
	ret
;+--------------------------------------------------------------+
;| IBTOS - Integer Signed Byte to String Automatic Length	|
;| Input  : Acc  Signed byte					|
;| Output : Z--> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IBTOS:
	push	Acc					;save number and useds
	push	AccH
	push	AccT
	push	AccTH
	push	Acc					;save signal	
	tst	Acc					;see if positive 
	brpl	_IBTOS00				;yes, branch
	neg	Acc					;else two complement Acc
_IBTOS00:
	clr	AccH					;clear the rest
	clr	AccT
	clr	AccTH
_ICONV:
	rcall	_ILTOSC					;Convert to ASCII and put into _DF_STR
	rcall	_IS_THOUSANDS				;see if need thousands separator
	rcall	_IS_ZERO_LEFT				;process zero left
	pop	Acc					;restore signal
	clc						;assume a positive number
	tst	Acc					;really positive ?
	brpl	_IBTOS20				;yes, branch
	sec						;else set negative
_IBTOS20:
	rcall	_IS_SIGNAL				;insert signal
	lds	Acc,_IS_SIZE				;get actual size
	push	Acc					;save it
	ldi	Acc,_DF_STR_BUF_SIZE-1			;set max size
	sts	_IS_SIZE,Acc
	rcall	_IS_OVERFLOW				;see overflow and get Z pointer of string
	pop	Acc					;restore size
	sts	_IS_SIZE,Acc
	pop	AccTH					;restore useds
	pop	AccT
	pop	AccH
	pop	Acc					;restore number
	ret
;+--------------------------------------------------------------+
;| IBTOSF - Integer Signed Byte to String Formatted Length	|
;| Input  : Acc  Signed byte					|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IBTOSF:
	rcall	_IBTOS					;call same but unformatted size routine
_I_GET_FP:
	rcall	_IS_OVERFLOW				;fill with * if overflow
	push	Acc
	ldi	ZL,low(_DF_STR+_DF_STR_BUF_SIZE-1)	;point to end of IS_BUFFER
	ldi	ZH,high(_DF_STR+_DF_STR_BUF_SIZE-1)
	lds	Acc,_IS_SIZE				;get actual size
	sub	ZL,Acc					;subtract of Z to obtain correct address
	ldi	Acc,0
	sbc	ZH,Acc
	pop	Acc
	ret
;+--------------------------------------------------------------+
;| IUBTOSF - Integer Signed Byte to String Formatted Length	|
;| Input  : Acc  Unsiged byte					|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IUBTOSF:
	rcall	_IUBTOS					;call same but unformatted size routine
	rjmp	_I_GET_FP				;see overflow condition and get Z pointer
;+--------------------------------------------------------------+
;| IUTOS - Integer Unsigned Word to String Automatic Length	|
;| Input  : Acc:AccH  Unsigned Word				|
;| Output : Z--> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IUTOS:
	push	AccT					;save useds
	push	AccTH
	clr	AccT
	clr	AccTH
	rcall	_ILTOS					;integer signed long to string
	pop	AccTH
	pop	AccT
	ret
;+--------------------------------------------------------------+
;| ITOS - Integer Signed Word to String Automatic Length	|
;| Input  : Acc:AccH  Signed word 				|
;| Output : Z --> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_ITOS:
	push	Acc					;save number and useds
	push	AccH
	push	AccT
	push	AccTH
	push	AccH					;save signal	
	tst	AccH					;see if positive 
	brpl	_ITOS00					;yes, branch
	com	Acc					;�lse two complement Acc:AccH
	com	AccH
	subi	Acc,-1
	sbci	AccH,-1
_ITOS00:
	clr	AccT					;clear the rest
	clr	AccTH
	rjmp	_ICONV					;start conversion
;+--------------------------------------------------------------+
;| ITOSF - Integer Signed Word to String Formatted Length	|
;| Input  : Acc:AccH  Signed Word				|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_ITOSF:
	rcall	_ITOS					;call same but unformatted size routine
	rjmp	_I_GET_FP				;see overflow condition and get Z pointer
;+--------------------------------------------------------------+
;| IUTOSF - Integer Unsigned Word to String Formatted Length	|
;| Input  : Acc:AccH  Unsigned Word				|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IUTOSF:
	rcall	_IUTOS					;call same but unformatted size routine
	rjmp	_I_GET_FP				;see overflow condition and get Z pointer
;+--------------------------------------------------------------+
;| ILTOS - Integer unsigned Long to String Automatic Length	|
;| Input  : Acc:AccH:AccT:AccTH  Signed long			|
;| Output : Z --> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IULTOS:
	push	Acc					;save number and useds
	push	AccH
	push	AccT
	push	AccTH
	rcall	_ILTOSC					;Convert to ASCII and put into _DF_STR
	rcall	_IS_THOUSANDS				;see if need thousands separator
	rcall	_IS_ZERO_LEFT				;process zero left
	clc						;always positive number
	rjmp	_IBTOS20				;start conversion 
;+--------------------------------------------------------------+
;| ILTOS - Integer Signed Long to String Automatic Length	|
;| Input  : Acc:AccH:AccT:AccTH  Signed long			|
;| Output : Z --> Formatted Automatic Length			|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_ILTOS:
	push	Acc					;save number and useds
	push	AccH
	push	AccT
	push	AccTH
	push	AccTH					;save signal	
	tst	AccTH					;see if positive 
	brpl	_ILTOS00				;yes, branch
	com	Acc					;�lse two complement Acc:AccH:AccT:AccTH
	com	AccH
	com	AccT
	com	AccTH
	subi	Acc,-1
	sbci	AccH,-1
	sbci	AccT,-1
	sbci	AccTH,-1
_ILTOS00:
	rjmp	_ICONV					;start conversion
;+--------------------------------------------------------------+
;| ILTOSF - Integer Signed Long to String Formatted Length	|
;| Input  : Acc:AccH:AccT:AccTH  Signed Long			|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_ILTOSF:
	rcall	_ILTOS					;call same but unformatted size routine
	rjmp	_I_GET_FP				;see overflow condition and get Z pointer
;+--------------------------------------------------------------+
;| IULTOSF - Integer Unsigned Long to String Formatted Length	|
;| Input  : Acc:AccH:AccT:AccTH  Unsigned Long			|
;| Output : Z--> Formatted Length				|
;| Destroy: Flags						|
;+--------------------------------------------------------------+
_IULTOSF:
	rcall	_IULTOS					;call same but unformatted size routine
	rjmp	_I_GET_FP				;see overflow condition and get Z pointer
		
;+---------------------------------------------------+
;| POWER OF 10 TABLE USED IN INTEGER CONVERSIONS     |
;+---------------------------------------------------+
_I_TB_PW10:
	.DW	LWRD(1000000000),HWRD(1000000000)
	.DW	LWRD(100000000)	,HWRD(100000000)
	.DW	LWRD(10000000)	,HWRD(10000000)
	.DW	LWRD(1000000) 	,HWRD(1000000)
	.DW	LWRD(100000)	,HWRD(100000)
	.DW	LWRD(10000)	,HWRD(10000)
	.DW	LWRD(1000)	,HWRD(1000)
	.DW	LWRD(100)	,HWRD(100)
	.DW	LWRD(10)	,HWRD(10)
	.DW	LWRD(1)		,HWRD(1)
#endif
.exit

