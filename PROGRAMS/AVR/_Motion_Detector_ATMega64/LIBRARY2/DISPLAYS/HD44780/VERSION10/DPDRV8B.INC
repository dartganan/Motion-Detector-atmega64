;+--------------------------------------------------------------+
;| DISPLAY DRIVE FOR HD44780 HITACHI CHIP               	|
;| 8 BITS INTERFACE                                     	|
;| NEED FILES 							|
;|								|
;|	FILE		DIRECTORY				|
;|								|
;|	GLOBDEFS.INC	Avr\Library\Defs\GlobDefs.inc 		|
;|								|
;| BY JO�O DARTAGNAN ANTUNES OLIVEIRA                   	|
;| DATE:12/12/2000 VERSION 1.0                          	|
;>--------------------------------------------------------------<
;| D I S P L A Y  D R I V E                             	|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM     	|
;+--------------------------------------------------------------+
;| C O M M E N T S  A N D  U P D A T E S 			|
;+--------------------------------------------------------------+-----------------------------------------------+
;| 02/02/2002 22:05 _DISP_CMD,_DISP_DATA Routines removed CLI instruction 					|
;|                  _DISP_Init still cleaning interrupts during our execution					|
;+--------------------------------------------------------------------------------------------------------------+


;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;|												|
;| _DISP_BUSY		VERIFY BUSY STATE OF DISPLAY 						|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|												|
;| _DISP_CMD		SEND COMMANDS TO DISPLAY						|
;|			SAMPLE USING DISP_CMD TO CLEAR DISPLAY					|
;|												|
;|			LDI 	ACC,DISP_CMD_CLEAR						|
;|			RCALL	_DISP_CMD							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_CMD_NO_BUSY	SEND COMMANDS TO DISPLAY WITHOUT VERIFY BUSY				|
;|												|
;|			OBS: INTERNAL USE ONLY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DATA		SEND DATA TO DISPLAY							|
;|			SAMPLE USING DISP_DATA TO SEND CHARACTER '*' TO DISPLAY			|
;|												|
;|			LDI	ACC,'*'								|
;|			RCALL	_DISP_DATA							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_INIT		INITIALIZE DISPLAY							|
;|												|
;|			RCALL	_DISP_INIT							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_DELAY		DELAY 1 MILESECONDS							|
;|			SAMPLE USING DISP_DELAY TO WAIT FOR 100ms				|
;|												|
;|			LDI	ACC,100								|
;|			RCALL	_DISP_DELAY							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_SEND_STR	SEND STRING INTO FLASH MEMORY TO DISPLAY				|
;|			SAMPLE USING DISP_SEND_STR TO SEND STRING "HELLO WORD"			|
;|												|
;|			LDI	ZL,LOW(MSG*2)	;Z-->MESSAGE					|
;|			LDI	ZH,HIGH(MSG*2)							|
;|			RCALL	_DISP_SEND_STR	;CALL FUNCTION					|
;|			RJMP	CONTINUE							|
;|		MSG:	.DB 	"HELLO WORD",0	;DEFINE MESSAGE INTO FLASH			|
;|----------------------------------------------------------------------------------------------|
;| _DISP_SEND_STR_S	SEND STRING INTO SRAM MEMORY TO DISPLAY					|
;|			SAMPLE USING DISP_SEND_STR_S TO SEND STRING "HELLO WORD"		|
;|												|
;|			LDI	ZL,LOW(MSG*2)	;Z-->MESSAGE					|
;|			LDI	ZH,HIGH(MSG*2)							|
;|			RCALL	_DISP_SENDM_STR	;CALL FUNCTION					|
;|		.DSEG				;MSG ALREADY DEFINED INTO SRAM WITH MESSAGE	|
;|		MSG:				;HELLO WORLD					|
;|		.CSEG				;RETURN TO CODE SEGMENT				|
;|----------------------------------------------------------------------------------------------|
;| _DISP_LOCATE		SET CURSOR POSITION							|
;|			SAMPLE USING DISP_LOCATE TO POSITIONE CURSOR AT LINE 2 COLUMN 10	|
;|												|
;|			LDI	ACC,2								|
;|			LDI	ACCH,10								|
;|			RCALL	_DISP_LOCATE							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_CLS		CLEAR ENTIRE SCREEN							|
;|			SAMPLE CODE AS FOLLOWING						|
;|												|
;|			RCALL	_DISP_CLS							|
;|----------------------------------------------------------------------------------------------|
;| _DISP_HOME		SET CURSOR POSITION AT FIRST ROW AND FIRST COLUMN			|
;|			SAMPLE CODE AS FOLLOWING						|
;|												|
;|			RCALL	_DISP_HOME							|
;+----------------------------------------------------------------------------------------------+
;| OBS: CALL FUNCTION DISP_INIT BEFORE USE ANY OTHER FUNCTION TO INITIALIZE THIS DRIVE CORRECTLY|
;+----------------------------------------------------------------------------------------------+

