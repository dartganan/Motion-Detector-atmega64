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
;| DATA:21/10/2002					|
;>------------------------------------------------------<
;| A V R   M 1 2 8  U A R T  R O U T I N E S	  	|
;+------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM	|
;+------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _USART_SET_COMM_PORT	DEFINE COMM PORT							|
;|												|
;|			LDI	ACC,COMM1		;DEFINE COMMUNICATION PORT1		|
;|			RCALL	_USART_SET_COMM_PORT						|
;|----------------------------------------------------------------------------------------------|
;| _USART_GET_COMM_PORT	GET ACTUAL COMM PORT							|
;|												|
;|			RCALL	_USART_GET_COMM_PORT	;AFTER THIS CALL ACC=COMM PORT	 	|
;|----------------------------------------------------------------------------------------------|
;| _USART_SET_NORMAL_SPEED  SET USART NORMAL SPEED 						|
;|												|
;|			RCALL	_USART_SET_NORMAL_SPEED	;NOW USART IN NORMAL SPEED	 	|
;|----------------------------------------------------------------------------------------------|
;| _USART_SET_DOUBLE_SPEED  SET USART DOUBLE SPEED 						|
;|												|
;|			RCALL	_USART_SET_DOUBLE_SPEED	;NOW USART IN DOUBLE SPEED	 	|
;|----------------------------------------------------------------------------------------------|
;| _USART_SET_BAUD_RATE DEFINE USART BAUD RATE	 						|
;|												|
;|			RCALL	_USART_SET_NORMAL_SPEED	;FOR RIGHT SPEED SETTINGS		|
;|			LDIAW	9600			;SET USART FOR 9600 BAUDS		|
;|			RCALL	_USART_SET_BAUD_RATE	;NOW USART OPERATING IN 9600 BAUDS	|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX_ENABLE	ENABLE/DISABLE TX							|
;|												|
;|			LDI	ACC,_ON		;ENABLE TX					|
;|			RCALL	_USART_TX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_RX_ENABLE	ENABLE/DISABLE RX							|
;|												|
;|			LDI	ACC,_ON		;ENABLE RX					|
;|			RCALL	_USART_RX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX_INT_ENABLE	ENABLE/DISABLE TX INTERRUPT 						|
;|												|
;|			LDI	ACC,_ON		;ENABLE TX					|
;|			RCALL	_USART_TX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_RX_INT_ENABLE	ENABLE/DISABLE RX INTERRUPT						|
;|												|
;|			LDI	ACC,_ON		;ENABLE RX					|
;|			RCALL	_USART_RX_ENABLE						|
;|----------------------------------------------------------------------------------------------|
;| _USART_TX		SEND A BYTE TO USART							|
;|												|
;|			LDI	ACC,'A'		;send a ascii 'A'				|
;|			RCALL	_USART_TX							|
;|												|
;| OBS: This routine wait last transmission done before send a new byte				|
;+----------------------------------------------------------------------------------------------+
;| _USART_RX		WAIT A BYTE FROM USART							|
;|												|
;|			LDIAWT	100		;wait 100ms for receive a byte			|
;|						;set this register to -1 to infinit wait	|
;|			RCALL	_USART_RX							|
;|												|
;+----------------------------------------------------------------------------------------------+
#ifndef __INTEGER_DIV_U32U32U32__
	.INCLUDE "MATH\INTEGER\DIV_U32U32U32\DIV_U32U32U32.INC"
