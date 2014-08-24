
	.NOLIST				; Disable listfile generation.
	;.include "tn10def.inc"		; ���������� HAL ���������������� "ATtiny10"
	;.include "tn13Adef.inc" 	; ���������� HAL ���������������� "ATtiny13A"
	.include "tn2313def.inc"	; ���������� HAL ���������������� "ATtiny2313"
	;.include "m8def.inc"		; ���������� HAL ���������������� "ATmega8"
	;.include "m16def.inc"		; ���������� HAL ���������������� "ATmega16"
	;.include "m2560def.inc" 	; ���������� HAL ���������������� "ATmega2560"
	.include "macrobaselib.inc"	; ���������� ������� ����������������.
	.include "macroapp.inc"		; ���������� ����������������, ������������ ��� ���������� ������ ����������.
	.include "RTOS_macro.inc"	; �������, ����������� ���������� ��������� RTOS
	.LIST				; Reenable listfile generation.
	;.LISTMAC			; Turn macro expansion on?	(��� �������, ���������� ���� ��������� �������� � ������������������� ���� - ������, �� ������� ��������, �.�. ���������� ����� ������.)

	.include "data.inc"		; ������ ���������:	(���������� ����������� ����� ����������������!)
					;	��������� � ���������� ���������; 
					;	������� SRAM � ����������; 
					;	������� EEPROM.


;***************************************************************************
;*
;*  FLASH (������� ����)
;*
;***************************************************************************
			.CSEG

		.ORG	0x0000		; (RESET) 
		RJMP	RESET
		;.include "ivectors_tiny10.inc"		; ������� �������� �� ����������� ���������� ��� "ATtiny10"
		;.include "ivectors_tiny13.inc"		; ������� �������� �� ����������� ���������� ��� "ATtiny13(A)"
		.include "ivectors_tiny2313.inc"	; ������� �������� �� ����������� ���������� ��� "ATtiny2313"
		;.include "ivectors_mega8.inc"		; ������� �������� �� ����������� ���������� ��� "ATmega8"
		;.include "ivectors_mega16.inc"		; ������� �������� �� ����������� ���������� ��� "ATmega16"
		;.include "ivectors_mega2560.inc"	; ������� �������� �� ����������� ���������� ��� "ATmega2560"


;***** BEGIN Interrupt handlers section ************************************


;---------------------------------------------------------------------------
;
; "������ �������� ���� RTOS" (Main Timer Service) 
; �������� � ���������� ���������� �� ����������� �������/��������.
;
; (����������: ��� ������������� �� ������������� ������� ������� ��� ������ - ������� ������� ��� 
; ������������� "RTOS_INIT" � ������� �������� ���������� "ivectors.inc".)
;
;---------------------------------------------------------------------------

RTOS_TIMER_HANDLER:
;===========================================================================
; RTOS Here	(��������: ���� ���� ������ ������������� � ���������� ����������� �������, ����������� ������ 1��!)
;===========================================================================
	.if defined(RTOS_FEATURE_PLANTASK) && (defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER))	; (������ �������� ������������ ������ � ���� �������)
		RTOS_TIMER_SERVICE				; ������ �������� RTOS 
	.endif

	.ifdef	RTOS_USE_TIMER0_OVERFLOW
		PUSH	temp
	.ifndef	TCNT0L
		; � ������ 8-���������� Timer/Counter0
		OUTI	TCNT0,	256-Low(CTimerDivider)		; ������������������� ��������� �������� ��������
	.else
		; � ������ 16-���������� Timer/Counter0
		; Note: To do a 16-bit write, the high byte must be written before the low byte. For a 16-bit read, the low byte must be read before the high byte.
		; 	It is important to notice that accessing 16-bit registers are atomic operations. Aware interrupts, use cli/sei!
		OUTI	TCNT0H,	High(65536-CTimerDivider)
		OUTI	TCNT0L,	Low (65536-CTimerDivider)	; ������������������� ��������� �������� ��������
	.endif
		POP	temp
	.endif
;===========================================================================
		RETI					; ������� �� ����������



;***** END Interrupt handlers section 


;***** ������������� *******************************************************
RESET:
		STACKINIT	; ������������� �����
		RAMFLUSH	; ������� ������
		GPRFLUSH	; ������� ���


;***** BEGIN Internal Hardware Init ****************************************


; ������������� USART:

