	.file	"GCPad_16Mhz.cpp"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.section	.text._ZL10GCPad_sendPhh,"ax",@progbits
	.type	_ZL10GCPad_sendPhh, @function
_ZL10GCPad_sendPhh:
/* prologue: function */
/* frame size = 0 */
	movw r30,r24
	sbi 42-32,5
	ldi r25,lo8(-128)
.L2:
	cbi 43-32,5
	ld r24,Z
	and r24,r25
	mov r18,r25
	ldi r19,lo8(0)
	brne .+2
	rjmp .L3
/* #APP */
 ;  50 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  51 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
	sbi 43-32,5
	movw r24,r18
	asr r25
	ror r24
	mov r25,r24
	tst r24
	brne .L4
	adiw r30,1
	subi r22,lo8(-(-1))
/* #APP */
 ;  61 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  62 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  63 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
	ldi r25,lo8(-128)
	rjmp .L5
.L4:
/* #APP */
 ;  65 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  66 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  67 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  68 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
.L5:
/* #APP */
 ;  71 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  72 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  73 "GCPad_16Mhz.cpp" 1
	nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	rjmp .L6
.L3:
/* #APP */
 ;  75 "GCPad_16Mhz.cpp" 1
	nop
nop
nop

 ;  0 "" 2
 ;  76 "GCPad_16Mhz.cpp" 1
	nop
nop
nop

 ;  0 "" 2
 ;  77 "GCPad_16Mhz.cpp" 1
	nop
nop
nop

 ;  0 "" 2
 ;  78 "GCPad_16Mhz.cpp" 1
	nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	movw r24,r18
	asr r25
	ror r24
	mov r25,r24
	tst r24
	brne .L7
	adiw r30,1
	subi r22,lo8(-(-1))
/* #APP */
 ;  87 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  88 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  89 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
	ldi r25,lo8(-128)
	rjmp .L8
.L7:
/* #APP */
 ;  91 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  92 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  93 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  94 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
.L8:
	sbi 43-32,5
/* #APP */
 ;  99 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
 ;  100 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
 ;  101 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
 ;  102 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
 ;  103 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
.L6:
	tst r22
	breq .+2
	rjmp .L2
/* #APP */
 ;  108 "GCPad_16Mhz.cpp" 1
	nop

 ;  0 "" 2
 ;  109 "GCPad_16Mhz.cpp" 1
	nop

 ;  0 "" 2
/* #NOAPP */
	cbi 43-32,5
/* #APP */
 ;  114 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  115 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  116 "GCPad_16Mhz.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
	sbi 43-32,5
/* epilogue start */
	ret
	.size	_ZL10GCPad_sendPhh, .-_ZL10GCPad_sendPhh
	.section	.text._Z10GCPad_readv,"ax",@progbits
.global	_Z10GCPad_readv
	.type	_Z10GCPad_readv, @function
_Z10GCPad_readv:
	push r14
	push r15
	push r16
	push r17
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
	movw r26,r28
	adiw r26,1
	ldi r30,lo8(_ZZ10GCPad_readvE3C.8)
	ldi r31,hi8(_ZZ10GCPad_readvE3C.8)
	ldi r24,lo8(3)
.L12:
	ld r0,Z+
	st X+,r0
	subi r24,lo8(-(-1))
	brne .L12
/* #APP */
 ;  206 "GCPad_16Mhz.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(3)
	call _ZL10GCPad_sendPhh
	cbi 42-32,5
	sbi 43-32,5
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L29:
	sbic 41-32,5
	rjmp .L29
/* #APP */
 ;  148 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  153 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	in r24,41-32
	andi r24,lo8(32)
	st Z,r24
	ldi r24,hi8(raw_joy_data+63)
	cpi r30,lo8(raw_joy_data+63)
	cpc r31,r24
	breq .L14
.L24:
	sbis 41-32,5
	rjmp .L24
	adiw r30,1
	rjmp .L29
.L14:
/* #APP */
 ;  211 "GCPad_16Mhz.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r16,lo8(0)
	ldi r17,hi8(0)
	ldi r22,lo8(1)
	mov r14,r22
	mov r15,__zero_reg__
	rjmp .L16
.L20:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L17
	movw r24,r14
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L30
.L17:
	movw r24,r14
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L30:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L19
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L19:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L20
	subi r16,lo8(-(8))
	sbci r17,hi8(-(8))
	adiw r30,1
	cpi r16,64
	cpc r17,__zero_reg__
	breq .L21
