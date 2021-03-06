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
;| DATA: 2/02/2000						|
;>--------------------------------------------------------------<
;| A D S 8 3 2 0  R O U T I N E S (DOUBLE CHIP ACCESS)		|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
#ifndef __ADCS_ADS8320_DOUBLE__
	#define __ADCS_ADS8320_DOUBLE__
		
	.message "+---------------------+"
	.message "|                     |"
	.message "| ADCS ADS8320 DOUBLE |"
	.message "|                     |"
	.message "+---------------------+"
	
	;>------------------------------------------------------<
	;| I M P L E M E N T E D  F U N C T I O N S 		|
	;+------------------------------------------------------+---------------------------------------+
	;| _ADS8320D_INIT	INITIALIZE ADS8320 CHIP							|
	;|			INITIALIZATION EXAMPLE							|
	;|												|
	;|.EQU	_ADS8320D_BIT_CLK	= 0		;ADS8320 CLOCK BIT				|
	;|.EQU	_ADS8320D_BIT_CS	= 1		;ADS8320 CHIP SELECT BIT			|
	;|.EQU	_ADS8320D_BIT_DATA1	= 2		;ADS8320 DATA BIT 1				|
	;|.EQU	_ADS8320D_BIT_DATA2	= 3		;ADS8320 DATA BIT 2				|
	;|												|
	;|.EQU	_ADS8320D_PORT_OUT= PORTB		;ADS8320 OUTPUT PORT				|
	;|.EQU	_ADS8320D_PORT_IN = PINB			;ADS8320 INPUT PORT			|
	;|.EQU	_ADS8320D_PORT_DIR= DDRB			;ADS8320 DIRECTION PORT			|
	;|												|
	;|			RCALL	_ADS8320D_INIT	;AFTER THIS POINT ALL ADS8320 ROUTINES WORKING 	|
	;|						;AT PORTB					|
	;|----------------------------------------------------------------------------------------------|
	;|												|
	;| _ADS8320D_GET		TO GET VALUE FROM ADS8320_CHIP					|
	;|												|
	;|			RCALL	_ADS8320D_GET	;AFTER THIS POINT Acc,AccH HAVE A 16 BIT VALUE	|
	;+----------------------------------------------------------------------------------------------+
	
	;   |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+
	;|   ADS8320 INITIALIZE		  |
	;|  				  |
	;|\			          |
	;| | Input  : None	          |
	;|/  Output : None	          |
	;|   Destroy: None	          |
	;|   			       	  |
	;+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |
	;   \__\__\__\__\__\__\__\__\__\________________________________________________________________________
	;				/
	;				| Obs:  After this initialization CLK,CS,DATA are in high level
	;				|	CLK,CS  = Output
	;				|	DATA	= Input
	;				\_______________________________________________________________________
	_ADS8320D_INIT:
		push	Acc
		in	Acc,SREG					;Save SREG state
		cli							;to handdling ports correctly
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CLK		;CLK  high
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CS		;CS   high
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_DATA1		;DATA1 high with pull up
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_DATA2		;DATA1 high with pull up
		sbi	_ADS8320D_PORT_DIR,_ADS8320D_BIT_CLK		;CLK  Output
		sbi	_ADS8320D_PORT_DIR,_ADS8320D_BIT_CS		;CS   Output
		cbi	_ADS8320D_PORT_DIR,_ADS8320D_BIT_DATA1		;DATA Input
		cbi	_ADS8320D_PORT_DIR,_ADS8320D_BIT_DATA2		;DATA Input
		out	SREG,Acc					;Restore SREG state
		pop	Acc
		ret
	;   |  |  |  |  |  |  |  |  |  |  |
	;+--+--+--+--+--+--+--+--+--+--+--+--+
	;|   ADC832 GET VALUE		     |
	;|  				     |
	;|\			             |
	;| | Input  : None		     |
	;|/  Output : Acc,AccH Value DATA1   |
	;|            AccT,AccTH Value DATA2 |
	;|   Destroy: None	             |
	;|   			       	     |
	;+--+--+--+--+--+--+--+--+--+--+--+--+ 
	;   |  |  |  |  |  |  |  |  |  |  |
	;  _\__\__\__\__\__\__\__\__\__\__\______________________________________________________________________________________________
	; /
	; | ADS8320 Basic Timing Diagrams
	; |
	; | __			<------------------------------------- Complete Cycle ----------------------------------------------->
	; | CS/SHDN	--------+											+-----------+
	; |			|											|	    |
	; |			+---------------------------------------------------------------------------------------+	    |
	; |
	; |			  <---- Sample ----->
	; | DCLOCK	--------+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +------
	; |			| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
	; |			+-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+ +-+
	; |
	; |				Hi-z
	; | DOUT1 	----------------------------+   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+------------
	; |					    | 0 |B15|B14|B13|B12|B11|B10|B9 |B8 |B7 |B6 |B5 |B4 |B3 |B2 |B1 |B0 |
	; |					    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	; |				Hi-z
	; | DOUT2 	----------------------------+   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+------------
	; |					    | 0 |B15|B14|B13|B12|B11|B10|B9 |B8 |B7 |B6 |B5 |B4 |B3 |B2 |B1 |B0 |
	; |					    +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	; |
	; \______________________________________________________________________________________________________________________________
	.MACRO	_ADSGETBIT
		cbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CLK
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CLK
		sbic	_ADS8320D_PORT_IN,_ADS8320D_BIT_DATA1
		ori	@0,@2
		sbic	_ADS8320D_PORT_IN,_ADS8320D_BIT_DATA2
		ori	@1,@2
	.ENDMACRO
	.MACRO	_ADSCLOCK
		cbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CLK
		sbi	_ADS8320D_PORT_OUT,_ADS8320D_BIT_CLK
	.ENDMACRO
	_ADS8320D_GET:
		clr		Acc
		clr		AccH
		clr		AccT
		clr		AccTH
		cbi		_ADS8320D_PORT_OUT,_ADS8320D_BIT_CS	;CS LOW
		_ADSCLOCK						;5 clocks to sample data
		_ADSCLOCK
		_ADSCLOCK
		_ADSCLOCK
		_ADSCLOCK
		_ADSCLOCK						;place zero at DOUT
		
		_ADSGETBIT	AccH,AccTH,0x80				;get D15..D0 bits
		_ADSGETBIT	AccH,AccTH,0x40
		_ADSGETBIT	AccH,AccTH,0x20
		_ADSGETBIT	AccH,AccTH,0x10
		_ADSGETBIT	AccH,AccTH,0x08
		_ADSGETBIT	AccH,AccTH,0x04
		_ADSGETBIT	AccH,AccTH,0x02
		_ADSGETBIT	AccH,AccTH,0x01
		_ADSGETBIT	Acc,AccT,0x80
		_ADSGETBIT	Acc,AccT,0x40
		_ADSGETBIT	Acc,AccT,0x20
		_ADSGETBIT	Acc,AccT,0x10
		_ADSGETBIT	Acc,AccT,0x08
		_ADSGETBIT	Acc,AccT,0x04
		_ADSGETBIT	Acc,AccT,0x02
		_ADSGETBIT	Acc,AccT,0x01
		sbi		_ADS8320D_PORT_OUT,_ADS8320D_BIT_CS	;CS HIGH
		_ADSCLOCK
		_ADSCLOCK
		ret
#endif

