.MESSAGE "<-- RTOS ver.2.1.0 alpha -->"
;=== BEGIN "RTOS-kernel.inc" ===============================================

; ���� RTOS
; (��������������: ���� ��� ������ �� �����, ���������������, ��� ������ �� ������������ � ���������� ����!)



.ifdef	RTOS_FEATURE_PLANTASK
;***************************************************************************
;*
;*  ������� ��������������� �����,
;*  ������ ���������� ��� ������ ���� ������� RTOS
;*
;***************************************************************************



;---------------------------------------------------------------------------
; 
; ���������� ������� ����� (���������)
;
; ��. �������� ���� http://easyelectronics.ru/avr-uchebnyj-kurs-operacionnaya-sistema-dispetcher-zadach.html
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

; ��� ����������.

; �������: � ������ ��� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ����� ZH:ZL (���������� ����������� ��� "Indirect Jump" � ������), ������ ��������� �� ������ - ��������� �������� ������������ �������� ������.
; �������: � ������  � ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ������ ������ ��������� (���� ������ �� ������������ ������ ������!) - ������ ���������� ��������� TEMP1, TEMP2, TEMP3, TEMP4 � ZH:ZL.
; ����������: ����, � ������ ������, ������������� ��������� ��������, �.�. ������ ����� ���������� �� ���������� "MAIN", ����� ��������� ���������� ����� - ����� ���������� ���������, ������, �� ���������.
; �������: SREG ����������� ������.

;----- Code


