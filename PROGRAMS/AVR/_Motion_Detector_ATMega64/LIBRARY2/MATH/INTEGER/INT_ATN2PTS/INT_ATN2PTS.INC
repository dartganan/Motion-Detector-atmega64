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
;+--------------------------------------------------------------+---+-----------+
;|										|
;| DATE:17/12/2001 VERSION 1.0							|
;>------------------------------------------------------------------------------<
;| I N T E G E R  R O U T I N E S	     					|
;| I N T E G E R  A R C  T A N G E N T  2  P O I N T S				|
;+------------------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM				|
;+------------------------------------------------------------------------------+
#ifndef	__INTEGER_ATN2PTS__
	#define __INTEGER_ATN2PTS__
	.message "[ (MATH) INTEGER ATN2PTS ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef __INTEGER_ATN_S16S16S16__
		.INCLUDE	"MATH\INTEGER\INT_ATN\INT_ATN.INC"
	#endif
	;+----------------------------------------------------------------------+
	;| COMPUTE ANGLE BETWEEN TWO POINTS ONE CENTRAL WITH COORDINATES	|
	;| IS P(CX,CY) AND OTHER P(PX,PY)					|
	;|									|
	;| INPUT	: AccH:Acc   Center CX point				|
	;|		  AccTH:AccT Center CY point				|
	;|		  X	     PX point					|
	;|		  Y	     PY point					|
	;| OUTPUT	: AccH:Acc   Angle in degrees				|
	;| Destroy	: r0..r11						|
	;| Time		:							|
	;+----------------------------------------------------------------------+
	_INT_ATN2PTS:
		;/---------------------------------------------
		;| compute deltas to see that quadrant PX are
		;\---------------------------------------------
		sub	XL,Acc				;Delta X=PX-CX
		sbc	XH,AccH
		sub	YL,AccT				;Delta Y=PY-CY
		sbc	YH,AccTH
		tst	XH				;X>=0?
		brmi	_INT_ATN2PTS20			;no, branch to 2o or 3o quadrant
		tst	YH				;Y>=0?
		brmi	_INT_ATN2PTS10			;no, branch to 4o quadrant
		;/---------------------------------------------
		;| 1o quadrant return atn(y/x)
		;\---------------------------------------------
		rcall	_INT_ATN_XY			;compute atn(y/x)
		ret
		;/---------------------------------------------
		;| 4o quadrant return 360-atn(y/x)
		;\---------------------------------------------
	_INT_ATN2PTS10:
		rcall	_INT_ATN_XY			;compute 360+atn(y/x)
		subi	Acc,low(-360)			
		sbci	AccH,high(-360)
		ret
	_INT_ATN2PTS20:
		;/---------------------------------------------
		;| 2o and 3o  quadrant return 180+atn(y/x)
		;\---------------------------------------------
		rcall	_INT_ATN_XY			;compute 180+atn(y/x)
		subi	Acc,low(-180)			
		sbci	AccH,high(-180)
		ret
			
#endif
.exit

