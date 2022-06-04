; Blink con delay DEFINITIVO.asm
; Created: 15/03/2022 12:42:42
; Author : nacho

ser r16
out ddrb, r16

loop:

    ser r16
    out portb, r16

	;delay 500ms
	ldi r22, 40
	for3:
		clr r21
		for2:
			clr r20
			for1:
				dec r20
				brne for1
			dec r20
			brne for2
		dec r22
		brne for3
	clr r16
	out portb, r16

	;delay 500ms
	ldi r22, 40
	for6:
		clr r21
		for5:
			clr r20
			for4:
				dec r20
				brne for4
			dec r20
			brne for5
		dec r22
		brne for6

	rjmp loop