RTOS_METHOD_ProcessTaskQueue:

	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY					; �����������! ���� ������-��, ��������� �������� - �������, � ������ (�� �������������) ������...
		PUSH	temp1
		PUSH	temp2
		PUSH	temp3
		PUSH	temp4
	.endif

		;=== ������ ����������� ������					; (����������: ������+������ ����������� ������ - ������ ���� � ������ ����������!)
		INR	temp4,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������

		; ��������� ��� ������� ������ � ����������:
		; ����������: ��� ������� ���� � ������ �������, ����� �������� ��� ������� ������ == 0 (����.��� "��� ������") - � ����� ������, �����, ����� ���������� ������ ��������� ������ "�������� ����" (Task_Idle)...
		LDI	ZL,	Low (RTOS_TaskQueue)				; ��������� ����� ������ "������� �����" -> � ������� Z
		LDI	ZH,	High(RTOS_TaskQueue)
		LD	temp3,	Z						; ��������� ������ ���� �� ������� RTOS_TaskQueue[0] -> temp3

		; �������� "������� �����" �� ���� ������� �����:
		LDI	temp2,	RTOS_TaskQueueSize-1				; ���������� ����� (�������������)
	Loop__RTOS_METHOD_ProcessTaskQueue:
		LDD	temp1,	Z+1 						; RTOS_TaskQueue[i+1] -> temp1
		ST	Z+,	temp1						; temp1 -> RTOS_TaskQueue[i++]
		CPI	temp1,	$00						; ���������� RTOS_TaskQueue[i] �� ����.����� "��� ������"
		BREQ	EndShift__RTOS_METHOD_ProcessTaskQueue			; ���� �����? ������, ����� ������� ���� - ���������� ��� �����...
		DEC	temp2							; ���������� ����� (�������������)
		BRNE	Loop__RTOS_METHOD_ProcessTaskQueue
		ST	Z,	temp2						; � ��������� ������� ������� ��������� ����.��� "��� ������":	RTOS_TaskQueue[RTOS_TaskQueueSize]=0	(������, ��� ������, temp2==0)
	EndShift__RTOS_METHOD_ProcessTaskQueue:
	
		OUTR	SREG,	temp4						; Restore global interrupt flag
		;=== ����� ����������� ������


		; �������� �������� ����� ����� ����� � ������������ ������:
		; ����������: �������� ���������� �� "��������� ���������" - ��. ���������� ����� � "AVR. ������� ����. ��������� �� ��������� ���������" (�) http://easyelectronics.ru/avr-uchebnyj-kurs-vetvleniya.html
		; (������� ���������: temp3 == ��� ������� ������ � ����������)

	.ifdef	MAPPED_FLASH_START
		; ������� ���� ��� AVR8L Instruction Set: ��� ��� ��� R0:R15, � ��� ���������� LPM (������ �� LD).
		; see recipe: http://www.avrfreaks.net/index.php?module=PNphpBB2&file=viewtopic&t=82385&highlight=
		; ���������: ��� �������� �� ATtiny10!
		
		MOV	ZL,	temp3						; ������ ������� ������ -> ZL
		CLR	ZH
		LSL	ZL							; temp1 *= 2
		ADC	ZH,	ZH						; saving carry bit	(������: � ����������, � ���� ������������ �������-������� ZH ����� � ��� ������ �������-��������. ��� � ��� - ������ ����. � ����� ��� ��������� �������������, ������ ��� �� �������� =0, � ���� �����.)
		SUBI	ZL,	Low (-(RTOS_TaskProcs*2 + MAPPED_FLASH_START))
		SBCI	ZH,	High(-(RTOS_TaskProcs*2 + MAPPED_FLASH_START))	; Add
		
		LD	temp1,	Z+						; ��������� �� "��������� �������" ��.���� ������ �������� -> temp1
		LD	temp2,	Z						; ��������� �� "��������� �������" ��.���� ������ �������� -> temp2
		
	.else	//������� ���� ��� ��������� AVR

		MOV	ZL,	temp3						; ������ ������� ������ -> ZL
		CLR	ZH
		LSL	ZL							; temp1 *= 2
		ADC	ZH,	ZH						; saving carry bit	(������: � ����������, � ���� ������������ �������-������� ZH ����� � ��� ������ �������-��������. ��� � ��� - ������ ����. � ����� ��� ��������� �������������, ������ ��� �� �������� =0, � ���� �����.)
		SUBI	ZL,	Low (-RTOS_TaskProcs*2)
		SBCI	ZH,	High(-RTOS_TaskProcs*2)				; Add
	
		LPM	temp1,	Z+						; ��������� �� "��������� �������" ��.���� ������ �������� -> temp1
		LPM	temp2,	Z						; ��������� �� "��������� �������" ��.���� ������ �������� -> temp2
	.endif

		; ������� � ���������� ������:

		MOV	ZL,	temp1						; ��������� ����� ����� ����� � ������ -> � ������� Z
		MOV	ZH,	temp2

		OUTR	SREG,	temp4						; Restore SREG also (to be transparent)
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		POP	temp4
		POP	temp3
		POP	temp2
		POP	temp1
	.endif

		; ����: ����� ������������, � ��������������� "������ ������" �� ��.
		; ����: ������ ������������ RCALL, �� ������ � ������������ ������� RJMP, �������������� ��������� � ���� ����� �������� "����� ������".
		; ��������� �����: ��������� ����� ��������� ������������ �������������� ����� ����� ������� RET �� ���� �������� ���������. ����� �������, CALL + RET === JMP

		IJMP 								; ������ ������!!!
										; ������, ��� ����, ���������������� ������������� �����:
										; 	������ �������� - ���� �� ������ �� RCALL �� �������� �����.
										; 	������, � ����� � ��� ����� ����� ��������...
										; 	� ������ � ������ �� IJMP, ������� ���� �� ������.
										; 	�� ��� �� ������� - ���� �� ������ �� ������ �� RET!



;---------------------------------------------------------------------------
;
; �������� ������ � ������� RTOS_TaskQueue, 
; ������� �� ����������
;
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

;.def	TaskCode	= (��.����)	; (�������, �� ����������)	�����-������ ������ � ������� RTOS_TaskProcs

; �������: � ������ ��� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ����� ������������, ������ ��������� �� ������ - ��������� �������� ������������ �������� ������.
; �������: � ������  � ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ������ ������ ��������� (���� ������ �� ������������ ������ ������!) - ������ ���������� ��������� TEMP1, TEMP2, ZH:ZL.
; �������: SREG ����������� ������.

;----- Code


