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
;+--------------------------------------------------------------------------------------------
;| Source
;|     Path : C:\NEPER_THE_MATH\PROJETOS\MOTION_DETECTOR\VERSIONS\_FREE_VERSION_\PROGRAMS\AVR\BMPS\
;|     File : BNC_MACHO_ANI_4.BMP
;|
;| Destiny
;|     Path : C:\NEPER_THE_MATH\PROJETOS\MOTION_DETECTOR\VERSIONS\_FREE_VERSION_\PROGRAMS\AVR\BMPS\
;|     File : BNC_MACHO_ANI_4.ASM
;|
;| Destiny Image Properties
;|     Size   :   464 Bytes
;|     Width  :   128 Pixels
;|     Height :    29 Pixels
;|     Width  :    16 Bytes
;|     Colors :     2 Levels (1 bit)
;+--------------------------------------------------------------------------------------------

;( IMAGE CONTENTS )

;                                                                                                         1                           
;               1         2         3         4         5         6         7         8         9         0         1         2       
;     01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
;     |         |         |         |         |         |         |         |         |         |         |         |         |       
;   0-################################################################################################################################-   0
;   1-################################################################################################################################-   1
;   2-######################################################################        #########    ############################         -   2
;   3-###################################################################### ###### ######### ## ############################         -   3
;   4-###################################################################### #    # ######### ## ############################  # # # #-   4
;   5-###################################################################### ###### ######### ## ############################  # # # #-   5
;   6-###################################################################### #    #           ## ##############                # # # #-   6
;   7-#################################################################      ######  ### #### ## #############                 # # # #-   7
;   8-###############################################################   #### #    #  ###  ### ## #############  ############## # # # #-   8
;   9-####################################################            ###### ######   ### ### ## #############  ############## # # # #-   9
;  10-                                                     ################# #    # # ### ### ## #############  ############## # # #  -  10
;  11-#################################################### ################# ###### #  ##  ## ## #############  ############## # # ## -  11
;  12-#################################################### ################# #    # ## ###    ## #############  #####  ####### # #  # -  12
;  13-#################################################### ################# ###### ##  ##### ## #############  #### ## ###### # ## # -  13
;  14-#################################################### ################# #    # ### ##### ## #############  #### ## ###### #  # # -  14
;  15-#################################################### ################# ###### ###  #### ## #############  #### ## ###### ## # # -  15
;  16-#################################################### ################# #    # ####      ## #############  #####  ####### ## # # -  16
;  17-#################################################### ################# ###### ######### ## #############  ##############  # # # -  17
;  18-                                                     ################# #    # ######### ## #############  ##############  # # # -  18
;  19-####################################################            ###### ###### ######### ## #############  ############### # # # -  19
;  20-###############################################################   #### #    # ######### ## #############  ############### # # # -  20
;  21-#################################################################      ###### ######### ## #############                  # # # -  21
;  22-###################################################################### #    #           ## ##############                 # # ##-  22
;  23-###################################################################### ###### ######### ## ############################   # #  #-  23
;  24-###################################################################### #    # ######### ## ############################   # ## #-  24
;  25-###################################################################### ###### ######### ## ############################         -  25
;  26-######################################################################        #########    ############################         -  26
;  27-################################################################################################################################-  27
;  28-################################################################################################################################-  28
;     |         |         |         |         |         |         |         |         |         |         |         |         |       
;                                                                                                         1                           
;               1         2         3         4         5         6         7         8         9         0         1         2       
;     01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567

;( IMAGE DATA )

BMP_BNC_MACHO_ANI_4:

	;+------------+
	;| BMP Header |
	;+------------+
	.DW      1 ;Bits Color 1 bit
	.DW    128 ;Width Pixels
	.DW     29 ;Height Pixels
	.DW     16 ;Width Bytes
	.DW    464 ;Size Bytes
	;+------------+
	;|  BMP Data  |
	;+------------+
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x01,0xE0,0x00,0x00,0x01,0xFF
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x01,0x20,0x00,0x00,0x01,0xFF
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF4,0x01,0x20,0x00,0x00,0x01,0xAA
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x01,0x20,0x00,0x00,0x01,0xAA
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF7,0xFF,0x20,0x00,0x7F,0xFF,0xAA
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x06,0x21,0x20,0x00,0xFF,0xFF,0xAA
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC2,0xF6,0x31,0x20,0x00,0xC0,0x00,0xAA
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0x02,0x07,0x11,0x20,0x00,0xC0,0x00,0xAA
	.DB 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x02,0xF5,0x11,0x20,0x00,0xC0,0x00,0xAB
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0x05,0x99,0x20,0x00,0xC0,0x00,0xA9
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0xF4,0x8F,0x20,0x00,0xC1,0x80,0xAD
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0x04,0xC1,0x20,0x00,0xC2,0x40,0xA5
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0xF4,0x41,0x20,0x00,0xC2,0x40,0xB5
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0x04,0x61,0x20,0x00,0xC2,0x40,0x95
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0xF4,0x3F,0x20,0x00,0xC1,0x80,0x95
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0x04,0x01,0x20,0x00,0xC0,0x00,0xD5
	.DB 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x02,0xF4,0x01,0x20,0x00,0xC0,0x00,0xD5
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0x02,0x04,0x01,0x20,0x00,0xC0,0x00,0x55
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC2,0xF4,0x01,0x20,0x00,0xC0,0x00,0x55
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x04,0x01,0x20,0x00,0xFF,0xFF,0xD5
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF7,0xFF,0x20,0x00,0x7F,0xFF,0xD4
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x01,0x20,0x00,0x00,0x01,0xD6
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF4,0x01,0x20,0x00,0x00,0x01,0xD2
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x01,0x20,0x00,0x00,0x01,0xFF
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x01,0xE0,0x00,0x00,0x01,0xFF
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.DB 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

