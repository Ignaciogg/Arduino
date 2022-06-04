;
; PRACTICA 2 LDR.asm
;
; Created: 24/05/2022 12:04:22
; Author : nacho
;

.cseg
	ldi r16, 0xff
	out ddrb, r16				;Activar salida para encender los LED.
	out ddrd, r16				;Puerto para ver el valor convertido.
	RCALL config_ADC			;Configurar el conversos ADC

loopGeneral:
	;Disparo de la lectura
	LDS R17, ADCSRA				; -> ADCSRA: The ADC Control and Status register A
	ori r17, (1<<ADSC)			;Aciva el bit de la conversion ADC por software
	sts ADCSRA, R17				;Dispara la Operacion ADC
esperaADC:
	;Espera activa hasta que la lectura termine
	clr r17 
	lds r17, ADCSRA				;Carga ek registro ADCSRA para chequear si la conversion ha terminado 
	sbrc r17, ADSC				;Si el bit ADSC esta activo, la conversion no ha terminado, entonces saltar RJMP 
	rjmp esperaADC

	;Cargar el valor de la conversion
	LDS R16, ADCH				;En ADCH, tenemos el valor de la conversion con 8 bits.
	out portd, r16				;Muestra con Led´s el valor leido por el puerto "d"

/*************************************************************************************
 Código ejemplo de Sergio
**************************************************************************************/

	/*;If (valor > 128)
	cpi r16, 0b10000000			;Comparamos contra 128
	brpl encender				;Si es mayor, saltamos a encender
	;Parte Then
	ldi r20, 0x00
	out portb, r20				;Apagamos el puerto b
	rjmp seguir
	;parte Else
encender:
	ldi r20, 0xff
	out portb, r20				;Encendemos el puerto b
	;fin if
seguir:

	rjmp loopGeneral*/

/*************************************************************************************
 Mi solución
**************************************************************************************/

	;If (valor < 87)
	cpi r16, 87            ;menor que 87
	brlo encender2				;Si es menor, saltamos a encender2

	;Else (valor < 178)
	cpi r16, 178           ;menor que 178
	brlo encender1				;Si es menor, saltamos a encender1

	;Parte Then
	ldi r20, 0x00
	out portb, r20				;Apagamos el puerto b
	rjmp seguir

encender2:
	ldi r20, 0xff				;Encendemos dos puertos b
	out portb, r20	
	jmp seguir

encender1:
	ldi r20, 0b000000001				;Encendemos un puerto b
	out portb, r20				

seguir:
	rjmp loopGeneral

/*************************************************************************************
  Funcion Inicializa el ADC
**************************************************************************************/
config_ADC: 
	push r16
	LDI R16, (1<<ADEN)							;ADC Enable
	ORI R16, (0<<ADATE)							;ADC Auto Trigger Enable
	ORI R16, (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)	;ADPSx=3: ADC Prescaler Select Bits (ADPS2:0=111 -> 128)
	STS ADCSRA, R16								; -> ADCSRA: The ADC Control and Status register A
	LDI R16, (0<<ADTS2)|(0<<ADTS1)|(1<<ADTS0)	;ADTSx=1: trigger source Analog Comparator
	STS ADCSRB, R16								; -> ADCSRB: The ADC Control and Status register B
	LDI R16, (1<<MUX0)							;MUXx=1: input channel 1: MUX5:0=00001
	ORI R16, (0<<REFS1)|(1<<REFS0)				;AREF=1: internal 5V reference (REFS1:0=01)
	ORI R16, (1<<ADLAR)							;ADC 8 bits resolution
	STS ADMUX, R16								; -> ADMUX: The ADC multiplexer Selection Register
	LDI R16, (1<<ADC1D)							;ADC disable digital input circuitry for channel 1 (saves energy)
	STS DIDR0, R16								; -> DIDR0: Digital Input Disable Register
	LDI R16, (0<<PRADC)							;ADC disable the power reduction saving for the ADC circuitry (not necesary)
	STS PRR, R16								; -> PRR: Power Reduction Register
	pop r16
	RET
