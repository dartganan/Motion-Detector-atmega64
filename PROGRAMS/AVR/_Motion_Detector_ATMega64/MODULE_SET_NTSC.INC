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
;	 -----------------------------------------------------------------------------------------------
;	|												|
;=======> 	 M O D U L E  -  D E F I N E  T V  S Y S T E M  T O  N T S C 				|
;	| 												|
;	 -----------------------------------------------------------------------------------------------
	_MODULE_SET_SYSTEM_NTSC:
		movw	ZL,XL						;Z=MENU
		ldi	Acc,_ON						;NTSC ON
		ldi	AccH,_MENU_CONFIGURAR_SISTEMA_ITEM_NTSC
		call	_MENU_SET_ITEM_CHECK
		ldi	Acc,_OFF					;PAL OFF		
		ldi	AccH,_MENU_CONFIGURAR_SISTEMA_ITEM_PAL
		call	_MENU_SET_ITEM_CHECK
		ldi	Acc,_VIDEO_OUT_NTSC				;set system
		call	_VIDEO_OUT_SET_SYSTEM
		call	fn_SAVE_MENU
		ret
