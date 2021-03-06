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
;| DATE:17/12/2001 VERSION 1.0							|
;>------------------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     					|
;| S I G N E D  I N T E G E R  T O  S T R I N G 				|
;+------------------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM				|
;+------------------------------------------------------------------------------+
;+----------------------------------------------------------------------+
;| _ITOS								|
;| Convert signed word to string ASCII with 0 terminator		|
;|								  	|
;| Input  : AccH:Acc unsigned word input			  	|
;| Output : Z--> SRAM string output with 0			  	|
;+----------------------------------------------------------------------+
#ifndef	__INTEGER_ITOS__
	#define __INTEGER_ITOS__
	.message "[ (MATH) INTEGER ITOS ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef	__INTEGER_UITOS__
		.INCLUDE "MATH\INTEGER\UITOS\UITOS.INC"
	#endif
	_ITOS:
		push	Acc				;save useds
		push	AccH
		tst	AccH				;negative ?
		brpl	_ITOS00				;no, branch
		com	Acc				;complement input value
		com	AccH
		subi	Acc,-1
		sbci	AccH,-1
		rcall	_UITOS				;convert
		sbiw	ZL,1
		ldi	Acc,'-'				;set negative
		st	Z,Acc
		rjmp	_ITOS10
	_ITOS00:
		rcall	_UITOS				;call unsigned to string
	_ITOS10:
		pop	AccH
		pop	Acc
		ret
#endif
.exit

