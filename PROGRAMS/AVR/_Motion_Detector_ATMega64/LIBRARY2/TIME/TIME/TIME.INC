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
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA			|
;| DATA:14/03/2001					|
;>------------------------------------------------------<
;| T I M E  R O U T I N E S	 			|
;+------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM	|
;+------------------------------------------------------+
#ifndef __TIME_TIME__
	#define __TIME_TIME__
	.message "+-----------+"
	.message "|           |"
	.message "| TIME TIME |"
	.message "|           |"
	.message "+-----------+"
	;>------------------------------------------------------<
	;| I M P L E M E N T E D  F U N C T I O N S 		|
	;+------------------------------------------------------+-----------------------------------------------+
	;| _DELAY_MILESECONDS	Wait N Mileseconds passed by Acc:AccH register pair				|
	;|			Sample using _DelayMile to wait a 15ms						|
	;|													|
	;|			LDI	ACCT,LOW(15)		;ACC:ACCH=Time Contante in this case 15ms	|
	;|			LDI	ACCTH,HIGH(15)								|
	;|			RCALL	_DELAY_MILESECONDS	;Make a delay					|
	;+------------------------------------------------------------------------------------------------------
	+
	;   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|  Make a mileseconds Delay MACRO 		 |
	;|\			          		 |
	;| | Destroy : None				 |
	;|/  			          		 |
	;|   	        		  		 |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\__\__\__\__\__\_________________________________________________________
	.MACRO	_M_DELAY_MILESECONDS
		push	Acc
		push	AccH
		ldi	Acc,low(@0)
		ldi	AccH,high(@0)
		rcall	_DELAY_MILESECONDS
		pop	AccH
		pop	Acc
	.ENDMACRO
	;   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|  Make a mileseconds Delay	  		 |
	;|  				  		 |
	;|\			          		 |
	;| | Input  : Acc,AccH Mileseconds Delay	 |
	;|/  Output : None	          		 |
	;|   Destroy: Flags	          		 |
	;|   			       	  		 |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\__\__\__\__\__\______________________
	;				/
	;				| A Mileseconds Equation Time
	;				| T=3+2+4*N-1+2
	;				| Resolving N for T=1ms we have
	;				| N=(F-6000)/4000
	;				\____________________________________
	_DELAY_MILESECONDS:
		push	Acc			;save Useds
		push	AccH
		push	AccT
		push	AccTH
	_DELAY_M00:
		subi	Acc,1			;time end ?
		sbci	AccH,0
		brcs	_DELAY_M20		;yes, branch to the end
		ldi	AccT,low((_AVR_CLOCK-6000)/4000)	;start 1ms delay
		ldi	AccTH,high((_AVR_CLOCK-6000)/4000)
	_DELAY_M10:
		subi	AccT,1
		sbci	AccTH,0
		brcc	_DELAY_M10
		rjmp	_DELAY_M00
	_DELAY_M20:
		pop	AccTH
		pop	AccT
		pop	AccH
		pop	Acc
		ret
#endif
.exit