RTOS_METHOD_AddTask:

	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	temp1
		PUSH	temp2
		PUSH	ZL
		PUSH	ZH
	.endif

		;=== ������ ����������� ������					; (����������: ������+������ ����������� ������ - ������ ���� � ������ ����������!)
		INR	temp2,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������

		; ��������� ������� ������� �� �����, ���� ������ ������ ������:
		LDI	ZL,	Low (RTOS_TaskQueue)
		LDI	ZH,	High(RTOS_TaskQueue)
	Loop__RTOS_METHOD_AddTask:
		LD	temp1,	Z+
		CPI	temp1,	$00						; ��������: ������ ������?
		BREQ	Found__RTOS_METHOD_AddTask				; ���� ������ - ��������� ������������ �� ���� �������...
		CPI	ZL,	Low (RTOS_TaskQueue + RTOS_TaskQueueSize)	; ������� �����:
		BRNE	Loop__RTOS_METHOD_AddTask				; 	���� ����� � Z �� ����� �� ����� ������...
		CPI	ZH,	High(RTOS_TaskQueue + RTOS_TaskQueueSize)	; (����: ������, ����� �� ������������ ������������ ����������-�������, ����� ���������� �������)
		BRNE	Loop__RTOS_METHOD_AddTask

		; (���������: ������ ��� �������, �� ��� � �� ����� ������ ������ - ������!)
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	ErrorCode
		LDI	ErrorCode,	10					; ��� ������: 10 - "������� �����" �����������, �� ���� �������� ����� ������!
		RCALL	RTOS_METHOD_ErrorHandler
		POP	ErrorCode
	.endif
		RJMP	End__RTOS_METHOD_AddTask

		; ���� ����� ������ ����� (����������, ��� ������� "�����" �������) - �� ��������� ������� ��������� �������:
	Found__RTOS_METHOD_AddTask:
		ST	-Z,	TaskCode

	End__RTOS_METHOD_AddTask:
		OUTR	SREG,	temp2						; Restore global interrupt flag
		;=== ����� ����������� ������

	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		POP	ZH
		POP	ZL
		POP	temp2
		POP	temp1
	.endif
		RET




;***************************************************************************
;*
;*  �������� ��������������� �����
;*
;***************************************************************************



.if defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER)
;---------------------------------------------------------------------------
; 
; ������ �������� RTOS
; (����������: ������ ��������� ����� �������� � ������ � ������������ ���������� RTOS_TIMER_SERVICE)
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

; ��� ����������.

; �������: � ������ ��� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY: �� ������ ���������� ��� - ��������� �������� ������������ �������� ������.
; �������: � ������  � ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ������ ������ ��������� - ������ ���������� ��������� TEMP1, TEMP2, TEMP3, YH:YL, ZH:ZL � TaskCode.
; ����������: ����, � ������ ������, ������������� ��������� ��������, �.�. ������ ����� ���������� �� ���������� "MAIN", ����� ��������� ���������� ����� - ����� ���������� ���������, ������, �� ���������.
; �������: SREG ����������� ������.

;----- Code


RTOS_METHOD_TimerService:

	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	temp1
		PUSH	temp3
	.endif
		;=== ������ ����������� ������					; (����������: ������+������ ����������� ������ - ������ ���� � ������ ����������!)
		INR	temp3,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������
		
		LDS	temp1,	RTOS_TimerServiceCounter			; ������� ������� "���������� ��, � ������� ������� ��������� ������� ������" -> temp1
		TST	temp1
		BRNE	DoProcessing__RTOS_METHOD_TimerService			; ���� "��� ������ 1�� ��� �����", �� ���� ���������� ��� ����...
		RJMP	Exit__RTOS_METHOD_TimerService				; �����, ��� �� �����...
	DoProcessing__RTOS_METHOD_TimerService:
		
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	YL	
		PUSH	YH
		PUSH	ZL
		PUSH	ZH
		PUSH	temp2
		PUSH	TaskCode
	.endif


