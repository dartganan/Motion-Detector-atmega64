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
;+--------------------------------------------------------------+---+---+
;|									|
;| DATA:12/04/2001							|
;>----------------------------------------------------------------------<
;| L I N E  1 6  B I T  R O U T I N E 					|
;+----------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM			|
;| AND USE A BRESEHAM ALGORITHM 					|
;+----------------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N S 		|
;+------------------------------------------------------+---------------------------------------+
;| _GRAPH_LINE_TO	Draw a line segment							|
;|												|
;|			ldi	Acc,LOW(100)	;Draw a line at (100,75) to (200,300)		|
;|			ldi	AccH,HIGH(100)	;Source coordinates (100,75)			|
;|			ldi	AccT,LOW(75)							|
;|			ldi	AccTH,HIGH(75)							|
;|			rcall	_GRAPH_MOVE_TO	;move to start coordinates			|
;|			ldi	XL,LOW(200)	;Destiny coordinates (200,300)			|
;|			ldi	XH,HIGH(200)							|
;|			ldi	YL,LOW(300)							|
;|			ldi	YH,HIGH(300)							|
;|			ldi	Acc,ON		;pixel on					|
;|			rcall	_GRAPH_LINE_TO	;Draw a line segment				|
;|----------------------------------------------------------------------------------------------+
;| _GRAPH_MOVE_TO	Move Graph Cursor							|
;|			see _GRAPH_LINE_TO example						|
;|----------------------------------------------------------------------------------------------+
;| _GRAPH_MOVE_TO_EX	Move Graph Cursor							|
;|			see _GRAPH_LINE_TO example but use X,Y register to set new start point	|
;|----------------------------------------------------------------------------------------------+
;| _GRAPH_GET_POINT	Get start point of line or end point if _GRAPH_LINE_TO is performated	|
;+----------------------------------------------------------------------------------------------+
#ifndef __GRAPH_LINE__
	#define	__GRAPH_LINE__
	.message "+------------------+"
	.message "|                  |"
	.message "| GRAPH LINE       |" 
	.message "|                  |"
	.message "+------------------+"
	;+----------------------------------------+
	;| LineTo				  |
	;| Input: X =  end coordinate	 	  |
	;|	  Y =  end coordinate	 	  |
	;|	  _GraphXStart = start coordinate |
	;|	  _GraphYStart = start coordinate | 
	;|	  Acc 0 reset 0xff set pixel	  |
	;|					  |
	;| Destroy: r0..r13			  |
	;| Obs:					  |
	;| _GraphXStart,_GraphYStart is updated	  |
	;| with X,Y end coordinates
	;+----------------------------------------+
	;					   \______________________________________________________________________________________________
	.DEF	_Graph_CtL   	= r0			;Ct
	.DEF	_Graph_CtH   	= r1
	.DEF	_Graph_DL    	= r2			;D
	.DEF	_Graph_DH	= r3 		
	.DEF	_Graph_XnL	= r4			;Xn
	.DEF	_Graph_XnH	= r5
	.DEF	_Graph_YnL	= r6			;Yn
	.DEF	_Graph_YnH	= r7
	.DEF	_Graph_XdL	= r8			;Xd
	.DEF	_Graph_XdH	= r9
	.DEF	_Graph_YdL	= r10			;Yd
	.DEF	_Graph_YdH	= r11
	.DEF	_Graph_tmpL	= r12			;temp
	.DEF	_Graph_tmpH	= r13
	
	.DSEG
		_GraphXStart:		.byte 2		;define Start X coordinate into SRAM
		_GraphYStart:		.byte 2		;define Start Y coordinate into SRAM
	.CSEG
	
	_GRAPH_LINE_TO:
		push	XL
		push	XH
		push	YL
		push	YH
		push	AccH
		push	Acc
		ldi	Acc,1				;xn=1;xd=1
		mov	_Graph_XnL,Acc
		mov	_Graph_XdL,Acc
		clr	Acc
		mov	_Graph_XnH,Acc
		mov	_Graph_XdH,Acc
		lds	Acc,_GraphXStart+0		;Xend-X
		sub	Acc,XL
		sts	_GraphXStart+0,Acc
		lds	Acc,_GraphXStart+1
		sbc	Acc,XH
		sts	_GraphXStart+1,Acc
		brpl	_GRAPH_L00			;branch if plus
		lds	Acc,_GraphXStart+0		;abs(Xend-X)
		com	Acc
		sts	_GraphXStart+0,Acc
		lds	Acc,_GraphXStart+1
		com	Acc
		sts	_GraphXStart+1,Acc
		lds	Acc,_GraphXStart+0
		subi	Acc,low(-1)
		sts	_GraphXStart+0,Acc
		lds	Acc,_GraphXStart+1
		sbci	Acc,high(-1)
		sts	_GraphXStart+1,Acc
		ldi	Acc,0xff
		mov	_Graph_XnL,Acc			;Xn=-1
		mov	_Graph_XnH,Acc	
		mov	_Graph_XdL,Acc			;Xd=-1
		mov	_Graph_XdH,Acc
	_GRAPH_L00:
		ldi 	Acc,1				;Yn=1;yd=1
		mov	_Graph_YnL,Acc
		mov	_Graph_YdL,Acc
		clr	Acc
		mov	_Graph_YnH,Acc
		mov	_Graph_YdH,Acc
		lds	Acc,_GraphYStart+0		;Yend-Y
		sub	Acc,YL
		sts	_GraphYStart+0,Acc
		lds	Acc,_GraphYStart+1
		sbc	Acc,YH
		sts	_GraphYStart+1,Acc
		brpl	_GRAPH_L10			;branch if plus
		lds	Acc,_GraphYStart+0		;abs(Yend-Y)
		com	Acc
		sts	_GraphYStart+0,Acc
		lds	Acc,_GraphYStart+1
		com	Acc
		sts	_GraphYStart+1,Acc
		lds	Acc,_GraphYStart+0
		subi	Acc,low(-1)
		sts	_GraphYStart+0,Acc
		lds	Acc,_GraphYStart+1
		sbci	Acc,high(-1)
		sts	_GraphYStart+1,Acc
		ldi	Acc,0xff			;yn=-1
		mov	_Graph_YnL,Acc
		mov	_Graph_YnH,Acc
		mov	_Graph_YdL,Acc			;yd=-1
		mov	_Graph_YdH,Acc
	_GRAPH_L10:
		lds	Acc,_GraphXStart+0
		lds	AccH,_GraphYStart+0
		cp	Acc,AccH			;Xend=A>Yend=B
		lds	Acc,_GraphXStart+1
		lds	AccH,_GraphYStart+1
		cpc	Acc,AccH
		brge	_GRAPH_L20			;yes, jump
		clr	Acc				;xn=0
		mov	_Graph_XnL,Acc
		mov	_Graph_XnH,Acc
		lds	Acc,_GraphXStart+1		;A<->B
		push	Acc
		lds	Acc,_GraphXStart+0
		lds	_Graph_tmpL,_GraphYStart+0
		lds	_Graph_tmpH,_GraphYStart+1
		sts	_GraphXStart+0,_Graph_tmpL
		sts	_GraphXStart+1,_Graph_tmpH
		sts	_GraphYStart+0,Acc
		pop	Acc
		sts	_GraphYStart+1,Acc
		rjmp	_GRAPH_L30
	_GRAPH_L20:	
		clr	Acc				;yn=0
		mov	_Graph_YnL,Acc
		mov 	_Graph_YnH,Acc
	_GRAPH_L30:
		lds	_Graph_CtL,_GraphXStart+0	;ct=a
		lds 	_Graph_CtH,_GraphXStart+1
		ldi	Acc,1				;ct=a+1
		add	_Graph_CtL,Acc
		ldi	Acc,0
		adc	_Graph_CtH,Acc
		lds	_Graph_DL,_GraphYStart+0	;d=b
		lds	_Graph_DH,_GraphYStart+1
		lsl	_Graph_DL			;d=b*2
		rol	_Graph_DH
		lds	Acc,_GraphXStart+0
		sub	_Graph_DL,Acc			;d=b*2-a
		lds	Acc,_GraphXStart+1
		sbc	_Graph_DH,Acc
	;+---------------+
	;| do while ct>0 |
	;+---------------+
	_GRAPH_WHILE:
		tst	_Graph_CtH
		brmi	_GRAPH_LDONE			;yes, done
		tst	_Graph_CtL
		brne	_GRAPH_L35
		tst	_Graph_CtH
		brne	_GRAPH_L35
		rjmp	_GRAPH_LDONE
	_GRAPH_L35:
		pop	Acc				;get pixel
		push	Acc				;save again
		rcall	_DISP_PSET			;set a pixel
		ldi	Acc,1
		sub	_Graph_CtL,Acc			;ct--
		ldi	Acc,0
		sbc	_Graph_CtH,Acc
		tst	_Graph_DH			;d>0 ?
		brpl	_GRAPH_L40			;yes, jump
		add	XL,_Graph_XnL			;x+=xn
		adc	XH,_Graph_XnH
		add	YL,_Graph_YnL			;y+=yn
		adc	YH,_Graph_YnH
		lds	Acc,_GraphYStart+0
		mov	_Graph_tmpL,Acc			;tmp=b
		lds	Acc,_GraphYStart+1
		mov	_Graph_tmpH,Acc
		rjmp	_GRAPH_L50
	_GRAPH_L40:
		add	XL,_Graph_XdL			;x+=xd
		adc	XH,_Graph_XdH
		add	YL,_Graph_YdL			;y+=yd
		adc	YH,_Graph_YdH
		lds	Acc,_GraphYStart+0
		mov	_Graph_tmpL,Acc			;tmp=b
		lds	Acc,_GraphYStart+1
		mov	_Graph_tmpH,Acc
		lds	Acc,_GraphXStart+0
		sub	_Graph_tmpL,Acc			;tmp=b-a
		lds	Acc,_GraphXStart+1
		sbc	_Graph_tmpH,Acc	
	_GRAPH_L50:
		lsl	_Graph_tmpL			;tmp*=2
		rol	_Graph_tmpH
		add	_Graph_DL,_Graph_tmpL		;d+=tmp*2
		adc	_Graph_DH,_Graph_tmpH
		rjmp	_GRAPH_WHILE
	_GRAPH_LDONE:
		pop	Acc
		pop	AccH
		pop	YH
		sts	_GraphYStart+1,YH		;Ystart=Yend
		pop	YL
		sts	_GraphYStart+0,YL
		pop	XH
		sts	_GraphXStart+1,XH		;Xstart=Xend
		pop	XL
		sts	_GraphXStart+0,XL
		ret
	;+------------------------------------------+
	;| MoveTo				    |
	;| Input: Acc : AccH = x coordinate to move |
	;|	  AccT:AccTH = y coordinate to move |
	;+------------------------------------------+
	;					     \___________________________________________________________________________________________
	_GRAPH_MOVE_TO:
		sts	_GraphXStart+0,Acc
		sts	_GraphXStart+1,AccH
		sts	_GraphYStart+0,AccT
		sts	_GraphYStart+1,AccTH
		ret
	;+------------------------------------------+
	;| MoveToEx				    |
	;| Input: X coordinate to move 		    |
	;|	  Y coordinate to move 		    |
	;+------------------------------------------+
	;					     \___________________________________________________________________________________________
	_GRAPH_MOVE_TO_EX:
		sts	_GraphXStart+0,XL
		sts	_GraphXStart+1,XH
		sts	_GraphYStart+0,YL
		sts	_GraphYStart+1,YH
		ret
	;+------------------------------------------+
	;| get Point of virtual pen		    |
	;| Output:X coordinate 			    |
	;|	  Y coordinate	 		    |
	;+------------------------------------------+
	;					     \___________________________________________________________________________________________
	_GRAPH_GET_POINT:
		lds	XL,_GraphXStart+0
		lds	XH,_GraphXStart+1
		lds	YL,_GraphYStart+0
		lds	YH,_GraphYStart+1
		ret
#endif
.exit

