.IFNDEF	_RTOSMACRO__INCLUDED_
.EQU	_RTOSMACRO__INCLUDED_ = 1
.MESSAGE "Note: <RTOS-macro.inc> have included, only once!" 
.include "macrobaselib.inc"	; ���������� ������� ���������������� (���������)
;=== BEGIN "RTOS-macro.inc" ================================================

; �������, ����������� ���������� ����������� ��������� RTOS (API)
; (����������: ����� ��, ��� ��� ����� ������������, �� RTOS, � ���������� ���� �������.)


;***************************************************************************
;*
;*  ���������� ��������� � ���������� ��������� ��� RTOS
;*
;***************************************************************************


; ����������� �������� ���������� ����� ��������� ��������� ����������� RTOS, �������� ������ ���� (����������� ��� ������� ������ � ������ ��):

.EQU	RTOS_FEATURE_PLANTASK = 1		; ��������� ������� ��������������� �����	(���������������� ������, � ������� �������)
.EQU	RTOS_FEATURE_PLANTIMER = 1		; ��������� ��������������� ����� �� �������	(���������� ���������� ������ �� <X> �����������)
.EQU	RTOS_FEATURE_PLANWAITER = 1		; ��������� �������� �������������		(��������������� ����� �� ��������� ���� � ������ ���, ����������� ��������/�����, ��� � ������ SRAM:  ������ ������, ����/���-������ ��� � ����� ����������?  ��� ����/���-������ ��� � ����� ����?)

;.EQU	RTOS_OPTIMIZE_FOR_SMALL_MEMORY = 1	; ����������� ��� ������ ��� ����� ������� ��!	(��������� � ��������� ������� RTOS ������ ��������� ������, ��������� ����������� ��������, �������� �������������� ������ �� ����, ��������� �� ��� ������ ����� - ����� �������������� ����� ���� RTOS, � �������, �������������� ������������/��������� ����� ��� ��� ����)
						; 
						; �����: ��� ������ ������������ ������� RTOS (� ������ �� ���������� ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY) - � ����� ��������� �������������� �������� 8 (������) ����!
						; 	(��� ���� �������������: ��������� ������ ��������� ���������� ������... � ����� ������, ������ ���������� ���������� ���������� "��������� ������"... �, �� ���� �����, ����� RCALL-�������� ������������ ������ ������ �����������, � �.�. ������������ PLAN_TASK-������...)
						; 	(��� ���������: ��. ����������� � "������ �������� RTOS", ����...)
						; 
						; � ����, � ���������� ���� �����, �� ����������� ������ ����������� (RCALL) ����� ������ ������ ����������� - ����������� ����������� ����������� ������� ����� (��� �������, ��� ������� ��������; � ��� PUSH/POP ����������, ���� ������� ������������ � ����)!
						; �������: ��� ��������� �������� SRAM ��� "������� �����", "���� ��������" � "�����������" (������������� � <RTOS_data.inc>).
						; ���������� ��� - ����� ������������ ��� ���������� ������... 
						;
						; ��������: �� �� ATtiny10 (��� 32 ������ SRAM), ����� ������������ RTOS:
						; 	� ����� �������� (-3�����) 
						; 	� ����� �������� ��������� (-3�����), 
						; 	� ������� �������� 5��. ����� (-5����), 
						; 	�� ������ ��� ������� "��������� ������" (-1����); 
						; 	��� (-8 ����) �� ���� ��� RTOS; 
						; 	������� 12 ���� SRAM �� ���������� ������ (10 �� ������ + 2 ��� �����). Profit!
						; 	/������: ��� ����, � �������� ���� ������� ����� �������� 350-450 ���� - ��������� �� ���������� ������./

; ������ �����: ��������� ��������� �������� - � �������� ������������� RTOS_INIT, USART_INIT... (�� ���������� ��������� �������������, ����� ������� �������)


;---------------------------------------------------------------------------


; ��������� ����������, ������������ � ���� RTOS:
;.def	temp	= R16	; (���������� � macrobaselib.inc)
;.def	temp1	= R16	; (���������� � macrobaselib.inc)
;.def	temp2	= R17	; (���������� � macrobaselib.inc)
;.def	temp3	= R18	; (���������� � macrobaselib.inc)
;.def	temp4	= R19	; (���������� � macrobaselib.inc)



; ����������� ������������ ��������� ��� �����������-������� ���� RTOS:
; (��������: �� ����������� ����� "��������� ��������" - ������ ������������������ �� � "������������� ����������"!)

