; * NAME: banner.asm
; * AUTHOR: Adin Geist
; * DESCRIPTION: Assembly function that is called inside main.c to print the game's banner

.686	
.model flat, stdcall
.stack 4096

include Irvine32.inc

;global data declarations
.DATA
CR EQU 0Dh
LF EQU 0Ah

; Create data that will store the banner to be printed

welcome BYTE    "$$\     $$\         $$\        $$\                                   ", CR, LF
	    BYTE    "\$$\   $$  |        $$ |       $$ |                                  ", CR, LF
	    BYTE    " \$$\ $$  /$$$$$$\  $$$$$$$\ $$$$$$\   $$$$$$$$\  $$$$$$\   $$$$$$\  ", CR, LF
	    BYTE    "  \$$$$  / \____$$\ $$  __$$\\_$$  _|  \____$$  |$$  __$$\ $$  __$$\ ", CR, LF
	    BYTE    "   \$$  /  $$$$$$$ |$$ |  $$ | $$ |      $$$$ _/ $$$$$$$$ |$$$$$$$$ |", CR, LF
		BYTE    "    $$ |  $$  __$$ |$$ |  $$ | $$ |$$\  $$  _/   $$   ____|$$   ____|", CR, LF
		BYTE    "    $$ |  \$$$$$$$ |$$ |  $$ | \$$$$  |$$$$$$$$\ \$$$$$$$\ \$$$$$$$\ ", CR, LF
		BYTE    "    \__|   \_______|\__|  \__|  \____/ \________| \_______| \_______|", CR, LF,0


.CODE

asm_print_banner PROC C 
    lea edx, welcome ; Load the address storing the welcome banner data
    call WriteString ; Write the banner onto the console

    ret ; Return to the C program
asm_print_banner ENDP ; End the assembly program
		
END
