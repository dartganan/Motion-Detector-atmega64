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
;| DATA:16/11/2000						|
;>--------------------------------------------------------------<
;| D F T  6 4  P O I N T S  8  B I T S				|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N  		|
;+------------------------------------------------------+---------------------------------------+
;|												|
;| _DFT_64B		Calculate DFT for a frequency index					|
;|												|
;|			Steps to use this routine						|
;|												|
;|			Fill _DFT_DATA_BUF with 64 data into SRAM at sample frequency FS	|
;|			after this set Acc with index of frequency to get amplitute 		|
;|			into Acc:AccH								|
;|												|
;|			Example of use:								|
;|												|
;|			supose that FS=6000Hz  FIndex0 =FS/64		<- Min Frequency	|
;|					       FIndex31=FIndex0*32	<- Max Frequency	|
;|												|
;|			FIndex0 = 93.75Hz ... FIndex31=3000.00					|
;|												|
;|			Now, if i need to obtain amplitude of Frequency 750Hz			|
;|												|
;|			ldi	Acc,8		;FIndex8=int(750/FIndex0)			|
;|			rcall	_DDFTB		;After this point Acc:AccH have a Amplitude of  |
;|						;Index Frequency = 750Hz			|
;+----------------------------------------------------------------------------------------------+
;+---------------------------------+-------------------------------------------------------------------------------------------------------
;| 64 Entry Points Sine Table Byte |
;+---------------------------------+
_DFT_SIN_TABLE_B:
.db  0x00,0x0C,0x19,0x25,0x31,0x3C,0x47,0x51
.db  0x5A,0x62,0x6A,0x70,0x75,0x7A,0x7D,0x7E
.db  0x7F,0x7E,0x7D,0x7A,0x75,0x70,0x6A,0x62
.db  0x5A,0x51,0x47,0x3C,0x31,0x25,0x19,0x0C
.db  0x00,0xF4,0xE7,0xDB,0xCF,0xC4,0xB9,0xAF
.db  0xA6,0x9E,0x96,0x90,0x8B,0x86,0x83,0x82
.db  0x81,0x82,0x83,0x86,0x8B,0x90,0x96,0x9E
.db  0xA6,0xAF,0xB9,0xC4,0xCF,0xDB,0xE7,0xF4
;+-------------------------------------------------------------------+---------------------------------------------------------------------
;| Calculate one point DFT with 8 bits accuracy	64 points	     |	
;|								     |
;| The DFT for one point frequency is take according following       |
;| formula							     |
;|								     |
;|      | 63			   |   | 63			  |  |
;|	| --			   |   | --			  |  |
;|	| \  			   |   | \  			  |  |
;|  v =	| /  dt(i)*sin(2*pi*f*i/64)| + | /  dt(i)*cos(2*pi*f*i/64)|  |
;|	| --			   |   | --			  |  |
;|	| i=0			   |   | i=0			  |  |
;|      \--- s=sine accumulator --/    \-- c=co-sine accumulator -/  |
;|								     |
;| note that correct vectored sum are performed as follow	     |
;|								     |
;|	     --------						     |
;|	    / 2	  2 						     |
;|	c=|/a  + b	<- more complex to perform and calculate     |
;|								     |
;| but due to speed matters the equation it transforms in 	     |
;|								     |
;|      c=|a|+|b|	<- less complex and less time need	     |
;|								     |
;| doing this transformation a mistake is introduced for a factor of |
;| 1.414 that is soothed by the dynamics of input signal dt(i)  and  |
;| larger values of output function.			     	     |
;|								     |
;| Input  : Acc      Frequency index 1..32			     |
;| Output : Acc:AccH Output Frequency Power			     |
;|		     value 2030 when signal at 0o		     |
;|		     value 2870 when signal 45o out of fase	     |	
;| Destroy: r0..r13						     |
;| 								     |
;| Average timing = 5258 clocks					     |
;|		    4Mhz 1314us					     |
;|		    8Mhz  657us					     |
;|		 14.3Mhz  368us					     |
;|								     |
;+-------------------------------------------------------------------+
	
.DEF	_it	= r0			;integer number for multiply input
.DEF	_fr	= r1			;fractionary number for multiply input too
.DEF	_rlow	= r2			;multiply result low value
.DEF	_rhigh	= r3			;multiply result high value
.DEF	_slow	= r4			;sine accumulator low value
.DEF	_shigh	= r5			;sine accumulator high value
.DEF	_clow	= r6			;co-sine accumulator low value
.DEF	_chigh	= r7			;co-sine accumulator high value
.DEF	_sf	= r8			;frequency index sine
.DEF	_cf	= r9			;frequency index co-sine
.DEF	_i	= r10			;data index
.DEF	_zero	= r11			;zero value
.DEF	_n64	= r12
.DEF	_tmp	= r13			;temporary register
.EQU	_DFT_SIZE	= 64		;DFT number of points
.EQU	_DFT_MAX_VALUE	= 2870
.DSEG
_DFT_DATA_BUF:	.BYTE 64		;64 byte to hold a sampled data
_DFT_DATA_PTR:	.BYTE 1			;Hold a pointer to put data into DFT data buffer
.CSEG
_DFT_64B:
	clr	_zero			;set zero value
	clr	_slow			;sine accumulator =0 
	clr	_shigh
	clr	_clow			;co-sine accumulator = 0
	clr	_chigh
	clr	_i			;i=0
	clr	_sf			;sf=0
	clr	_cf			;cf=16 = 64/4 because cosine is 90o out of fase 
	inc	_cf
	lsl	_cf
	lsl	_cf
	lsl	_cf
	lsl	_cf
	mov	_n64,_cf		;n64=cf*4=64
	lsl	_n64
	lsl	_n64			;now  n64=64
	ldi	YL,low(_DFT_DATA_BUF)	;Y->data buffer
	ldi	YH,high(_DFT_DATA_BUF)	
