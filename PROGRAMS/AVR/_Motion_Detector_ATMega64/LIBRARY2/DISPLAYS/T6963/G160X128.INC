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
;| DATE:12/12/2000 VERSION 1.0					|
;>--------------------------------------------------------------<
;| D I S P L A Y  D R I V E  1 6 0  X  1 2 8			|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;| PART I ROUTINES USAGE AND DESCRIPTIONS 			|
;+--------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;|												|
;|*DISPLAY_RESET	RESET DISPLAY INTERFACE							|
;|			OBS: INTERNAL USE ONLY							|
;+----------------------------------------------------------------------------------------------+
;| _DISP_STATUS_READ	READ DISPLAY STATUS. IN THIS CASE GET STATE OF BUSY FLAG		|
;|			SAMPLE USING _DISP_STATUS_READ						|
;|												|
;|			RCALL	_DISP_STATUS_READ	After this call Acc have a status bits	|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DATA_READ	READ DATA FROM DISPLAY WITHOUT BUSY FLAG CHECK				|
;|												|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DATA_WRITE	WRITE DATA INTO DISPLAY WITHOUT BUSY FLAG CHECK				|
;|												|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_COMMAND_WRITE	SEND COMMAND TO DISPLAY WITHOUT BUSY FLAG CHECK				|
;|												|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_CMD		SEND A COMMAND TO DISPLAY WITH BUSY FLAG CHECK				|
;|			SAMPLE USING _DISP_CMD TO SET CURSOR DIRECTION INCREMENT TO RIGHT	|
;|												|
;|			LDI	ACC,_DISP_CMD_CSRDIR_RIGHT					|
;|			RCALL	_DISP_CMD							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_DATAAUTO	SEND A DATA TO DISPLAY WITH BUSY FLAG CHECK				|
;|			SAMPLE USING _DISP_DATAAUTO TO SEND '*'					|
;|												|
;|			LDI	ACC,'*'								|
;|			RCALL	_DISP_DATAAUTO							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_INIT		INITIALIZE DISPLAY							|
;|*_INIT_DISPLAY										|
;|												|
;|			RCALL	_DISP_INIT							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_SET_LC		SET CURSOR POSITION ON SCREEN						|
;|			SAMPLE SETTING CURSOR AT LINE 2 COLUMN 5				|
;|												|
;|			LDI	XL,5		Column 5					|
;|			LDI	YL,2		Line 2						|
;|			RCALL	_DISP_SET_LC							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_LOCATE		SET CURSOR POSITION ON SCREEN						|
;|			SAMPLE SETTING CURSOR AT LINE 2 COLUMN 5				|
;|												|
;|			LDI	ACC,5		Row 5						|
;|			LDI	ACCH,2		Colunm 2						|
;|			RCALL	_DISP_LOCATE							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_SET_XY		SET CURSOR POSITION ON GRAPH SCREEN					|
;|			SAMPLE SETTING CURSOR AT X=2,Y=5					|
;|												|
;|			LDI	XL,4		Column 5					|
;|			LDI	YL,1		Line 2						|
;|			RCALL	_DISP_SET_XY							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_SEND_STR	SEND STRING INTO FLASH MEMORY TO DISPLAY				|
;|			SAMPLE USING _DISP_SEND_STR TO SEND STRING "HELLO WORD"			|
;|												|
;|			LDI	ZL,LOW(MSG*2)							|
;|			LDI	ZH,HIGH(MSG*2)							|
;|			RCALL	_DISP_SEND_STR							|
;|			RJMP	CONTINUE							|
;|		MSG:	.DB 	"HELLO WORD",0							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_SEND_STR_S	SEND STRING INTO FLASH MEMORY TO DISPLAY				|
;|			SAMPLE USING _DISP_SEND_STR TO SEND STRING "HELLO WORD"			|
;|			SAME AS _DISP_SEND_STR SAMPLE BUT DATA MUST BE INTO SRAM		|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_CLS_TEXT	FILL DISPLAY TEXT AREA WITH SPECIFIC CHARACTER CODE			|
;|			SAMPLE FILLING TEXT AREA WITH CHARACTER 'A'		 		|
;|												|
;|			LDI	ACC,'A'								|
;|			RCALL	_DISP_CLS_TEXT							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_CLS_GRAPH	CLEAR DISPLAY GRAPH AREA 						|
;|			SAMPLE CLEANING GRAPH AREA				 		|
;|												|
;|			RCALL	_DISP_CLS_GRAPH							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_PSET		SET A PIXEL ON DISPLAY GRAPH AREA					|
;|			SAMPLE SETTING A PIXEL AT COORDINATES (160,120)		 		|
;|												|
;|			LDI	XL,LOW(160)							|
;|			LDI	XH,HIGH(160)							|
;|			LDI	YL,LOW(120)							|
;|			LDI	YH,HIGH(120)							|
;|			LDI	ACC,0XFF		zero clear a pixel other value set	|
;|			RCALL	_DISP_PSET							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_POINT		GET A PIXEL ON DISPLAY GRAPH AREA					|
;|			SAMPLE GETTING A PIXEL AT COORDINATES (160,120)		 		|
;|												|
;|			LDI	XL,LOW(160)							|
;|			LDI	XH,HIGH(160)							|
;|			LDI	YL,LOW(120)							|
;|			LDI	YH,HIGH(120)							|
;|			RCALL	_DISP_POINT		Acc=0 if pixel clear otherwise Acc=0xff	|
;+----------------------------------------------------------------------------------------------+
;| OBS: CALL FUNCTION _DISP_INIT BEFORE USE ANY OTHER FUNCTION TO INITIALIZE THIS DRIVE CORRECTLY|
;|	ROUTINES WITH * ARE COMPATIBLE TO OTHER GRAPH DISPLAYS					|
;+----------------------------------------------------------------------------------------------+
;+------------------------------------------------------------------------------------------------------+
;| HARDWARE DEFINITIONS	  										|
;|													|
;| BEFORE USE THIS DRIVE SET FOLLOWING PARAMETERS							|
;|													|
;| _DISP_PORT_DATA_OUT		TO DEFINE AVR PORT TO SEND DATA AND COMMAND TO DISPLAY			|
;| _DISP_PORT_DATA_IN											|
;| _DISP_PORT_DATA_DIR											|
;|													|
;| _DISP_PORT_CMD_OUT		TO DEFINE AVR PORT THAT HAVE BITS TO CONTROL DISPLAY INTERFACE		|
;| _DISP_PORT_CMD_IN											|
;| _DISP_PORT_CMD_DIR											|
;|													|
;| _DISP_BIT_WR			SELECT BIT NUMBER INTO _DISP_PORT_CMD FOR EACH SIGNAL RS,RW AND E	|
;| _DISP_BIT_RD												|
;| _DISP_BIT_CE												|
;| _DISP_BIT_CD												|
;| _DISP_BIT_RESET											|
;|													|
;| EXAMPLE:												|
;|													|
;|.EQU	_DISP_BIT_WR   	=7				;DISPLAY CONTROL SIGNALS BITS			|
;|.EQU	_DISP_BIT_RD   	=6										|
;|.EQU	_DISP_BIT_CE	=5										|
;|.EQU	_DISP_BIT_CD	=4										|
;|.EQU	_DISP_BIT_RESET	=3										|
;|													|
;|.EQU	_DISP_PORT_DATA_OUT	= PORTA			;PORT TO SEND DATA TO DISPLAY			|
;|.EQU	_DISP_PORT_DATA_IN	= PINA			;PORT TO READ DATA FROM DISPLAY			|
;|.EQU	_DISP_PORT_DATA_DIR	= DDRA			;PORT TO DEFINE DATA DIRECTION			|
;|.EQU	_DISP_PORT_CMD_OUT	= PORTC			;PORT TO SEND COMMANDS TO DISPLAY		|
;|.EQU	_DISP_PORT_CMD_IN	= PINC			;ONLY FOR OBJECT COMPATIBITY			|
;|.EQU	_DISP_PORT_CMD_DIR	= DDRC			;ONLY FOR OBJECT COMPATIBITY			|
;|													|
;+------------------------------------------------------------------------------------------------------+
;+-----------------------------------+
;| DISPLAY CONSTANTS AND DEFINITIONS |
;+===================================+==========================================================
.EQU	_DISP_STA0 =0 				;DISPLAY STATUS BITS
.EQU	_DISP_STA1 =1 
.EQU	_DISP_STA2 =2 
.EQU	_DISP_STA3 =3 
.EQU	_DISP_STA4 =4 
.EQU	_DISP_STA5 =5
.EQU	_DISP_STA6 =6 
.EQU	_DISP_STA7 =7 
.EQU	_DISP_STA0M =0B00000001			;DISPLAY STATUS BITS MASK
.EQU	_DISP_STA1M =0B00000010 
.EQU	_DISP_STA2M =0B00000100
.EQU	_DISP_STA3M =0B00001000
.EQU	_DISP_STA4M =0B00010000
.EQU	_DISP_STA5M =0B00100000
.EQU	_DISP_STA6M =0B01000000
.EQU	_DISP_STA7M =0B10000000
.EQU	_DISP_HARD_COLUNMS = 32			;NUMBER OF COLUNMS SET IN HARDWARE
.EQU	_DISP_HARD_LINES   = 8	 		;NUMBER OF LINES SET IN HARDWARE
.EQU	_DISP_SHOW_COLUNMS = 20			;SHOWED COLUNMS 
.EQU	_DISP_SHOW_LINES   = 16			;SHOWED LINES
.EQU	_NUMCHARS = _DISP_HARD_COLUNMS*_DISP_HARD_LINES
.EQU	_DISP_TEXT_ADDRESS  = 0X0000
.EQU	_DISP_GRAPH_ADDRESS = 0X1000
.EQU	_DP_PAGE0	= 0X00
.EQU	_DP_PAGE1 	= 0X80
.EQU	SETCONTROLWORD	= 0X40
.EQU		TEXTHOMEADDR	= 0X00
.EQU		TEXTAREA	= 0X01
.EQU		GRAPHHOMEADDR	= 0X02
.EQU		GRAPHAREA	= 0X03
.EQU	MODESET		= 0X80
.EQU		ORMODE		= 0X00
.EQU		EXORMODE	= 0X01
.EQU		ANDMODE		= 0X03
.EQU		TEXTATTRMODE	= 0X04
.EQU		CGROM		= 0X00
.EQU		CGRAM		= 0X08
.EQU	REGISTERSSETTING= 0X20
.EQU		CURSORPOINTER	= 0X01
.EQU		OFFSETREGISTER	= 0X02
.EQU		ADDRESSPOINTER	= 0X04
.EQU	DATAAUTO	= 0XB0
.EQU		DATAAUTOWRITE	= 0X00
.EQU		DATAAUTOREAD	= 0X01
.EQU		AUTORESET	= 0X02
.EQU	DISPLAYMODE	= 0X90
.EQU		DISPLAYOFF	= 0X00
.EQU		BLINKOFF	= 0X00
.EQU		BLINKON		= 0X01
.EQU		CURSOROFF	= 0X00
.EQU		CURSORON	= 0X02
.EQU		TEXTOFF		= 0X00
.EQU		TEXTON		= 0X04
.EQU		GRAPHOFF	= 0X00
.EQU		GRAPHON		= 0X08
.EQU	BITSETRESET	= 0XF0
.EQU		BITRESET	= 0X00
.EQU		BITSET		= 0X08
.EQU	_DISP_WIDTH		= 160
.EQU	_DISP_HEIGHT		= 128
.EQU	_DISP_SCALE_WIDTH	= 160
.EQU	_DISP_SCALE_HEIGHT	= 128
;+------------------------------+
;| ROUTINES INCLUSION FILE	|
;+------------------------------+
.INCLUDE	"DISPLAYS\T6963\DPDRV1.INC"	;DRIVE ROUTINES INCLUDE
.EXIT