;+------------------------------------------------------------------------------------------------------+
;| HARDWARE DEFINITIONS                                                                                 |
;|                                                                                                      |
;| BEFORE USE THIS DRIVE SET FOLLOWING PARAMETERS                                                       |
;|                                                                                                      |
;| _DISP_PORT_DATA_OUT          TO DEFINE AVR PORT TO SEND DATA AND COMMAND TO DISPLAY                  |
;| _DISP_PORT_DATA_IN                                                                                   |
;| _DISP_PORT_DATA_DIR                                                                                  |
;|                                                                                                      |
;| _DISP_PORT_CMD_OUT           TO DEFINE AVR PORT THAT HAVE BITS TO CONTROL DISPLAY INTERFACE          |
;| _DISP_PORT_CMD_IN                                                                                    |
;| _DISP_PORT_CMD_DIR                                                                                   |
;|                                                                                                      |
;| _DISP_BIT_RS                 SELECT BIT NUMBER INTO _DISP_PORT_CMD FOR EACH SIGNAL RS,RW AND E       |
;| _DISP_BIT_RW                                                                                         |
;| _DISP_BIT_E                                                                                          |
;|                                                                                                      |
;| EXAMPLE:                                                                                             |
;|                                                                                                      |
;|.EQU  _DISP_BIT_RS    = 2                     ;REGISTER SELECT BIT (HIGH=DATA,LOW=COMMAND)            |
;|.EQU  _DISP_BIT_RW    = 3                     ;READ WRITE BIT  (HIGH=READ,LOW=WRITE)                  |
;|.EQU  _DISP_BIT_E     = 4                     ;ENABLE BIT HIGH ACTIVATE                               |
;|                                                                                                      |
;|.EQU  _DISP_PORT_DATA_OUT     = PORTB         ;DISPLAY DATA OUTPUT PORT                               |
;|.EQU  _DISP_PORT_DATA_IN      = PINB          ;DISPLAY DATA INPUT PORT                                |
;|.EQU  _DISP_PORT_DATA_DIR     = DDRB          ;DISPLAY DATA DIRECTION                                 |
;|                                                                                                      |
;|.EQU  _DISP_PORT_CMD_OUT      = PORTD         ;DISPLAY COMMAND PORT OUT                               |
;|.EQU  _DISP_PORT_CMD_IN       = PIND          ;DISPLAY COMMAND PORT IN                                |
;|.EQU  _DISP_PORT_CMD_DIR      = DDRD          ;DISPLAY COMMAND PORT DIRECTION                         |
;|                                                                                                      |
;+------------------------------------------------------------------------------------------------------+

;+------------------------+
;| HARDWARE EQUATES       |
;+------------------------+----------------------------------------------------------------------------------