_DDFTB00:
	ld	_tmp,Y			;tmp=data(i)
	ldi	ZL,low(_DFT_SIN_TABLE_B*2)	;Z-->sine table
	ldi	ZH,high(_DFT_SIN_TABLE_B*2)
	add	ZL,_sf			;get sin(2*pi*f*i/64)
	adc	ZH,_zero
	lpm
	mov	_fr,_it			;put values into correct position
	mov	_it,_tmp
	rcall	_IF_MUL_UBSFB		;r=dt(i)*sin(2*pi*f*i/64)
	add	_slow,_rlow		;s=s+r sine accumulator step
	adc	_shigh,_rhigh
	ld	_tmp,Y+			;tmp=data(i) and point Y to next data
	ldi	ZL,low(_DFT_SIN_TABLE_B*2)	;Z-->co-sine table
	ldi	ZH,high(_DFT_SIN_TABLE_B*2)
	add	ZL,_cf			;get cos(2*pi*f*i/64)
	adc	ZH,_zero
	lpm
	mov	_fr,_it			;put values into correct position
	mov	_it,_tmp
	rcall	_IF_MUL_UBSFB		;r=dt(i)*cos(2*pi*f*i/64)
	add	_clow,_rlow		;c=c+r co-sine accumulator step
	adc	_chigh,_rhigh	
	add	_sf,Acc			;next frequency
	cp	_sf,_n64		;sine table end ?
	brlo	_DDFTB10		;no, branch
	sub	_sf,_n64		;else _sf=_sf-64
_DDFTB10:
	add	_cf,Acc			;next frequency
	cp	_cf,_n64		;co-sine table end ?
	brlo	_DDFTB20		;no, branch
	sub	_cf,_n64		;else _cf=_cf-64
_DDFTB20:
	inc	_i			;i=i+1
	cp	_i,_n64			;last data ?
	brlo	_DDFTB00		;no, get next sum
	tst	_shigh			;s=abs(s)
	brpl	_DDFTB30		;branch if positive
	com	_slow			;else s=-s
	com	_shigh
	ldi	Acc,1
	add	_slow,Acc
	ldi	Acc,0
	adc	_shigh,Acc
_DDFTB30:
	tst	_chigh			;c=abs(c)
	brpl	_DDFTB40		;branch if positive
	com	_clow			;else c=-c
	com	_chigh
	ldi	Acc,1
	add	_clow,Acc
	ldi	Acc,0
	adc	_chigh,Acc
_DDFTB40:
	mov	Acc,_slow		;Acc=abs(s)+abs(c)
	mov	AccH,_shigh
	add	Acc,_clow
	adc	AccH,_chigh
	ret
;+------------------------------+----------------------------------------------------------------------------------------------------------
;| clear DFT data pointer	|
;| input  :none			|
;| output :none			|
;| destroy:none			|
;+------------------------------+
_DFT_DATA_CLEAR:
	push	Acc			;save Useds
	clr	Acc
	sts	_DFT_DATA_PTR,Acc	;clear data pointer
	pop	Acc			;restore useds
	ret
;+------------------------------+----------------------------------------------------------------------------------------------------------
;| put dat into DFT data buffer	|
;| input  :Acc data		|
;| output :cy=1 data buffer full|
;| destroy:none			|
;+------------------------------+
_DFT_DATA_ADD:
	push	AccH			;save Useds
	push	ZL
	push	ZH
	lds	AccH,_DFT_DATA_PTR	;get pointer
	cpi	AccH,_DFT_SIZE		;data pointer > DFT_SIZE ?
	brlo	_DFT_DATA_AD00		;no, branch
	sec				;set buffer full and exit
	rjmp	_DFT_DATA_ADD_EXIT	;exit this function
_DFT_DATA_AD00:
	ldi	ZL,low(_DFT_DATA_BUF)	;Z-->data buffer
	ldi	ZH,high(_DFT_DATA_BUF)
	add	ZL,AccH			;Z-->data buffer[AccH]
	ldi	AccH,0
	adc	ZH,AccH
	st	Z,Acc			;store new data into dft buffer
	lds	AccH,_DFT_DATA_PTR	;DFT_DATA_PTR++
	inc	AccH
	sts	_DFT_DATA_PTR,AccH
	clc				;set buffer no full
_DFT_DATA_ADD_EXIT:
	pop	ZH
	pop	ZL
	pop	AccH			;restore useds
	ret

