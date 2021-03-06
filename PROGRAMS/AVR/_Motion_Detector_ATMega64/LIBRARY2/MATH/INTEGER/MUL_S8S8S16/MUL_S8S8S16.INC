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
;| M U L T I P L Y 							|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;+----------------------------------------------------------------------+
;+----------------------------------------------------------------+
;| MUL_S8S8S16							  |
;| Multiply signed 8bits x signed 8bits result signed 16bits	  |
;|								  |
;| Input  : R0    1st operand					  |
;|	    R1    2nd operand					  |
;| Output : R2:R3 Result					  |
;| Destroy: R1,R4,R5,Flags					  |
;| Time   : Min=79+3 rcall  Max=108+3 rcall			  | 
;+----------------------------------------------------------------+
#ifndef	__INTEGER_MUL_S8S8S16__
	#define __INTEGER_MUL_S8S8S16__
	.message "[ (MATH) INTEGER MUL S8S8S16 ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef	__INTEGER_MUL_U8U8U16__
		.INCLUDE "MATH\INTEGER\MUL_U8U8U16\MUL_U8U8U16.INC"
	#endif		
	_MUL_S8S8S16:
		mov	R5,R0				;multiply signals
		eor	R5,R1				;R5 signal product
		sbrc	R0,7				;complement R0 if negative
		neg	R0
		sbrc	R1,7				;complement R1 if negative
		neg	R1
		rcall	_MUL_U8U8U16			;multiply unsigned
		sbrs	R5,7				;skip if result is negative
		rjmp	_MUL_S8S8S16_00			;else done
		com	R2				;complemente result
		com	R3	
		clr	R5
		inc	R5
		add	R2,R5
		dec	R5
		adc	R3,R5
	_MUL_S8S8S16_00:
		ret
#endif
.exit

