.IFNDEF	_MACROAPP__INCLUDED_
.EQU	_MACROAPP__INCLUDED_ = 1
.MESSAGE "Note: <macroapp.inc> have included, only once!" 
;=== BEGIN "macroapp.inc" ==================================================
; ���������� ����������������, ������������ ��� ���������� ������ ����������.
; (����������: ��� ������� - "������������������"!)



;***** BEGIN "RTOS Tasks" section ******************************************

; ���������� ��� �������-�������� ����� (������������ ��� ��������� �������� PLAN_TASK_*):
; ��������: ����������� �� ��� ��� ������ - ��� ������������ ������ ��� �������� ���������������� ����������� ����, � �� ������������ ����� ���� RTOS...
.equ	TS_Idle		= 0		; 
.equ	TS_Send		= 1		; 
.equ	TS_Led_Blink	= 2		; 
.equ	TS_TextTime	= 3		; 
.equ	TS_Task4	= 4		; 
.equ	TS_Test5	= 5		; 
.equ	TS_Test6	= 6		; 
.equ	TS_Test7	= 7		; 
.equ	TS_Test8	= 8		; 
.equ	TS_Test9	= 9		; 

;***** END "RTOS Tasks" section



	.MACRO 	LED_OFF
		CBI		LED_P,LED
	.ENDM


	.MACRO 	LED_ON
		SBI		LED_P,LED
	.ENDM


	.MACRO 	ENABLE_ON
		SBI		ENABLE_P,ENABLE
	.ENDM


	.MACRO 	ENABLE_OFF
		CBI		ENABLE_P,ENABLE
	.ENDM



;=== END "macroapp.inc" ====================================================
.ENDIF