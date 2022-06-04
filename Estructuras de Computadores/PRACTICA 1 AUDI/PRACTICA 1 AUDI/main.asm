;
; PRACTICA 1 AUDI.asm
;
; Created: 10/05/2022 13:10:02
; Author : nacho
;

ser r18
out ddrd, r18

main:
	LDI r16, 1
	LDI r17, 128
	LDI r18, 129 //1000001

	repetir:
		out portd, r18
		call delay200

		lsl r16
		lsr r17
		or r18, r16
		or r18, r17
		cpi r18, 255
		brne repetir

	out portd, r18
	call delay1000

	ldi r18, 0
	out portd, r18
	call delay500

	rjmp main

delay200:
	push r26
	push r27
	push r28

    ldi  r26, 17
    ldi  r27, 60
    ldi  r28, 204
L1: dec  r28
    brne L1
    dec  r27
    brne L1
    dec  r26
    brne L1

	pop r28
	pop r27
	pop r26
	ret

delay500:
	push r22
	push r23
	push r24

	ldi  r22, 41
    ldi  r23, 150
    ldi  r24, 128
L2: dec  r24
    brne L1
    dec  r23
    brne L1
    dec  r22
    brne L1

	pop r24
	pop r23
	pop r22
	ret


delay1000:
	push r19
	push r20
	push r21

    ldi  r19, 82
    ldi  r20, 43
    ldi  r21, 0
L3: dec  r21
    brne L1
    dec  r20
    brne L1
    dec  r19
    brne L1
    lpm

	pop r21
	pop r20
	pop r19
    nop