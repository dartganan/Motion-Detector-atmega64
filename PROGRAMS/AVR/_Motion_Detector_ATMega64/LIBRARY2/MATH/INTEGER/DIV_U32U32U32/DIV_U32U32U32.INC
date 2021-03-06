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
;| DATE:17/12/2001 VERSION 1.0						|
;>----------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     				|
;| D I V I D E  							|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;+----------------------------------------------------------------------+
;+----------------------------------------------------------------------+
;| DIV_U32U32U32							|
;| Divide unsigned 32bits/unsigned 32bits result unsigned 32bits	|
;|								  	|
;| Input  : R0:R1:R2 :R3  Dividend				  	|
;|	    R4:R5:R6 :R7  Divisor				  	|
;| Output : R0:R1:R2 :R3  Quotient				  	|
;|	    R8:R9:R10:R11 Rest						|
;| Destroy: Flags						  	|
;| Time   : Min=566+3 rcall  Max=662+3 rcall			  	|
;+----------------------------------------------------------------------+
#ifndef	__INTEGER_DIV_U32U32U32__
	#define __INTEGER_DIV_U32U32U32__
	.message "[ (MATH) INTEGER DIV U32U32U32 ]"
	_DIV_U32U32U32:
		push	Acc			;Save Acc
		ldi	Acc,32			;quotient bit count
		clr	R8			;clear rest
		clr	R9
		clr	R10
		clr	R11
		clc
	_DIV_U32U32U32_00:
		rol	R0			;see if dividend fit inside divisor 
		rol	R1
		rol	R2
		rol	R3
		rol	R8
		rol	R9
		rol	R10
		rol	R11
		cp	R8,R4			;fit ?
		cpc	R9,R5
		cpc	R10,R6
		cpc	R11,R7
		brcS	_DIV_U32U32U32_10	;branch if no
		sub	R8,R4			;else subtract divisor from dividend 
		sbc	R9,R5
		sbc	R10,R6
		sbc	R11,R7
	_DIV_U32U32U32_10:
		dec	Acc			;next bit
		brne	_DIV_U32U32U32_00	;until all done
		rol	r0			;place last bit result into quotient
		rol	r1
		rol	r2
		rol	r3
		com	r0			;because all bits are inverted during computation
		com	r1
		com	r2
		com	r3
		pop	Acc
		ret
#endif
.exit