.ifdef	RTOS_FEATURE_PLANTIMER

		;== ���������� �������
		; ��. �������� ���� http://easyelectronics.ru/avr-uchebnyj-kurs-operacionnaya-sistema-tajmernaya-sluzhba.html

		LDI	YL,	Low (RTOS_TimersPool)				; ��������� ����� ������ "���� ��������" -> � ������� Y
		LDI	YH,	High(RTOS_TimersPool)

	LoopTimer__RTOS_METHOD_TimerService:
		LD	temp1,	Y						; ��������� "������������� ������" ���������� ������� -> temp1

		CPI	temp1,	$00						; ��������: ���� "��� ������"?
		BREQ	SkipTimer__RTOS_METHOD_TimerService			; �� ������� � ���������� ��������...


		; (���������: ������� ������� �������� - ���� ���������������� ��� �������)
		LDS	temp1,	RTOS_TimerServiceCounter			; ������� ������� "���������� ��, � ������� ������� ��������� ������� ������" -> temp1

		LDD	ZL,	Y+1						; 
		SUB	ZL,	temp1 						; ��������� ��.���� "�������� �������"
		STD	Y+1,	ZL						;

		LDD	ZH,	Y+2						;
		SBCI	ZH,	0						; ��������� ��.���� "�������� �������"	(���� ��� ��� ������� "carry bit")
		STD	Y+2,	ZH						;

		BRCC 	SkipTimer__RTOS_METHOD_TimerService			; ���� �� ���� ���� ("������� �������">=0), �� ���� ��� �� �������...


		; (���������: "������� �������" �������� �� ���� - ��������� ��������� ������)
		LD	TaskCode,	Y					; ��������� ���������
		RCALL	RTOS_METHOD_AddTask					; �������� ��������� �����	(������: �� ����������/�������� �������� TEMP1, TEMP2, ZH:ZL � TaskCode.)
		
		LDI	temp1,	$00
		ST	Y,	temp1						; ������ ���������� - ������ �������������...


	SkipTimer__RTOS_METHOD_TimerService:
		SUBI	YL,	Low (-3)					; ��������� � ���������� �������:
		SBCI	YH,	High(-3)					; 	����� Y+=3
		CPI	YL,	Low (RTOS_TimersPool + RTOS_TimersPoolSize*3)	; ������� �����:
		BRNE	LoopTimer__RTOS_METHOD_TimerService			; 	���� ����� � Y �� ����� �� ����� ������...
		CPI	YH,	High(RTOS_TimersPool + RTOS_TimersPoolSize*3)	; (����: ������, �����, �� ������������ ������������ ����������-�������, ����� ���������� �������)
		BRNE	LoopTimer__RTOS_METHOD_TimerService

.endif	//RTOS_FEATURE_PLANTIMER



.ifdef	RTOS_FEATURE_PLANWAITER

		;== ���������� ����������

		LDI	YL,	Low (RTOS_WaitersPool)				; ��������� ����� ������ "���� �����������" -> � ������� Y
		LDI	YH,	High(RTOS_WaitersPool)

	LoopWaiter__RTOS_METHOD_TimerService:
		LD	temp1,	Y						; ��������� "������������� ������" ���������� ���������� -> temp1

		CPI	temp1,	$00						; ��������: ���� "��� ������"?
		BREQ	SkipWaiter__RTOS_METHOD_TimerService			; �� ������� � ���������� ��������...


		; (���������: ������� ������� �������� - ���� ��������� ��� �������)
		LDD	ZL,	Y+1						; ������������� "12-������ ����� ����������� ������ ������" -> Z
		LDD	ZH,	Y+2
		ANDI	ZH,	(0b1111<<WAITER_ADDRESS_HIGH)
		LD	ZL,	Z						; ���������� "���������� ������������ �����" -> ZL (�� ��������� ����������)

		LDD	ZH,	Y+2
		BST	ZH,	WAITER_BIT_STATE				; "��������� ��������� ����" -> T
		ANDI	ZH,	(0b111<<WAITER_BIT_POSITION)
		SWAP	ZH							; "����� ������� [0..7] ������������ ����" -> ZH (�� ��������� ����������)

		; ������: ���������� INC/DEC �� ������������ "Carry Bit" (������������ ��������������� ������������)!
		; Manual: The C flag in SREG is not affected by the operation, thus allowing the INC/DEC instruction to be used on a loop counter in multiple-precision computations.
		ROL	ZL							; (��������������� ���������)
		INC	ZH
	ShiftBitForWaiter__RTOS_METHOD_TimerService:
		ROR	ZL							; �������� ������ (� �������������� "Carry Bit")
		DEC	ZH							; ���������� ����� (�������������)
		BRNE	ShiftBitForWaiter__RTOS_METHOD_TimerService

		ANDI	ZL,	(1<<0)						; "����������� ���" -> � ������� ������ ZL
		BLD	ZH,	0						; "��������� ��������� ����" -> � ������� ������ ZH	(������, ����� �����, ZH=0)
		EOR	ZL,	ZH						; �������� "������������ ���" ������: ���� ���� ���������, �� ZL ������ �������.
		BRNE 	SkipWaiter__RTOS_METHOD_TimerService			; ���� �� ���� - ������� �� ���������...


		; (���������: ������� ��������� - ��������� ��������� ������)
		LD	TaskCode,	Y					; ��������� ���������
		RCALL	RTOS_METHOD_AddTask					; �������� ��������� �����	(������: �� ����������/�������� �������� TEMP1, TEMP2, ZH:ZL � TaskCode.)

		LDI	temp1,	$00
		ST	Y,	temp1						; ������ ���������� - ���������� �������������...


	SkipWaiter__RTOS_METHOD_TimerService:
		SUBI	YL,	Low (-3)					; ��������� � ���������� �������:
		SBCI	YH,	High(-3)					; 	����� Y+=3
		CPI	YL,	Low (RTOS_WaitersPool + RTOS_WaitersPoolSize*3)	; ������� �����:
		BRNE	LoopWaiter__RTOS_METHOD_TimerService			; 	���� ����� � Y �� ����� �� ����� ������...
		CPI	YH,	High(RTOS_WaitersPool + RTOS_WaitersPoolSize*3)	; (����: ������, �����, �� ������������ ������������ ����������-�������, ����� ���������� �������)
		BRNE	LoopWaiter__RTOS_METHOD_TimerService

