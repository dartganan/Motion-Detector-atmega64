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
;+--------------------------------------------------------------+
;| DISPLAY DRIVE FOR SED1335 EPSON CHIP				|
;| NEED FILES 							|
;|								|
;|	FILE		DIRECTORY				|
;|								|
;|	GLOBDEFS.INC	Avr\Library\Defs\GlobDefs.inc 		|
;|								|
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA				|
;| DATE:12/12/2000 VERSION 1.0					|
;>--------------------------------------------------------------<
;| D I S P L A Y  D R I V E  3 2 0  X  2 4 0			|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;| PART I ROUTINES USAGE AND DESCRIPTIONS 			|
;+--------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;|												|
;| _DISPLAY_RESET	RESET DISPLAY INTERFACE							|
;|			OBS: INTERNAL USE ONLY							|
;+----------------------------------------------------------------------------------------------+
;| _DISP_FILL		FILL VRAM AREA WITH SPECIFIC PATTERN					|
;|			SAMPLE USING _DISP_FILL TO FILL VRAM AREA 0000H TO 1000H WITH 0XAA	|
;|												|
;|			LDI	XL,LOW(0000H)							|
;|			LDI	XH,HIGH(0000H)							|
;|			LDI	YL,LOW(1000H)							|
;|			LDI	YH,HIGH(1000H)							|
;|			LDI	ACC,0XAA							|
;|			RCALL	_DISP_FILL							|
;|----------------------------------------------------------------------------------------------|												|
;| _DISP_SET_CURSOR_ADDR SET CURSOR ADDRESS AT SPECIFIC ADDRESS					|
;|			 SAMPLE USING _DISP_SET_CURSOR_POS TO SET CURSOR AT POSITION 1234H	|
;|												|
;|			LDI	ACC,LOW(1234H)							|
;|			LDI	ACCH,HIGH(1234H)						|
;|			RCALL	_DISP_SET_CURSOR_POS						|
;|----------------------------------------------------------------------------------------------|
;| _DISP_GET_CURSOR_ADDR GET ACTUAL ADDRESS OF CURSOR						|
;|			SAMPLE USING _DISP_GET_CURSOR_POS					|
;|												|
;|			RCALL	_DISP_GET_CURSOR_POS	After this call Acc and AccH		|
;|							have a address of cursor		|
;|----------------------------------------------------------------------------------------------|
;| _DISP_STATUS_READ	READ DISPLAY STATUS. IN THIS CASE GET STATE OF BUSY FLAG		|
;|			SAMPLE USING _DISP_STATUS_READ						|
;|												|
;|			RCALL	_DISP_STATUS_READ	After this call Acc bit 6 have a state	|
;|							of busy bit				|
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
;| _DISP_SPEED		DEFINE DISPLAY INTERFACE SPEED						|
;|			SAMPLE USING DISP_SPEED							|
;|												|
;|			LDI	ACC,_DP_SLOW		Set Display Interface to Slow Speed	|
;|			RCALL	DISP_SPEED							|
;|			LDI	ACC,_DP_FAST		Set Display Interface to Fast Speed	|
;|			RCALL	DISP_SPEED							|
;| 												|
;| SED1335 GENERATE A FLICKS ON SCREEN AT EACH COMMAND OR DATA RECEIVED , WHEN USER DEFINE	|
;| A SLOW SPEED INTERFACE LESS FLICKS THEN GENERATE ON SCREEN					|
;|----------------------------------------------------------------------------------------------|
;| _DISP_BUSY		READ BUSY STATE OF DISPLAY						|
;|												|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_CMD		SEND A COMMAND TO DISPLAY WITH BUSY FLAG CHECK				|
;|			SAMPLE USING DISP_CMD TO SET CURSOR DIRECTION INCREMENT	TO RIGHT	|
;|												|
;|			LDI	ACC,DISP_CMD_CSRDIR_RIGHT					|
;|			RCALL	_DISP_CMD							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DATA		SEND A DATA TO DISPLAY WITH BUSY FLAG CHECK				|
;|			SAMPLE USING DISP_DATA TO SEND '*'					|
;|												|
;|			LDI	ACC,'*'								|
;|			RCALL	_DISP_DATA							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DATAR		GET DATA FROM DISPLAY WITH BUSY FLAG CHECK				|
;|			SAMPLE USING DISP_DATAR							|
;|												|
;|			RCALL	_DISP_DATAR		After this Acc have a data read		|
;|----------------------------------------------------------------------------------------------|
;| _DISP_INIT		INITIALIZE DISPLAY							|
;| _INIT_DISPLAY										|
;|												|
;|.EQU	_DISP_BIT_WR   =7				;WRITE SIGNAL				|
;|.EQU	_DISP_BIT_RD   =6				;READ SIGNAL				|
;|.EQU	_DISP_BIT_CS   =5				;CHIP SELECT SIGNAL			|
;|.EQU	_DISP_BIT_A0   =4				;AO SIGNAL (DATA,COMMAND)		|
;|.EQU	_DISP_BIT_RESET=3				;RESET SIGNAL				|
;|												|
;|.EQU	_DISP_PORT_DATA_OUT	= PORTA			;PORT TO SEND DATA TO DISPLAY		|
;|.EQU	_DISP_PORT_DATA_IN	= PINA			;PORT TO READ DATA FORM DISPLAY		|
;|.EQU	_DISP_PORT_DATA_DIR	= DDRA			;PORT TO DEFINE DATA DIRECTION		|
;|.EQU	_DISP_PORT_CMD_OUT	= PORTB			;PORT TO SEND COMMANDS TO DISPLAY	|
;|.EQU	_DISP_PORT_CMD_IN	= PINB			;PORT TO SEND COMMANDS TO DISPLAY	|
;|.EQU	_DISP_PORT_CMD_DIR	= DDRB			;PORT TO SEND COMMANDS TO DISPLAY	|
;|												|
;|			RCALL	_DISP_INIT		;After this call display read to use	|
;|							;using PORTA for data transfer		|
;|							;and PORTB for controls			|
;|----------------------------------------------------------------------------------------------|
;|_DISP_SET_LC		SET CURSOR POSITION ON SCREEN						|
;|			SAMPLE SETTING CURSOR AT LINE 2 COLUMN 5				|
;|												|
;|			LDI	XL,5		Column 5					|
;|			LDI	YL,2		Line 2						|
;|			RCALL	_DISP_SET_LC							|
;|----------------------------------------------------------------------------------------------|
;|*_DISP_LOCATE		SET CURSOR POSITION ON SCREEN						|
;|			SAMPLE SETTING CURSOR AT LINE 2 COLUMN 5				|
;|												|
;|			LDI	ACC,5		row 5						|
;|			LDI	ACCH,2		colunm 2					|
;|			RCALL	_DISP_LOCATE							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_SET_XY		SET CURSOR POSITION ON GRAPH SCREEN					|
;|			SAMPLE SETTING CURSOR AT X=2,Y=5					|
;|												|
;|			LDI	XL,5		Column 5					|
;|			LDI	YL,2		Line 2						|
;|			RCALL	_DISP_SET_XY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_CALC_XY	GET GRAPH CURSOR ADDRESS OF COORDINATES X,Y				|
;|			SAMPLE GETTING ADDRESS CURSOR AT X=2,Y=5				|
;|												|
;|			LDI	XL,5		Column 5					|
;|			LDI	YL,2		Line 2						|
;|			RCALL	_DISP_CALC_XY	After This Call Acc,AccH Have a Cursor Address	|
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
;| _DISP_CLS_TEXT	FILL DISPLAY TEXT AREA WITH SPECIFIC CHARACTER CODE			|
;|			SAMPLE FILLING TEXT AREA WITH CHARACTER 'A'		 		|
;|												|
;|			LDI	ACC,'A'								|
;|			RCALL	_DISP_CLS_TEXT							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_CLS_GRAPH	CLEAR DISPLAY GRAPH AREA 						|
;|			SAMPLE CLEANING GRAPH AREA				 		|
;|												|
;|			RCALL	_DISP_CLS_GRAPH							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_PSET		SET A PIXEL ON DISPLAY GRAPH AREA					|
;|			SAMPLE SETTING A PIXEL AT COORDINATES (160,120)		 		|
;|												|
;|			LDI	XL,LOW(160)							|
;|			LDI	XH,HIGH(160)							|
;|			LDI	YL,LOW(120)							|
;|			LDI	YH,HIGH(120)							|
;|			LDI	ACC,0XFF		zero clear a pixel other value set	|
;|			RCALL	_DISP_PSET							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_POINT		GET A PIXEL ON DISPLAY GRAPH AREA					|
;|			SAMPLE GETTING A PIXEL AT COORDINATES (160,120)		 		|
;|												|
;|			LDI	XL,LOW(160)							|
;|			LDI	XH,HIGH(160)							|
;|			LDI	YL,LOW(120)							|
;|			LDI	YH,HIGH(120)							|
;|			RCALL	_DISP_POINT		Acc=0 if pixel clear otherwise Acc=0xff	|
;|----------------------------------------------------------------------------------------------|
;| _DISP_GRAPH_POKE	PUT BYTE INTO GRAPH AREA VRAM						|
;|			SAMPLE PUTING BYTE 0X55 AT ADDRESS 40 OF GRAPH VRAM AREA 		|
;|												|
;|			LDI	XL,LOW(40)							|
;|			LDI	XH,HIGH(40)							|
;|			LDI	ACC,0X55							|
;|			RCALL	_GRAPH_POKE	;after this point _DISP_DATA routine can be used|
;|						;to put subsequential bytes with auto increment |
;|						;graph address increment			|
;+----------------------------------------------------------------------------------------------+
;| OBS: CALL FUNCTION DISP_INIT BEFORE USE ANY OTHER FUNCTION TO INITIALIZE THIS DRIVE CORRECTLY|
;+----------------------------------------------------------------------------------------------+
;+----------------------------------------------------------------------+
;| LCD DISPLAY ROTINES FOR SED1335F MICROCONTROLES USING 320X240 PIXELS	|
;| SINGLE PLANE 8X8 CHARACTER SIZE					|
;+----------------------------------------------------------------------+
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
;| _DISP_BIT_WR			SELECT BIT NUMBER INTO DISP_PORT_CMD FOR EACH SIGNAL RS,RW AND E	|
;| _DISP_BIT_RD												|
;| _DISP_BIT_CS												|
;| _DISP_BIT_A0												|
;| _DISP_BIT_RESET											|
;|													|
;| EXAMPLE:												|
;|													|
;|.EQU	_DISP_BIT_WR   =7				;WRITE SIGNAL					|
;|.EQU	_DISP_BIT_RD   =6				;READ SIGNAL					|
;|.EQU	_DISP_BIT_CS   =5				;CHIP SELECT SIGNAL				|
;|.EQU	_DISP_BIT_A0   =4				;AO SIGNAL (DATA,COMMAND)			|
;|.EQU	_DISP_BIT_RESET=3				;RESET SIGNAL					|
;|													|
;|.EQU	_DISP_PORT_DATA_OUT	= PORTB			;PORT TO SEND DATA TO DISPLAY			|
;|.EQU	_DISP_PORT_DATA_IN	= PINB			;PORT TO READ DATA FORM DISPLAY			|
;|.EQU	_DISP_PORT_DATA_DIR	= DDRB			;PORT TO DEFINE DATA DIRECTION			|
;|.EQU	_DISP_PORT_CMD_OUT	= PORTB			;PORT TO SEND COMMANDS TO DISPLAY		|
;|.EQU	_DISP_PORT_CMD_IN	= PINB			;PORT TO SEND COMMANDS TO DISPLAY		|
;|.EQU	_DISP_PORT_CMD_DIR	= DDRB			;PORT TO SEND COMMANDS TO DISPLAY		|
;|													|
;+------------------------------------------------------------------------------------------------------+
;+-------------------------------
;| ROUTINES INCLUSION FILE
;+-------------------------------
.INCLUDE	"Displays\Sed1335\DPDRV.INC"	;DRIVE ROUTINES INCLUDE

