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
;| DATE:24/05/2001 VERSION 1.0							|
;>--------------------------------------------------------------+---------------<
;| D O U B L E   F L O A T  P O I N T  R O U T I N E S		|
;| M U L T I P L Y  A N D  D I V I D E 				|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;+---------------------------------------+
;| VERIFY POSSIBILITY OF UNDERFLOW OR    |
;| OVERFLOW BEFORE MULTIPLY OR DIVIDE AND|
;| PREPARE OP1 AND OP2 AND CLEAR FLOAT   |
;| POINT ACCUMULATOR			 |
;|					 |
;| Input  : None			 |
;| Output : Acc=56 if ok		 |
;|	    Exception code if not ok	 |
;|          AccH signal product		 |
;| Destroy: flags    		         |
;| Clear  : op1,op2 exponent             |
;| Clocks : max 52			 |
;+---------------------------------------+
#ifndef __MATH_FLOAT_MULDIV__
	#define __MATH_FLOAT_MULDIV__
	.message "[ (MATH) FLOAT MULTIPLY & DIVIDE ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef __MATH_FLOAT_INFINIT__
		.INCLUDE	"MATH\FLOAT_DOUBLE\INFINIT_RESULT\DFINF.INC"
	#endif
	#ifndef __MATH_FLOAT_NORMALIZE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\NORMALIZE\DFNORM.INC"
	#endif
	_DFREADY:
		mov	Acc,_op1_e		;Op1 = 0 ?
		tst	Acc
		brne	_DFREAD10		;No, branch
		rjmp	_DFCLEAR		;else clear product
	_DFREAD10:
		subi	Acc,_FBIAS		;extract FBIAS
		mov	_op1_e,Acc
		mov	Acc,_op2_e
		subi	Acc,_FBIAS
		mov	_op2_e,Acc
		mov	Acc,_op1_e		;see if signals are iguals 
		eor	Acc,_op2_e
		bst	Acc,7			;signals not the same ?
		brts	_DFREAD30		;no, branch
		bst	_op1_e,7		;signals are positive ?
		brtc	_DFREAD20		;see, possibility of overflow
		mov	Acc,_op1_e		;Acc=OpE1+OpE2
		add	Acc,_op2_e
		bst	Acc,7			;signal bit igual 0?
		brts	_DFREAD30		;no, branch
		rjmp	_DFUNDER		;set underflow
	_DFREAD20:	
		mov	Acc,_op1_e		;Acc=OpE1+OpE2
		add	Acc,_op2_e
		bst	Acc,7			;signal bit igual 1?
		brtc	_DFREAD30		;no, branch
		mov	Acc,_op1_s
		eor	Acc,_op2_s
		mov	_acc_s,Acc		;set signal of overflow
		rjmp	_DFOVER			;set overflow
	_DFREAD30:	
		mov	AccH,_op1_s		;save signals multiply/divide
		eor 	AccH,_op2_s
		andi	AccH,0x80		;mask signal bit
		ldi	Acc,0x80
		or	_op1_s,Acc		;set MSB of operands
		or	_op2_s,Acc
		rcall	_DFCLEAR		;clear Accumulator
		mov	_acc_e,_op1_e		;Accumulator expoent=OpE1+OpE2+FBIAS
		add	_acc_e,_op2_e
		clr	_op1_e			;used obtain more precision
		clr	_op2_e
		ldi	Acc,56			;number of bits to divide or multiply
		ret
	;+---------------------------------------+
	;| FLOAT POINT MULTIPLY			 |
	;|                                       |
	;| Input  : Op1  multiplicando           |
	;|          Op2  multiplier	         |
	;| Output : Floating Accumulator         |
	;|          Acc  Exception type          |
	;| Destroy: Op1,Op2,Acc,AccH,flags       |
	;| Clocks : Max 1244+52                  |
	;|          81us at 16Mhz		 |
	;+---------------------------------------+
	.MACRO	_DFMMUL
		rcall	_DFMUL
	.ENDMACRO
	_DFMUL:
		mov	Acc,_op2_e		;Op2 = 0 ?
		tst	Acc
		brne	_DFMUL00		;No,branch
		rjmp	_DFCLEAR		;else clear product
	_DFMUL00:
		rcall	_DFREADY		;ready to multiply
		cpi	Acc,56			;correctly ready ?
		breq	_DFMUL30		;yes, branch
		ret				;no return with error code
	_DFMUL30:
		push	AccH			;save signal multiply
		mov	AccH,_op1_0		;multiply 1st byte
		rcall	_DFMUL8
		mov	AccH,_op1_1		;multiply 2st byte
		rcall	_DFMUL8
		mov	AccH,_op1_2		;multiply 3st byte
		rcall	_DFMUL8
		mov	AccH,_op1_3		;multiply 4st byte
		rcall	_DFMUL8
		mov	AccH,_op1_4		;multiply 5st byte
		rcall	_DFMUL8
		mov	AccH,_op1_5		;multiply 6st byte
		rcall	_DFMUL8
		mov	AccH,_op1_s		;multiply 7st byte
		rcall	_DFMUL8
		pop	AccH			;restore signal multiply
		mov	Acc,_acc_e		;save old state of exponent to verify possibility of overflow
		inc	_acc_e
		eor	Acc,_acc_e
		brpl	_DFMULDIV		;no, continue
		mov	Acc,_acc_e		;see if overflow ?
		dec	Acc
		cpi	Acc,0x7F
		brne	_DFMULDIV
		rjmp	_DFMUL65		;else set overflow
	_DFMULDIV:
		bst	_acc_s,7		;need normalization ?
		brts	_DFMUL60		;no, branch
		lsl	_op1_e			;else normalize
		rol	_acc_0
		rol	_acc_1
		rol	_acc_2
		rol	_acc_3
		rol	_acc_4
		rol	_acc_5
		rol	_acc_s
		dec	_acc_e
	_DFMUL60:
		bst	_op1_e,7		;need to round ?
		brtc	_DFMUL80		;no, branch
		subi	_acc_0,-1
		sbci	_acc_1,-1
		sbci	_acc_2,-1
		sbci	_acc_3,-1
		sbci	_acc_4,-1
		sbci	_acc_5,-1
		sbci	_acc_s,-1
		brcs	_DFMUL80		;need 2nd normalization ?, no branch
		cpi	_acc_e,0x7F		;seed if overflow
		brne	_DFMUL70		;no, branch
	_DFMUL65:
		mov	_acc_s,AccH		;get signal 
		rjmp	_DFOVER			;set overflow
	_DFMUL70:
		ror	_acc_s			;else normalize again
		ror	_acc_5
		ror	_acc_4
		ror	_acc_3
		ror	_acc_2
		ror	_acc_1
		ror	_acc_0
		mov	Acc,_acc_e		;save old state of exponent to verify possibility of overflow again
		inc	_acc_e
		eor	Acc,_acc_e
		brmi	_DFMUL65
	_DFMUL80:	
		subi	_acc_e,-_FBIAS		;restore FBIAS
		andi	_acc_s,0X7F		;include signals multiply
		or	_acc_s,AccH
		ldi	Acc,_FPoK
		ret
	;+-----------------------------------------------------------
	;| MULTIPLY 8 BITS FOR EACH LOOP TO REDUCE TIME CONSUMPTION 
	;+-----------------------------------------------------------
	_DFMUL8:
		ldi	Acc,8			;bit count=8
	_DFMUL8_00:
		ror	AccH			;get bit
		brcc	_DFMUL8_10		;if shifted bit0 = 0 then branch
		add	_acc_0,_op2_0		;else accumulate
		adc	_acc_1,_op2_1
		adc	_acc_2,_op2_2
		adc	_acc_3,_op2_3
		adc	_acc_4,_op2_4
		adc	_acc_5,_op2_5
		adc	_acc_s,_op2_s
	_DFMUL8_10:
		ror	_acc_s			;shift right accumulator
		ror	_acc_5
		ror	_acc_4
		ror	_acc_3
		ror	_acc_2
		ror	_acc_1
		ror	_acc_0
		ror	_op1_e			;to obtain more presicion
		dec	Acc			;next bit
		brne	_DFMUL8_00		;until done all bits
		ret
	;+----------------------------------------------+
	;| FLOAT POINT DIVISION FRACTIONARY DIVISION 	|
	;|                                         	|
	;| Input  : Op1  dividend                  	|
	;|          Op2  divisor               	   	|
	;| Output : Floating Accumulator quotient  	|
	;|	    only fractionary part		|
	;|          Acc Exception type	           	|
	;| Destroy: Op1,Op2,AccH,flags 		   	|
	;| Clocks : Max 2281+52                    	|
	;|          146us at 16Mhz		   	|
	;+----------------------------------------------+
	_DFDIVFRAC:
		ser	Acc			;set fractionary division
		sts	_DF_FRAC,Acc
		rcall	_DFDIV			;call DIVISION ROUTINE
		clr	Acc			;set no fractionary division
		sts	_DF_FRAC,Acc
		ret
	
	;+-----------------------------------------+
	;| FLOAT POINT DIVISION			   |
	;|                                         |
	;| Input  : Op1  dividend                  |
	;|          Op2  divisor               	   |
	;| Output : Floating Accumulator quotient  |
	;|          Acc Exception type	           |
	;| Destroy: Op1,Op2,Acc,AccH,flags 	   |
	;| Clocks : Max 2281+52                    |
	;|          146us at 16Mhz		   |
	;+-----------------------------------------+
	.MACRO	_DFMDIV
		rcall	_DFDIV
	.ENDMACRO
	_DFDIV:
		mov	Acc,_op2_e		;Op2 = 0 ?
		tst	Acc
		brne	_DFDIV00		;No,branch
		mov	_acc_s,_op1_s
		eor	_acc_s,_op2_s
		rjmp	_DFDIV0			;else set Division by zero code
	_DFDIV00:
		mov	Acc,_op2_e		;change op2 signal to start division
		subi	Acc,_FBIAS		;extract fbias
		neg	Acc			;-op2
		subi	Acc,-_FBIAS		;set  fbias
		mov	_op2_e,Acc
		rcall	_DFREADY		;ready to divide
		cpi	Acc,56			;correctly ready ?
		breq	_DFDIV105		;yes, branch
		ret				;no return with error code
	_DFDIV105:
		push	AccH			;16+52+2223+42
		subi	Acc,-2			;increment bit count byte 2 to obtain more precision
	
	;>>-------- SEE IF FRACTIONARY DIVISION ACTIVATE --------<<
		push	Acc			;see if DF_FS igual 0xff
		lds	Acc,_DF_FRAC		
		tst	Acc
		pop	Acc
		breq	_DFDIV106		;no, branch
		add	Acc,_acc_e		;my counter plus integer diference
		ldi	_acc_e,0		;because a fractionary division
	_DFDIV106:
	;>>------------------------------------------------------<<
		clr	AccH
	_DFDIV10:
		sub	_op1_0,_op2_0		;verify if op1>=op2
		sbc	_op1_1,_op2_1
		sbc	_op1_2,_op2_2
		sbc	_op1_3,_op2_3
		sbc	_op1_4,_op2_4
		sbc	_op1_5,_op2_5
		sbc	_op1_s,_op2_s
		sbc	_op2_e,AccH		;only to obtain more precision
		set
		brcc	_DFDIV20		;no, branch
		clt
		add	_op1_0,_op2_0		;Op1-Op2
		adc	_op1_1,_op2_1
		adc	_op1_2,_op2_2
		adc	_op1_3,_op2_3
		adc	_op1_4,_op2_4
		adc	_op1_5,_op2_5
		adc	_op1_s,_op2_s
		adc	_op2_e,AccH		;only to obtain more precision
	_DFDIV20:
		lsl	_op1_0			;Op1<<1
		rol	_op1_1
		rol	_op1_2
		rol	_op1_3
		rol	_op1_4
		rol	_op1_5
		rol	_op1_s
		rol	_op2_e
		bld	_op1_e,6		;t flag to accumulator
		rol	_op1_e
		rol	_acc_0			;shift result into quocient
		rol	_acc_1
		rol	_acc_2
		rol	_acc_3
		rol	_acc_4
		rol	_acc_5
		rol	_acc_s
		dec	Acc			;next bit
		brne	_DFDIV10		;until done all bits
		ror	_acc_s			;restore configuration of accumulator
		ror	_acc_5
		ror	_acc_4
		ror	_acc_3
		ror	_acc_2
		ror	_acc_1
		ror	_acc_0
		ror	_op1_e
		pop	AccH
	;>>-------- SEE IF FRACTIONARY DIVISION ACTIVATE --------<<
		push	Acc
		lds	Acc,_DF_FRAC		;see FS igual 0xff again		
		tst	Acc
		pop	Acc
		breq	_DFDIV40
		andi	_acc_s,~(1<<7)		;exclude any possibility of value great than one
	_DFDIV30:
		sbrc	_acc_s,7		;see if fraction into correctly position
		rjmp	_DFDIV40		;yes, branch
		lsl	_op1_e
		rol	_acc_0			;else normalize
		rol	_acc_1
		rol	_acc_2
		rol	_acc_3
		rol	_acc_4
		rol	_acc_5
		rol	_acc_s
		subi	_acc_e,1
		cpi	_acc_e,-39		;see if expoent below -39
		brge	_DFDIV30		;no, adjust again
		rjmp	_DFUNDER		;else set underflow
	_DFDIV40:
	;>>------------------------------------------------------<<
		rjmp	_DFMULDIV		;MULTIPLY/DIVIDE CHECK AND EXPOENT RESTORE
	;+-----------------------------------------+
	;| FLOAT POINT MULTIPLY BY 2		   |
	;|                                         |
	;| Input  : Floating accumulator number    |
	;| Output : Floating accumulator result    |
	;|          Acc Exception type	           |
	;| Destroy: Op1,Op2,Acc,AccH,flags 	   |
	;+-----------------------------------------+
	.MACRO	_DFMMUL2
		rcall	_DFMUL2
	.ENDMACRO
	_DFMUL2:
		mov	Acc,_acc_e		;get exponent acctual estate
		inc	_acc_e
		eor	Acc,_acc_e		;signal changed ?
		brpl	_DFMUL200		;no, branch
		rjmp	_DFOVER			;clear result "overflow"
	_DFMUL200:
		ldi	Acc,_FPOK
		ret
	;+-----------------------------------------+
	;| FLOAT POINT DIVISION	BY 2		   |
	;|                                         |
	;| Input  : Floating accumulator number    |
	;| Output : Floating accumulator result    |
	;|          Acc Exception type	           |
	;| Destroy: Op1,Op2,Acc,AccH,flags 	   |
	;+-----------------------------------------+
	.MACRO	_DFMDIV2
		rcall	_DFDIV2
	.ENDMACRO
	_DFDIV2:
		mov	Acc,_acc_e		;get exponent actual estate
		dec	_acc_e
		eor	Acc,_acc_e		;signal changed ?
		brpl	_DFDIV200		;no, branch
		rjmp	_DFCLEAR		;clear result "undeflow"
	_DFDIV200:
		ldi	Acc,_FPOK
		ret
#endif
.exit
	

