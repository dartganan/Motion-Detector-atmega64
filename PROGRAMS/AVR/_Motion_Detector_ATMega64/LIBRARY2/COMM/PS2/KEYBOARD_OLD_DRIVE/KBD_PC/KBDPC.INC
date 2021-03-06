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
;| DATA:22/03/2001					|
;>------------------------------------------------------<
;| P S 2  K E Y B O A R D  P R O T O C O L 		|
;+------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM	|
;+------------------------------------------------------+
#ifndef __KDBPC__
	#define __KBDPC__
	
	.message "+-----------+"
	.message "|   KDBPC   |" 
	.message "+-----------+"
	;+-------------------------+
	;| KEYBOARD RESPONSE CODES |
	;+-------------------------+
	.EQU	_PS2KBD_CODE_ACK	= 0XFA
	.EQU	_PS2KBD_CODE_POWERGOOD	= 0XAA
	.EQU	_PS2KBD_CODE_ECHO	= 0XEE
	.EQU	_PS2KBD_CODE_RESEND	= 0XFE
	.EQU	_PS2KBD_CODE_OVERFLOW	= 0X00
	;+-------------------------+
	;| KEYBOARD COMMANDS	   |
	;+-------------------------+
	.EQU	_PS2KBD_CMD_DISABLE		= 0XF5
	.EQU	_PS2KBD_CMD_ECHO		= 0XEE
	.EQU	_PS2KBD_CMD_ENABLE		= 0XF4
	.EQU	_PS2KBD_CMD_READID		= 0XF2
	.EQU	_PS2KBD_CMD_RESEND		= 0XFE
	.EQU	_PS2KBD_CMD_RESET		= 0XFF
	.EQU	_PS2KBD_CMD_SETSCANCODE		= 0XF0
	.EQU	_PS2KBD_CMD_SETTYPEMATIC	= 0XF7
	.EQU	_PS2KBD_CMD_SETMAKEBREAK	= 0XF8
	.EQU	_PS2KBD_CMD_SETMAKE		= 0XF9
	.EQU	_PS2KBD_CMD_TYPEMAKEBREAK 	= 0XFA
	.EQU	_PS2KBD_CMD_DEFAULT		= 0XF6
	.EQU	_PS2KBD_CMD_SETKEYTYPE		= 0XFB
	.EQU	_PS2KBD_CMD_SETKEYMAKEBREAK	= 0XFC
	.EQU	_PS2KBD_CMD_SETKEYAGAIN		= 0XFD
	.EQU	_PS2KBD_CMD_SETSTATUS		= 0XED
	.EQU	_PS2KBD_CMD_RATEDELAY		= 0XF3
	;+-------------------------+
	;| SRAM VARIABLES INDEXES  |
	;+-------------------------+
	.DSEG
				;+------------------------------+
				;| RATEDELAY			|
				;|				|
				;| BIT 7 = 0 NOT USED		|
				;| BIT 6..5 = REPEAT DELAY	|
				;|	      00 -  250 Ms	|
				;|	      01 -  500 Ms	|
				;|	      10 -  750 Ms	|
				;|	      11 - 1000 Ms	|
				;|				|
				;| BIT 4..0 = REPEAT RATE	|
				;|	      00000 - 30 c/sec	|
				;|	      11111 -  2 c/sec	|
				;+------------------------------+
	_PS2KBD_RATEDELAY:	 .BYTE 1
				;+------------------------------+
				;| STATUS INDICATOR		|
				;|				|
				;| BIT 7..3 = 0			|
				;| BIT 2    = CAPS LOCK		|
				;| BIT 1    = NUM LOCK		|
				;| BIT 0    = SCROLL LOCK	|
				;|				|
				;| 1 = Status ON		|
				;+------------------------------+
	.EQU	_PS2KBD_CAPS_LOCK_BIT	= 2
	.EQU	_PS2KBD_NUM_LOCK_BIT	= 1
	.EQU	_PS2KBD_SCROLL_LOCK_BIT	= 0
	_PS2KBD_STATUSINDICATOR: .BYTE 1
	_PS2KBD_SCANCODE: 	.BYTE 1 	;scan code nunber 1,2 or 3
	.CSEG
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   KBD INITIALIZE		  |
	;|  				  |
	;|\			          |
	;| | Input  : None	          |
	;|/  Output : None	          |
	;|   Destroy: None	          |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	_PS2KBD_INIT:
		push	Acc
		in	Acc,SREG					;Save SREG state
		cli							;to handdling ports correctly
		sbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;set keyboard clock to high state
		sbi	_PS2KBD_DATA_PORT_OUTPUT,_PS2KBD_DATA_BIT	;Set keyboard data to high state
		sbi	_PS2KBD_CLOCK_PORT_DIR,_PS2KBD_CLOCK_BIT	;Set keyboard clock as output
		sbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT		;Set keyboard data as output	
		out	SREG,Acc					;Restore SREG state
		pop	Acc
		ret
	;+---------------------------------------+
	;|					 |
	;| START ENTRY FOR KEYBOARD HANDDLE	 |
	;|					 |
	;+---------------------------------------+----------------------------------------------------------------------------
	_PS2KBD_HANDDLE:
		cbi	_PS2KBD_CLOCK_PORT_DIR,_PS2KBD_CLOCK_BIT	;set DATA and CLOCK to input
		cbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT
		nop
		sbis	_PS2KBD_CLOCK_PORT_�NPUT,_PS2KBD_CLOCK_BIT	;see if CLOCK=high and DATA=low, request PC comm status
		rjmp	_PS2KBD_EXIT					;no, exit handdle
		sbic	_PS2KBD_DATA_PORT_�NPUT,_PS2KBD_DATA_BIT
		rjmp	_PS2KBD_EXIT					;no, exit handdle
		rcall	_PS2KBD_RXD					;get data
		brcc	_PS2KBD_H00					;continue if no error
		rjmp	_PS2KBD_EXIT					;else exit handdle
	_PS2KBD_H00:
		clr	AccH						;set to max time 
		out	TCNT0,AccH					;set Time out condition
	;+----------------------------------
	;| COMMAND IS SET STATUS ?
	;+----------------------------------
		cpi	Acc,_PS2KBD_CMD_SETSTATUS			;get status ?
		brne	_PS2KBD_H20					;no, branch
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rcall	_PS2KBD_RXD					;get data again
		brcc	_PS2KBD_H10
		rjmp	_PS2KBD_EXIT					;exit if error
	_PS2KBD_H10:
		sts	_PS2KBD_STATUSINDICATOR,Acc			;store status
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rjmp	_PS2KBD_EXIT
	;+----------------------------------
	;| COMMAND IS ECHO ?
	;+----------------------------------
	_PS2KBD_H20:
		cpi	Acc,_PS2KBD_CMD_ECHO				;echo ?
		brne	_PS2KBD_H30
		ldi	Acc,_PS2KBD_CODE_ECHO				;awnser echo
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT
	;+----------------------------------
	;| COMMAND IS TYPEMATIC RATE/DELAY ?
	;+----------------------------------
	_PS2KBD_H30:
		cpi	Acc,_PS2KBD_CMD_RATEDELAY			;typematic rate/delay ?
		brne	_PS2KBD_H50
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rcall	_PS2KBD_RXD					;get data again
		brcc	_PS2KBD_H40
		rjmp	_PS2KBD_EXIT					;exit if error
	_PS2KBD_H40:
		sts	_PS2KBD_RATEDELAY,Acc				;store status
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| COMMAND IS ENABLE KEYBOARD ?
	;+----------------------------------
	_PS2KBD_H50:
		cpi	Acc,_PS2KBD_CMD_ENABLE				;enable ?
		brne	_PS2KBD_H60
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser ack
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| COMMAND IS DISABLE KEYBOARD ?
	;+----------------------------------
	_PS2KBD_H60:
		cpi	Acc,_PS2KBD_CMD_DISABLE				;disable ?
		brne	_PS2KBD_H70
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser ack
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| COMMAND IS RESET KEYBOARD ?
	;+----------------------------------
	_PS2KBD_H70:
		cpi	Acc,_PS2KBD_CMD_RESET				;reset ?
		brne	_PS2KBD_H80
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser ack
		rcall	_PS2KBD_TXD
		ldi	Acc,_PS2KBD_CODE_POWERGOOD			;test ok
		rcall	_PS2KBD_TXD
		ldi	Acc,0b01111111					;set defalt ratedelay 
									;delay 1000Ms
									;rate 2 char/s
		sts	_PS2KBD_RATEDELAY,Acc
		ldi	Acc,(1<<_PS2KBD_NUM_LOCK_BIT)			;set locks indicator
									;caps lock off
									;num lock on
									;scroll lock off
		sts	_PS2KBD_STATUSINDICATOR,Acc
		ldi	Acc,2						;set scan code 2
		sts	_PS2KBD_SCANCODE,Acc
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| COMMAND IS READ ID KEYBOARD ?
	;+----------------------------------
	_PS2KBD_H80:
		cpi	Acc,_PS2KBD_CMD_READID				;read id ?
		brne	_PS2KBD_H90
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser ack
		rcall	_PS2KBD_TXD
		ldi	Acc,0XAB					;awnser 0XAB
		rcall	_PS2KBD_TXD
		ldi	Acc,0X83					;awnser 0X83
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| SET SCAN CODE
	;+----------------------------------
	_PS2KBD_H90:
		cpi	Acc,_PS2KBD_CMD_SETSCANCODE			;set scan code ?
		brne	_PS2KBD_H120
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rcall	_PS2KBD_RXD					;get data again
		brcc	_PS2KBD_H100
		rjmp	_PS2KBD_EXIT					;exit handdle
	_PS2KBD_H100:
		tst	Acc						;zero ?
		breq	_PS2KBD_H110
		sts	_PS2KBD_SCANCODE,Acc				;store status
		ldi	Acc,_PS2KBD_CODE_ACK
		rcall	_PS2KBD_TXD					;awnser ack
		rjmp	_PS2KBD_EXIT					;exit handdle
	_PS2KBD_H110:
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser ack and scancode set if zero sented
		rcall	_PS2KBD_TXD
		lds	Acc,_PS2KBD_SCANCODE				;last scan code
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT					;exit handdle
	;+----------------------------------
	;| COMMAND IS SET DEFAULT KEYBOARD ?
	;+----------------------------------
	_PS2KBD_H120:
		cpi	Acc,_PS2KBD_CMD_DEFAULT				;default set ?
		brne	_PS2KBD_H130
		ldi	Acc,_PS2KBD_CODE_ACK				;awnser	ack
		rcall	_PS2KBD_TXD
		rjmp	_PS2KBD_EXIT					;exit handdle
	_PS2KBD_H130:
	_PS2KBD_EXIT:
		ret
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   PS2KBD_RXD			  |
	;|   				  |
	;|\  Input  : none	          |
	;| | Output : Acc data arrived	  |
	;|/	      Cy=0 if data ok     |
	;|   Destroy: Flags	          |
	;|   obs    : Interrupts disable  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________________________
	;				/ KEYBOARD RECEPTION EVENTS NUMBERS	 						\
	;				|											|
	;				| EVENT NUMBER  -   2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 					|
	;				| 		    5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0			|
	;				| KEYBOARD CLOCK  ---+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-----------------	|
	;				|		   ^ | | | | | | | | | | | | | | | | | | | | | |			|
	;				|		   | +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ 			|
	;				|		   |									|
	;				| 		   +- RECEIVE CONDITION	(CLOCK HIGH DATA LOW)				|
	;				| KEYBOARD DATA    | +---+---+---+---+---+---+---+---+---+---+---+			|
	;				|		   v | D0| D1| D2| D3| D4| D5| D6| D7| P | 1 | 0 +-------------------	|
	;				|		  ---+---+---+---+---+---+---+---+---+---+---+---+			|
	;				|		      \------ DATA BITS -------------/	       zero inseted by KEYBOARD |
	;				|       		      Send by computer 	       ^   ^				|
	;				|						       |   |				|
	;				|						       |   +---- STOP BIT		|
	;				|						       +-------- PARITY BIT (ODD)	|
	;				| REMARKS										|
	;				| EVENT NUMBER     STATUS								|
	;				|											|
	;				|          24	    Clear parity counter						|
	;				|		    Program data bit as input						|
	;				|		    Put Clock line low							|
	;				|          23	    get DO clock line high						|
	;				|          21	    get D1 clock line high						|
	;				|          19	    get D2 clock line high						|
	;				|          17	    get D3 clock line high						|
	;				|          15	    get D4 clock line high						|
	;				|          13	    get D5 clock line high						|
	;				|          11	    get D6 clock line high						|
	;				|           9	    get D7 clock line high						|
	;				|           7	    get P clock line high						|
	;				|           5	    get (Stop Bit) clock line high					|
	;				|		    clock line low							|
	;				|           3      Program data bit as output						|
	;				|	     2      process data decode							|
	;				| 24..4 step 2	    Output clock line low						|
	;				\---------------------------------------------------------------------------------------/
	_PS2KBD_RXD:
		cli
		push	AccH						;save useds
		push	AccT
		cbi	_PS2KBD_CLOCK_PORT_DIR,_PS2KBD_CLOCK_BIT	;set DATA and CLOCK to input
		cbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT
		nop
	_PS2KBD_R00:
		in	Acc,_PS2KBD_CLOCK_PORT_�NPUT			;see if CLOCK=high and DATA=low, request PC comm status
		in	AccH,_PS2KBD_DATA_PORT_�NPUT
		andi	Acc,(1<<_PS2KBD_CLOCK_BIT)			;mask bits
		andi	AccH,(1<<_PS2KBD_DATA_BIT)
		or	Acc,AccH					;bound bits
		cpi	Acc,(1<<_PS2KBD_CLOCK_BIT)			;request ?
		brne	_PS2KBD_R00					;no, wait
		rcall	_PS2KBD_Del40					;wait 40us
		sbi	_PS2KBD_CLOCK_PORT_DIR,_PS2KBD_CLOCK_BIT	;set CLOCK as output DATA as input
		cbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT
		clr	AccH						;parity counter
		clr	Acc
		ldi	AccT,8						;bit counter
	PCTO00:
		cbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock low
		rcall	_PS2KBD_Del40					;wait 40us
		clc
		sbic	_PS2KBD_DATA_PORT_�NPUT,_PS2KBD_DATA_BIT	;read DATA
		sec
		brcc	PCTO10						;jump if parity even
		inc	AccH						;else increment parity bit
	PCTO10:
		ror	Acc						;Acc<--new bit
		sbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock high
		rcall	_PS2KBD_Del40					;wait 40us
		dec	AccT						;until all bits
		brne	PCTO00
		cbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock low
		rcall	_PS2KBD_Del40					;wait 40us
		push	Acc						;save data
		clc
		sbic	_PS2KBD_DATA_PORT_�NPUT,_PS2KBD_DATA_BIT	;read DATA in that case parity bit
		sec
		rol	Acc	
		dec	AccH
		eor	Acc,AccH					;parity odd ?
		andi	Acc,1						;see bit 0
		breq	PCTO20						;no, set error
		pop	Acc						;restore data
	PCTO15:
		rcall	_PS2KBD_PCSTOP
		clc							;com ok
	PCTOEXIT:
		rcall 	_PS2KBD_Del40					;wait 40us
		pop	AccT						;restore useds
		pop	AccH
		ret
	PCTO20:
		pop	Acc
		rcall	_PS2KBD_PCSTOP
		sec
		rjmp	PCTOEXIT
	;+----------------------------
	;| SEND PARITY AND STOP BITS 
	;+----------------------------
	_PS2KBD_PCSTOP:
		sbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock high stop bit
		rcall	_PS2KBD_Del40					;wait 40us
		cbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock low
		rcall	_PS2KBD_Del40					;wait 40us
		sbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT		;set DATA as output
		cbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock low (zero ack)
		rcall	_PS2KBD_Del40					;wait 40us
		sbi	_PS2KBD_CLOCK_PORT_OUTPUT,_PS2KBD_CLOCK_BIT	;clock high
		rcall	_PS2KBD_Del40					;wait 40us
		cbi	_PS2KBD_DATA_PORT_DIR,_PS2KBD_DATA_BIT		;set DATA as input
		ret
	;+---------------------------------------+
	;| AVR-->PC TRANSMISSION ROTINE		 |
	;| without Handdle interrupt		 |
	;| Input:Acc data to be sented		 |
	;| Obs: Interrupts disable		 |
	;+=======================================+======================================================
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   PS2KBD_TXD			  |
	;|   				  |
	;|\			          |
	;| | Input  : None		  |
	;|/  Output : C Ack bit read      |
	;|   Destroy: Acc	          |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________________________
	;				/ KEYBOARD TRANSMITION EVENTS NUMBERS	 						\
	;				|											|
	;				| EVENT NUMBER -   2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 					|
	;				| 		    5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0			|
	;				| KEYBOARD CLOCK  --+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-----------------	|
	;				|		    | | | | | | | | | | | | | | | | | | | | | |				|
	;				|		    +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ 			|
	;				|											|
	;				| KEYBOARD DATA     +---+---+---+---+---+---+---+---+---+---+---+			|
	;				|		 ---+ 0 | D0| D1| D2| D3| D4| D5| D6| D7| P | 1 +---------------	|
	;				|		    +---+---+---+---+---+---+---+---+---+---+---+			|
	;				|		      ^  \------ DATA BITS -------------/				|
	;				|       START BIT ----+				   	   ^   ^			|
	;				|							   |   |			|
	;				|							   |   +---- STOP BIT		|
	;				|							   +-------- PARITY BIT (ODD)	|
	;				| REMARKS										|
	;				| EVENT NUMBER     STATUS								|
	;				|											|
	;				|          24	    Output (Start Bit) 							|
	;				|		    Clear parity counter						|
	;				|		    Programa data bit as output						|
	;				|		    Put Clock line low							|
	;				|          22	    Output DO clock line low						|
	;				|          20	    Output D1 clock line low						|
	;				|          18	    Output D2 clock line low						|
	;				|          16	    Output D3 clock line low						|
	;				|          14	    Output D4 clock line low						|
	;				|          12	    Output D5 clock line low						|
	;				|          10	    Output D6 clock line low						|
	;				|           8	    Output D7 clock line low						|
	;				|           6	    Output P clock line low						|
	;				|           4	    Output (Stop Bit)							|
	;				|		    clock line low							|
	;				|           2      Program data bit as input						|
	;				|											|
	;				| 25..3 step 2	    Output clock line High						|
	;				\---------------------------------------------------------------------------------------/
	_PS2KBD_TXD:
		cli
		push	Acc				;save useds
		push	AccH
		push	AccT
		push	Acc
		ldi	Acc,_PORTC_MASK & ~((1<<KEYBOARD_DATA_BIT) | (1<<KEYBOARD_CLOCK_BIT)) ;set DATA e CLOCK input
		out	_DDRC,Acc
	AVRTO05:					;wait clock release
		sbis	_PINC,KEYBOARD_CLOCK_BIT
		rjmp	AVRTO05
		ldi	Acc,_PORTC_MASK | (1<<KEYBOARD_DATA_BIT) ;set DATA BIT as output
		out	_DDRC,Acc
		rcall	Del40
		pop	Acc
		cbi	_PORTC,KEYBOARD_DATA_BIT	;put start bit into line
		rcall	PCCLOCK				;pulse a clock
		clr	AccH				;parity counter
		ldi	AccT,8				;bit counter
	AVRTO00:
		lsr	Acc				;output a bit
		brcc	AVRTO10
		sbi	_PORTC,KEYBOARD_DATA_BIT	;send ONE bit
		inc	AccH				;parity counter++
		rjmp	AVRTO20
	AVRTO10:
		cbi	_PORTC,KEYBOARD_DATA_BIT	;send ZERO bit
	AVRTO20:
		rcall	PCCLOCK				;pulse a clock
		dec	AccT				;until all bit sented
		brne	AVRTO00
		bst	AccH,0				;Output a parity bit
		brts	AVRTO30
		sbi	_PORTC,KEYBOARD_DATA_BIT	
		rjmp	AVRTO40
	AVRTO30:
		cbi	_PORTC,KEYBOARD_DATA_BIT
	AVRTO40:
		rcall	PCCLOCK				;pulse a clock
		sbi	_PORTC,KEYBOARD_DATA_BIT	;Output as STOP bit
		rcall	PCCLOCK				;pulse a clock
		ldi	Acc,_PORTC_MASK			;set DATA BIT as input
		out	_DDRC,Acc
		pop	AccT				;restore useds
		pop	AccH
		pop	Acc
		ret
	;+-----------------------------------------------+
	;| Make a PC clock 				 |
	;+-----------------------------------------------+----------------------------------------------
	PCCLOCK:
		cbi	_PORTC,KEYBOARD_CLOCK_BIT	;put clock bit low
		rcall	Del40				;Delay 20us
		sbi	_PORTC,KEYBOARD_CLOCK_BIT	;put clock bit high
		rcall	Del40				;Delay 20us
		ret
	;+-----------------------------------------------+
	;| Make a 40us Delay 		          1	 |
	;| time equation: (9+Acc*3+3(rcall))*----------- |
	;|				      AVR_CLOCK  |
	;+-----------------------------------------------+----------------------------------------------
	.EQU	_PS2KBD_DELAYTIME = 40			;delay time in microseconds
	_PS2KBD_Del40:
		push	Acc
		ldi	Acc,((_PS2_DELAYTIME*_AVR_CLOCK)/1000000-11)/3
	_PS2KBD_Del00:
		dec	Acc
		brne	_PS2KBD_Del00
		pop	Acc
		ret
#endif
.exit

