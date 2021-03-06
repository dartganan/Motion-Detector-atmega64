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
;| N O R M A L I Z A T I O N 	 				|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
#ifndef __MATH_FLOAT_NORMALIZE__
	
	#define __MATH_FLOAT_NORMALIZE__
	.message "[ (MATH) FLOAT NORMALIZE ]"
	;+--------------------------------------------------------------------------+
	;| PONTO FLUTUANTE NORMALIZACAO                                             |
	;| The float point normalization assume that source number don�t normalized |
	;| the most significant bit out of position. The normalization then shift   |
	;| all bits of mantisa and respective decrement of exponent until the most  |
	;| significant bit reach your position.				    	    | 
	;|                                                                          |
	;| Input  : Floating Accumulator unnormalized number                        |
	;| Saida:   Floating Accumulator normalized	                            |
	;|          Acc FPok or Funderflow					    |
	;| Destroy: flags                                                           |
	;+--------------------------------------------------------------------------+
	_DFNORM:
		clr	Acc			;mantisa = 0 ?
		cp	_acc_0,acc
		cpc	_acc_1,acc
		cpc	_acc_2,acc
		cpc	_acc_3,acc
		cpc	_acc_4,acc
		cpc	_acc_5,acc
		cpc	_acc_s,acc
		brne	_DFNORM00		;no, branch
		clr	_acc_e			;else clear accumulator result
		rjmp	_DFNORM40		;set ok
	_DFNORM00:
		subi	_acc_e,_FBIAS		;extract BIAS
	_DFNORM10:
		bst	_acc_s,7		;normalized ?
		brts	_DFNORM30		;yes, done
		lsl	_acc_0			;left shift to normalize
		rol	_acc_1
		rol	_acc_2
		rol	_acc_3
		rol	_acc_4
		rol	_acc_5
		rol	_acc_s
		cpi	_acc_e,0x80		;2^-128 before expoent decrement ?
		brne	_DFNORM20		;no, branch
		rjmp	_DFUNDER		;set underflow
	_DFNORM20:
		dec	_acc_e			;expoent--
		rjmp	_DFNORM10
	_DFNORM30:
		subi	_acc_e,-_FBIAS		;restore BIAS
	_DFNORM40:
		ldi	Acc,_FPOK		;Float Pointer Ok
		ret
#endif
.exit

