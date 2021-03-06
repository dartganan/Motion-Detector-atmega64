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
;+----------------------------------------------------------------------+
;|									|
;| DATA:3/12/2002							|
;>----------------------------------------------------------------------<
;| D R A W  1 6  B I T  R O U T I N E 					|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;| AND USE A BRESEHAM ALGORITHM 					|
;+----------------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _DRAW_SET_POINT	Set a DRAW start point of vectors					|
;|												|
;|			ldi	XL,LOW(200)	;set Start point to P(200,300)			|
;|			ldi	XH,HIGH(200)							|
;|			ldi	YL,LOW(300)							|
;|			ldi	YH,HIGH(300)							|
;|			rcall	_DRAW_SET_POINT	;set a new point to start draw			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_GET_POINT	get a DRAW start point							|
;|												|
;|			rcall	_DRAW_GET_POINT	;X,Y has a start point of draw			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_SET_CENTER_POINT	Set a DRAW center point of vectors for rotation			|
;|												|
;|			ldi	XL,LOW(200)	;set Start point to P(200,300)			|
;|			ldi	XH,HIGH(200)							|
;|			ldi	YL,LOW(300)							|
;|			ldi	YH,HIGH(300)							|
;|			rcall	_DRAW_SET_CENTER_POINT	;set a new point to start draw		|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_GET_CENTER_POINT	get a DRAW center point						|
;|												|
;|			rcall	_DRAW_GET_CENTER_POINT	;X,Y has a center point			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_SET_SCALE	set scale range 1/256 to 256			 			|
;|												|
;|			ldi	Acc,low(256)		;set scale to 1				|
;|			ldi	AccH,high(256)							|
;|			rcall	_DRAW_SET_SCALE		;draw appear with scale=1		|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_GET_SCALE	get scale of draw 					 		|
;|												|
;|			rcall	_DRAW_GET_SCALE		;Acc:AccH Scale				|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_SET_ROTATE_ANGLE	set rotate angle of draw		 			|
;|												|
;|			ldi	Acc,low(120)		;set rotate angle of draw to 120	|
;|			ldi	AccH,high(120)							|
;|			rcall	_DRAW_SET_ROTATE_ANGLE	;set angle				|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_GET_ROTATE_ANGLE	get rotate angle of draw 			 		|
;|												|
;|			rcall	_DRAW_GET_ROTATE_ANGLE	;Acc:AccH rotate angle			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_SAVE_POINT	save start point for future use			 			|
;|												|
;|			rcall	_DRAW_SAVE_POINT	;start move saved 			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW_RESTORE_POINT	restore old saved start point is restored to start point 		|
;|												|
;|			rcall	_DRAW_RESTORE_POINT	;X,Y last point saved			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _DRAW		begin a new draw. the drawing must be into a Flash address 		|
;|			and terminated with 0 to signalling end of draw				|
;|												|
;|			ldi	ZL,low(Rect*2)		;Draw a rect with 3x10			|
;|			ldi	ZH,high(Rect*2)							|
;|			rcall	_DRAW			;start draw				|
;|		Rect:	.db "3U10R3D10L",0							|
;|												|
;|  DRAW Commands describle below								|
;|												|
;| 	Ortogonal moves										|
;|												|
;| 		nnnU - draw a vector up    ( 90o)						|
;| 		nnnR - draw a vector right (  0o)						|
;| 		nnnD - draw a vector down  (270o)						|
;| 		nnnL - draw a vector left  (180o)						|
;|												|
;| 	Diagonal moves										|
;|												|
;| 		nnnE - draw a vector up and right   ( 45o)					|
;| 		nnnF - draw a vector down and right (315o)					|
;| 		nnnG - draw a vector down and left  (227o)					|
;| 		nnnH - draw a vector up and left    (135o)					|
;|												|
;| 	Free moves										|
;|												|
;|		sxxx,syyyP - draw a vector to PX+xxx,PY+yyy					|
;|												|
;| 	Pen control										|
;|												|
;|		W - Turn pen on									|
;|		B - Turn pen off								|
;|												|
;|	A command draw string below draw a text "DRAW" at screen				|
;|												|
;| 	.db "10U4R2F6D2G4LB8RW10U4R2F2D2G4LB4RW4FB2RW8U2E2R2F4D6LB6RW4DB2R10UW10D5E5F10U",0	|
;+----------------------------------------------------------------------------------------------+
#ifndef __GRAPH_DRAW__
	
	#define	__GRAPH_DRAW__
	
	.message "+------------------+"
	.message "|                  |"
	.message "| GRAPH DRAW       |" 
	.message "|                  |"
	.message "+------------------+"
	.EQU	_DRAW_AXIS_X_INC	= 0			;constants to axis process, the values represent a bit field number
	.EQU	_DRAW_AXIS_X_DEC	= 1
	.EQU	_DRAW_AXIS_Y_INC	= 2
	.EQU	_DRAW_AXIS_Y_DEC	= 3
	.DSEG
		_DRAW_X:	.byte	2				;start X point
		_DRAW_Y:	.byte	2				;start Y point
		_DRAW_S:	.byte	2				;fractionary scale
		_DRAW_ROT:	.byte	2				;rotation
		_DRAW_P1:	.byte	2				;to hold 1 parameter
		_DRAW_P2:	.byte	2				;to hold 2 parameter
		_DRAW_CTRL:	.byte	1				;control bits describle below
		_DRAW_SAVE_X:	.byte	2				;save X point
		_DRAW_SAVE_Y:	.byte	2				;save Y point
	.CSEG
	.EQU	_DRAW_CTRL_BIT_PEN	= 0			;bit 0=1 pen on
	.EQU	_DRAW_CTRL_BIT_COMMA	= 1			;bit 1=1 comma finded
	.EQU	_DRAW_CTRL_BIT_S_P1	= 2			;bit 2=0 P1 is a positive number
	.EQU	_DRAW_CTRL_BIT_S_P2	= 3			;bit 3=0 P2 ia a positive number
	;+--------------------------------------+
	;| Set DRAW center point to rotate	|
	;| 					|
	;| Input  : X,Y center point		|
	;| Output : None			|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_SET_CENTER_POINT:
		rjmp	_ROT2D_SET_CENTER_POINT
	;+--------------------------------------+
	;| Get DRAW center point to rotate	|
	;| 					|
	;| Input  : None			|
	;| Output : X,Y center point		|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_GET_CENTER_POINT:
		rjmp	_ROT2D_GET_CENTER_POINT
		
	;+------------------------------+
	;| Set DRAW start coordinate	|
	;| 				|
	;| Input  : X,Y start point	|
	;| Output : None		|
	;| Destroy: None		|
	;+------------------------------+
	_DRAW_SET_POINT:
		sts	_DRAW_X+0,XL
		sts	_DRAW_X+1,XH
		sts	_DRAW_Y+0,YL
		sts	_DRAW_Y+1,YH
		ret
	;+------------------------------+
	;| Get DRAW start coordinate	|
	;| 				|
	;| Input  : None		|
	;| Output : X,Y start point	|
	;| Destroy: None		|
	;+------------------------------+
	_DRAW_GET_POINT:
		lds	XL,_DRAW_X+0
		lds	XH,_DRAW_X+1
		lds	YL,_DRAW_Y+0
		lds	YH,_DRAW_Y+1
		ret
	;+------------------------------+
	;| Set DRAW save start point	|
	;| 				|
	;| Input  : None		|
	;| Output : None		|
	;| Destroy: None		|
	;+------------------------------+
	_DRAW_SAVE_POINT:
		push	Acc
		lds	Acc,_DRAW_X+0
		sts	_DRAW_SAVE_X+0,Acc
		lds	Acc,_DRAW_X+1
		sts	_DRAW_SAVE_X+1,Acc
		lds	Acc,_DRAW_Y+0
		sts	_DRAW_SAVE_Y+0,Acc
		lds	Acc,_DRAW_Y+1
		sts	_DRAW_SAVE_Y+1,Acc
		pop	Acc
		ret
	;+-------------------------------+
	;| Get DRAW old saved start point|
	;| and update start point	 |
	;| 				 |
	;| Input  : None		 |
	;| Output : X,Y start point	 |
	;| Destroy: None		 |
	;+-------------------------------+
	_DRAW_RESTORE_POINT:
		lds	XL,_DRAW_SAVE_X+0
		lds	XH,_DRAW_SAVE_X+1
		lds	YL,_DRAW_SAVE_Y+0
		lds	YH,_DRAW_SAVE_Y+1
		sts	_DRAW_X+0,XL
		sts	_DRAW_X+1,XH
		sts	_DRAW_Y+0,YL
		sts	_DRAW_Y+1,YH
		ret
	;+--------------------------------------+
	;| Set DRAW scale			|
	;| 					|
	;| Input  : Acc:AccH Scale 1/256 to 256	|
	;| Output : None			|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_SET_SCALE:
		sts	_DRAW_S+0,Acc
		sts	_DRAW_S+1,AccH
		ret
	;+--------------------------------------+
	;| Get DRAW scale			|
	;| 					|
	;| Input  : None			|
	;| Output : Acc:AccH Scale 1/256 to 256	|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_GET_SCALE:
		lds	Acc,_DRAW_S+0
		lds	AccH,_DRAW_S+1
		ret
	;+--------------------------------------+
	;| Set DRAW Rotation			|
	;| 					|
	;| Input  : Acc:AccH Angle Rotation	|
	;| Output : None			|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_SET_ROTATE_ANGLE:
		sts	_DRAW_ROT+0,Acc
		sts	_DRAW_ROT+1,AccH
		ret
	;+--------------------------------------+
	;| get DRAW Rotation			|
	;| 					|
	;| Input  : None			|
	;| Output : Acc:AccH Angle Rotation	|
	;| Destroy: None			|
	;+--------------------------------------+
	_DRAW_GET_ROTATE_ANGLE:
		lds	Acc,_DRAW_ROT+0
		lds	AccH,_DRAW_ROT+1
		ret
		
	;+------------------------------------------------------+
	;| DRAW	routine used to draw image using vectors	|
	;|							|
	;| Commands description					|
	;|							|
	;| Ortogonal moves					|
	;|							|
	;| 	nnnU - draw a vector up    ( 90o)		|
	;| 	nnnR - draw a vector right (  0o)		|
	;| 	nnnD - draw a vector down  (270o)		|
	;| 	nnnL - draw a vector left  (180o)		|
	;|							|
	;| Diagonal moves					|
	;|							|
	;| 	nnnE - draw a vector up and right   ( 45o)	|
	;| 	nnnF - draw a vector down and right (315o)	|
	;| 	nnnG - draw a vector down and left  (227o)	|
	;| 	nnnH - draw a vector up and left    (135o)	|
	;|							|
	;| Free moves						|
	;|							|
	;|	sxxx,syyyP - draw a vector to PX+xxx,PY+yyy	|
	;|							|
	;| Pen control						|
	;|							|
	;|	W - Turn pen on					|
	;|	B - Turn pen off				|
	;|							|
	;| Input  : Z --> Draw commands into flash		|
	;| Output : None					|
	;| Destroy: Flags,R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11	|
	;|          R12,R13,R14,15				|		
	;+------------------------------------------------------+
	_DRAW:
		push	Acc					;Save Useds
		push	AccH
		push	AccT
		push	AccTH
		push	XL
		push	XH
		push	YL
		push	YH
		rcall	_DRAW_GET_ROTATE_ANGLE			;get rotate angle
		rcall	_ROT2D_SET_ANGLE			;set angle
		rcall	_DRAW_GET_POINT
		rcall	_ROT2D_SET_POINT			;set point to rotate
		rcall	_ROT2D_ROTATE				;rotate this point
		rcall	_ROT2D_GET_ROTATED_POINT		;get a rotated point
		rcall	_GRAPH_MOVE_TO_EX			;assume a initial point
		clr	Acc
		sts	_DRAW_P1+0,Acc				;clear P1
		sts	_DRAW_P1+1,Acc
		sts	_DRAW_P2+0,Acc				;clear P2
		sts	_DRAW_P2+1,Acc
		ldi	Acc,(1<<_DRAW_CTRL_BIT_PEN)		;pen allways start on
								;comma off
								;P1 & P2 positive
		sts	_DRAW_CTRL,Acc	
		sbiw	ZL,1					;only to process command correctly
	_DRAW_NEXT:
		adiw	ZL,1					;next command
		lpm						;get command
		mov	Acc,r0					;move command to Acc
		cpi	Acc,0					;end command string ?
		brne	_DRAW00					;no, brach
	;+--------------------------------------
	;| Process command string termination
	;+--------------------------------------
		pop	YH
		pop	YL
		pop	XH
		pop	XL
		pop	AccTH					;restore useds
		pop	AccT
		pop	AccH
		pop	Acc
		ret						;end routine
	_DRAW00:
		cpi	Acc,','					;parameter separator ?
		brne	_DRAW10					;no, branch
	;+--------------------------------------
	;| Process (,) comma parameters separator
	;+--------------------------------------
		lds	Acc,_DRAW_CTRL				;set comma control
		ori	Acc,(1<<_DRAW_CTRL_BIT_COMMA)
		sts	_DRAW_CTRL,Acc
		rjmp	_DRAW_NEXT				;next command
	_DRAW10:
		cpi	Acc,'-'					;negative ?
		brne	_DRAW10_00				;no, branch
	;+--------------------------------------
	;| Process (-) signal
	;+--------------------------------------
		lds	Acc,_DRAW_CTRL				;see comma state
		sbrc	Acc,_DRAW_CTRL_BIT_COMMA		;skip if comma clear ?
		rjmp	_DRAW10_05
		ori	Acc,(1<<_DRAW_CTRL_BIT_S_P1)		;turn p1 signal on
		rjmp	_DRAW10_07				;update
	_DRAW10_05:
		ori	Acc,(1<<_DRAW_CTRL_BIT_S_P2)		;turn p2 signal on
	_DRAW10_07:
		sts	_DRAW_CTRL,Acc
		rjmp	_DRAW_NEXT				;next command
	_DRAW10_00:
		cpi	Acc,'0'					;numbers ?
		brlo	_DRAW15					;no, branch
		cpi	Acc,'9'+1
		brsh	_DRAW15					;no, branch
	;+--------------------------------------
	;| Process numbers
	;+--------------------------------------
		andi	Acc,0x0f				;tranform ascii digit to bcd
		mov	r0,Acc
		lds	r1,_DRAW_CTRL				;comma seted ?
		sbrc	r1,_DRAW_CTRL_BIT_COMMA			;no, skip
		rjmp	_DRAW11	
		lds	Acc,_DRAW_P1+0				;else data from P1
		lds	AccH,_DRAW_P1+1
		rjmp	_DRAW12
	_DRAW11:
		lds	Acc,_DRAW_P2+0				;assume data from P2
		lds	AccH,_DRAW_P2+1
	_DRAW12:
		rcall	_DRAW_X10
		lds	r1,_DRAW_CTRL				;comma seted ?
		sbrc	r1,_DRAW_CTRL_BIT_COMMA			;no, skip
		rjmp	_DRAW13	
		sts	_DRAW_P1+0,Acc				;else store data into P1
		sts	_DRAW_P1+1,AccH
		rjmp	_DRAW14
	_DRAW13:
		sts	_DRAW_P2+0,Acc				;store data into P2
		sts	_DRAW_P2+1,AccH
	_DRAW14:
		rjmp	_DRAW_NEXT
	;+--------------------------------------
	;| Acc:AccH=Acc:AccH*10+r0
	;+--------------------------------------
	_DRAW_X10:
		lsl	Acc
		rol	AccH
		mov	AccT,Acc
		mov	AccTH,AccH
		lsl	Acc
		rol	AccH
		lsl	Acc
		rol	AccH
		add	Acc,AccT
		adc	AccH,AccTH
		add	Acc,r0
		ldi	AccT,0
		adc	AccH,AccT
		ret
	_DRAW15:
		andi	Acc,~(0x20)				;turn all characters up case
		cpi	Acc,'U'					;command U ?
		brne	_DRAW20					;no, branch
	;+--------------------------------------
	;| Process U command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_Y_DEC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW20:
		cpi	Acc,'R'					;command R ?
		brne	_DRAW30					;no, branch
	;+--------------------------------------
	;| Process R command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_INC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW30:
		cpi	Acc,'D'					;command D ?
		brne	_DRAW40					;no, branch
	;+--------------------------------------
	;| Process D command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_Y_INC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW40:
		cpi	Acc,'L'					;command L ?
		brne	_DRAW50					;no, branch
	;+--------------------------------------
	;| Process L command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_DEC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW50:
		cpi	Acc,'E'					;command E ?
		brne	_DRAW60					;no, branch
	;+--------------------------------------
	;| Process E command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_INC) | (1<< _DRAW_AXIS_Y_DEC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW60:
		cpi	Acc,'F'					;command F ?
		brne	_DRAW70					;no, branch
	;+--------------------------------------
	;| Process F command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_INC) | (1<<_DRAW_AXIS_Y_INC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW70:
		cpi	Acc,'G'					;command G ?
		brne	_DRAW80					;no, branch
	;+--------------------------------------
	;| Process G command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_DEC) | (1<<_DRAW_AXIS_Y_INC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW80:
		cpi	Acc,'H'					;command G ?
		brne	_DRAW90					;no, branch
	;+--------------------------------------
	;| Process W command
	;+--------------------------------------
		ldi	Acc,(1<<_DRAW_AXIS_X_DEC) | (1<<_DRAW_AXIS_Y_INC)
		rcall	_DRAW_PROCESS				;perform draw process
		rjmp	_DRAW_NEXT				;next BIT
	_DRAW90:
		cpi	Acc,'W'					;command W ?
		brne	_DRAW100				;no, branch
	;+--------------------------------------
	;| Process W command (pen on)
	;+--------------------------------------
		lds	Acc,_DRAW_CTRL				;get control
		ori	Acc,(1<<_DRAW_CTRL_BIT_PEN)		;turn pen on
		sts	_DRAW_CTRL,Acc
		rjmp	_DRAW_NEXT				;update
	_DRAW100:
		cpi	Acc,'B'					;command B ?
		brne	_DRAW110				;no, branch
	;+--------------------------------------
	;| Process B command (pen off)
	;+--------------------------------------
		lds	Acc,_DRAW_CTRL				;get control
		andi	Acc,~(1<<_DRAW_CTRL_BIT_PEN)		;turn pen off
		sts	_DRAW_CTRL,Acc
		rjmp	_DRAW_NEXT				;update
	_DRAW110:
		cpi	Acc,'P'					;command P ?
		brne	_DRAW120				;no, branch
	;+--------------------------------------
	;| Process P command Free Move
	;+--------------------------------------
		rcall	_DRAW_P1XS				;P1xScale
		lds	r0,_DRAW_CTRL				;get control
		ldi	Acc,(1<<_DRAW_AXIS_X_INC)		;assume inc axis x
		sbrc	r0,_DRAW_CTRL_BIT_S_P1			;skip next instruction if positive
		ldi	Acc,(1<<_DRAW_AXIS_X_DEC)		;else dec axis x
		mov	r0,Acc					;get move parameters
		lds	r1,_DRAW_P1+0				;get p1
		lds	r2,_DRAW_P1+1
		rcall	_DRAW_AXIS_MOVE				;move Axis X with P1
		rcall	_DRAW_P2XS				;P2xScale
		lds	r0,_DRAW_CTRL				;get control
		ldi	Acc,(1<<_DRAW_AXIS_Y_INC)		;assume inc axis x
		sbrc	r0,_DRAW_CTRL_BIT_S_P2			;skip next instruction if positive
		ldi	Acc,(1<<_DRAW_AXIS_Y_DEC)		;else dec axis x
		mov	r0,Acc					;get move parameters
		lds	r1,_DRAW_P2+0				;get p1
		lds	r2,_DRAW_P2+1
		rcall	_DRAW_AXIS_MOVE				;move Axis Y with P2
		rcall	_DRAW_PROCESS_2ND			;process second entry
	_DRAW120:
		rjmp	_DRAW_NEXT
	;+--------------------------------------
	;| Directional commands process
	;+--------------------------------------
	_DRAW_PROCESS:
		rcall	_DRAW_P1XS				;multiply point by scale
		mov	r0,Acc					;get move parameters
		lds	r1,_DRAW_P1+0				;get p1
		lds	r2,_DRAW_P1+1
		rcall	_DRAW_AXIS_MOVE				;move axis
	_DRAW_PROCESS_2ND:
		rcall	_DRAW_GET_POINT				;Get A new point
		rcall	_ROT2D_SET_POINT			;set point to rotate
		rcall	_ROT2D_ROTATE				;rotate this point
		rcall	_ROT2D_GET_ROTATED_POINT		;get a rotated point
		lds	Acc,_DRAW_CTRL				;set pen state
		sbrs	Acc,_DRAW_CTRL_BIT_PEN			;skip if yes
		rjmp	_DRAW_PR10
		ldi	Acc,_ON					;set line on
		rcall	_GRAPH_LINE_TO				;draw a line
	;	rcall	_GRAPH_MOVE_TO_EX			;<--- used only for test porpose
		rjmp	_DRAW_PR20
	_DRAW_PR10:
		rcall	_GRAPH_MOVE_TO_EX			;else only update coordinates
	;+--------------------------------------
	;| clear default after command execution
	;+--------------------------------------
	_DRAW_PR20:
		clr	Acc
		sts	_DRAW_P1+0,Acc				;clear P1
		sts	_DRAW_P1+1,Acc
		sts	_DRAW_P2+0,Acc				;clear P2
		sts	_DRAW_P2+1,Acc
		ldi	Acc,(1<<_DRAW_CTRL_BIT_PEN)		;pen on
								;comma off
								;P1 & P2 positive
		sts	_DRAW_CTRL,Acc	
		ret
		
	;+----------------------------------------------+
	;| Process Axis offset according		|
	;|						|
	;| Input   : r0 bit 0=1 X+=nnn			|
	;|	 	bit 1=1 X-=nnn			|
	;|		bit 2=1 Y+=nnn			|
	;|		bit 3=1 Y-=nnn			|
	;|	     r1:r2 offset			|
	;| Output  : None				|
	;| Destroy :Flags,r0,r1,r2,Acc,AccH,AccT,AccTH	|
	;+----------------------------------------------+
	_DRAW_AXIS_MOVE:
		rcall	_DRAW_GET_POINT				;get actual point
		sbrs	r0,_DRAW_AXIS_X_INC			;increment X axis by offset ?
		rjmp	_DRAW_AXIS00				;no, branch
	;+--------------------
	;| X+=nnn
	;+--------------------	
		add	XL,r1					;X+=nnn
		adc	XH,r2
	_DRAW_AXIS00:
		sbrs	r0,_DRAW_AXIS_X_DEC			;derement X axis by offset ?
		rjmp	_DRAW_AXIS10				;no, branch
	;+--------------------
	;| X-=nnn
	;+--------------------	
		sub	XL,r1					;X-=nnn
		sbc	XH,r2
	_DRAW_AXIS10:
		sbrs	r0,_DRAW_AXIS_Y_INC			;increment Y axis by offset ?
		rjmp	_DRAW_AXIS20				;no, branch
	;+--------------------
	;| Y+=nnn
	;+--------------------	
		add	YL,r1					;Y+=nnn
		adc	YH,r2
	_DRAW_AXIS20:
		sbrs	r0,_DRAW_AXIS_Y_DEC			;decrement Y axis by offset ?
		rjmp	_DRAW_AXIS30				;no, branch
	;+--------------------
	;| Y-=nnn
	;+--------------------	
		sub	YL,r1					;Y-=nnn
		sbc	YH,r2
	_DRAW_AXIS30:
		rcall	_DRAW_SET_POINT				;update
		ret
	;+--------------------
	;| multiply P1*Scale
	;+--------------------
	_DRAW_P1XS:
		push	Acc
		lds	r0,_DRAW_P1+0				;get P1
		lds	r1,_DRAW_P1+1
		rcall	_DRAW_MUL				;multiply
		sts	_DRAW_P1+0,r9				;store new value into P1 again
		sts	_DRAW_P1+1,r10
		pop	Acc
		ret	
	;+--------------------
	;| multiply P2*Scale
	;+--------------------
	_DRAW_P2XS:
		push	Acc
		lds	r0,_DRAW_P2+0				;get P1
		lds	r1,_DRAW_P2+1
		rcall	_DRAW_MUL				;multiply
		sts	_DRAW_P2+0,r9				;store new value into P1 again
		sts	_DRAW_P2+1,r10
		pop	Acc
		ret	
	;+--------------------
	;| multiply p1 or p2 x scale
	;+--------------------
	_DRAW_MUL:
		clr	r2
		clr	r3
		lds	r4,_DRAW_S+0				;get Scale
		lds	r5,_DRAW_S+1
		clr	r6
		clr	r7
		rcall	_MUL_U32U32U32				;Multiply
		ret	
#endif
.exit
			

