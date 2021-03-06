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
;| D O U B L E  F L O A T  P O I N T  R O U T I N E S		|
;| I N T E G E R  T O  F L O A T  C O N V E R S I O N S		|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
#ifndef	__MATH_FLOAT_XTOF__
	#define __MATH_FLOAT_XTOF__
	.message "[ (MATH) FLOAT X TO F ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	
	#ifndef __MATH_FLOAT_INFINIT__
		.INCLUDE	"MATH\FLOAT_DOUBLE\INFINIT_RESULT\DFINF.INC"
	#endif
	
	#ifndef __MATH_FLOAT_NORMALIZE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\NORMALIZE\DFNORM.INC"
	#endif
	;+---------------------------------------------------+
	;| CONVERT LONG INTEGER SIGNED NUMBER TO FLOAT POINT |
	;|                                                   |
	;| Input  : Acc:AccH:AccT:AccTH signed long integer  |
	;| Output : Floating Accumulator		     |
	;|	    Acc FPok or FUndeflow		     |
	;| Destroy: Flags	                             |
	;+---------------------------------------------------+
	_DFLTOF:
		push	AccTH			;save useds
		clr	_acc_2			;clear garbage at rest of accumulator
		clr	_acc_1
		clr	_acc_0
		cpi	Acc,0			;Accumulator igual zero ?
		brne	_DFLTOF00		;no, branch
		cpi	AccH,0
		brne	_DFLTOF00		;no, branch
		cpi	AccT,0
		brne	_DFLTOF00		;no, branch
		cpi	AccTH,0
		brne	_DFLTOF00		;no, branch
		clr	_acc_3			;else clear float point accumulator
		clr	_acc_4
		clr	_acc_5
		clr	_acc_s
		clr	_acc_e
		ldi	Acc,_FPOK		;set float point ok
		pop	AccTH			;restore useds
		ret
	_DFLTOF00:
		mov	_acc_s,AccTH		;set accumulator to start normalization
		mov	_acc_5,AccT
		mov	_acc_4,AccH
		mov	_acc_3,Acc
		bst	AccTH,7			;negative number ?
		brtc	_DFLTOF10		;no, branch
		com	_acc_3			;else only positive values
		com	_acc_4
		com	_acc_5
		com	_acc_s
		subi	_acc_3,-1
		sbci	_acc_4,-1
		sbci	_acc_5,-1
		sbci	_acc_s,-1
	_DFLTOF10:	
		ldi	_acc_e,_FBIAS+31	;set expoent
		rcall	_DFNORM			;normalize number
		andi	_acc_s,0x7F		;clear signal bit
		andi	AccTH,0x80		;glue with signal
		or	_acc_s,AccTH
		pop	AccTH
		ret				;end conversion
	;+---------------------------------------------------+
	;| CONVERT INTEGER SIGNED NUMBER TO FLOAT POINT      |
	;|                                                   |
	;| Input  : Acc:AccH signed integer 	 	     |
	;| Output : Floating Accumulator		     |
	;|	    Acc FPok or FUndeflow		     |
	;| Destroy: Flags                                    |
	;+---------------------------------------------------+
	_DFITOF:
		clr	AccT			;assume a positive word
		clr	AccTH
		tst	AccH			;negative word ?
		brpl	_DFITOF00		;no, branch
		ser	AccT			;else turn if negative 
		ser	AccTH
	_DFITOF00:
		rjmp	_DFLTOF
	;+---------------------------------------------------+
	;| CONVERT SIGNED BYTE NUMBER TO FLOAT POINT         |
	;|                                                   |
	;| Input  : Acc signed byte		  	     |
	;| Output : Floating Accumulator		     |
	;|	    Acc FPok or FUndeflow		     |
	;| Destroy: Flags                                    |
	;+---------------------------------------------------+
	_DFATOF:
	_DFBTOF:
		clr	AccH			;assume a positive word
		clr	AccT
		clr	AccTH
		tst	Acc			;negative byte ?
		brpl	_DFATOF00		;no, branch
		ser	AccH
		ser	AccT			;else turn if negative 
		ser	AccTH
	_DFATOF00:
		rjmp	_DFLTOF
#endif
.exit