#endif
#ifndef __AVR_M128_USART__
	#define __AVR_M128_USART__
	.message "+----------------+"
	.message "|                |"
	.message "| AVR M128 USART |"
	.message "|                |"
	.message "+----------------+"
	.DSEG
	_USART_COMM_PORT:	.BYTE 1
	.CSEG
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART SET COMM PORT	  |
	;|  				  |
	;|\  Input : Acc COMM 0 or 1      |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_SET_COMM_PORT:
		sts	_USART_COMM_PORT,Acc
		ret	
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART GET COMM PORT	  |
	;|  				  |
	;|\  Input : None		  |
	;| | Output: None		  |
	;|/  Destroy:Acc comm port	  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_GET_COMM_PORT:
		lds	Acc,_USART_COMM_PORT
		ret	
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART SET NORMAL SPEED	  |
	;|  				  |
	;|\  Input : None		  |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_SET_NORMAL_SPEED:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0							;COMM 0 ?
		breq	_USART_SET_NOR10					;yes, branch
		cpi	Acc,1							;COMM 1 ?
		breq	_USART_SET_NOR20					;yes, branch
	_USART_SET_NOR00:
		pop	Acc
		ret
	_USART_SET_NOR10:
		;comm 0
		in	Acc,UCSR0A
		andi	Acc,~(1<<U2X0)
		out	UCSR0A,Acc
		rjmp	_USART_SET_NOR00
	_USART_SET_NOR20:
		;comm 1
		lds	Acc,UCSR1A
		andi	Acc,~(1<<U2X1)
		sts	UCSR1A,Acc
		rjmp	_USART_SET_NOR00
	
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   USART SET DOUBLE SPEED	  |
	;|  				  |
	;|\  Input : None		  |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	_USART_SET_DOUBLE_SPEED:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0							;COMM 0 ?
		breq	_USART_SET_DOU10					;yes, branch
		cpi	Acc,1							;COMM 1 ?
		breq	_USART_SET_DOU20					;yes, branch
	_USART_SET_DOU00:
		pop	Acc
		ret
	_USART_SET_DOU10:
		;comm 0
		in	Acc,UCSR0A
		ori	Acc,(1<<U2X0)
		out	UCSR0A,Acc
		rjmp	_USART_SET_DOU00
	_USART_SET_DOU20:
		;comm 1
		lds	Acc,UCSR1A
		ori	Acc,(1<<U2X1)
		sts	UCSR1A,Acc
		rjmp	_USART_SET_DOU00
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART BAUD RATE		  |
	;|  				  |
	;|\  Input : AccH:Acc BAUD RATE   |
	;| | Output: None		  |
	;|/  Destroy:Flags,r0..r11	  |
	;|   obs: assume normal speed	  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_______
	;				       \
	;					+---------------------------------------+
	;					| BAUD rate equation 			|
	;					|					|
	;					|		AVR_CLOCK		|
	;					| BAUD = ---------------------		|
	;					|		16(UBRR+1)		|
	;					|					|
	;					| Resolvin in UBRR,			|
	;					|					|
	;					|		AVR_CLOCK		|
	;					| UBRR = ------------------ - 1		|
	;					|		16.BAUD			|
	;					+---------------------------------------+
	_USART_SET_BAUD_RATE:
		push	Acc
		push	AccH
		mov	r4,Acc							;move baud rate to appropriate registers
		mov	r5,AccH
		clr	r6
		clr	r7
		ldi	Acc,4							;multiply by 16
	_USART_SET_BAU00:
		lsl	r4
		rol	r5
		rol	r6
		rol	r7
		dec	Acc
		brne	_USART_SET_BAU00
		ldi	Acc,low(lwrd(_AVR_CLOCK))				;get avr clock
		mov	r0,Acc
		ldi	Acc,high(lwrd(_AVR_CLOCK))				;get avr clock
		mov	r1,Acc
		ldi	Acc,low(hwrd(_AVR_CLOCK))				;get avr clock
		mov	r2,Acc
		ldi	Acc,high(hwrd(_AVR_CLOCK))				;get avr clock
		mov	r3,Acc
		call	_DIV_U32U32U32						;AVR_CLOCK/16*BAUD
		ldi	Acc,-1
		add	r0,Acc							;AVR_CLOCK/(16*BAUD)-1
		adc	r1,Acc
		adc	r2,Acc
		adc	r3,Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0							;COMM 0 ?
		breq	_USART_SET_BAU20					;yes, branch
		cpi	Acc,1							;COMM 1 ?
		breq	_USART_SET_BAU30					;yes, branch
	_USART_SET_BAU10:
		pop	AccH
		pop	Acc
		ret
	_USART_SET_BAU20:
		;COMM 0
		out	UBRR0L,r0						;set baud rate to comm 0
		sts	UBRR0H,r1
		rjmp	_USART_SET_BAU10
		;COMM 1
	_USART_SET_BAU30:
		sts	UBRR1L,r0						;set baud rate to comm 1
		sts	UBRR1H,r1
		rjmp	_USART_SET_BAU10
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE TX	  |
	;|  				  |
	;|\  Input : Acc ON/OFF	          |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_TX_ENABLE:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_TX_10					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_TX_20					;yes, branch
	_USART_TX_00:
		pop	Acc
		ret
	_USART_TX_10:
		;comm 0
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TX_10_10					;no, branch
		in	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<TXEN0)
	_USART_TX_10_00:
		out	UCSR0B,Acc
		rjmp	_USART_TX_00
	_USART_TX_10_10:
		in	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<TXEN0)
		rjmp	_USART_TX_10_00
	_USART_TX_20:
		;comm 1
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TX_20_10					;no, branch
		lds	Acc,UCSR1B					;enable tx comm 1
		ori	Acc,(1<<TXEN1)
	_USART_TX_20_00:
		sts	UCSR1B,Acc
		rjmp	_USART_TX_00
	_USART_TX_20_10:
		lds	Acc,UCSR1B					;disable tx comm 1
		andi	Acc,~(1<<TXEN1)
		rjmp	_USART_TX_20_00
	
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE TX INTERRUPT   |
	;|  				  	|
	;|\  Input : Acc ON/OFF	          	|
	;| | Output: None		  	|
	;|/  Destroy:None		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_TX_INT_ENABLE:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_TXI_10					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_TXI_20					;yes, branch
	_USART_TXI_00:
		pop	Acc
		ret
	_USART_TXI_10:
		;comm 0
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TXI_10_10				;no, branch
		in	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<TXCIE0)
	_USART_TXI_10_00:
		out	UCSR0B,Acc
		rjmp	_USART_TXI_00
	_USART_TXI_10_10:
		in	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<TXCIE0)
		rjmp	_USART_TXI_10_00
	_USART_TXI_20:
		;comm 1
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_TXI_20_10				;no, branch
		lds	Acc,UCSR1B					;enable tx comm 1
		ori	Acc,(1<<TXCIE1)
	_USART_TXI_20_00:
		sts	UCSR1B,Acc
		rjmp	_USART_TXI_00
	_USART_TXI_20_10:
		lds	Acc,UCSR1B					;disable tx comm 1
		andi	Acc,~(1<<TXCIE1)
		rjmp	_USART_TXI_20_00
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE RX	  |
	;|  				  |
	;|\  Input : Acc ON/OFF	          |
	;| | Output: None		  |
	;|/  Destroy:None		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_RX_ENABLE:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_RX_10					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_RX_20					;yes, branch
	_USART_RX_00:
		pop	Acc
		ret
	_USART_RX_10:
		;comm 0
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_RX_10_10					;no, branch
		in	Acc,UCSR0B					;enable tx comm 0
		ori	Acc,(1<<RXEN0)
	_USART_RX_10_00:
		out	UCSR0B,Acc
		rjmp	_USART_RX_00
	_USART_RX_10_10:
		in	Acc,UCSR0B					;disable tx comm 0
		andi	Acc,~(1<<RXEN0)
		rjmp	_USART_RX_10_00
	_USART_RX_20:
		;comm 1
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_RX_20_10					;no, branch
		lds	Acc,UCSR1B					;enable tx comm 1
		ori	Acc,(1<<RXEN1)
	_USART_RX_20_00:
		sts	UCSR1B,Acc
		rjmp	_USART_RX_00
	_USART_RX_20_10:
		lds	Acc,UCSR1B					;disable tx comm 1
		andi	Acc,~(1<<RXEN1)
		rjmp	_USART_RX_20_00
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART ENABLE/DISABLE RX INTERRUPT	|
	;|  				 	|
	;|\  Input : Acc ON/OFF	          	|
	;| | Output: None		  	|
	;|/  Destroy:None		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\_________________________________________________________________________
	_USART_RX_INT_ENABLE:
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_RXI_10					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_RXI_20					;yes, branch
	_USART_RXI_00:
		pop	Acc
		ret
	_USART_RXI_10:
		;comm 0
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_RXI_10_10				;no, branch
		in	Acc,UCSR0B					;enable rx comm 0
		ori	Acc,(1<<RXCIE0)
	_USART_RXI_10_00:
		out	UCSR0B,Acc
		rjmp	_USART_RXI_00
	_USART_RXI_10_10:
		in	Acc,UCSR0B					;disable rx comm 0
		andi	Acc,~(1<<RXCIE0)
		rjmp	_USART_RXI_10_00
	_USART_RXI_20:
		;comm 1
		pop	Acc
		push	Acc
		cpi	Acc,_ON						;ON ?
		brne	_USART_RXI_20_10				;no, branch
		lds	Acc,UCSR1B					;enable rx comm 1
		ori	Acc,(1<<RXCIE1)
	_USART_RXI_20_00:
		sts	UCSR1B,Acc
		rjmp	_USART_RXI_00
	_USART_RXI_20_10:
		lds	Acc,UCSR1B					;disable rx comm 1
		andi	Acc,~(1<<RXCIE1)
		rjmp	_USART_RXI_20_00
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART TX DIRECT		  |
	;|  				  |
	;|\  Input : Acc byte to be send  |
	;| | Output: None		  |	
	;|/  Destroy:none		  |
	;|   				  |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	_USART_TX:
		push	r0
		push	Acc
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_TX10E					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_TX20E					;yes, branch
		pop	Acc
	_USART_TX00:
		pop	r0
		ret
	_USART_TX10E:
		pop	Acc
	_USART_TX10:
		;COMM 0
		sbis	UCSR0A,UDRE0					;wait last transmission done
		rjmp	_USART_TX10					;*<NOCHANGE>*
		out	UDR0,Acc					;set data to be sent
		rjmp	_USART_TX00
	_USART_TX20E:
		pop	Acc
	_USART_TX20:
		;COMM 1
		lds	r0,UCSR1A
		sbrs	r0,UDRE1					;wait last transmission done
		rjmp	_USART_TX20
		sts	UDR1,Acc					;set data to be sent
		rjmp	_USART_TX00					;*<NOCHANGE>*
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   UART RX DIRECT			|
	;|  					|
	;|\  Input : AccT:AccTH Mileseconds	|
	;| |			Time Out	|
	;| | Output: cy=1 if time out occur 	|
	;| |	     Acc byte received		|
	;|/  Destroy:none		  	|
	;|   				  	|
	;|   			       	  	|
	;+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\__\__\____________________________________________________________________
	;					/
	;					| Obs: if Time Out = 65535 then this routine no exit until
	;					|      UDR not receive a byte]
	;					\_________________________________________________________________
	_USART_RX:
		lds	Acc,_USART_COMM_PORT
		cpi	Acc,0						;COMM 0 ?
		breq	_USART_RXM10					;yes, branch
		cpi	Acc,1						;COMM 1 ?
		breq	_USART_RXM20					;yes, branch
	_USART_RXM00:
		ret
	_USART_RXM10:
		;COMM 0
		push	AccT			;save useds
		push	AccTH
		cpi	AccT,0xFF		;see if time out = 65535
		brne	_USART_RX30		;no, branch to timeout receive
		cpi	AccTH,0xFF
		brne	_USART_RX30		;no, branch to timeout receive
	_USART_RX00:
		sbis	UCSR0A,RXC0		;wait character reception for infinit time until character received
		rjmp	_USART_RX00
	_USART_RX10:
		in	Acc,UDR0		;get reseived data
		clc				;set no time out 
	_USART_RX20:
		pop	AccTH			;restore useds
		pop	AccT
		ret
	_USART_RX30:
		subi	AccT,1			;decrement mileseconds time counter
		sbci	AccTH,0
		brcs	_USART_RX20		;time expired? branch if yes
		push	AccT			;save AccT:AccTH
		push	AccTH
		ldi	AccT,low((_AVR_CLOCK-2000)/6000)		;load constant for 1ms delay
		ldi	AccTH,high((_AVR_CLOCK-2000)/6000)
		#ifdef __DEBUG_MODE__
			ldi	AccT,1
			ldi	AccTH,0
		#endif
	_USART_RX40:
		subi	AccT,1			;1ms delay expired ?
		sbci	AccTH,0
		brcs	_USART_RX50		;yes, brach
		sbis	UCSR0A,RXC0		;else see if data received
		rjmp	_USART_RX40		;no, wait data and process time out
		pop	AccTH			;received, restore AccT:AccTH and get data
		pop	AccT
		rjmp	_USART_RX10
	_USART_RX50:
		pop	AccTH			;restore AccT:AccTH
		pop	AccT
		rjmp	_USART_RX30		;decrement mileseconds time counter
		;COMM 1
	_USART_RXM20:
		push	r0
		push	AccT			;save useds
		push	AccTH
		cpi	AccT,0xFF		;see if time out = 65535
		brne	_USART_RX130		;no, branch to timeout receive
		cpi	AccTH,0xFF
		brne	_USART_RX130		;no, branch to timeout receive
	_USART_RX100:
		lds	r0,UCSR1A
		sbrs	r0,RXC1			;wait character reception for infinit time until character received
		rjmp	_USART_RX100
	_USART_RX110:
		lds	Acc,UDR1		;get reseived data
		clc				;set no time out 
	_USART_RX120:
		pop	AccTH			;restore useds
		pop	AccT
		pop	r0
		ret
	_USART_RX130:
		subi	AccT,1			;decrement mileseconds time counter
		sbci	AccTH,0
		brcs	_USART_RX120		;time expired? branch if yes
		push	AccT			;save AccT:AccTH
		push	AccTH
		ldi	AccT,low((_AVR_CLOCK-2000)/6000)		;load constant for 1ms delay
		ldi	AccTH,high((_AVR_CLOCK-2000)/6000)
		#ifdef __DEBUG_MODE__
			ldi	AccT,1
			ldi	AccTH,0
		#endif
	_USART_RX140:
		subi	AccT,1			;1ms delay expired ?
		sbci	AccTH,0
		brcs	_USART_RX150		;yes, brach
		lds	r0,UCSR1A
		sbrs	r0,RXC1			;else see if data received
		rjmp	_USART_RX140		;no, wait data and process time out
		pop	AccTH			;received, restore AccT:AccTH and get data
		pop	AccT
		rjmp	_USART_RX110
	_USART_RX150:
		pop	AccTH			;restore AccT:AccTH
		pop	AccT
		rjmp	_USART_RX130		;decrement mileseconds time counter
#endif
.exit

