;
; Blink 5 leds.asm
;
; Created: 15/03/2022 13:40:41
; Author : nacho
;


; Replace with your application code
ser r16
out ddrd, r16         ;Configuro el puerto B como salida 

start:

    ;Blink 1
    ldi r16, 0b01010101
    out portd, r16    ;Saco por el puerto B todo unos

    ;delay 500ms
    ldi r22, 40 ;Bulce externo. Cuenta 40x256x256
    for3_1:
        clr r21 ;Bucle medio. Cuenta 256x256
        for2_1:
            clr r20 ;Bucle interno. Cuenta 256
            for1_1:
                dec r20
                brne for1_1 ;Fin bucle interno
            dec r21
            brne for2_1 ;Fin bucle medio
        dec r22
        brne for3_1 ;Fin bucle externo
    ;Fin delay 500ms


    ;Blink 2
    ldi r16, 0b10101010
    out portd, r16 ;Saco por el puerto B todo ceros

        ; Assembly code auto-generated
    ; by utility from Bret Mulvey
    ; Delay 8 000 000 cycles
    ; 500ms at 16 MHz

        ldi  r18, 41
        ldi  r19, 150
        ldi  r20, 128
    L1: dec  r20
        brne L1
        dec  r19
        brne L1
        dec  r18
        brne L1


    ;delay 500ms
    ;ldi r22, 40 ;Bulce externo. Cuenta 40x256x256
    ;for3_2:
        ;clr r21 ;Bucle medio. Cuenta 256x256
        ;for2_2:
        ;    clr r20 ;Bucle interno. Cuenta 256
        ;    for1_2:
        ;        dec r20
        ;        brne for1_2 ;Fin bucle interno
        ;    dec r21
        ;    brne for2_2 ;Fin bucle medio
        ;dec r22
    ;    brne for3_2 ;Fin bucle externo
    ;Fin delay 500ms


    rjmp start