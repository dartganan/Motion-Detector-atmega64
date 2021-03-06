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
;+--------------------------------------------------------------+---+-------------------+
;|											|
;| DATA:1/12/2002									|
;>--------------------------------------------------------------------------------------<
;| P O I N T  R O T A T I O N  2  D I M E N S I O N 			 		|
;+--------------------------------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM					|
;+--------------------------------------------------------------------------------------+
;>------------------------------------------------------<
;| I M P L E M E N T E D  F U N C T I O N  		|
;+------------------------------------------------------+---------------------------------------+
;| _ROT2D_SET_POINT	set point to be rotated							|
;|												|
;|			ldi	XL,low(315)		;set point to rotate p(315,100)		|
;|			ldi	XH,high(315)							|
;|			ldi	YL,low(100)							|
;|			ldi	YH,high(100)							|
;|			rcall	_ROTD2_SET_POINT	;set					|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_GET_POINT	return point to be rotated						|
;|												|
;|			rcall	_ROT2D_GET_POINT	;X,Y a point to be rotated		|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_SET_CENTER_POINT	set center point of rotation					|
;|												|
;|			ldi	XL,low(160)		;set center point to p(160,120)		|
;|			ldi	XH,high(160)							|
;|			ldi	YL,low(120)							|
;|			ldi	YH,high(120)							|
;|			rcall	_ROTD2_SET_CENTER_POINT	;set					|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_GET_CENTER_POINT	return center point						|
;|												|
;|			rcall	_ROT2D_GET_CENTER_POINT	;X,Y with center point			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_SET_ANGLE	set Angle to rotate a point						|
;|												|
;|			ldi	Acc,low(60)		;set angle to 60 degree			|
;|			ldi	AccH,high(60)							|
;|			rcall	_ROTD2_SET_ANGLE	;set					|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_GET_ANGLE	return angle of rotation						|
;|												|
;|			rcall	_ROT2D_GET_ANGLE	;Acc:AccH with Angle			|
;|												|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_ROTATE	start point rotation							|
;|												|
;|			rcall	_ROT2D_ROTATE		;rotate point p(x,y) with center in	|
;|							;p(cx,cy) by Angle			|
;|							;use _ROT2D_GET_ROTATED_POINT		|
;|							;to obtain a rotated point		|
;+----------------------------------------------------------------------------------------------+
;| _ROT2D_GET_ROTATED_POINT	get rotated point						|
;|												|
;|			rcall	_ROT2D_ROTATE		;rotate point p(x,y) with center in	|
;|							;p(cx,cy)				|
;|							;use _ROT2D_GET_ROTATED_POINT		|
;|							;to obtain a rotated point		|
;+----------------------------------------------------------------------------------------------+
;+----------------------------------------------------------------------+
;| Perform point rotation using the below equation			|
;| at p(Xi,Yi) with angle A between Normal and P(Xi,Yi)-P(Xf,Yf) segment|
;|									|
;|		    							|
;|    P(Cx,Cy) O---------------o P(x,y) <-- before rotation		|
;|	 	\ A  /							|
;|	  	 \  /							|
;|	   	  \/							|
;|	    	   \							|
;|	     	    \							|
;|	      	     \							|
;|	       	      \							|
;|		       \						|
;|	       P(Rx,Ry) O <-- after rotation				|
;|									|
;|	P(Rx)=P(Cx)+(a*K-b*J)						|
;|	P(Ry)=P(Cy)+(b*K+a*J)						|
;|									|
;|	where								|
;|		a=P(x)-P(cx)						|
;|		b=P(y)-P(cy)						|
;|		K=cos(Angle rotation)					|
;|		J=sin(Angle rotation)					|
;+----------------------------------------------------------------------+
;+------------------------------------------------------------------------------------------+
;| Equation definition			                                                    |
;|                                                                                          |
;| (1) - rotation of point P(px,py) with center at P(cx,cy) by a alfa angle	            |
;|                                                                                          |
;|       +--------------------                                                              |
;|       |        2         2                                                               |
;|    r=\| (px-cx) + (py-cy)        distance between points                                 |
;|                                                                                          |
;|  alfa=atn2pts(cx,cy,px,py)       angle between p(px,py) and P(cx,cy)                     |
;|                                                                                          |
;|  npx=cx+r*cos(alfa+beta)         rotation of x axis by beta angle                        |
;|  npy=cy+r*sin(alfa+beta)         rotation of y axis by beta angle                        |
;|                                                                                          |
;| Simplifying                                                                              |
;|                                                                                          |
;|               (px,py)                                a                                   |
;|                o             a=py-cy         sin(A)=---                                  |
;|               /|             b=px-cx                 c                                   |
;|              / |                +---------                                               |
;|           c /  | a              |  2   2             b                                   |
;|            /   |             c=\| a + b      cos(A)=---                                  |
;|           /)A  |                                     c                                   |
;|          o-----+                                                                         |
;|      (cx,cy) b                                                                           |
;|                                                                                          |
;| using a law of sun of sine and co-sines, follow                                          |
;|                                                                                          |
;|  sin(A+k)=sin(A)*cos(k)+sin(k)*cos(A)        k=rotation angle	                    |
;|  cos(A+k)=cos(A)*cos(k)-sin(A)*sin(k)                                                    |
;|                                                                                          |
;| replacing by equation (1)                                                                |
;|                                                                                          |
;|            a         b                                                                   |
;|  sin(A+k)=--- k + j ---              cos(k) and sin(k) are constants then,             |
;|            c         c               k=cos(k), j=sin(k)                                  |
;|                                                                                          |
;|            b         a                                                                   |
;|  cos(A+k)=--- k - j ---                                                                  |
;|            c         c                                                                   |
;|                                                                                          |
;|       +---------                                                                         |
;|       |  2   2                                                                           |
;|    c=\| a + b                     distance between P(px,py) and P(cx,cy)                 |
;|                                                                                          |
;|             b         a                                                                  |
;|  npx=cx+c*(--- k - j ---)         rotation of x axis by beta angle			    |
;|             c         c                                                                  |
;|                                                                                          |
;|             a         b                                                                  |
;|  npy=cy+c*(--- k + j ---)         rotation of y axis by beta angle                       |
;|             c         c                                                                  |
;|                                                                                          |
;| (2) - Cutting c=distance, we have a final equation 	                                    |
;|                                                                                          |
;| npx=cx+(b*k-a*j)        <-- no need distance to rotate				    |
;| npy=cy+(a*k+b*j)                                                                         |
;|                                                                                          |
;+------------------------------------------------------------------------------------------+
#ifndef	__INTEGER_ROTATION2D__
	#define __INTEGER_ROTATION2D__
	.message "[ (MATH) INTEGER ROTATION2D ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef	__INTEGER_DISCRETE_COSINE__
		.INCLUDE	"MATH\TRIGONOMETRY\DISCRETE_COSINE\DISCRETE_COSINE.INC"
	#endif
	#ifndef	__INTEGER_DISCRETE_SINE__
		.INCLUDE	"MATH\TRIGONOMETRY\DISCRETE_SINE\DISCRETE_SINE.INC"
	#endif
	.DSEG
		_ROT2D_X:		.byte	2				;start point
		_ROT2D_Y:		.byte	2
		_ROT2D_CX:		.byte	2				;center point
		_ROT2D_CY:		.byte	2
		_ROT2D_ROT_X:		.byte	2				;rotate point
		_ROT2D_ROT_Y:		.byte	2
		_ROT2D_A:		.byte	2				;rotation angle
	.CSEG
	;+------------------------------+
	;| Set start point coordinates	|
	;|				|
	;| Input  : X start x point	|
	;|	    Y start y point	|
	;| Output : None		|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_SET_POINT:
		sts	_ROT2D_X+0,XL
		sts	_ROT2D_X+1,XH
		sts	_ROT2D_Y+0,YL
		sts	_ROT2D_Y+1,YH
		ret
	;+------------------------------+
	;| Get start point coordinates	|
	;| 				|
	;| Input  : None		|
	;| Output : X start x point	|
	;|	    Y start y point	|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_GET_POINT:
		lds	XL,_ROT2D_X+0
		lds	XH,_ROT2D_X+1
		lds	YL,_ROT2D_Y+0
		lds	YH,_ROT2D_Y+1
		ret
	;+------------------------------+
	;| Set center point coordinates	|
	;|				|
	;| Input  : X center x point	|
	;|	    Y center y point	|
	;| Output : None		|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_SET_CENTER_POINT:
		sts	_ROT2D_CX+0,XL
		sts	_ROT2D_CX+1,XH
		sts	_ROT2D_CY+0,YL
		sts	_ROT2D_CY+1,YH
		ret
	;+------------------------------+
	;| Get start point coordinates	|
	;| 				|
	;| Input  : None		|
	;| Output : X start x point	|
	;|	    Y start y point	|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_GET_CENTER_POINT:
		lds	XL,_ROT2D_CX+0
		lds	XH,_ROT2D_CX+1
		lds	YL,_ROT2D_CY+0
		lds	YH,_ROT2D_CY+1
		ret
	;+------------------------------+
	;| Get start point coordinates	|
	;| 				|
	;| Input  : None		|
	;| Output : X start x point	|
	;|	    Y start y point	|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_GET_ROTATED_POINT:
		lds	XL,_ROT2D_ROT_X+0
		lds	XH,_ROT2D_ROT_X+1
		lds	YL,_ROT2D_ROT_Y+0
		lds	YH,_ROT2D_ROT_Y+1
		ret
	;+------------------------------+
	;| Set rotation angle		|
	;|				|
	;| Input  : Acc:AccH Angle	|
	;| Output : None		|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_SET_ANGLE:
		sts	_ROT2D_A+0,Acc
		sts	_ROT2D_A+1,AccH
		ret
	;+------------------------------+
	;| Get rotation angle		|
	;| 				|
	;| Input  : None		|
	;| Output : Acc:AccH angle	|
	;| Destroy: None		|
	;+------------------------------+
	_ROT2D_GET_ANGLE:
		lds	Acc,_ROT2D_A+0
		lds	AccH,_ROT2D_A+1
		ret
	;+--------------------------------------+
	;| Rotate a point			|
	;|					|
	;| Input  : None			|
	;| Output : None			|
	;| Destroy: Flags,R0,R1,R2,R3,R4,R5,R6	|
	;|		  R7,R8,R9,R10,R11,R12	|
	;|		  R13,R14,R15		|
	;| Obs: 				|
	;|	used equation			|
	;|					|
	;|	P(Rx)=P(Cx)+(a*KC-b*JS)		|
	;|	P(Ry)=P(Cy)+(b*KC+a*JS)		|
	;|					|
	;|	a=P(x)-P(cx)			|
	;|	b=P(y)-P(cy)			|
	;|	KC=cos(Angle rotation)		|
	;|	JS=sin(Angle rotation)		|
	;+--------------------------------------+
	_ROT2D_ROTATE:
		push	Acc				;save useds
		push	AccH
		push	AccT
		push	AccTH
		push	XL
		push	XH
		push	YL
		push	YH
	;+------------------
	;| X coordinate
	;+------------------
		rcall	_ROT2D_PA			;get A Parm
		lds	Acc,_ROT2D_A+0			;get angle
		lds	AccH,_ROT2D_A+1
		rcall	_DSCOS16B			;Acc:AccH=a*KC
		push	Acc				;save it
		push	AccH
		rcall	_ROT2D_PB			;get B Parm
		lds	Acc,_ROT2D_A+0			;get angle
		lds	AccH,_ROT2D_A+1
		rcall	_DSSIN16B			;Acc:AccH=b*JS
		pop	AccTH				;restore a*KC
		pop	AccT
		sub	AccT,Acc			;AccT:AccTH=a*KC-b*JS
		sbc	AccTH,AccH
		rcall	_ROT2D_GET_CENTER_POINT		;AccT:AccTH=P(cx)+(a*KC-b*JS)
		add	AccT,XL
		adc	AccTH,XH
		sts	_ROT2D_ROT_X+0,AccT		;store new rotate X
		sts	_ROT2D_ROT_X+1,AccTH
	;+------------------
	;| Y coordinate
	;+------------------
		rcall	_ROT2D_PB			;get B Parm
		lds	Acc,_ROT2D_A+0			;get angle
		lds	AccH,_ROT2D_A+1
		rcall	_DSCOS16B			;Acc:AccH=b*KC
		push	Acc				;save it
		push	AccH
		rcall	_ROT2D_PA			;get A Parm
		lds	Acc,_ROT2D_A+0			;get angle
		lds	AccH,_ROT2D_A+1
		rcall	_DSSIN16B			;Acc:AccH=b*JS
		pop	AccTH				;restore a*KC
		pop	AccT
		add	AccT,Acc			;AccT:AccTH=b*KC+a*JS
		adc	AccTH,AccH
		rcall	_ROT2D_GET_CENTER_POINT		;AccT:AccTH=P(cx)+(a*KC-b*JS)
		add	AccT,YL
		adc	AccTH,YH
		sts	_ROT2D_ROT_Y+0,AccT		;store new rotate X
		sts	_ROT2D_ROT_Y+1,AccTH
		pop	YH
		pop	YL
		pop	XH
		pop	XL
		pop	AccTH
		pop	AccT
		pop	AccH
		pop	Acc
		ret	
	;+---------------------
	;| return a=AccT:AccTH=P(x)-P(cx)
	;+---------------------
	_ROT2D_PA:
		rcall	_ROT2D_GET_POINT		;compute A=AccT:AccTH=P(x)-P(cx)
		mov	AccT,XL
		mov	AccTH,XH
		rcall	_ROT2D_GET_CENTER_POINT
		sub	AccT,XL
		sbc	AccTH,XH
		ret
	;+---------------------
	;| return b=AccT:AccTH=P(y)-P(cy)
	;+---------------------
	_ROT2D_PB:
		rcall	_ROT2D_GET_POINT		;compute A=AccT:AccTH=P(y)-P(cy)
		mov	AccT,YL
		mov	AccTH,YH
		rcall	_ROT2D_GET_CENTER_POINT
		sub	AccT,YL
		sbc	AccTH,YH
		ret
#endif
.exit
	

