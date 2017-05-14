;
; Faculty of Engineering & Environment
; CM0506 - Small Embedded Systems
; Assignment 2 of 2 - Asteroids
;
; Authors
; Alain Dobeli / W15039869
; Tom Batchelor / W15042283
; 
; Version
; 08.03.2017
;
;
;
; Asteroids Bit Flip Function
;
; bitflip.s
; This file contains assembly code for flipping image bit patterns.
;
		PRESERVE8							; preserve eight-byte alignment of the stack
											;
		THUMB								;
		AREA 	|.text|, CODE, READONLY		;
		EXPORT bitflip						;
											;
		ENTRY								; entry point of the program
bitflip	FUNCTION							;
		PUSH {R1, R2, LR}					; save working registers onto a stack
		MOV R2, R0							; move value from register 2 to register 0
		RBIT R1, R2							; reverse the bit order (in a 32-bit word)
		LSR R0, R1, #24						; "24" because it is the closest multiple of 8 (when multiplied by 8 = 192) to the actual comma-separated list of 200 byte values from the asteroid image
		POP {R1, R2, LR}					; restore saved registers from a stack
		BX LR								; return value
		ENDFUNC								;
		END									; end of the program