;		USART_INIT


; ������������� ������:

;		...


;***** END Internal Hardware Init 


;***** BEGIN External Hardware Init ****************************************
;***** END External Hardware Init 


;***** BEGIN "Run once" section (������ ������� ���������) *****************


; ������������� RTOS:

;===========================================================================
; RTOS Here	(��������: ���� ���� ������ ������������� � ���� ������������� ����������������, ����������� �������� ����� "Reset"!)
;===========================================================================
		RTOS_INIT
;===========================================================================


; ������������� � ��������� ������ "������������� �����":

		; "������" ��� ����� ������������ ��� "������������", ��� ��� ��� ������ ��� � ������, � "��������" �������� ����������, � ������ ����� ����������� �� ����...
		; �� ���� ������ ����� ���������� � "��� ������������", ������� RCALL - ����� ��� ���������� ��������� (��� ���������, �����, ��� ������� ����).
		; � ����� ��������� � "��� ������" (����� ����������� ������ PLAN_TASK) - ���������� � ������� ����� �������.
;		RCALL		Task1		; ������ �������� �� ��, ��� ������� RCALL �� ��������� �� �����, 
;		PLAN_TASK	TS_Task1	; � ����� API �� �������� ������������� ������.


;***** END "Run once" section 

;***** BEGIN "Main" section ************************************************

MAIN:

;===========================================================================
; RTOS Here	(��������: ���� ���� ������ ������������� � ������ "Main", ��������� ���������!)
;===========================================================================
		SEI					; ��������� ���������� (����� ����� ������ ��������� ���������� ������, ����� �������� ���������� ������������ ����������� ��������� �������)	(����������: ���� �� �� �� ������ � ���������� ������� ��������� ������������, ��� ������������ �� �������������, �� ���������� ��� ���������� SEI � "Run once" section...)
		WDR					; Reset Watch DOG (���� �� "���������" "������", �� ��� ������� ����� ����� � ���� reset ��� ����������)
	.if defined(RTOS_FEATURE_PLANTASK) && (defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER))	; (������ �������� ������������ ������ � ���� �������)
		RJMP	RTOS_METHOD_TimerService	; ������ �������� RTOS 
		RTOS_METHOD_TimerService_ExitPoint:
	.endif
	.if defined(RTOS_FEATURE_PLANTASK)
		RCALL 	RTOS_METHOD_ProcessTaskQueue	; ��������� ��������� ������� �����
	.endif
		RJMP 	MAIN				; ���������� �������� ���� ��������� RTOS
;===========================================================================


;***** END "Main" section 

;***** BEGIN Procedures section ********************************************

	; ��������! � ������� �� ��������, ��� ��������, ������ � ���������, 
	; ���������� � ������� ������ ��������� - �������� �����! 
	; �������, ����� ������ ���� �������� ������ �� ���������, ������� 
	; ������� ������������. (��������� - ������� /* ���������������� */.)

	;.include "genproclib.inc"	; ���������� ����������� �����������.



;***** END Procedures section 

