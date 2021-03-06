;=== BEGIN "ivectors.inc" ==================================================

; ��� ������������� "������� �������� �� ����������� ����������" - ��������� ����! 
; ���� ��� ��� ������, �� ����� ������� ����� ������������ �����, ��� ������ �����. ��� ��� �� ��������...
; (�������� �� ������ ���� � ��������� ���� - ��� ���������� ������������ ��������� �� ������ ������...)
;
; ��� �������: 
; ����������� ������ "INTERRUPT VECTORS" �� HAL-����� ���������������� (��������, "tn85def.inc"),
; �������� ��� ".equ" �� ".org"... � �������� �� ���� ������ ����� ������� RETI.
; (��� �� ��� ������, ���� ���������� �������� ������� - ����� ������� ���������� ����, ������ ����������.)


; ***** INTERRUPT VECTORS ************************************************
; ���������� HAL ���������������� "ATtiny13(A)"

			.ORG	INT0addr		; External Interrupt 0
			RETI
			.ORG	PCI0addr		; External Interrupt Request 0
			RETI

			.ORG	OVF0addr		; Timer/Counter0 Overflow
		.ifdef	RTOS_USE_TIMER0_OVERFLOW
			RJMP	RTOS_TIMER_HANDLER
		.else
			RETI
		.endif

			.ORG	ERDYaddr		; EEPROM Ready
			RETI
			.ORG	ACIaddr			; Analog Comparator
			RETI
			.ORG	OC0Aaddr		; Timer/Counter Compare Match A
			RETI
			.ORG	OC0Baddr		; Timer/Counter Compare Match B
			RETI
			.ORG	WDTaddr			; Watchdog Time-out
			RETI
			.ORG	ADCCaddr		; ADC Conversion Complete
			RETI


			.ORG	INT_VECTORS_SIZE	; ����� ������� ����������


;=== END "ivectors.inc" ====================================================
