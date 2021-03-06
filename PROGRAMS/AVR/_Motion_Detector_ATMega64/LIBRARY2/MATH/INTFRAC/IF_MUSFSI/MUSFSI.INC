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
;+--------------------------------------------------------------+---+
;|								|
;| DATA:dECEMBER,2,2002						|
;>--------------------------------------------------------------<
;| M U L T I P L Y						|
;| S I G N E D  F R A C T I O N  X  S I G N E D  I N T E G E R	|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N  		|
;+------------------------------------------------------+---------------------------------------+
;| _IF_MUL_SFSI		Multiply signed fraction by signed integer				|
;|												|
;|			ldi	Acc,100								|
;|			mov	r0,Acc			;r0=100					|
;|			ldi	Acc,34*128/100							|
;|			mov	r1,Acc			;r1=0.34				|
;|			RCALL	_IF_MUL_SFSI		;r2:r3 signed 16 product		|
;|												|
;+----------------------------------------------------------------------------------------------+
;+--------------------------------------------------------------+--------------------------------------------------------------------------
;| Multiply Unsigned Integer 8 Bits By Signed Fraction 8 bits	|
;|								|
;| Number formats						|
;|								|
;| Signed Fraction						|
;|								|
;|  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0		|
;| +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+		|
;| | s| i| f| f| f| f| f| f| f| f| f| f| f| f| f| f|		|
;| +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+		|
;|								|
;| s = signal 0=positive 1=negative				|
;| i = integer part 						|
;| f = fractionary part						|
;|								|
;| Signed Integer						|
;|								|
;|  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0		|
;| +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+		|
;| | s| i| i| i| i| i| i| i| i| i| i| i| i| i| i| i|		|
;| +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+		|
;|								|
;| Input  : R0:R1  Signed Fractionary number			|
;|	    R2:R3  Signed integer number			|
;|								|
;| Output : R4:R5  Signed Integer result			|
;|								|
;| Destroy: R0,R1,R2,R3,R6,Flags				|
;|								|
;| Clocks: min 156 with rcall					|
;|	   max 187 with rcall					|
;+--------------------------------------------------------------+
#ifndef __INTFRAC_SFSI__
	#define	__INTFRAC_SFSI__
	.message "[ (MATH) INTFRAC SFSI ]"
	_IF_MUL_SFSI:
		push	Acc					;save useds
		mov	r4,r1					;multiply signals
		eor	r4,r3
		tst	r3					;integer part is negative ?
		brpl	_IF_MSFSI05				;no, branch
		com	r2					;else complement integer part
		com	r3
		ldi	Acc,1
		add	r2,Acc
		ldi	Acc,0
		adc	r3,Acc
	_IF_MSFSI05:
		ldi	Acc,15					;number of bits into fractionary part
		clr	r5					;clear my accumulator
		clr	r6
		rol	r0					;to exclude a signal
		rol	r1
	_IF_MSFSI00:						;to get a bit
		rol	r0
		rol	r1
		brcc	_IF_MSFSI10				;carry=0, branch
		add	r5,r2					;else accumulate integer part
		adc	r6,r3
	_IF_MSFSI10:
		lsr	r3					;integer part div 2
		ror	r2
		dec	Acc
		brne	_IF_MSFSI00				;until all  done
		sbrs	r4,7					;skip if result is negative
		rjmp	_IF_MSFSI20				;else branch
		com	r5					;complemente if negative	
		com	r6
		ldi	Acc,1
		add	r5,Acc
		ldi	Acc,0
		adc	r6,Acc
	_IF_MSFSI20:
		mov	r4,r5					;move to apropriate registers
		mov	r5,r6
		pop	Acc					;restore useds
		ret
#endif
.exit

