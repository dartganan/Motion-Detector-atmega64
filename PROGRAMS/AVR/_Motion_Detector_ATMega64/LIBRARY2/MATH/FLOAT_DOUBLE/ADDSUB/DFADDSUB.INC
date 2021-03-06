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
;| A D D I T I O N  A N D  S U B T R A C T I O N		|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;+-------------------------------------+
;| FLOAT POINT ADDITION		       |
;|                                     |
;| Input  : Op1 1st parcel             |
;|          Op2 2nd parcel             |
;| Output : Floating Accumulator       |
;|          Acc Exception type         |
;| Destroy:Op1,Op2,Acc,AccH,flags      |
;+-------------------------------------+
#ifndef __MATH_FLOAT_ADDSUB__
	
	#define __MATH_FLOAT_ADDSUB__
	.message "[ (MATH) FLOAT ADDITION  & SUBTRACTION ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef __MATH_FLOAT_INFINIT__
		.INCLUDE	"MATH\FLOAT_DOUBLE\INFINIT_RESULT\DFINF.INC"
	#endif
	#ifndef __MATH_FLOAT_NORMALIZE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\NORMALIZE\DFNORM.INC"
	#endif
	.MACRO	_DFMADD
		rcall	_DFADD
	.ENDMACRO
	_DFADD:
		tst	_op1_e			;Op1 = 0 ?
		brne	_DFADD00		;no, branch
		mov	Acc,_op2_e
		subi	Acc,_FBIAS
		mov	_op2_e,Acc
	_DFADD004:
		mov	_acc_0,_op2_0		;result=op2
		mov	_acc_1,_op2_1
		mov	_acc_2,_op2_2
		mov	_acc_3,_op2_3
		mov	_acc_4,_op2_4
		mov	_acc_5,_op2_5
		mov	_acc_s,_op2_s
		mov	_acc_e,_op2_e
		rjmp	_DFADDOK		;set Fadd Ok
	_DFADD00:
		tst	_op2_e			;Op2 = 0 ?
		brne	_DFADD10		;no, branch
		mov	Acc,_op1_e
		subi	Acc,_FBIAS
		mov	_op1_e,Acc
	_DFADD05:
		mov	_acc_0,_op1_0		;result=op1
		mov	_acc_1,_op1_1
		mov	_acc_2,_op1_2
		mov	_acc_3,_op1_3
		mov	_acc_4,_op1_4
		mov	_acc_5,_op1_5
		mov	_acc_s,_op1_s
		mov	_acc_e,_op1_e
	_DFADDOK:
		subi	_acc_e,-_FBIAS		;restore FBIAS
		ldi	Acc,_FPOk		;set Floating Add Ok
		ret	
	_DFADD10:
		mov	Acc,_op1_e		;extract FBIAS
		subi	Acc,_FBIAS
		mov	_op1_e,Acc
		mov	Acc,_op2_e
		subi	Acc,_FBIAS
		mov	_op2_e,Acc
	
		mov	Acc,_op1_e		;Acc=OpE1-OpE2
		sub	Acc,_op2_e
		cpi	Acc,56			;Acc>56 ?
		brge	_DFADD05		;yes, result=Op1
		cpi	Acc,-55			;Acc<-55 ?
		brlt	_DFADD004		;yes, result=Op2
		mov	_acc_e,_op1_e		;Result Exp.=Op1 Exp.		
		bst	_op1_s,7		;save Operands Signals
		bld	AccH,1			;AccH bit 1=Op1 Signal
		bst	_op2_s,7
		bld	AccH,2			;AccH bit 2=Op2 Signal
		push	Acc
		ldi	Acc,0x80		;set MSB bits of operands
		or	_op1_s,Acc		;set MSB bits of operands
		or	_op2_s,Acc
		pop	Acc
		cpi	Acc,0			;Acc=0 ?
		brne	_DFADD20		;No, Branch
		rjmp	_DFADD50		;direct without normalization
	_DFADD20:
		brlt	_DFADD35		;Acc<0 , Branch
	_DFADD30:
		lsr	_op2_s			;else normalize op2
		ror	_op2_5
		ror	_op2_4
		ror	_op2_3
		ror	_op2_2
		ror	_op2_1
		ror	_op2_0
		dec	Acc
		brne	_DFADD30		;until normalized
		rjmp	_DFADD50
	_DFADD35:
		neg	Acc			;Acc=Abs(Acc)
	_DFADD40:
		lsr	_op1_s			;normalize op1
		ror	_op1_5
		ror	_op1_4
		ror	_op1_3
		ror	_op1_2
		ror	_op1_1
		ror	_op1_0
		dec	Acc
		brne	_DFADD40		;until normalized
		mov	_acc_e,_op2_e		;Result Exp.=Op2 Exp.
	_DFADD50:
		andi	AccH,0b00000110		;verify signals	
		cpi	AccH,0
		breq	_DFADD53		;iguals, branch
		cpi	AccH,0b00000110
		breq	_DFADD53
		rjmp	_DFADD60		;no, goto subtraction
	_DFADD53:
		mov	_acc_0,_op1_0		;Acc=Op1+Op2
		add	_acc_0,_op2_0
		mov	_acc_1,_op1_1
		adc	_acc_1,_op2_1
		mov	_acc_2,_op1_2
		adc	_acc_2,_op2_2
		mov	_acc_3,_op1_3
		adc	_acc_3,_op2_3
		mov	_acc_4,_op1_4
		adc	_acc_4,_op2_4
		mov	_acc_5,_op1_5
		adc	_acc_5,_op2_5
		mov	_acc_s,_op1_s
		adc	_acc_s,_op2_s
		bst	AccH,1			;Result Signal=Op1 Signal
		bld	AccH,3			;AccH Bit 3 = Result Signal
		brcc	_DFADD54		;no, need normalization, brach
		ror	_acc_s			;else normalize result
		ror	_acc_5
		ror	_acc_4
		ror	_acc_3
		ror	_acc_2
		ror	_acc_1
		ror	_acc_0
		cpi	_acc_e,0x7E		;overflow ?
		breq	_DFADD55		;yes, branch
		inc	_acc_e			;Ace++
	_DFADD54:
		subi	_acc_e,-_FBIAS		;add FBIAS into AcE
		rjmp	_DFADD80		;Normalize
	_DFADD55:
		bst	AccH,3
		bld	_acc_s,7
		rjmp	_DFOVER			;set overflow
	_DFADD60:
		mov	_acc_0,_op1_0		;Acc=Op1-Op2
		sub	_acc_0,_op2_0
		mov	_acc_1,_op1_1
		sbc	_acc_1,_op2_1
		mov	_acc_2,_op1_2
		sbc	_acc_2,_op2_2
		mov	_acc_3,_op1_3
		sbc	_acc_3,_op2_3
		mov	_acc_4,_op1_4
		sbc	_acc_4,_op2_4
		mov	_acc_5,_op1_5
		sbc	_acc_5,_op2_5
		mov	_acc_s,_op1_s
		sbc	_acc_s,_op2_s
		bst	AccH,1			;Assume Result Signal=Op1 Signal
		bld	AccH,3			;AccH Bit 3 = Result Signal
		brcc	_DFADD70		;no need 2 complement, Branch
		bst	AccH,2			;Result Signal=Op2 Signal
		bld	AccH,3			;AccH Bit 3 = Result Signal
		com	_acc_0			;else 2 complement result
		com	_acc_1
		com	_acc_2
		com	_acc_3
		com	_acc_4
		com	_acc_5
		com	_acc_s
		subi	_acc_0,-1
		sbci	_acc_1,-1
		sbci	_acc_2,-1
		sbci	_acc_3,-1
		sbci	_acc_4,-1
		sbci	_acc_5,-1
		sbci	_acc_s,-1
	_DFADD70:
		subi	_acc_e,-_FBIAS		;add FBIAS into AcE
		rcall	_DFNORM			;normalize
		cpi	Acc,_FPOk		;Normalization Ok ?
		breq	_DFADD80		;yes, branch
		ret
	_DFADD80:
		bst	AccH,3			;join Result Signal
		bld	_acc_s,7
		ret
	;+-------------------------------------+
	;| FLOAT POINT ADDITION		       |
	;|                                     |
	;| Input  : Op1   1st parcel           |
	;|          Op2   2nd parcel           |
	;| Output : Floating Accumulator       |
	;|     	    Acc Exception type	       |	
	;| Destroy:Op1,Op2,Acc,AccH,flags      |
	;+-------------------------------------+
	.MACRO	_DFMSUB
		rcall	_DFSUB
	.ENDMACRO
	_DFSUB:
		ldi	Acc,0x80		;Change Op2 signal
		eor	_op2_s,Acc
		rjmp	_DFADD			;go to Floating Addition		
#endif
.exit