.def	TaskCode		= R25	; �����-������ ������ � ������� RTOS_TaskProcs

.def	PoolParameter2byte	= R24	; 2-�� ���� �������� ���� (�������� ������� ��. � RTOS_data.inc)	/������������ ��� ��������� ������� � ��������� ��������/
.def	PoolParameter3byte	= R23	; 3-�� ���� �������� ���� (�������� ������� ��. � RTOS_data.inc)	/������������ ��� ��������� ������� � ��������� ��������/

.def	ErrorCode		= R24	; ��� ��������� ������	/��������� � RTOS_METHOD_ErrorHandler/




;***************************************************************************
;*
;*  ������ ���������� ����� � �������
;*
;***************************************************************************


.ifdef	RTOS_FEATURE_PLANTASK
;---------------------------------------------------------------------------
; ������� ��������������� ����� (���������������� ������, � ������� �������)



	; �������� ������ � ������� RTOS_TaskQueue, ������� �� ����������
	; �������: ��� ��������� � ��������� ���������� - ������ ���������� ���� ���������, ���������� � ������ "Subroutine Register Variables", ���������������� ���������� ������, � � ����������� �� �������� ������ �����������...
	; ������ ������:  PLAN_TASK tasknumber
	.MACRO 	PLAN_TASK
		.if @0==$00
		.ERROR "system <Task_Idle> is forbidden to use!"
		.endif
		LDI	TaskCode,	@0			; ��������� ���������
		RCALL	RTOS_METHOD_AddTask			; �������� ��������� �����
	.ENDM



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; �������� ������ � ������� RTOS_TaskQueue, ������� �� ����������	(���������� ������)
	; �������: ������ ������ �� ������ ���������� ��� (���� ��� �������� ����������) - ��������� �������� ������������ �������� ������.
	; ������ ������:  SAFE_PLAN_TASK tasknumber
	.MACRO 	SAFE_PLAN_TASK
		PUSH	TaskCode				; �����, ��������� ������ ������������ ��������!
		PLAN_TASK	@0
		POP	TaskCode
	.ENDM
	.endif



.endif	//RTOS_FEATURE_PLANTASK

.ifdef	RTOS_FEATURE_PLANTIMER
;---------------------------------------------------------------------------
; ��������������� ����� �� ������� (���������� ���������� ������ �� <X> �����������)



	; ���������� (�������� ��� ��������) ������ � ���� RTOS_TimersPool, ��� ����������� ������� ������
	; �������: ��� ��������� � ��������� ���������� - ������ ���������� ���� ���������, ���������� � ������ "Subroutine Register Variables", ���������������� ���������� ������, � � ����������� �� �������� ������ �����������...
	; ������ ������:  PLAN_TIMER tasknumber,delay
	.MACRO	PLAN_TIMER
		.if @0==$00
		.ERROR "system <Task_Idle> is forbidden to use!"
		.endif
		.if @1==0
		.ERROR "0ms time delay is forbidden for <RTOS_METHOD_AddTimer> method!"
		.endif
		LDI	TaskCode,		@0		; ��������� ���������
		LDI	PoolParameter2byte,	Low (@1 -1)	; ����������: �����, ��������� (-1) ��������, ����� � ��������� ������ RTOS_METHOD_TimerService: ��� ������������� ��������, "��� �� �������� �������" ������ ��� "��� ������� �������� ��������" (��� ������� ����������)
		LDI	PoolParameter3byte,	High(@1 -1)
		RCALL	RTOS_METHOD_AddTimer			; �������� ��������� �����
	.ENDM



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ���������� (�������� ��� ��������) ������ � ���� RTOS_TimersPool, ��� ����������� ������� ������	(���������� ������)
	; �������: ������ ������ �� ������ ���������� ��� (���� ��� �������� ����������) - ��������� �������� ������������ �������� ������.
	; ������ ������:  SAFE_PLAN_TIMER tasknumber,delay
	.MACRO	SAFE_PLAN_TIMER
		PUSH	TaskCode				; �����, ��������� ������ ������������ ��������!
		PUSH	PoolParameter2byte
		PUSH	PoolParameter3byte
		PLAN_TIMER	@0,@1
		POP	PoolParameter3byte
		POP	PoolParameter2byte
		POP	TaskCode
	.ENDM
	.endif



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ������ (�������) ������ �� ���� RTOS_TimersPool, ��� ������ ����������� ������� ������
	; �������: ��� ��������� � ��������� ���������� - ������ ���������� ���� ���������, ���������� � ������ "Subroutine Register Variables", ���������������� ���������� ������, � � ����������� �� �������� ������ �����������...
	; ������ ������:  REMOVE_TIMER tasknumber
	.MACRO 	REMOVE_TIMER
		.if @0==$00
		.ERROR "system <Task_Idle> is forbidden to use!"
		.endif
		LDI	TaskCode,	@0			; ��������� ���������
		RCALL	RTOS_METHOD_RemoveTimer			; �������� ��������� �����
	.ENDM
	.endif



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ������ (�������) ������ �� ���� RTOS_TimersPool, ��� ������ ����������� ������� ������	(���������� ������)
	; �������: ������ ������ �� ������ ���������� ��� (���� ��� �������� ����������) - ��������� �������� ������������ �������� ������.
	; ������ ������:  SAFE_REMOVE_TIMER tasknumber
	.MACRO 	SAFE_REMOVE_TIMER
		PUSH	TaskCode				; �����, ��������� ������ ������������ ��������!
		REMOVE_TIMER	@0
		POP	TaskCode
	.ENDM
	.endif



