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
;+------------------------------------------------------+
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA			|
;| DATA: 24/03/2008					|
;>------------------------------------------------------<
;| H A N D D L E S  I N T E R R U P T S  F O R 		|
;| A T M E G A 1 6 2			 		|
;+------------------------------------------------------+
#ifndef __M162_HANDDLES__
	#define __M162_HANDDLES__
	#ifndef __GLOBAL_DEFINITIONS__
		.message "/--------------------------------\"
		.message "|GLOBAL DEFINITIONS NOT INCLUDED |"
		.message "\--------------------------------/"
	#endif
	.message "+--------------+"
	.message "|              |"
	.message "| M162 HANDDLES |"
	.message "|              |"
	.message "+--------------+"
	;+----------------------------------+
	;| HANDDLE SRAM VECTORS DEFINITIONS |
	;+----------------------------------+
	.DSEG
		_HDC_START:
		_HDC_INT0_VECT:		.BYTE 2		;use thats labels names to set handdle address routines
		_HDC_INT1_VECT:		.BYTE 2
		_HDC_INT2_VECT:		.BYTE 2
		_HDC_PCINT0_VECT:	.BYTE 2
		_HDC_PCINT1_VECT:	.BYTE 2
		_HDC_TIMER3_CAPT_VECT:	.BYTE 2
		_HDC_TIMER3_COMPA_VECT:	.BYTE 2
		_HDC_TIMER3_COMPB_VECT:	.BYTE 2
		_HDC_TIMER3_OVF_VECT:	.BYTE 2
		_HDC_TIMER2_COMP_VECT:	.BYTE 2
		_HDC_TIMER2_OVF_VECT:	.BYTE 2
		_HDC_TIMER1_CAPT_VECT:	.BYTE 2
		_HDC_TIMER1_COMPA_VECT:	.BYTE 2
		_HDC_TIMER1_COMPB_VECT:	.BYTE 2
		_HDC_TIMER1_OVF_VECT:	.BYTE 2
		_HDC_TIMER0_COMP_VECT:	.BYTE 2
		_HDC_TIMER0_OVF_VECT:	.BYTE 2
		_HDC_SPI_VECT:		.BYTE 2
		_HDC_USART0_RXC_VECT:	.BYTE 2
		_HDC_USART1_RXC_VECT:	.BYTE 2
		_HDC_USART0_UDRE_VECT:	.BYTE 2
		_HDC_USART1_UDRE_VECT:	.BYTE 2
		_HDC_USART0_TXC_VECT:	.BYTE 2
		_HDC_USART1_TXC_VECT:	.BYTE 2
		_HDC_EE_RDY_VECT:	.BYTE 2
		_HDC_ANA_COMP_VECT:	.BYTE 2
		_HDC_SPM_RDY_VECT:	.BYTE 2
		_HDC_END:
	.CSEG
	;+----------------------------------+
	;| HANDDLE JUMP VECTORS DEFINITIONS |
	;+----------------------------------+
	jmp	_HDC_INIT		;jump to initialization routine
	jmp	_HDC_INT0_PROC
	jmp	_HDC_INT1_PROC
	jmp	_HDC_INT2_PROC
	jmp	_HDC_PCINT0_PROC
	jmp	_HDC_PCINT1_PROC
	jmp	_HDC_TIMER3_CAPT_PROC
	jmp	_HDC_TIMER3_COMPA_PROC
	jmp	_HDC_TIMER3_COMPB_PROC
	jmp	_HDC_TIMER3_OVF_PROC
	jmp	_HDC_TIMER2_COMP_PROC
	jmp	_HDC_TIMER2_OVF_PROC
	jmp	_HDC_TIMER1_CAPT_PROC
	jmp	_HDC_TIMER1_COMPA_PROC
	jmp	_HDC_TIMER1_COMPB_PROC
	jmp	_HDC_TIMER1_OVF_PROC
	jmp	_HDC_TIMER0_COMP_PROC
	jmp	_HDC_TIMER0_OVF_PROC
	jmp	_HDC_SPI_PROC
	jmp	_HDC_USART0_RXC_PROC
	jmp	_HDC_USART1_RXC_PROC
	jmp	_HDC_USART0_UDRE_PROC
	jmp	_HDC_USART1_UDRE_PROC
	jmp	_HDC_USART0_TXC_PROC
	jmp	_HDC_USART1_TXC_PROC
	jmp	_HDC_EE_RDY_PROC
	jmp	_HDC_ANA_COMP_PROC
	jmp	_HDC_SPM_RDY_PROC
	;+----------------------
	;| INITIALIZE STACK
	;+----------------------
	.EQU	RAMSTART	= 0X100
	_HDC_INIT:
		ldi	Acc,low(RAMEND)
		out	SPL,Acc
		ldi	Acc,high(RAMEND)
		out	SPH,Acc
	
	.INCLUDE "DEFS\SRAM_INITIALIZATION\SRAM_INIT.INC"		;SRAM & Stack initialization include file
		jmp	_MAIN_INIT		;jump to main initializations
	;+----------------------------------+
	;| HANDDLE MACRO		    |
	;+----------------------------------+
	.MACRO	_M_HANDDLE
		push	ZL
		push	ZH
		lds	ZL,@0+0
		lds	ZH,@0+1
		rjmp	_HDC_EXIT
	.ENDMACRO
	;+----------------------------------+
	;| HANDDLES 			    |
	;+----------------------------------+
	_HDC_INT0_PROC:
		_M_HANDDLE	_HDC_INT0_VECT
	_HDC_INT1_PROC:
		_M_HANDDLE	_HDC_INT1_VECT
	_HDC_INT2_PROC:
		_M_HANDDLE	_HDC_INT2_VECT
	_HDC_PCINT0_PROC:
		_M_HANDDLE	_HDC_PCINT0_VECT
	_HDC_PCINT1_PROC:
		_M_HANDDLE	_HDC_PCINT1_VECT
	_HDC_TIMER3_CAPT_PROC:
		_M_HANDDLE	_HDC_TIMER3_CAPT_VECT
	_HDC_TIMER3_COMPA_PROC:
		_M_HANDDLE	_HDC_TIMER3_COMPA_VECT
	_HDC_TIMER3_COMPB_PROC:
		_M_HANDDLE	_HDC_TIMER3_COMPB_VECT
	_HDC_TIMER3_OVF_PROC:
		_M_HANDDLE	_HDC_TIMER3_OVF_VECT
	_HDC_TIMER2_COMP_PROC:
		_M_HANDDLE	_HDC_TIMER2_COMP_VECT
	_HDC_TIMER2_OVF_PROC:
		_M_HANDDLE	_HDC_TIMER2_OVF_VECT
	_HDC_TIMER1_CAPT_PROC:
		_M_HANDDLE	_HDC_TIMER1_CAPT_VECT
	_HDC_TIMER1_COMPA_PROC:
		_M_HANDDLE	_HDC_TIMER1_COMPA_VECT
	_HDC_TIMER1_COMPB_PROC:
		_M_HANDDLE	_HDC_TIMER1_COMPB_VECT
	_HDC_TIMER1_OVF_PROC:
		_M_HANDDLE	_HDC_TIMER1_OVF_VECT
	_HDC_TIMER0_COMP_PROC:
		_M_HANDDLE	_HDC_TIMER0_COMP_VECT
	_HDC_TIMER0_OVF_PROC:
		_M_HANDDLE	_HDC_TIMER0_OVF_VECT
	_HDC_SPI_PROC:		
		_M_HANDDLE	_HDC_SPI_VECT
	_HDC_USART0_RXC_PROC:
		_M_HANDDLE	_HDC_USART0_RXC_VECT
	_HDC_USART1_RXC_PROC:
		_M_HANDDLE	_HDC_USART1_RXC_VECT
	_HDC_USART0_UDRE_PROC:
		_M_HANDDLE	_HDC_USART0_UDRE_VECT
	_HDC_USART1_UDRE_PROC:
		_M_HANDDLE	_HDC_USART1_UDRE_VECT
	_HDC_USART0_TXC_PROC:
		_M_HANDDLE	_HDC_USART0_TXC_VECT
	_HDC_USART1_TXC_PROC:
		_M_HANDDLE	_HDC_USART1_TXC_VECT
	_HDC_EE_RDY_PROC:
		_M_HANDDLE	_HDC_EE_RDY_VECT
	_HDC_ANA_COMP_PROC:
		_M_HANDDLE	_HDC_ANA_COMP_VECT
	_HDC_SPM_RDY_PROC:
		_M_HANDDLE	_HDC_SPM_RDY_VECT
	;------------------<<  COMMON RETURN ADDRESS   >>----------------------------------------------------
	_HDC_EXIT:
		icall				;call routine
		pop	ZH
		pop	ZL
		reti				;return from interrupt
	_HDC_RETURN:
		ret				;return from interrupt routine
	
#endif
.exit