.endif	//RTOS_FEATURE_PLANWAITER


	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		POP	TaskCode
		POP	temp2
		POP	ZH
		POP	ZL
		POP	YH
		POP	YL
	.endif
		LDI	temp1, 0
		STS	RTOS_TimerServiceCounter, temp1				; ��������� ����������� - ������� �������.
		
	Exit__RTOS_METHOD_TimerService:
		OUTR	SREG,	temp3						; Restore global interrupt flag
		;=== ����� ����������� ������
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		POP	temp3
		POP	temp1
	.endif
		RJMP	RTOS_METHOD_TimerService_ExitPoint			; �������� ���� 
										; 	(���� ���� ��������: ��������� ������ ����� RTOS ����� ���������� ������ �� ������ �������������� ����� � ���� "�������� �����".)
										; 	(� ���� ��� ���������� ����� � ������: ��������� �� ���� ������� ������ ���������� ��������� ������������ RTOS_METHOD_AddTask...)
.endif	//RTOS_FEATURE_PLANTIMER || RTOS_FEATURE_PLANWAITER



;---------------------------------------------------------------------------
;
; ������ ����������� ���������� ������
; 
; RTOS_METHOD_AddTimer:		���������� ������ � ���� RTOS_TimersPool
; 
; RTOS_METHOD_AddWaiter:	���������� ���������� � ���� RTOS_WaitersPool
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

;.def	TaskCode		= (��.����)	; (�������, �� ����������)	�����-������ ������ � ������� RTOS_TaskProcs
;.def	PoolParameter2byte	= (��.����)	; (�������, �� ����������)	2-�� ���� �������� ���� (�������� ������� ��. � RTOS_data.inc)
;.def	PoolParameter3byte	= (��.����)	; (�������, �� ����������)	3-�� ���� �������� ���� (�������� ������� ��. � RTOS_data.inc)

; �������: � ������ ��� ����������� RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ����� ������������, ������ ��������� �� ������ - ��������� �������� ������������ �������� ������.
; �������: � ������  � ������������ RTOS_OPTIMIZE_FOR_SMALL_MEMORY: ������ ������ ��������� (���� ������ �� ������������ ������ ������!) - ������ ���������� ��������� TEMP1, TEMP2, TEMP3, TEMP4, ZH:ZL.
; �������: SREG ����������� ������.

;----- Code


.ifdef	RTOS_FEATURE_PLANTIMER
RTOS_METHOD_AddTimer:
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		PUSH	temp2
		PUSH	temp3
		PUSH	temp4
	.endif
		LDI	ZL,	Low (RTOS_TimersPool + RTOS_TimersPoolSize*3)	; ��������� ����� ����� ���� -> � ������� Z
		LDI	ZH,	High(RTOS_TimersPool + RTOS_TimersPoolSize*3)
		LDI	temp2,	RTOS_TimersPoolSize				; ���������� ����� (�������������)
		RJMP	RTOS_METHOD_AddPoolElement
.endif	//RTOS_FEATURE_PLANTIMER



.ifdef	RTOS_FEATURE_PLANWAITER
RTOS_METHOD_AddWaiter:
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		PUSH	temp2
		PUSH	temp3
		PUSH	temp4
	.endif
		LDI	ZL,	Low (RTOS_WaitersPool + RTOS_WaitersPoolSize*3)	; ��������� ����� ����� ���� -> � ������� Z
		LDI	ZH,	High(RTOS_WaitersPool + RTOS_WaitersPoolSize*3)
		LDI	temp2,	RTOS_WaitersPoolSize				; ���������� ����� (�������������)
		;RJMP	RTOS_METHOD_AddPoolElement
.endif	//RTOS_FEATURE_PLANWAITER