.endif	//RTOS_FEATURE_PLANTIMER

.ifdef	RTOS_FEATURE_PLANWAITER
;---------------------------------------------------------------------------
; ��������������� ����� �� ��������� ���� � ����������� ��������/����� ��� � ������ SRAM (�������� �������������: ������, ���� ��� � ����� ����������?  ���� ��� � ����� ����?)



	; ���������� (�������� ��� ��������) ���������� � ���� RTOS_WaitersPool, ��� ��������� ������� ������
	; �������: ��� ��������� � ��������� ���������� - ������ ���������� ���� ���������, ���������� � ������ "Subroutine Register Variables", ���������������� ���������� ������, � � ����������� �� �������� ������ �����������...
	; ������ ������:  PLAN_WAITER tasknumber,address,bit,state
	.MACRO	PLAN_WAITER
		.if @0==$00
		.ERROR "system <Task_Idle> is forbidden to use!"
		.endif
		.if @1<0 || @1>0xFFF
		.ERROR "'address' parameter must be in [0..0xFFF]!"
		.endif
		.if @2<0 || @2>7
		.ERROR "'bit' parameter must be in [0..7]!"
		.endif
		.if @3!=0 && @3!=1
		.ERROR "'state' parameter must be [0 or 1]!"
		.endif
		LDI	TaskCode,		@0		; ��������� ���������
		LDI	PoolParameter2byte,	Low(@1)
		LDI	PoolParameter3byte,	(@3<<WAITER_BIT_STATE)|(@2<<WAITER_BIT_POSITION)|(High(@1)<<WAITER_ADDRESS_HIGH)
		RCALL	RTOS_METHOD_AddWaiter			; �������� ��������� �����
	.ENDM



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ���������� (�������� ��� ��������) ���������� � ���� RTOS_WaitersPool, ��� ��������� ������� ������	(���������� ������)
	; �������: ������ ������ �� ������ ���������� ��� (���� ��� �������� ����������) - ��������� �������� ������������ �������� ������.
	; ������ ������:  SAFE_PLAN_WAITER tasknumber,address,bit,state
	.MACRO	SAFE_PLAN_WAITER
		PUSH	TaskCode				; �����, ��������� ������ ������������ ��������!
		PUSH	PoolParameter2byte
		PUSH	PoolParameter3byte
		PLAN_WAITER	@0,@1,@2,@3
		POP	PoolParameter3byte
		POP	PoolParameter2byte
		POP	TaskCode
	.ENDM
	.endif



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ������ (�������) ���������� �� ���� RTOS_WaitersPool, ��� ������ ��������� ������� ������
	; �������: ��� ��������� � ��������� ���������� - ������ ���������� ���� ���������, ���������� � ������ "Subroutine Register Variables", ���������������� ���������� ������, � � ����������� �� �������� ������ �����������...
	; ������ ������:  REMOVE_WAITER tasknumber
	.MACRO 	REMOVE_WAITER
		.if @0==$00
		.ERROR "system <Task_Idle> is forbidden to use!"
		.endif
		LDI	TaskCode,	@0			; ��������� ���������
		RCALL	RTOS_METHOD_RemoveWaiter		; �������� ��������� �����
	.ENDM
	.endif



	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
	; ������ (�������) ���������� �� ���� RTOS_WaitersPool, ��� ������ ��������� ������� ������	(���������� ������)
	; �������: ������ ������ �� ������ ���������� ��� (���� ��� �������� ����������) - ��������� �������� ������������ �������� ������.
	; ������ ������:  SAFE_REMOVE_WAITER tasknumber
	.MACRO 	SAFE_REMOVE_WAITER
		PUSH	TaskCode				; �����, ��������� ������ ������������ ��������!
		REMOVE_WAITER	@0
		POP	TaskCode
	.ENDM
	.endif



