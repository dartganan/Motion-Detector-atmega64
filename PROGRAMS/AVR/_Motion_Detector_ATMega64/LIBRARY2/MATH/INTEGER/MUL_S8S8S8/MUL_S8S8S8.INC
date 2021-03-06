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
;+--------------------------------------------------------------+
;| MUL_S8S8S8							|
;| Multiply signed 8bits x signed 8bits result signed 8bits	|
;|								|
;| Input  : R0 1st operand					|
;|	    R1 2nd operand					|
;| Output : R2 Result						|
;| Destroy: R1,R3,Flags						|
;| Time	  : 43+3 rcall						|
;+--------------------------------------------------------------+
#ifndef	__INTEGER_MUL_S8S8S8__
	#define __INTEGER_MUL_S8S8S8__
	.message "[ (MATH) INTEGER MUL S8S8S8 ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef	__INTEGER_MUL_U8U8U8__
		.INCLUDE "MATH\INTEGER\MUL_U8U8U8\MUL_U8U8U8.INC"
	#endif
	_MUL_S8S8S8:
		mov	r3,r0				;multiply signals
		eor	r3,r1
		sbrc	r0,7				;complement 1st operand if negative
		neg	r0
		sbrc	r1,7				;complement 2nd operand if negative
		neg	r1
		rcall	_MUL_U8U8U8			;multiply unsigned
		sbrc	r3,7				;skip if result is positive
		neg	r2				;else complement result
		ret
#endif
.exit

