; 	 ____________________________________________________________________________________________
;	|////////////////////////////////////////////////////////////////////////////////////////////|
;	|///|											|////|
;	|///| ALGORITHM	 : CIRCULAR QUEUE							|////|
;	|///| VERSAO	 : 1.0									|////|
;	|///| PROGRAMADOR: JO�O DARTAGNAN ANTUNES OLIVEIRA				   	|////|
;	|///| DATA 	 : 6 de Agosto de 2007 (23:51)						|////|
;	|///| DESCRI��O	 : Implementa algoritmo de fila circular				|////|
;	|///| OBJETIVO:	   Implementar funcoes necessarias para um fila de dados do tipo (FIFO) |////|
;	|///| 		   "Fist In First Out", Primeiro a Entrar Primeiro a Sair Circular, ou 	|////|
;	|///|	           seja quando o dado inserido atingir o tamanho m�ximo da fila a inser-|////|
;	|///|		   �ao dar-se-a no inicio.						|////|
;	|///| RESOURCES	 : Area na fila definido na SRAM					|////|
;	|///|											|////|
;	|///| OBS:	   This software is intended to use in AVR with SRAM			|////|
;	|///|___________________________________________________________________________________|////|
;-------|////////////////////////////////////////////////////////////////////////////////////////////|
;	>--------------------------------------------------------------------------------------------<
;-------|											     |
;	|   				C I R C U L A R  Q U E U E  8 B I T S 		     	     |
;	| 											     |
;+------+--------------------------------------------------------------------------------------------+


;			 ///////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\
;			//						\\
;----------------------||  	       	 CIRCULAR QUEUE 8 BITS		||---------------------------
;			\\						//
			 \\\\\\\\\\\\\\\\\\\\\\\/////////////////////////

#ifndef __CQUEUE8__

	#define __CQUEUE8__

	.message "+------------------------+"
	.message "|                        |"
	.message "| CIRCULAR QUEUE 8 BITS  |"
	.message "|                        |"
	.message "+------------------------+"

;>--------------------------------------------------------------<
;| OBJECT     : CQUEUE8						|
;>--------------------------------------------------------------<
;| DESCRIPTION: This object has a lot of function to control	|
;|		data input and output from a circular queue	|
;|		using a FIFO(First In First Out) queue, and 	|
;|		max data buffer size 255 bytes			|
;>--------------------------------------------------------------<

;>--------------------------------------------------------------<
;| This object recquire a defined region in SRAM with Size of   |
;| of queue in bytes+3 to hold queue information		|
;| Below a sample queue definition				|
;>--------------------------------------------------------------<

;
;.DSEG
;	MY_CIRCULAR_BUFFER:		.BYTE	SIZE_OF_CIRCULAR_BUFFER+4		;always define
;											;size of circular 
;											;queue + 3 to hold
;											;queue information
;.CSEG


;-------------------------< CONSTANT DEFINITONS >------------------------

.EQU	_CQUEUE8_OFFSET_SIZE	= 0						;offset of size of queue
.EQU	_CQUEUE8_OFFSET_G	= 1						;offset of Get pointer of queue
.EQU	_CQUEUE8_OFFSET_P	= 2						;offset Put pointer of queue
.EQU	_CQUEUE8_OFFSET_DATA	= 3						;offset of data

;-------------------------< DEVICE DEFINITIONS >-------------------------

.DSEG
.CSEG