.if defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER)
RTOS_METHOD_AddPoolElement:
		;=== ������ ����������� ������					; (����������: ������+������ ����������� ������ - ������ ���� � ������ ����������!)
		INR	temp4,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������


		; ����: ���� ������������ ������� (��� �������), � ������, ���� ������ ������ (��� ������)
		LDI	temp3,	0						; ���������� ������� "��� �� �����" ��������� �� ������ ������ � ����	(temp3=0)
	Loop__RTOS_METHOD_AddPoolElement:
		SUBI	ZL,	Low (3)						; ��������� � ���������� :
		SBCI	ZH,	High(3)						; 	����� Z-=3

		LD	temp1,	Z						; ��������� "������������� ������" ���������� ��������
		CP	temp1,	TaskCode					; ���������: ��������� �� � ������� �������?
		BREQ	UpdateExisting__RTOS_METHOD_AddPoolElement		; ���� ��������� - ��� ��������� ���� �������...

		CPI	temp1,	$00						; ��������: ������ ������?
		BRNE	NotEmpty__RTOS_METHOD_AddPoolElement
		MOV	temp3,	temp2						; ���� ����� ������ ������ - ���������� � ��������, �� �������...	������: �.�. ������������ temp3=<�������> ���������� �� ������������� ��������, �� � temp3=(<����������� ������> + 1).
	NotEmpty__RTOS_METHOD_AddPoolElement:

		DEC	temp2							; ���������� ����� (�������������)
		BRNE	Loop__RTOS_METHOD_AddPoolElement


		; (���������: ������������ ������� �� ������ - ����� �������� �����...)
		TST	temp3							; ��������� ������: ���� "�� �����" ��������� �� ������ ������ � ����?	(temp3==0 ?)	������: ��� �������� temp3==0 ���� �� ����� - �.�. ���� ��� <����������� �������>==0, ��� �� ����� temp3==1.
		BRNE	FillNew__RTOS_METHOD_AddPoolElement			; ���� ���� ����� - ������� � ���������� ������ ��������


		; (���������: ������ ���� ���, �� ��� � �� ����� ������ ������ - ������!)
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		PUSH	ErrorCode
		LDI	ErrorCode,	20					; ��� ������: 20 - "���" ����������, �� ���� ���������������� ����� �������!	(����������: ������ Z==<������ ������ ����>, ��� �������������: ����� ������ ���?)
		RCALL	RTOS_METHOD_ErrorHandler
		POP	ErrorCode
	.endif
		RJMP	End__RTOS_METHOD_AddPoolElement


	FillNew__RTOS_METHOD_AddPoolElement:
		DEC	temp3							; ����������� ���������, ������:		temp3 = <����������� ������> ������ ������
		ADD	temp2,	temp3						; �������� � ������, ������������ ������ ����:	temp2 = 3*temp3
		ADD	temp2,	temp3						; 	(������: ����� �����, ���� temp2==0)
		ADD	temp2,	temp3						; 	(������: temp3<=85, ������� ����� ������� ����� �� �� �������, 3*85=255)
		CLR	temp3
		ADD	ZL,	temp2						; ��������� ��������� Z �� ��������� ������ ������, ��������� �����
		ADC	ZH,	temp3						; 	(������: ����� �����, ���� Z==<������ ������ ����>)
		ST	Z,	TaskCode					; ����������������� �������
	UpdateExisting__RTOS_METHOD_AddPoolElement:
		STD	Z+1,	PoolParameter2byte
		STD	Z+2,	PoolParameter3byte


	End__RTOS_METHOD_AddPoolElement:
		OUTR	SREG,	temp4						; Restore global interrupt flag
		;=== ����� ����������� ������
	.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY
		POP	temp4
		POP	temp3
		POP	temp2
		POP	temp1
		POP	ZH
		POP	ZL
	.endif
		RET
.endif	//RTOS_FEATURE_PLANTIMER || RTOS_FEATURE_PLANWAITER





.ifndef	RTOS_OPTIMIZE_FOR_SMALL_MEMORY	; ����������: ������ ������ �������� ���������������, ��� ��� ����� ��������.
;***************************************************************************
;*
;*  �������������� (�������������� � ����������) ������ RTOS
;*
;***************************************************************************



;---------------------------------------------------------------------------
; 
; ������ ����������� ���������� ������
; 
; RTOS_METHOD_RemoveTimer:	������� ������ �� ���� RTOS_TimersPool
; 
; RTOS_METHOD_RemoveWaiter:	������� ���������� �� ���� RTOS_WaitersPool
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

