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
;=======> 			M O D U L E  -  S H O W  D I G I T A L  C L O C K 			|
;	| 												|
;	 -----------------------------------------------------------------------------------------------
;	| Description:											|
;	| 	This Module show digital RTC clock				 			|
;	 -----------------------------------------------------------------------------------------------

	_MODULE_SHOW_DIGITAL_CLOCK:
		call	_VIDEO_OUT_CLS
		call	fn_UFO_ANIMATION_CLEAR			;initialize UFO
	_MODULE_SHOW_DIGITAL_CLOCK_10:
		call	_KEYBOARD_READ_FLASH			;wait key release
		cpi	Acc,_KEY_NONE				;no key pressed?
		brne	_MODULE_SHOW_DIGITAL_CLOCK_10		;no, wait
		call	_KEYBOARD_DEBOUNCE			;debounce time
		;//////////////////////
		;/// GET RTC STATE
		;//////////////////////
		call	_BOOT_GET_RTC_STATE			;clock set?
		cpi	Acc,_ON
		breq	_MODULE_SHOW_DIGITAL_CLOCK_20		;yes, branch
		rjmp	_MODULE_SHOW_ANALOG_CLOCK_RTC_NOT_SET	;else show RTC not set message
	_MODULE_SHOW_DIGITAL_CLOCK_20:
		;//////////////////
		;// DRAW DIGITAL CLOCK
		;//////////////////
	_MODULE_SHOW_DIGITAL_CLOCK_LOOP:
		call	_VIDEO_OUT_SYNC_WITH_VERTICAL		;synchronize with vertical retrace
		ldiw	X,0
		ldiw	Y,32
		call	_RTC_GET_HOUR				;get hour
		call	_UBTOSU					;convert to ascii
		adiw	ZL,2
		ld	Acc,Z+					;show hour
		cpi	Acc,'0'					;zero ?
		brne	_MODULE_SHOW_DIGITAL_CLOCK_LOOP_10	;yes, branch
		ldi	Acc,10
	_MODULE_SHOW_DIGITAL_CLOCK_LOOP_10:
		call	fn_DRAW_BIG_DIGIT
		ld	Acc,Z+
		call	fn_DRAW_BIG_DIGIT
		addiw	X,16
		call	_RTC_GET_MINUTE				;get minute
		call	_UBTOSU					;convert to ascii
		adiw	ZL,2
		ld	Acc,Z+					;show minute
		call	fn_DRAW_BIG_DIGIT
		ld	Acc,Z+
		call	fn_DRAW_BIG_DIGIT
		call	_RTC_GET_SECOND
		andi	Acc,1
		sbrc	Acc,0
		ori	Acc,0xf7
		mov	Temp,Acc
		ldi	XL,60
		ldi	YL,40
		ldi	XH,8
		ldi	YH,4
		call	_RASTER_SET_BOX_FULL_FAST
		addi	YL,16
		call	_RASTER_SET_BOX_FULL_FAST
		;//////////////////
		;// UPDATE ANIMATION
		call	fn_UFO_ANIMATION
		;//////////////////
		;//////////////////
		;// CHECK KEYBOARD
		;//////////////////
		call	_KEYBOARD_READ_FLASH			;read keyboard
		cpi	Acc,_KEY_NONE				;none key pressed?
		lbreq	_MODULE_SHOW_DIGITAL_CLOCK_LOOP		;wait wait
		call	fn_MENU_DRAW_BACKGROUND			;redraw background
		call	_MENU_DRAW_STACK			;redraw menus in stack
		ret