.endif	//RTOS_FEATURE_PLANWAITER

;***************************************************************************
;*
;*  ������������� RTOS
;*
;***************************************************************************


	; ������������� RTOS
	; (��������: ���� ���� ������ ������������� � ���� ������������� ����������������, ����������� �������� ����� "Reset"!)
	; �������: ������ ���������� �������� TEMP1.

	.MACRO	RTOS_INIT
	.if defined(RTOS_FEATURE_PLANTASK)			; (��� ��������� "������� ��������������� �����" ��� ������� RTOS ���������!)


	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		.ifdef	RTOS_FEATURE_PLANTASK
		RCALL	RTOS_METHOD_ClearTaskQueue		; �������� ������� ����� RTOS
		.endif
		
		.ifdef	RTOS_FEATURE_PLANTIMER
		RCALL	RTOS_METHOD_ClearTimers			; �������� ������ �������� RTOS
		.endif

		.ifdef	RTOS_FEATURE_PLANWAITER
		RCALL	RTOS_METHOD_ClearWaiters		; �������� ������ ����������� RTOS
		.endif
	.endif	//RTOS_OPTIMIZE_FOR_SMALL_MEMORY


		.MESSAGE "Attention: check the main RTOS Timer setting! Are constant set correspond to CPU clock? Choose one free hardware Timer... Are Register bits correct?"


		; ��������������� ��������� ��� ��������� ��������� ������� ���� RTOS:
		;
		; ����������: 
		; 	���������� ����������� ������� ��, �������� ������������ -> 
		; 	�������� �������� "�������� �������� �������" ���, 
		; 	����� ������� �������/�������� Overflow/Compare ����������� ������ 1ms.
		; �����������: 
		; 	�������� ����������� ����� ������������ ������ �������������� ��������� ���������� �������� (�� ��������, �����, ��������� ��������� ��� ��������������� ���� ������������!)
		; 	�������� �������� ������ ���������� ����� ������! �� ��� ����� ������ - ����� ���������� � ��������� ����� �������: ��� 8-bit <=255 / ��� 16-bit <=65535.

		.equ CMainClock		= 8000000				; �������� ������� "CPU Clock" (Hz)
		.equ CTimerPrescaler	= 64					; ������������ ����������� �������: 1, 8, [32,] 64, [128,] 256, 1024
		.equ CTimerDivider	= CMainClock/CTimerPrescaler/1000	; �������� "�������� �������� �������", �.�. ����������� � �������������� �� ����� ~ 1ms



		; ������������:
		.IF FRAC(CMainClock/CTimerPrescaler/1000.0)>0
		.WARNING "CTimerDivider is fractional! Timings could lose accuracy..."
		.ENDIF

		; ������������ (������ ��� 8-bit ��������):
		.IF CTimerDivider>255
		.WARNING "CTimerDivider is incorrect (over byte capacity)! Use bigger CTimerPrescaler..."
		.ENDIF



		; ��������, ��� ���� RTOS, ���� �� ��������� ���������� ��������/���������.
		;
		; �.�. ������� �� ����� �����, � ����������� ��������� ��������, � � ������� ���� ��������/���� ���������, 
		; ����� ���� ���������� ������ ����������� ��� ���� ����������� (��������� ������� ����� ���� ������), 
		; �� ����� � ������������ ����������� ����������� ������� - ������, ������������ ����� �������������, �������!
		; �� ��� ���������� ������, ��� ���������� ��� ��������������� �������, ����������� ����������� �������: ��. ����� RTOS_USE_TIMER0_OVERFLOW


	.ifndef	RTOS_USE_TIMER0_OVERFLOW
	.ifdef	RTOS_USE_TIMER2_COMPARE

		; ������, ��� �� "������ ������������� DI HALT'��", ��� ��������� ������/������� ����������� � "������� ����������" - ���� �������� "������ ��������" �� ��� ���������� "Timer/CounterX Compare Match".
		; ���� ����� ������� � ���, ����� ����������� CTimerDivider ����� ������� � ������������� �������� ���������� - ����� � �������������� 1ms.
		; ��� ��������� ����������� ����������� "������ �������� RTOS", ���� �� ��������� ����������� "������� �������" (�� ���������� ������������ ���������, �������� ������� �������� �������� �������).
		; ������ ���� �������� �� ����� �������� "�������� �������" (CPU clock). � ������ �����! �� ����������� ��� �������, ����� ����������� "�������� �������" CMainClock � ��������� �������� "�������������" CTimerPrescaler �� ��������� �������� ������ �������� "��������" CTimerDivider - � ����� �������, ���������� ��������� ����������� �����...


		; ��������� ����������� 8-bit Timer/Counter2:
		; (Supported: ATmega8/ATmega16)
		
		OUTI	TCCR2,	1<<CTC2|4<<CS20			; ���������� ����� CTC (��������� ����� ���������� �������� ���������) � ������������ = clk/64
		OUTI	OCR2,	Low(CTimerDivider)		; ���������� �������� � ������� ���������
		OUTI	TCNT2,	0				; ���������� ��������� �������� ��������
		SETB	TIMSK,	OCIE2				; ��������� ����������: Timer/Counter2 Output Compare Match (�� ���������� �������� ���������)

	.else	//end of RTOS_USE_TIMER2_COMPARE

		; (*** ����� �� ������ �������� ���� ������ ��� ������������� ������� ***)

	.endif
	.else	//begin of RTOS_USE_TIMER0_OVERFLOW

		; ���� ������ ���������� �������� �������� CTimerDivider == 256 ��� 512, ������� ������� �������� �������� 
		; (��� ��������� ����� � ������ ��� ������������� ����������� "������� �������") - 
		; ����� �� ������ ������������ ����� ������� ������/�������, � ��� ������� ���������� "�� ������������" ("Timer/CounterX Overflow").

		; �� ���� ����������� ���������� �������� � �������� - ����� ������ ��������� ���������� ��� ���������, ������������, � ������ ���������� "Timer/CounterX Overflow".
		; ������, ��� "����� ����������" ������������ ������ � ����� ������������ � ������ ������/���������, ���������� ����� ������� ��������� ��������� (���, ������������ �� ���������� ������ 32kHz, � ��.) - ����� �������� ����� ��� RTOS!
		; �������, ����� ����� ���� ������, ���� ���������� ��� ������ ������: ������������� �������� ��������, � ������ ������� ����������� "�� ������������", ����������� �������������� � ������ �������� �������� CTimerDivider:
		; 	����� ���������� �������� ����� - ����� ������������ �������� � ������� �������� �� "������", � "�����"!
		; 	����������� ���������� Timer/Counter0 - ������� ���� �� ���� ��, � ������, ����� ������� �� ����������� (�� �����).
		; 	������ �������� RTOS_TIMER_SERVICE ������� �� ��� ���������� "�� ������������" ("Timer/Counter0 Overflow").
		; 	� ���� �� ����������, �� ������������ �������� � ������ �������� �������� � ����, ����� ������ ��� ������������������� ������� � "���������� �� ������������ = ���������� ��������":	OUTI TCNT0, 256-Low(CTimerDivider)

		; ����������: � ��������� ������� ������� ATtiny ������������ ������������ ������ (16-bit Timer/Counter0), �� �� 16-������! 
		; 	��� ���������������� �������� ����� �����:	OUTI TCNT0, (65536-CTimerDivider)
		; 	��� "�������� �������� �������" ��������:	CTimerDivider, ��� 16-bit, <=65535.
		; ������� ��� ������������� ������� ���������������� �������������, ����� ��������� �������� ���������� (��������� �� ATtiny10).



		; � ��������� ��, Timer/Counter0 ����� ���������, ��� � ���� �� ���� "Control Register TCCR0", � ���������... ������, �������� � ������������ ������ ������������� ����� TCCR0B - ��� � ��������� ��� ����� ���������, ��� ��������������� ����.
		.ifndef	TCCR0
			.equ	TCCR0 = TCCR0B
		.endif
		
		; ����������, ������ ��������� ��������� "Timer/Counter Interrupt Mask"... �������� �������� ���������.
		.ifndef	TIMSK
			.equ	TIMSK = TIMSK0
		.endif

		; �������������� ����� ����� ������������ ������������ (��� ������� ������������ � �����������, ���� �� �� ����, �� AVR):
		.if	CTimerPrescaler == 1
			.equ	CTimerPrescaler_CS 	= 0b001
		.elif	CTimerPrescaler == 8
			.equ	CTimerPrescaler_CS	= 0b010
		.elif	CTimerPrescaler == 64
			.equ	CTimerPrescaler_CS	= 0b011
		.elif	CTimerPrescaler == 256
			.equ	CTimerPrescaler_CS	= 0b100
		.elif	CTimerPrescaler == 1024
			.equ	CTimerPrescaler_CS	= 0b101
		.else
			.ERROR "CTimerPrescaler is incorrect! Use one of acceptable values: 1, 8, 64, 256, 1024"
		.endif


		; ��������� ����������� 8-bit Timer/Counter0:
		; (Supported: ����������� �� AVR)

		OUTI	TCCR0,	(CTimerPrescaler_CS<<CS00)	; ��������� ������ �� �������� �������, � ���������� ������������ = clk/64
		SETB	TIMSK,	TOIE0				; ��������� ����������: Overflow Interrupt Enable 

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

	.endif	//end of RTOS_USE_TIMER0_OVERFLOW
	.endif	//RTOS_FEATURE_PLANTASK
	.ENDM




