;**********************************************************
;  Microchip Enhanced PIC16F1xxx C Compiler (CC16E), v0.10.01
;  Fri Jan 23 09:41:31 2026
;**********************************************************

	.device	"pic16e"


	.segment	CODE2 (REL) foo:16
foo_$_f:	.equ	foo_$data$+0
foo_$_n:	.equ	foo_$data$+4
foo_$_m:	.equ	foo_$data$+6
foo_$1_ff:	.equ	foo_$data$+8
foo_$1:	.equ	foo_$data$+12
foo::
	movlw	foo_$data$+8
	movwf	FSR0L
	movlw	(foo_$data$+8)>>8
	movwf	FSR0H
	movlw	8
	.psel	foo, _copyPar
	call	_copyPar
; :: q_test.c #4: ff = n? f: m;
	.bsel	foo_$data$
	movf	foo_$_n, W
	iorwf	foo_$_n+1, W
	.psel	_copyPar, _$L3
	btfsc	3, 2
	goto	_$L3
	movf	foo_$_f, W
	movwf	foo_$1
	movf	foo_$_f+1, W
	movwf	foo_$1+1
	movf	foo_$_f+2, W
	movwf	foo_$1+2
	movf	foo_$_f+3, W
	movwf	foo_$1+3
	.psel	_$L3, _$L4
	goto	_$L4
	.psel	_$L4, _$L3
_$L3:
	.bsel	foo_$data$
	movf	foo_$_m, W
	movwf	112
	movf	foo_$_m+1, W
	movwf	113
	movlw	0
	btfsc	113, 7
	movlw	255
	movwf	114
	movwf	115
	.psel	_$L3, _int4ToFloatACC
	call	_int4ToFloatACC
	movf	112, W
	movwf	foo_$1
	movf	113, W
	movwf	foo_$1+1
	movf	114, W
	movwf	foo_$1+2
	movf	115, W
	movwf	foo_$1+3
	.psel	_int4ToFloatACC, _$L4
_$L4:
	.bsel	foo_$data$
	movf	foo_$1, W
	movwf	foo_$1_ff
	movf	foo_$1+1, W
	movwf	foo_$1_ff+1
	movf	foo_$1+2, W
	movwf	foo_$1_ff+2
	movf	foo_$1+3, W
	movwf	foo_$1_ff+3
	.psel	_$L4, foo
	return
; function(s) called::
	.fcall	foo, _int4ToFloatACC
	.fcall	foo, _copyPar

	.end