;.def	TaskCode	= (��.����)	; (�������, �� ����������)	�����-������ ������ � ������� RTOS_TaskProcs

; �������: ����� ������������, ������ ��������� �� ������ - ��������� �������� ������������ �������� ������.
; �������: SREG ����������� ������.

;----- Code


.ifdef	RTOS_FEATURE_PLANTIMER
RTOS_METHOD_RemoveTimer:
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		LDI	ZL,	Low (RTOS_TimersPool)
		LDI	ZH,	High(RTOS_TimersPool)
		LDI	temp1,	RTOS_TimersPoolSize				; ���������� ����� (�������������)
		RJMP	RTOS_METHOD_RemovePoolElement
.endif	//RTOS_FEATURE_PLANTIMER


.ifdef	RTOS_FEATURE_PLANWAITER
RTOS_METHOD_RemoveWaiter:
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		LDI	ZL,	Low (RTOS_WaitersPool)
		LDI	ZH,	High(RTOS_WaitersPool)
		LDI	temp1,	RTOS_WaitersPoolSize				; ���������� ����� (�������������)
		;RJMP	RTOS_METHOD_RemovePoolElement
.endif	//RTOS_FEATURE_PLANWAITER


.if defined(RTOS_FEATURE_PLANTIMER) || defined(RTOS_FEATURE_PLANWAITER)
RTOS_METHOD_RemovePoolElement:
		PUSH	temp2
		PUSH	temp3
		;=== ������ ����������� ������
		INR	temp3,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������
	Loop__RTOS_METHOD_RemovePoolElement:
		LD	temp2,	Z						; ��������� "������������� ������" ���������� ��������
		
		CP	temp2,	TaskCode					; ���������: ��������� �� � ������� �������?
		BRNE	NotFound__RTOS_METHOD_RemovePoolElement
		LDI	temp2,	$00						; = ����.��� "��� ������"
		ST	Z,	temp2						; ��������� �������
		RJMP	End__RTOS_METHOD_RemovePoolElement			; � ����� ��� �� �����, �.�. ������� � ����� ��������������� ����� ���� ������ ����...
	NotFound__RTOS_METHOD_RemovePoolElement:
		
		SUBI	ZL,	Low (-3)					; ��������� � ���������� ��������:
		SBCI	ZH,	High(-3)					; 	����� Z+=2
		DEC	temp1							; 	���������� ����� (�������������)
		BRNE	Loop__RTOS_METHOD_RemovePoolElement
		; (���������: �������� ��� ��������, �� ����� �������� - �����, ��� �� �����...)
		
	End__RTOS_METHOD_RemovePoolElement:
		OUTR	SREG,	temp3						; Restore global interrupt flag
		;=== ����� ����������� ������
		POP	temp3
		POP	temp2
		POP	temp1
		POP	ZH
		POP	ZL
		RET
.endif	//RTOS_FEATURE_PLANTIMER || RTOS_FEATURE_PLANWAITER



;---------------------------------------------------------------------------
; 
; ������� ������  (�������������)
; ������ ������ ������� �������� ������ ���, �� ����� ������������� ��, ����� ������� �������� ������ ��� �� ������.
; �����, �� ����� ����������, ��� ���������� �������������� ����������, ������ ������� ����� ������������� �������� ��� �������/���...
; 
; RTOS_METHOD_ClearTaskQueue:	�������� "������ �����" RTOS_TaskQueue 
; 
; RTOS_METHOD_ClearTimers:	�������� "��� ��������" RTOS_TimersPool
; 
; RTOS_METHOD_ClearWaiters:	�������� "��� �����������" RTOS_WaitersPool
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

; ��� ����������.

; �������: ������ ����� �� ������ ���������� ��� - ��������� �������� ������������ �������� ������.
; �������: SREG ����������� ������.

;----- Code


.ifdef	RTOS_FEATURE_PLANTIMER
RTOS_METHOD_ClearTimers:
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		LDI	ZL,	Low (RTOS_TimersPool)
		LDI	ZH,	High(RTOS_TimersPool)
		LDI	temp1,	RTOS_TimersPoolSize*3				; ���������� ����� (�������������)
		RJMP	RTOS_METHOD_ClearMemory
.endif	//RTOS_FEATURE_PLANTIMER