.if defined(RTOS_FEATURE_PLANTASK) && (defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER))	; (������ �������� ������������ ������ � ���� �������)
; Q: ����� ���������� ������ ������������� ��� "������ �������� ���� RTOS" (Main Timer Service)?
; 
; A1) ����� ������������ ������� ������/�������0 � ��� ������� ���������� "�� ������������" ("Timer/Counter0 Overflow") - �������� ��� �����������.
; ������ ����� ������������� ������������, �� ���������!
; �����������: �������� ��� ����������� �����, ���������� ������� ���������� ��������, ����� � ���������.
; �����������������: �����, ��� ���������� ������ ���������� ��������� �������, � ����������� �������� ����� ���������������� ������������� (�������� ��� ����������� �� AVR).
.EQU	RTOS_USE_TIMER0_OVERFLOW = 1

; A2) ���� �� ������ ������������ 8-������ Timer/Counter2 � "������� ����������" - �������� ��� �����������.
;.EQU	RTOS_USE_TIMER2_COMPARE = 1

; A3) ���� �� �� ������ ������������ ���� ���� ����� - �� ������ ��������������� ��� ������� �����������, 
; � ��������� ���� ��� � ��������������� ������ "else", ����� ����������� �������� ����������, ����...
.endif




; Q: �������: ��� ������, ���� CTimerDivider ����� �� ���������� ����� ������?
; A: 
; ������/������� ����� ��������� ������ ����� ����� �����! 
; � ��������, ����� ��������������� � ������� �������� ��� ����, ������ �������� ��� ������� �����...
; ��� ���������� ������, ��� ���������� �������� �������� - ������ �������� ����� �� ������� ������. 
; ����������� ����� ��� ��������� �������, � ������� �� ������ ������������...
; ����� �������, ���� ����� ������, �� ����� ��������������� �����������, ����������� ��� ������������ ������� ����� ��������.
; �� �����, "���������� ������� RTOS" - �� �������� �������������! �� ��� � ������������ ��, ��������, �������������� �� �� ������...
;
; ��������:
; 	��� CPU clock=8MHz,  prescaler=64  -> divider=125tic    ~ 1ms, ��� 1tic=0,008ms  -> ����������� = 0
; 	��� CPU clock=20MHz, prescaler=128 -> divider=156,25tic ~ 1ms, ��� 1tic=0,0064ms -> ����������� �������������=0.25/156=0,0016 ��� ����������=128*0.25=32clk, �.�. 16us �� 1ms	(�.�. ������ ������� �� 0,0016��� �� 1���)
; 	��� CPU clock=1MHz,  prescaler=32  -> divider=31,25tic  ~ 1ms, ��� 1tic=0,032ms  -> ����������� �������������=0.25/31=0,008 							(�.�. ������ ������� �� 0,008���  �� 1���)
; 	��� CPU clock=1MHz,  prescaler=8   -> divider=125tic    ~ 1ms, ��� 1tic=0,008ms  -> ����������� = 0
;
; ������������:
; 	��� ������, ��������, ��� divider=31,25tic - ����������� 0.25 ��� �� ��� ������� ������ ����� ����� 31.0...
; 	�������������, ��� ������� divider, ������ �� ���������� �������� divider<10, � ����� ����� ����� divider>100. ����� ����������� �� ������� ����� ����� �����������...