.EQU    _DISP_CMD_CLEAR         = 0B00000001            ;CLEAR ENTIRE DISPLAY AREA
.EQU    _DISP_CMD_HOME          = 0B00000010            ;RETURN DISPLAY CURSOR DO LINE 1 COLUMN 1
.EQU    _DISP_CMD_ENTRY_MODE_SET= 0B00000100            ;DISPLAY MODE SET
.EQU    _DISP_CMD_ON_OFF        = 0B00001000            ;DISPLAY ON/OFF
.EQU    _DISP_CMD_SHIFT         = 0B00010000            ;DISPLAY OR CURSOR SHIFT
.EQU    _DISP_CMD_FUNCTION_SET  = 0B00100000            ;FUNCTION SET
.EQU    _DISP_CMD_CGRAM_ADDR    = 0B01000000            ;SET CGRAM ADDRESS CG=CHARACTER GENERATOR
.EQU    _DISP_CMD_DDRAM_ADDR    = 0B10000000            ;SET DDRAM ADDRESS DD=DATA DISPLAY

.EQU    _DP_BIT_ID       = 1                     ;ID=1 cursor increment ID=0 cursor decrement
.EQU    _DP_BIT_S        = 0                     ;S=1 display shift
.EQU    _DP_BIT_D        = 2                     ;D=1 display ON
.EQU    _DP_BIT_C        = 1                     ;C=1 cursor ON
.EQU    _DP_BIT_B        = 0                     ;B=1 cursor blinking
.EQU    _DP_BIT_SC       = 3                     ;SC=1 display shift SC=0 cursor move
.EQU    _DP_BIT_RL       = 2                     ;RL=1 shift right RL=0 shift left
.EQU    _DP_BIT_DL       = 4                     ;DL=1 8 bits DL=0 4 bits interface
.EQU    _DP_BIT_N        = 3                     ;N=1 2 LINES N=0 1 LINE
.EQU    _DP_BIT_F        = 2                     ;F=1 5X10 DOTS F=0 5X8 DOTS
.EQU    _DP_BIT_BUSY = 7

;+-----------------------+
;| Display read busy bit |
;| cy = (high) busy      |
;+-----------------------+-----------------------------------------------------------------------------------

_DISP_BUSY:
        cbi	_DISP_PORT_CMD_OUT,_DISP_BIT_RS         ;RS=low
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RW         ;RW=high
        nop
        nop
        nop
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_E          ;pulse E
        nop
        nop
        clc
        cbi	_DISP_PORT_CMD_OUT, _DISP_BIT_E
        sbic    _DISP_PORT_DATA_IN,_DP_BIT_BUSY         ;read busy bit
        sec                                             ;turn busy on
        nop
        nop
        ret

;+------------------------+
;| Display write command  |
;| Input: Acc command     |
;+------------------------+-----------------------------------------------------------------------------------

_DISP_CMD:
	push	AccT					 ;to save SREG
	in	AccT,SREG
;	cli						 ;to correctly handdling of DATA and CMD ports
;above line removed at 2/2/2002 because my analisys reach conclusion that CLI instruction is very dangerous
;
_DISP_CMD00:
        rcall   _DISP_BUSY                               ;get busy state
        brcs    _DISP_CMD00                               ;wait exit busy
;+-------------------------------+
;| Display write command no busy |
;| Input: Acc command            |
;+-------------------------------+----------------------------------------------------------------------------
_DISP_CMD_NO_BUSY:
        push AccH
        ser     AccH                    	          ;program PORT DATA OUT as output
        out 	_DISP_PORT_DATA_DIR, AccH
        out     _DISP_PORT_DATA_OUT,Acc                   ;place data on port
        nop
        cbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RS           ;RS=low
        cbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RW           ;RW=high
        nop
        nop
        nop
        nop
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_E            ;pulse E
        nop
        nop
        nop
        nop
        cbi	_DISP_PORT_CMD_OUT, _DISP_BIT_E
        nop
        nop
        nop
        clr     AccH          	                          ;program PORT DATA OUT as input
        out	_DISP_PORT_DATA_DIR, AccH
        pop 	AccH
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RS           ;enabled RS and RW lines
        sbi 	_DISP_PORT_CMD_OUT, _DISP_BIT_RW
	out	SREG,AccT
	pop	AccT					;restore SREG
        ret