.ifdef	RTOS_FEATURE_PLANWAITER
RTOS_METHOD_ClearWaiters:
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		LDI	ZL,	Low (RTOS_WaitersPool)
		LDI	ZH,	High(RTOS_WaitersPool)
		LDI	temp1,	RTOS_WaitersPoolSize*3				; ���������� ����� (�������������)
		RJMP	RTOS_METHOD_ClearMemory
.endif	//RTOS_FEATURE_PLANWAITER


RTOS_METHOD_ClearTaskQueue:
		PUSH	ZL
		PUSH	ZH
		PUSH	temp1
		LDI	ZL,	Low (RTOS_TaskQueue)
		LDI	ZH,	High(RTOS_TaskQueue)
		LDI	temp1,	RTOS_TaskQueueSize				; ���������� ����� (�������������)
		;RJMP	RTOS_METHOD_ClearMemory

RTOS_METHOD_ClearMemory:
		PUSH	temp2
		PUSH	temp3
		LDI	temp2,	$00						; = ����.��� "��� ������"
		;=== ������ ����������� ������
		INR	temp3,	SREG						; Save global interrupt flag
		CLI								; ��������� ����������
	Loop__RTOS_METHOD_ClearMemory:
		ST	Z+,	temp2
		DEC	temp1							; ���������� ����� (�������������)
		BRNE	Loop__RTOS_METHOD_ClearMemory

		OUTR	SREG,	temp3						; Restore global interrupt flag
		;=== ����� ����������� ������
		POP	temp3
		POP	temp2
		POP	temp1
		POP	ZH
		POP	ZL
		RET



;---------------------------------------------------------------------------
; 
; ���� ��� ���������� ���� ���� RTOS ��������� ��������� ������ (kernel panic)...
; (��������: "������� �����" ��� ���������, � ���������� ��� ������ RTOS_METHOD_AddTask) ???
; 
; ������� ��������, � ����� �������, �����������, �������� ���������/������������ ����������� ��������� ������.
; � �����, ���� ErrorHandler-����� ���������� �� ���� ����, ��������� ��� ��� ������.
; ����� �����, �����-�� �������, ������������������ �� ������ ������������, �������� � ���, ��� ������ (��-���������)...
; 
; 
; ���������: ������������ ������� � ����� - ������������, ��� ������ ������ �����! 
; ��� ������? ����������� ������� RTOS_METHOD_ErrorHandler, ��� ��������� ����� ��������, �� ����� ������� ����������.
; � �����, � ��� ������� ����� ��������� �������� ���, ��������������� ����������� ��������, � production-�����������...
; 
;---------------------------------------------------------------------------

;----- Subroutine Register Variables

;.def	ErrorCode	= (��.����)	; (�������, �� ����������)			��� ��������� ������
;.def	TaskCode	= (��.����)	; (�������, �� ����������)			�����-������ ������, ��� ������������ � ������� ��������� ������
;.def	ZH:ZL		= (��.����)	; [�������, �� ����������, �� ������������]	����� ������ ����, ��� ������������� "����� ������ ���?"

; ��������: ��� ����� ����������� ����������� ������ ���� "register safe"! �.�. ������ ��������� ��� ������������ ������ "��������� ��������", ��������� ���������� ����� �� ������������ ����������... � ��� ����� � "������������ ��������" - ����� �� ���������, �� ����� ������ �� ���������� ������, �� ������������ ������!

;----- Code

	; ������ ������������ ����� ������ (��������� �������� ErrorCode):
	; 	 0 - ��� ������
	; 
	; 	10 - "������� �����"  �����������, �� ���� �������� ����� ������!
	; 		TaskCode = �����-������ ������, ������� �� ���� ��������
	; 
	; 	20 - "���" ����������, �� ���� ���������������� ����� �������!
	; 		TaskCode = �����-������ ������, ������� �� ���� ��������
	; 		Z        = ����� ������ ����, ��� ������������� � "����� ������ ���?"

RTOS_METHOD_ErrorHandler:
		;RJMP	RESET		; "���� ��� - ���� �����!" (�) ����������� ������� ������� ���� �������, �� ������ ����� ������...
		RET



.endif	//RTOS_OPTIMIZE_FOR_SMALL_MEMORY

.endif	//RTOS_FEATURE_PLANTASK	(������� ��������������� �����, ������ ���������� ��� ������ ���� ������� RTOS)


;=== END "RTOS-kernel.inc" =================================================
; coded by (c) DI HALT, 2008  http://easyelectronics.ru/
; coded by (c) Celeron, 2014  http://inventproject.info/