;---------------------------------------------------------------------------


	; ������������� USART
	; (����������: USART ���������� �� ��������� ��� RTOS! ������ ������ ��������������� ������������� ��� "�������" - 
	;              ������ �������� ��� � ������ ������������� ��, ���� �� ����������� ��� ������������ ����������...)
	; �������: ������ ���������� �������� TEMP1.

	.MACRO	USART_INIT

		.MESSAGE "Attention: check USART setting! Are constant set correspond to CPU clock? Are Register bits correct?"


		; ��������������� ��������� ��� ��������� �������� USART:
		.equ CXTAL		= 8000000				; �������� ������� "CPU Clock" (Hz)
		.equ CBaudRate		= 9600					; �������� ��������, �����������: 19200, 28800...
		.equ CBaudDivider	= CXTAL/(16*CBaudRate)-1		; ������� ��� "asynchronous normal mode" (U2X=0) (without "double speeding")


		OUTI 	UBRRL,	Low (CBaudDivider)				; "��������" ��������� ��������� CPU clock (�� �������� ��� ���������� �����������, � �.�. USART) 
		OUTI 	UBRRH,	High(CBaudDivider)				; � ����� �� ����������� "������ ��������" ����� ���������������� ���� (CBaudRate).
		OUTI 	UCSRA,	0						; (zero event flags - just cleaning)
		OUTI 	UCSRB,	(1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<TXCIE)	; (enable receiver and transmitter, enable it interrupts)

	.ifdef	URSEL
		; ����������: � ATmega8/16/32 ������������ ������������������� ����������� ��������� UBRRH/UCSRC - ������������, ����� ������ � URSEL...
		OUTI 	UCSRC,	(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0)		; (set 8-bit character size)
	.else
		; � ��������� �� - �� �����������...
		OUTI 	UCSRC,             (1<<UCSZ1)|(1<<UCSZ0)		; (set 8-bit character size)
	.endif

	.ENDM



