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
;+----------------------------------------------------------------------+
;| MUL_U32U32X10							|
;| Multiply unsigned 32bits number by 10 result back unsigned 32 bits	|
;|								  	|
;| Input  : r3:r2:r1:r0 unsigned 32 bits number			  	|
;| Output : r3:r2:r1:r0 unsigned 32 bits number			  	|
;| Destroy: r4,r5,r6,r7,Flags					  	|
;| Time   : 24+3rcall 1.68us at 16Mhz				  	|
;+----------------------------------------------------------------------+
#ifndef	__INTEGER_MUL_U32U32X10__
	#define __INTEGER_MUL_U32U32X10__
	.message "[ (MATH) INTEGER MUL U32U32X10 ]"
	_MUL_U32U32X10:
		lsl	r0					;x*2
		rol	r1
		rol	r2
		rol	r3
		mov	r4,r0					;t=x*2
		mov	r5,r1
		mov	r6,r2
		mov	r7,r3
		lsl	r0					;x*4
		rol	r1
		rol	r2
		rol	r3
		lsl	r0					;x*8
		rol	r1
		rol	r2
		rol	r3
		add	r0,r4					;x*2+x*8=x*(2+8)=x*10
		adc	r1,r5
		adc	r2,r6
		adc	r3,r7
		ret
#endif
.exit

