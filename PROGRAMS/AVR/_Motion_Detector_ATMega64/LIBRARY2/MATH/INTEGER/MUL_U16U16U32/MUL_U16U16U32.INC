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
;| DATE:2/4/2004 VERSION 1.0						|
;>----------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     				|
;| M U L T I P L Y 							|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;+----------------------------------------------------------------------+
;+----------------------------------------------------------------------+
;| MUL_U16U16U32							|
;| Multiply unsigned 16bits x unsigned 16bits result unsigned 32bits	|
;|								  	|
;| Input  : R0:R1  1st operand				  		|
;|	    R2:R3  2nd operand				  		|
;| Output : R4:R5:R6:R7 Result					  	|
;| Destroy: R0,R1,R2,R3,Flags					  	|
;| Time   : Max=242+3 rcall					  	|
;+----------------------------------------------------------------------+
#ifndef	__INTEGER_MUL_U16U16U32__
	#define __INTEGER_MUL_U16U16U32__
	.message "[ (MATH) INTEGER MUL U16U16U32 ]"
	_MUL_U16U16U32:
		push	Acc			;Save useds
		push	AccH
		clr	AccH
		clr	R4			;clear accumulator
		clr	R5
		clr	R6
		clr	R7
		ldi	Acc,16
	_MUL_U16U16U32_00:
		lsl	R4			;next power of 2
		rol	R5
		rol	R6
		rol	R7
		rol	R0			;get out bit of 1st operand
		rol	R1
		brcc	_MUL_U16U16U32_10	;branch if out bit zero
		add	R4,R2			;else accumulate
		adc	R5,R3
		adc	R6,AccH
		adc	R7,AccH
	_MUL_U16U16U32_10:
		dec	Acc
		brne	_MUL_U16U16U32_00
		pop	AccH			;restore useds
		pop	Acc
		ret
#endif
.exit