;***************************************************************************
;*
;*  ������ �������� RTOS
;*
;***************************************************************************


	.if defined(RTOS_FEATURE_PLANTASK) && (defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER))	; (������ �������� ������������ ������ � ���� �������)

	; ������ �������� RTOS	(������������� � ���������� ����������� ������� � ����������� ������ 1��)
	; �������: ������ ���������� ���������� �� ������ ���������� ��� - ��������� �������� ������������ �������� ������. �����, SREG ����������� ������.
	
	; ��������: ������ ���������� ���������� �������/���������� � �����  2+2=4 ����  (�� ����� �������� � ���������� ���������)! 
	; 	������, ��� ���� ��� �������� "����� ������ ������" �� ���� ������� RTOS, �� ����������� ������, ��� �������� ��� ����� ������� AVR (� 32-64 ������� SRAM)...
	; 	���������� �����, ��� ��� ������ ��������� ������ (� ������ � ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY) - ���� �� ���������� ���� ������, �� ������ �� ����� �������� � ����� �� +2 ����� (�� ����� ��������).
	; 	����� �������, ��� ������ ������ ���� RTOS (� ������ �� ���������� ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY) - � ����� ��������� �������������� �������� 6 ����, ������ ��� RTOS!	(��� ����, ��������� ������ ��������� ���������� ������... � �����, � ����� ������, ������ ���������� ���������� ���������� "��������� ������"...)
	; 	�� �� ����� ���������� ����� ��������� �������� (RCALL) ������ ������������, ���� �� ������ ������ �����������: ��������, ����������� ��������� ��������� ������ PLAN_TASK - ������� � ����� ��������� ��� +2 �����!
	; 	�����, �������, ��� ������ ������� ��������� 8 ���� � �����.	(��� ����, ��������� ������ ��������� ���������� ������... � ����� ������, ������ ���������� ���������� ���������� "��������� ������"... �, �� ���� �����, ����� �������� ������������, ������ ������ �����������, � �.�. PLAN_TASK-������...)

	.MACRO	RTOS_TIMER_SERVICE

		PUSHF						; ��������� temp � SREG � �����
		LDS	temp,	RTOS_TimerServiceCounter
		INC	temp					; ���������������� ������� ���������� ���������� ����������� ������� (RTOS_TIMER_SERVICE), ��������� � ������� ��������� ��������� ���������� ������ (RTOS_METHOD_TimerService)
		BREQ	Exit__RTOS_TIMER_SERVICE		; ���� ���� ������������, �� �� ��������� �������� ��������, � ���������� ���� ����������� ������� ����������... (��� ����� ����, � ������ ������� ������� ���������� ������������ ���������� ������, ������ 256��.)
		STS	RTOS_TimerServiceCounter,	temp
	Exit__RTOS_TIMER_SERVICE:
		POPF

	.ENDM
	.endif	//RTOS_FEATURE_PLANTASK