;***** BEGIN "RTOS Tasks" section ******************************************

	; "������ ������������� RTOS" - ��� ������ ������������: 
	;
	; 1) ����������
	;    ��� � ������� ��������� - ��� ����� "����� �����" (�����/���), 
	;    � ����� �� ��� �������������� ������ ����� RET (������ ������������ RETI - ��� �� ����������� ����������!)
	;
	; 2) ���������
	;    �� "������ ������������� RTOS" �� ����� ����� "����������� ����������":
	;    �.�. ��� ����������� �������������, ����� "��������� ����� RTOS", � ������� ������� - �� ����� ��������� ������ ��������������� ����� �������� ������������� �����, � ���������� ��������� ���������������� �������� � ���. 
	;    ����� �������, ������ ������ ����������� ������ ������� � SRAM � � ��������� �����/������!
	;    (����������: ���������� ����� ���������� ���������� ������ "����������� ����������", �� ��� �������� - ��. ����������� � �.5.1)
	;
	; 3) ������
	;    "������ RTOS" ������� �� ����������� ���������������, ����� CALL <�����>, ��� ������� ������������!
	;    ��� ������ ����������� ������ ������������� - ����� ��������� RTOS ("������� �����" � "��������� �����").
	; 3.1) ������ ����� ���� "������������� � �������" (�� �� ����������� ������ � ��������� ������� "RTOS_TaskProcs"):
	;      ���� "��� ����� ������" (����� PLAN_TASK),
	;      ��� "���������� ������, ������ �������� �����, � ��" (����� PLAN_TIMER, ��. ���� �.7),
	;      ��� "��� ����������� �������� �������, ��������� ����������� ����������, ��������� ����� � ������" (����� PLAN_WAITER, ��. ���� �.8).
	;      � ����� ������, �������� ���������� ����� �������� ������� �� ���������� ��������� - ����� ���� ����������, ��� ������� ����, ������ ����� "���������� �����"...
	; 3.2) �������, ��� ������ ���������� �������������� ���������������� � �������: 
	;      ������ <�����> � ��������� ������� "RTOS_TaskProcs", ��� ����������� ���������� ������� <������>;
	;      � ��� ���������� ����������� �������������, ����� ��������� ���� ����� ������������� ���������: .equ TS_TaskX = <������>.
	; 3.3) ���������� ����������: ��������� ������ "�������� ����" Task_Idle, ��� ������� =0 � ��������� �������. 
	;      ������� �� ����������/�� ���������� ������ Task_Idle � ������� �� ����������! (��� ����, ������ �������, �������������� RTOS: ����������� ��� ������ "������� �����")
	;
	; 4) ��� ����������
	;    �� ����� ���������� "������������� ������" ���������� �� ����� ���� �������� �������� ������� ������ "������", �� ������� RTOS! 
	;    (������ ���������� ���������� ��-��� ����� �������� ������������ ��� ���������� ������)
	; 4.1) ������� "������������� ������", �� ����� ������ ����������, ����� �� ��������� �� ����������� ������ � ���, SRAM, � ��������� �����/������ (���������� ���������� ���������, ��): 
	;      � ���� ���������� _������_ "������������� ��", ����� "����������� ��" - ��� ������ ������ �� ���� �������� ����������/���������� �� ���� ������ "������������� ���������", ��� �����!
	;      (�� ��������� ��������� ��� ������������ ��������, ������������ �������� ��� ���������� ����������� ��������, ������������ ������ �������� "������������� �������������� ��������������")
	; 4.2) � ������ �������, �� ����� ���������� ������, ��-��� ����� ��������� "������ ����������"! 
	;      �� ��� ���� ����������, ����������, ������ ��������� ���������� ���� ������������ ���, ����� ���� - ��� ��������.
	;      ������, ��������� ���������� ����� ������ �� ���������� ���� ����������� �������� (���������� SRAM � ��������� ��) - 
	;      ��� ������ � ������������ ����������� ��������� (��������, ��� ������ � EEPROM), � ���� ����� _�����������_ ������������ CLI/SEI ������������� ��������� ������!
	; 4.3) �������, ������� ������������ ��������� ���������� ������� (������ 1��), ����� �� �������� ������ RTOS!
	;      �����, ������ �� ����� ��������� ���������� ����������� - ��� ������ ���������� ������� ������, �� RTOS ������������� �������� ���������� (SEI).
	;         (��������, ���������� ����������� ����������: ������1 ��������� ����������, � ���������� ������ ������2 �� ����� ���������.
	;         �� ��� � ������������: ���� ��������� ����������, �� ����������� "��������� ������ RTOS" - ��� "���������� ������" ����� �������������, �� ���������� �������; � ���������� ����� ������� �� ��������� - ������� ��� ������ ��� ��������� ������ �� ���������...)
	;      ��� "����������� ������" ���������� ������������ � ������ ����� ������! ���������� ������ ����� ��������, ����� CLI/SEI, ���� ��������� ("������ ������������� ����� ������" ��� "������� ��� � ��������� ��������").
	;
	; 5) ����������� ����������������
	;    � "������������� �����", ���� _������ ����������_ , ������� �� �������� (���������� �����������) ����������� ����������: 
	;    ������ ����� �� �������� ���������� ��������� (���) ����� ����, ������, ��� ����� ������ ����������� �� ���������� �����������.
	;    (���������: �� ��������� ��������� � ���� ������� ������������ �������� ����� ����������� ���� ������, � ����� ��������������� ����� ���������� ����� �������!)
	;    �� ��� ����, �� ������������ ������������ ����� ��� ������� ������������� ������ ��� "��������� ����������"!
	; 5.1) [���������� 1:	��� ��������� ��������, ���������� ��������� ����������� ���� �����������, ����� ��������]
	;      ������, ��� ����������� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY, ��� ��������� ������ RTOS ��������� ���������� ������������ ��������� ����� ���� (�� ����������� ZH:ZL, � RTOS_METHOD_ProcessTaskQueue). 
	;      ����, ��� �� ������ ����������� ����������� ������ � ��������� ����� �������� - ���� ���������� ��������� ����� ���� ���������� ��� ������, ����� ������ ���������������� �������, ������ � ������������ ������� (������ �����������/���������������� � ������������ �������, ����������� �� �������� ��� ������� ��������)...
	;      �������, ���� �� �� �������� ����� ������� (��� "����������� ����������") ����� ������ ����������� (�.�. ����� ������� ������ ������ ��� �� ������������), �� ���� ��� ����������� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY - ����������� ��� ����������� �� �������������.
	;      �� ���, ��������� ��� ������ ��������� ������������ ��� � ����� - ��� � ������������ ����������... �� ��� ����� � ��������������� ������! (�������������� ����� ����������������.)
	; 5.2) [���������� 2:	�������� ��������, ����� ��������� �������� ������ ������ ������������]
	;      [��� ��� ������� ���������, �.�. � �� ������� � "boot loader", � �� ����������� ���� ������������ ���������������� �� � ���� ������!]
	;      ��� ������� ����� � ����, ��� ��� ��������� ��������������� ��������� �������� (��������, ��� ������������� ����������� "boot loader"), ����������� ����������� ������ ���� �� � ���������� �������, ������������ ��������������� �������� (R1:R0, RAMPX:X, RAMPY:Y, RAMPZ:Z, � ��. temp1, temp2, looplo, loophi, spmcrval), � ��� ��������� ����� �������� ������ � SRAM.
	;      ���� ������� "��������� ��������" ������������ (��. ���������� SPM), �� � ���� �����: ������ �������� ������������ ������ ������ ��� ��������� �� ������������!
	;      �������, ���������� "boot loader" ������������ �����. ������, ����� ������������ "boot loader", �� ������ RTOS � ���� ���������� ����� - ������� ���������� �����! � ��������� ��������������� � _������������_ ��������� �����... � ����� "boot loader" ����������, �� ��������������� (�� ����� ��������) ����� ������������� ����������� (RESET)...
	;      ����� �������, ������, ��� ��������� ���� ����� - �� ��� �������� ������������ �� ������� - �� ���������.
	;
	; 6) �������� ��������������
	;    �������������, �������� ������� ������� ���������� ������ ����������� �� ��������� - ������ �� ������� ����������� � ��� ��������� "������������� ������". 
	;    ������� ������ ����� ����������� ������������ ������: ���������������, 
	;    ��� ������� (��������� ����� IF-THEN-ELSE ����� �������� ��� ��������� ������, � ��������� ����� PLAN_TASK), 
	;    ��� ������ ����������� �������� ������� (������, ����������� ����� PLAN_TIMER),
	;    ��� ������ ������������� �������� �������� ������� ��� ��������� ����������� ����������: ������ ������ � ����, ���������� �������� � ADC ��� EEPROM, ������� ����������� � �.�. (������, ����������� ����� PLAN_WAITER)
	; 6.1) ��� ������ ������ ������ ���� ��������. ������ ������ ��� �������: ����������, �� ��� ����� � ���������, �� SRAM � ��������� �����/������; ������ �������� ������; ��������� ���������� � SRAM; ����������� (RET).
	; 6.2) ��� ������ ������ ������ ����������� ����������� ������, ��� � ����������� ����������. ������� �� ����������� "����� ��������" � ����! ������ �����, ��������� ������ �� ��� ���������: � ������ �����, ��������� � SRAM ��� ������������� ������ ������ ��������� ������ �����, � ������������ ������ ������ ��������� (����� PLAN_TIMER)...
	; 6.3) ����������, ��� ������� ������ ��������� ���������� ������ - ����������� ���������� ����� ����� ����������� "������������� �����". ����� ����������� ���� ������ - ������������� ������������ ������ �����: 
	;      ����� ������ �������, �������������� ������ �������, ���������: TS_TaskX_Begin, TS_TaskX_Check, TS_Task_Finish... 
	;      ����������� ������ � ��������� ������� "RTOS_TaskProcs" ���������������, ��������... 
	;      ������ �������� ���� � ��������� ��� �� �������... � �.�.
	;
	;
	; 7) ������� ��������� �������:
	; 	1. �������� ����� ������, ������ API-�����:  PLAN_TIMER tasknumber,delay
	; 	   - ���� � ���� ��� ���������� ������ ��� ������ <tasknumber>, �� ��� ������� "������� �������" ����� ��������� �� �������� <delay>.	/������ � �������� ������� ������� ������, �� ������� ����� ������������. ��������, ����� �� ������� ���� �������� �������. ������ � ��������������� ������, ������� ������������ watchdog./
	; 	   - �����, ��� ������� � ���� ���������� �����, �������������� ����� ������: ��� ������� ������ <tasknumber>, ����� ����� <delay> = [0..65535] ms.
	; 	2. ��� ������������� ��������������� ������ �� ������ �����, �� ���� ��� �� �������� - ����� ������� API-�����:  PLAN_TIMER tasknumber,delay
	; 	3. ����������������� ������ ����� �������������� ���������� ���, ���� �� - ������ ��� �� ����, ��� �� ���������...]
	; 	4. ��� ������������� �� ����� �������� ��������������� ������ ������ - ������� ������, ������ API-�����:  REMOVE_TIMER tasknumber
	; 	5. ������ ~1ms ��������� �������, �� ����������, ����������� "������ �������� RTOS", �������:
	; 	   - �������������� ��������, ���� �������� �������� � ����, �� (-1)...
	; 	   - ����, ��� ����, �������� �������� ��������� =0 ?
	; 		- �� ��������� ������ <tasknumber> ����������� � ������� RTOS_TaskQueue, �� ����������;
	; 		- � ��� ����������� ������, ��� ����, ��������������/�������������, ���������� ������� ����...
	; 	6. ������, ��������� � "��������� �������" RTOS_TaskQueue, ����� ��������� � ������� ����� �������...
	;    �������� �������: 
	; 	API-������ PLAN_TIMER/REMOVE_TIMER, ��� ����������, ������ ���������� � ��������� ���������: � ����������� �� ������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY, ���� ������ "������������ ��������", ���� ����� � "��������� ��������" (��. ����������� � ������ "Subroutine Register Variables", ���������������� ���������� ������).
	; 	��� ���������� ��������� ������������ ����� API-������: SAFE_PLAN_TIMER/SAFE_REMOVE_TIMER, ������� �������������� �� ������ ������ ������� ���������, �� ��� �������� ������ � ������ ����������� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY (��� ����, ������������ ���� - ����� �������������� ����������� ������).
	; 
	; 
	; 8) ������� ��������� ����������:
	; 	1. �������� ����� ����������, ������ API-�����:  PLAN_WAITER tasknumber,address,bit,state
	; 	   - ���� � ���� ��� ���������� ���������� ��� ������ <tasknumber>, �� ��� ������� "������� �������" ����� ���������� �� �������� <address,bit,state>.
	; 	   - �����, ��� ������� � ���� ���������� �����, �������������� ����� ����������: ��� ������� ������ <tasknumber>, ��� ����������� "�������� �������" (�������� ���� � ������), ������� ����� ��������� ����� ����� ������������� ����� (�������� ��������� ���� �������������� ������������, ������ 1ms).
	; 	2. ��� ������������� ��������������� ����������, �� ����� �������, �� ���� ��� �� �������� - ����� ������� API-�����:  PLAN_WAITER tasknumber,address,bit,state
	; 	3. ����������������� ���������� ����� �������������� ���������� ���, ���� �� - ������ ��� �� ����, ��� �� ���������...]
	; 	4. ��� ������������� �� ����� �������� ��������������� ������ ������ - ������� ����������, ������ API-�����:  REMOVE_WAITER tasknumber
	; 	5. ������ ~1ms ��������� �������, �� ����������, ����������� "������ �������� RTOS", �������:
	; 	   - ��������� �� ���� �������� ����������� � ����, ���������� ���� �� ������ �� ������ <address>, � ��������� � ��� �������� ���� ����� <bit>...
	; 	   - ���� �������� ���� = <state> ?
	; 		- �� ��������� ������ <tasknumber> ����������� � ������� RTOS_TaskQueue, �� ����������;
	; 		- � ��� ����������� ����������, ��� ����, ��������������/�������������, ���������� ������� ����...
	; 	6. ������, ��������� � "��������� �������" RTOS_TaskQueue, ����� ��������� � ������� ����� �������...
	;    �������� �������: 
	; 	API-������ PLAN_WAITER/REMOVE_WAITER, ��� ����������, ������ ���������� � ��������� ���������: � ����������� �� ������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY, ���� ������ "������������ ��������", ���� ����� � "��������� ��������" (��. ����������� � ������ "Subroutine Register Variables", ���������������� ���������� ������).
	; 	��� ���������� ������������ ������������ ����� API-������: SAFE_PLAN_WAITER/SAFE_REMOVE_WAITER, ������� �������������� �� ������ ������ ������� ���������, �� ��� �������� ������ � ������ ����������� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY (��� ����, ������������ ���� - ����� �������������� ����������� ������).