;-------------------------< MACROS  DEFINITONS >-------------------------

	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_INIT					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Init queue with specifield size and clear G and	|
	;|		P pointers					|
	;>--------------------------------------------------------------<
	;| INPUT      :	Acc	Queue Size				|
	;|		Z--->	Queue					|
	;| OUTPUT     :	None						|
	;| DESTROYED  :	r0,Flags					|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------+-------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue		|
	;|		ldi	Acc,my.queue.size	;Acc=my.queue.size	|
	;|		rcall	_CQUEUE8_INIT		;Initialize queu	|
	;>--------------------------------------------------------------+-------<
	;| OBSERVATION:	this function must be called before use of any 	|
	;|		other queue function				|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_INIT:
		mov	r0,Acc							;set queue size+1
		inc	r0
		std	Z+_CQUEUE8_OFFSET_SIZE,r0
		clr	r0
		std	Z+_CQUEUE8_OFFSET_G,r0					;G<-0
		std	Z+_CQUEUE8_OFFSET_P,r0					;P<-0
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_INC_P					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Increment queue P pointer			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	r1	Queue.P++				|
	;|		r0	Queue.Size				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue	|
	;|		rcall	_CQUEUE8_INC_P		;P++		|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_INC_P:
		ldd	r0,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		ldd	r1,Z+_CQUEUE8_OFFSET_P				;get queue P
		inc	r1						;P++
		cp	r1,r0						;P>Size
		brlo	_CQUEUE8_INC_P_10				;no, branch
		clr	r1						;else P<-0
	_CQUEUE8_INC_P_10:
		std	Z+_CQUEUE8_OFFSET_P,r1				;store back P
		ret
	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_DEC_P					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Decrement queue P pointer			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	r1	Queue.P--				|
	;|		r0	Queue.Size				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue	|
	;|		rcall	_CQUEUE8_DEC_P		;P--		|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_DEC_P:
		ldd	r0,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		ldd	r1,Z+_CQUEUE8_OFFSET_P				;get queue P
		tst	r1						;P==0?
		brne	_CQUEUE8_DEC_P_10				;no, branch
		dec	r0						;else P<-SIZE-1
		std	Z+_CQUEUE8_OFFSET_P,r0				;store back P
		inc	r0
		ret
	_CQUEUE8_DEC_P_10:
		dec	r1						;P--
		std	Z+_CQUEUE8_OFFSET_P,r1				;store back P
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_INC_G					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Increment queue G pointer			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	r1	Queue.G++				|
	;|		r0	Queue.Size				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue	|
	;|		rcall	_CQUEUE8_INC_G		;G++		|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_INC_G:
		ldd	r0,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		ldd	r1,Z+_CQUEUE8_OFFSET_G				;get queue G
		inc	r1						;G++
		cp	r1,r0						;G>Size
		brlo	_CQUEUE8_INC_G_10				;no, branch
		clr	r1						;else G<-0
	_CQUEUE8_INC_G_10:
		std	Z+_CQUEUE8_OFFSET_G,r1				;store back G
		ret
	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_DEC_G					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Decrement queue G pointer			|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	r1	Queue.G--				|
	;|		r0	Queue.Size				|
	;| DESTROYED  :	Flags						|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue	|
	;|		rcall	_CQUEUE8_DEC_G		;G--		|
	;>--------------------------------------------------------------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_DEC_G:
		ldd	r0,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		ldd	r1,Z+_CQUEUE8_OFFSET_G				;get queue G
		tst	r1						;G==0?
		brne	_CQUEUE8_DEC_G_10				;no, branch
		dec	r0						;else G<-SIZE-1
		std	Z+_CQUEUE8_OFFSET_G,r0				;store back G
		inc	r0
		ret
	_CQUEUE8_DEC_G_10:
		dec	r1						;P--
		std	Z+_CQUEUE8_OFFSET_G,r1				;store back G
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_PUT_DATA				|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Put new data into queue, and increment P pointer|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;|		Acc	Data					|
	;| OUTPUT     :	cy=1	if queue is full			|
	;| DESTROYED  :	r0,r1,Flags					|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------+-------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue		|
	;|		ldi	Acc,my.queue.data	;Acc<-my.queue.data	|
	;|		rcall	_CQUEUE8_PUT_DATA	;Put data into queue	|
	;|									|
	;|		after this if function return cy=1 means that queue is	|
	;|		full and data don't placed into queue			|
	;>--------------------------------------------------------------+-------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_PUT_DATA:
		ldd	r0,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		ldd	r1,Z+_CQUEUE8_OFFSET_G				;get queue G
		tst	r1						;G==0?
		brne	_CQUEUE8_PUT_DATA_10				;no, branch
		dec	r0						;else G<=size-1
		mov	r1,r0
		rjmp	_CQUEUE8_PUT_DATA_20
	_CQUEUE8_PUT_DATA_10:
		dec	r1						;G--
	_CQUEUE8_PUT_DATA_20:
		ldd	r0,Z+_CQUEUE8_OFFSET_P				;get queue P
		cp	r0,r1						;P==G?
		brne	_CQUEUE8_PUT_DATA_30				;no, branch
		sec							;cy=1 queue full
		ret
	_CQUEUE8_PUT_DATA_30:	
		clr	r1
		add	ZL,r0						;*Z(p)<-data
		adc	ZH,r1
		std	Z+_CQUEUE8_OFFSET_DATA,Acc
		sub	ZL,r0						;restore Z
		sbc	ZH,r1
		rcall	_CQUEUE8_INC_P					;P++
		clc							;cy=0,queue not full
		ret
	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_GET_DATA				|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Get data from queue, and increment G pointer	|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	Acc	Data					|
	;|		cy=1	if queue is empty			|
	;| DESTROYED  :	r0,r1,Flags					|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------+-------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue		|
	;|		rcall	_CQUEUE8_GET_DATA	;Get data from queue	|
	;|									|
	;|		after this if function return cy=1 means that queue is	|
	;|		empty and Acc is not a valid data			|
	;>--------------------------------------------------------------+-------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|																		|
	_CQUEUE8_GET_DATA:
		ldd	r0,Z+_CQUEUE8_OFFSET_G				;get queue G	
		ldd	r1,Z+_CQUEUE8_OFFSET_P				;get queue P
		cp	r0,r1						;G==P?
		brne	_CQUEUE8_GET_DATA_10				;no, branch
		sec							;cy=1 queue is empty
		ret
	_CQUEUE8_GET_DATA_10:
		clr	r1
		add	ZL,r0						;data<-*Z(p)
		adc	ZH,r1
		ldd	Acc,Z+_CQUEUE8_OFFSET_DATA
		sub	ZL,r0						;restore Z
		sbc	ZH,r1
		rcall	_CQUEUE8_INC_G					;G++
		clc							;cy=0, data retrived
		ret
		
	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_EMPTY					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Check if queue is empty				|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	cy=1	if queue is empty			|
	;| DESTROYED  :	r0,r1,Flags					|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------+-------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue		|
	;|		rcall	_CQUEUE8_EMPTY		;Check if queue empty	|
	;|									|
	;|		after this if function return cy=1 means that queue is	|
	;|		empty otherwise cy=0					|
	;>--------------------------------------------------------------+-------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|									
	_CQUEUE8_EMPTY:	
		ldd	r0,Z+_CQUEUE8_OFFSET_G				;get queue G	
		ldd	r1,Z+_CQUEUE8_OFFSET_P				;get queue P
	_CQUEUE8_EMPTY_10:
		cp	r0,r1						;G==P?
		brne	_CQUEUE8_EMPTY_20				;no, branch
		sec							;cy=1 queue is empty, or full if called by CQUEUE8_FULL
		ret
	_CQUEUE8_EMPTY_20:
		clc							;cy=0 queue not empty or not full if called by CQUEUE8_FULL
		ret

	;>--------------------------------------------------------------<
	;| OBJECT     : CQUEUE8						|
	;>--------------------------------------------------------------<
	;| FUNCTION   : CQUEUE8_FULL					|
	;>--------------------------------------------------------------<
	;| DESCRIPTION: Check if queue is full				|
	;>--------------------------------------------------------------<
	;| INPUT      :	Z--->	Queue					|
	;| OUTPUT     :	cy=1	if queue is full			|
	;| DESTROYED  :	r0,r1,Flags					|
	;| TIME	      :	----						|
	;>--------------------------------------------------------------+-------<
	;| USAGE      :	ldiw	Z,my.queue		;Z-->my.queue		|
	;|		rcall	_CQUEUE8_FULL		;Check if queue full	|
	;|									|
	;|		after this if function return cy=1 means that queue is	|
	;|		full otherwise cy=0					|
	;>--------------------------------------------------------------+-------<
	;| OBSERVATION:	---						|
	;+--------------------------------------------------------------+-------------------------------------------------------------------------------\
	;|									
	_CQUEUE8_FULL:	
		ldd	r0,Z+_CQUEUE8_OFFSET_G				;get queue G	
		ldd	r1,Z+_CQUEUE8_OFFSET_SIZE			;get queue size
		tst	r0						;G==0?
		brne	_CQUEUE8_FULL_10				;no, branch
		dec	r1						;else G<-SIZE-1
		mov	r0,r1
		rjmp	_CQUEUE8_FULL_20
	_CQUEUE8_FULL_10:
		dec	r0						;G--
		ldd	r1,Z+_CQUEUE8_OFFSET_P				;get queue P
	_CQUEUE8_FULL_20:
		rjmp	_CQUEUE8_EMPTY_10				;check
#endif
