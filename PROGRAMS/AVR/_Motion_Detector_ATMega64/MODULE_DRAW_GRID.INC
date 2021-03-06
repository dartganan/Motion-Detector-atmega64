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
;=======> 	 		M O D U L E  -  D R A W  G R I D  O N   S C R E E N			|
;	| 												|
;	 -----------------------------------------------------------------------------------------------
	_MODULE_DRAW_GRID:
		call	_VIDEO_OUT_CLS
		ldiw	X,4						;x=0
	_MODULE_DRAW_GRID_10:
		ldiw	Y,0						;Y=0
		ldiawt	SCREEN_HEIGHT
		ldi	Acc,_ON
		call	_RASTER_SET_VLINE
		addiw	X,8						;x+=8
		cpiw	X,SCREEN_WIDTH					;max width ?
		brlo	_MODULE_DRAW_GRID_10
		ldiw	Y,4						;Y=4
	_MODULE_DRAW_GRID_20:
		ldiw	X,0
		ldiawt	SCREEN_WIDTH
		ldi	Acc,_ON
		call	_RASTER_SET_HLINE
		addiw	Y,8						;y+=8
		cpiw	Y,SCREEN_HEIGHT					;max height ?
		brlo	_MODULE_DRAW_GRID_20
		call	_KEYBOARD_READ					;wait any key to return
		call	fn_MENU_DRAW_BACKGROUND				;redraw background
		call	_MENU_DRAW_STACK				;redraw menus in stack
		ret