;---------------------------------------------------------------------------
;Tasks
;---------------------------------------------------------------------------

; ��������� ������ "�������� ����" (����������� ��� ������ ���������)
; (�����: �������� �� ������, ������ �� ������� - ������������ �������� RTOS)
Task_Idle:
		; ����������: � ������ "Idle" ����� ������ ��� ������ �������, ������� � �����������.
		; ���� � ������ �������� � ������.
		NOP	
		RET

;-----------------------------------------------------------------------------
Task1:		RET
;-----------------------------------------------------------------------------
Task2:		RET
;-----------------------------------------------------------------------------
Task3:		RET
;-----------------------------------------------------------------------------
Task4:		RET
;-----------------------------------------------------------------------------
Task5:		RET
;-----------------------------------------------------------------------------
Task6:		RET
;-----------------------------------------------------------------------------
Task7:		RET
;-----------------------------------------------------------------------------
Task8:		RET
;-----------------------------------------------------------------------------
Task9:		RET
;-----------------------------------------------------------------------------



;===========================================================================
; RTOS Here	(��������: ���� ���� ������ ������������� � �������� ����!)
;===========================================================================
	.include "RTOS_kernel.inc"	; ���������� ���� ��
;===========================================================================

; ������� "��������� ���������" �� �������� ������ �����.
; �������:
; 	������ � ����, ��� � ��� ������� ������ ���� ��������� ��� ������������, ������� �� ������ �������� ��� "������ RTOS".
; 	������� ���������� ��������� �� �����, �� �� ������ ����������� ��������� ���������� ����� ������ (������ �������� � �������) - �� ����� ������ �� ����� ������ �����������/�������� ��� ������ � ���������� (� ������� �������� PLAN_TASK_*).
; 	��� �������� �������� � ��������� �������� ����������������, ������������� ���������� ����� ���������� ���������� ������� ������-�������, ��� ������ ������ (��. ����������� ������������� � ����� <macroapp.inc>).
; �������� ����:
; 	�.�. �������� ���� �������� � ����������� ���������������� �� ����������� "������-������� ������", �� � ����� ������������ ����� �������������� ������ �� ������ ���������� ������ ������!
; 	� ���� ����� ��� ���������� ������ �������������, �� �������, � ������ �����? ��� ����� �� ��������� ���������������� ��� ������������ ��������, � ������� ���������, �� ����� ������������� - � ����� ���� ������...
; �����������: 
; 	����������� ��������� ���������� ������������������ � ������� ����� = 256��.
; 	���������� ������� ����� = [0..255],
; 	������� ���� ������������ ��������� ������ Task_Idle, ��� ������� ==0

RTOS_TaskProcs:
	 	.dw	Task_Idle	; [  0]	��������� ������ "�������� ����"  (�����: ������ ���� ����������� �� �������=0 - ������������ �������� RTOS)
		.dw	Task1		; [  1]
		.dw	Task2		; [  2]
		.dw	Task3		; [  3]
		.dw	Task4		; [  4]
		.dw	Task5		; [  5]
		.dw	Task6		; [  6]
		.dw	Task7		; [  7]
		.dw	Task8		; [  8]
		.dw	Task9		; [  9]
		;...
		;.dw	Task255		; [255] ��������� ��������� ������, ������������������ � �������



;***** END "RTOS Tasks" section
; coded by (c) DI HALT, 2008 @ http://easyelectronics.ru/
; coded by (c) Celeron, 2014 @ http://we.easyelectronics.ru/my/Celeron/