;***************************************************************************
;*
;*  Old interface aliases, for backward compatibility.	
;*
;*  (Deprecated: Do not use it in new code!)
;*
;***************************************************************************


	.MACRO	SetTimerTask 
		PLAN_TIMER	@0,@1
	.ENDM


	.MACRO	SetTask
		PLAN_TASK		@0
	.ENDM


	.MACRO	TimerService
		RTOS_TIMER_SERVICE
	.ENDM


	.MACRO	INIT_RTOS
		RTOS_INIT
	.ENDM



;***************************************************************************
;*
;*  Some DI HALT's macro, distributed earlier with RTOS...
;*  Not recomended to usage, because makes code more obfuscated, without significant profit.
;*  Provided here for backward compatibility only!
;*
;*  (Note: It have not used in RTOS code, and it have not any relations to RTOS!)
;*
;***************************************************************************


;---------------------------------------------------------------------------
; SRAM memory read/write macros for Devices with AVR8L-based CPU: ATtiny10, ATtiny20, ATtiny40.
; Maked and used by DI HALT, but deprecated by Celeron.


	.MACRO	LDR
		.MESSAGE "Deprecated: macros LDR was improper! Use macros INR (macrobaselib.inc) instead!"
		;PUSH	ZL
		;PUSH	ZH
		;
		;LDI	ZL, Low(@1)
		;LDI	ZH, High(@1)
		;
		;LD	@0, Z
		;
		;POP	ZH
		;POP	ZL
		INR	@0, @1
	.ENDM


	.MACRO	STR
		.MESSAGE "Deprecated: macros STR was improper! Use macros OUTR (macrobaselib.inc) instead!"
		;PUSH	ZL
		;PUSH	ZH
		;
		;LDI	ZL, Low(@0)
		;LDI	ZH, High(@0)
		;
		;ST	Z,  @1
		;
		;POP	ZH
		;POP	ZL
		OUTR	@0, @1
	.ENDM


;---------------------------------------------------------------------------
; FLASH memory read/write helpers.


	; Read byte (unfortunately, only first in word) from Program Flash Memory, at particular Address @1 (in words), to particular Register @0.
	; Usage:  LDF register,label
	.MACRO	LDF
		.MESSAGE "Deprecated: macros LDF is clumsy! Do not use it at all!"
		PUSH	ZL
		PUSH	ZH

		LDI	ZL, Low(@1*2)
		LDI	ZH, High(@1*2)

		LPM	@0, Z

		POP	ZH
		POP	ZL
	.ENDM


	; Just load Program Label Address @0 to Z-register. (Supposedly, preparing to LPM instruction?)
	.MACRO	LDP
		LDI	ZL, Low(@0*2)
		LDI	ZH, High(@0*2)
	.ENDM

	; Synonym...
	.MACRO	LDPA
		LDP
	.ENDM


;---------------------------------------------------------------------------
; SRAM memory read/write helpers.


	; Just load SRAM Address @0 to X-register.
	.MACRO	LDX
		LDI	XL, Low(@0)
		LDI	XH, High(@0)
	.ENDM


	; Just load SRAM Address @0 to Y-register.
	.MACRO	LDY
		LDI	YL, Low(@0)
		LDI	YH, High(@0)
	.ENDM


	; Just load SRAM Address @0 to Z-register.
	.MACRO	LDZ
		LDI	ZL, Low(@0)
		LDI	ZH, High(@0)
	.ENDM



;=== END "RTOS-macro.inc" ==================================================
; coded by (c) DI HALT, 2008  http://easyelectronics.ru/
; coded by (c) Celeron, 2014  http://inventproject.info/
.ENDIF