;+------------------------+
;| Display write data     |
;| Input: Acc data        |
;+------------------------+-----------------------------------------------------------------------------------

_DISP_DATA:
	push	AccT					 ;to save SREG
	in	AccT,SREG
;	cli	 					 ;to correctly handdling of DATA and CMD ports
;above line removed at 2/2/2002 because my analisys reach conclusion that CLI instruction is very dangerous
;
_DISP_DATA00:
        rcall   _DISP_BUSY                                ;get busy state
        brcs    _DISP_DATA00                              ;wait exit busy
        push 	AccH
        ser     AccH                                      ;program PORT DATA OUT as output
        out	_DISP_PORT_DATA_DIR, AccH
        out     _DISP_PORT_DATA_OUT,Acc                   ;place data on port
        nop
        nop
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RS           ;RS=HIGH
        cbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RW           ;RW=HIGH
        nop
        nop
        nop
        nop
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_E            ;pulse E
        nop
        nop
        nop
        nop
        cbi	_DISP_PORT_CMD_OUT, _DISP_BIT_E
        nop
        nop
        nop
        clr     AccH                                      ;program PORT DATA OUT  as input
        out	_DISP_PORT_DATA_DIR, AccH
        pop 	AccH
        sbi     _DISP_PORT_CMD_OUT,_DISP_BIT_RS         ;enabled RS and RW lines
        sbi 	_DISP_PORT_CMD_OUT, _DISP_BIT_RW
	out	SREG,AccT				;restore SREG
	pop	AccT
        ret

;+------------------------+
;| Display initialize     |
;+------------------------+-----------------------------------------------------------------------------------
_DISP_INIT:
	push	AccT
	in	AccT,SREG				;to handdling ports corretly
	cli
        push 	Acc
        in      Acc,_DISP_PORT_CMD_DIR                   ;get actual direction of display command port
                                                         ;set display commands bits as output level one
        ori     Acc,(1<<_DISP_BIT_RS) | (1<<_DISP_BIT_RW) | (1<<_DISP_BIT_E)
                                                         ;send display command initialization
        out 	_DISP_PORT_CMD_DIR, Acc
        ser 	Acc
        out     _DISP_PORT_CMD_OUT,Acc                   ;set all bits high level
        clr 	Acc
        out     _DISP_PORT_DATA_DIR,Acc                  ;set display data port as input
	out	SREG,AccT				 ;restore SREG value
        ldi     Acc,15                                   ;wait 15ms
        rcall 	_DISP_DELAY
;+------------------------
;|DL=1 , N=1 , F=0
;+------------------------
        ldi     Acc,_DISP_CMD_FUNCTION_SET | (1<<_DP_BIT_DL) | (1<<_DP_BIT_N) | (0<<_DP_BIT_F)
        rcall 	_DISP_CMD
        ldi     Acc,_DISP_CMD_FUNCTION_SET | (1<<_DP_BIT_DL) | (1<<_DP_BIT_N) | (0<<_DP_BIT_F)
        rcall 	_DISP_CMD
;+------------------------
;|ID=1 , S=0
;+------------------------
        ldi     Acc,_DISP_CMD_ENTRY_MODE_SET | (1<<_DP_BIT_ID) | (0<<_DP_BIT_S)
        rcall	_DISP_CMD
;+------------------------
;|D=1 , C=0 , B=0
;+------------------------
        ldi     Acc,_DISP_CMD_ON_OFF | (1<<_DP_BIT_D) | (0<<_DP_BIT_C) | (0<<_DP_BIT_B)
        rcall	_DISP_CMD
        ldi 	Acc,_DISP_CMD_CLEAR
        rcall 	_DISP_CMD
        pop 	Acc
	pop	AccT
        ret

;+------------------------------+
;|COMMON FUNCTION INCLUSION	|
;+------------------------------+

.INCLUDE "\Dart\Avr\Library\Displays\HD44780\Version10\CommonF.Inc"		;INCLUDE COMMON FUNCTION FOR BOTH INTERFACES