.L16:
	movw r26,r16
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L20
.L21:
	ldi r24,lo8(gc_joy_data)
	ldi r25,hi8(gc_joy_data)
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	_Z10GCPad_readv, .-_Z10GCPad_readv
	.section	.text._Z11N64Pad_readv,"ax",@progbits
.global	_Z11N64Pad_readv
	.type	_Z11N64Pad_readv, @function
_Z11N64Pad_readv:
	push r14
	push r15
	push r16
	push r17
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r24,lo8(1)
	std Y+1,r24
/* #APP */
 ;  239 "GCPad_16Mhz.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	cbi 42-32,5
	sbi 43-32,5
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L47:
	sbic 41-32,5
	rjmp .L47
/* #APP */
 ;  148 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

 ;  0 "" 2
 ;  153 "GCPad_16Mhz.cpp" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	in r24,41-32
	andi r24,lo8(32)
	st Z,r24
	ldi r24,hi8(raw_joy_data+31)
	cpi r30,lo8(raw_joy_data+31)
	cpc r31,r24
	breq .L33
.L43:
	sbis 41-32,5
	rjmp .L43
	adiw r30,1
	rjmp .L47
.L33:
/* #APP */
 ;  244 "GCPad_16Mhz.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(n64_joy_data)
	ldi r31,hi8(n64_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r16,lo8(0)
	ldi r17,hi8(0)
	ldi r23,lo8(1)
	mov r14,r23
	mov r15,__zero_reg__
	rjmp .L35
.L39:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L36
	movw r24,r14
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L48
.L36:
	movw r24,r14
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L48:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L38
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L38:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L39
	subi r16,lo8(-(8))
	sbci r17,hi8(-(8))
	adiw r30,1
	cpi r16,32
	cpc r17,__zero_reg__
	breq .L40
.L35:
	movw r26,r16
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L39
.L40:
	ldi r24,lo8(n64_joy_data)
	ldi r25,hi8(n64_joy_data)
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	_Z11N64Pad_readv, .-_Z11N64Pad_readv
	.section	.text._Z10GCPad_initv,"ax",@progbits
.global	_Z10GCPad_initv
	.type	_Z10GCPad_initv, @function
_Z10GCPad_initv:
	push r17
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L50:
	st Z+,__zero_reg__
	ldi r24,hi8(raw_joy_data+64)
	cpi r30,lo8(raw_joy_data+64)
	cpc r31,r24
	brne .L50
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
.L51:
	st Z+,__zero_reg__
	ldi r24,hi8(gc_joy_data+8)
	cpi r30,lo8(gc_joy_data+8)
	cpc r31,r24
	brne .L51
	sts n64_joy_data,__zero_reg__
	sts n64_joy_data+1,__zero_reg__
	sts n64_joy_data+2,__zero_reg__
	sts n64_joy_data+3,__zero_reg__
/* #APP */
 ;  184 "GCPad_16Mhz.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	std Y+1,__zero_reg__
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	cbi 42-32,5
	sbi 43-32,5
	ldi r17,lo8(64)
.L53:
	sbis 41-32,5
	rjmp .L52
	subi r17,lo8(-(-1))
	brne .L53
.L52:
/* #APP */
 ;  193 "GCPad_16Mhz.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(500)
	ldi r25,hi8(500)
	call delayMicroseconds
	mov r24,r17
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	ret
	.size	_Z10GCPad_initv, .-_Z10GCPad_initv
.global	raw_joy_data
	.section	.bss.raw_joy_data,"aw",@nobits
	.type	raw_joy_data, @object
	.size	raw_joy_data, 64
raw_joy_data:
	.skip 64,0
.global	gc_joy_data
	.section	.bss.gc_joy_data,"aw",@nobits
	.type	gc_joy_data, @object
	.size	gc_joy_data, 8
gc_joy_data:
	.skip 8,0
.global	n64_joy_data
	.section	.bss.n64_joy_data,"aw",@nobits
	.type	n64_joy_data, @object
	.size	n64_joy_data, 4
n64_joy_data:
	.skip 4,0
	.section	.rodata._ZZ10GCPad_readvE3C.8,"a",@progbits
	.type	_ZZ10GCPad_readvE3C.8, @object
	.size	_ZZ10GCPad_readvE3C.8, 3
_ZZ10GCPad_readvE3C.8:
	.byte	64
	.byte	3
	.byte	0
