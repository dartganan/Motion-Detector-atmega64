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
;| DATE:23/01/2003 VERSION 1.0							|
;>--------------------------------------------------------------+---------------<
;| D O U B L E   F L O A T  P O I N T  R O U T I N E S		|
;| G E N E R A L  A R C  T A N G E N T				|
;+--------------------------------------------------------------+
;| THIS DRIVE HAS INTEND TO USE WITH AVRS WITH SRAM		|
;+--------------------------------------------------------------+
;+----------------------------------------------------------------------+	
;| DOUBLE FLOAT POINT ARC TANGENT					|
;|									|
;| Input  : Float Acc X							|
;| Output : Float Acc arc tangent of X,Acc Exception code		|
;| Destroy: Flags							|
;| Method : Describe below						|
;| 									|
;+----------------------------------------------+-----------------------+
;|						|
;| IMPLEMENTS A GENERAL ARC TANGENT FUNCTION	|
;|						|
;| The function SERATN compute a arc tangent but only	|
;| range from -(sqrt(2)-1) to (sqrt(2)+1), using this	|
;| function to calculate any value we need to 	|
;| perform the following steps.			|
;|						|
;| 1o See if arg less sqrt(2)-1, if yes compute	|
;| atn=SERATN(arg)				|
;|						|
;| 2o case else See if arg great than sqrt(2)+1,|
;| if yes compute atn=pi/2-SERATN(1/arg)	|
;|						|
;| 3o case else compute				|
;| atn=pi/4+SERATN((arg-1)/(arg+1))		|
;+----------------------------------------------+
#ifndef __MATH_FLOAT_ARC_TANGENT__
	
	#define __MATH_FLOAT_ARC_TANGENT__
	.message "[ (MATH) FLOAT ARC TANGENT ]"
	;+----------------------+
	;| NECESSARY INCLUDES	|
	;+----------------------+
	#ifndef __MATH_FLOAT_INFINIT__
		.INCLUDE	"MATH\FLOAT_DOUBLE\INFINIT_RESULT\DFINF.INC"
	#endif
	#ifndef __MATH_FLOAT_NORMALIZE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\NORMALIZE\DFNORM.INC"
	#endif
	#ifndef __MATH_FLOAT_ADDSUB__
		.INCLUDE	"MATH\FLOAT_DOUBLE\ADDSUB\DFADDSUB.INC"
	#endif
	#ifndef __MATH_FLOAT_MULDIV__
		.INCLUDE	"MATH\FLOAT_DOUBLE\MULDIV\DFMULDIV.INC"
	#endif
	
	#ifndef __MATH_FLOAT_UNARY__
		.INCLUDE	"MATH\FLOAT_DOUBLE\UNARY\UNARY.INC"
	#endif
	#ifndef __MATH_FLOAT_LOAD_STORE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\LOAD_STORE\DFLDXSTX.INC"
	#endif
	
	#ifndef __MATH_FLOAT_COMPARE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\COMPARE\DFCPM.INC"
	#endif
	#ifndef __MATH_FLOAT_ARC_TANGENT_SERIE__
		.INCLUDE	"MATH\FLOAT_DOUBLE\SERIES\ATN\SERIE_ATN.INC"
	#endif
	_DFATAN:
		push	ZL
		push	ZH
		push	_acc_s					;save arg signal
		rcall	_DFABS					;acc=abs arg
		rcall	_DFACCOP1				;op1=arg
		rcall	_DFLDSQRT2M1				;acc=sqrt2-1
		rcall	_DFACCOP2				;op2=acc
		_DFJP_GTEQ	_DFATN10			;arg>=sqrt2-1 ? branch if yes
		rcall	_DFOP1ACC				;acc=op1
		rcall	_DFSERATN				;compute arc tangent argument
	_DFATN_DONE:
		pop	Acc					;restore original signal
		andi	Acc,0x80				;only signal
		andi	_Acc_s,0x7f				;to bound at result
		or	_Acc_s,Acc
		pop	ZH
		pop	ZL
		ret
	_DFATN10:
		rcall	_DFLDSQRT2P1				;acc=sqrt2+1
		rcall	_DFACCOP2				;op2=acc
		_DFJP_LTEQ	_DFATN20			;arg<=sqrt2+1 ? branch
		rcall	_DFOP1ACC				;acc=op1
		rcall	_DFINV					;arg=1/arg
		rcall	_DFSERATN				;acc=SERATN(1/ARG)
		rcall	_DFACCOP2				;op2=arg
		rcall	_DFLDPI2				;acc=pi/2
		rcall	_DFACCOP1				;op1=acc
		rcall	_DFSUB					;acc=pi/2-SERATN(1/ARG)
		rjmp	_DFATN_DONE
	_DFATN20:
		rcall	_DFOP1ACC				;acc=arg
		_DFSTACC	_DF_P10				;_DF_P10=arg
		rcall	_DFLD1					;acc=1
		rcall	_DFACCOP2				;op2=acc
		rcall	_DFSUB					;acc=arg-1
		_DFSTACC	_DF_MAC				;_DF_MAC=arg-1
		_DFLDOP1	_DF_P10				;op1=arg
		rcall	_DFLD1					;acc=1
		rcall	_DFACCOP2				;op2=acc
		rcall	_DFADD					;acc=arg+1
		_DFLDOP1	_DF_MAC				;op1=arg-1
		rcall	_DFACCOP2				;op2=arg+1
		rcall	_DFDIV					;acc=(arg-1)/(arg+1)
		rcall	_DFSERATN				;acc=SERATN((arg-1)/(arg+1)
		rcall	_DFACCOP1				;op1=ACC
		rcall	_DFLDPI4				;acc=pi/4
		rcall	_DFACCOP2				;op2=acc
		rcall	_DFADD					;acc=pi/4+SERATN((arg-1)/(arg+1))
		rjmp	_DFATN_DONE
#endif
.exit

