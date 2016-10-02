	.text
	.code64
	.global addrs
	.global int_handler

isr_wrapper:
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	movq 72(%rsp), %rdi
	
	cld

	call int_handler
	
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rax
	
	addq $16, %rsp
	
	iretq

#define make_handler(n) \
int_handler ## n: \
	pushq $0; \
	pushq $n; \
	jmp isr_wrapper

#define make_error_handler(n) \
int_handler ## n: \
	pushq $n; \
	jmp isr_wrapper

make_handler(0)
make_handler(1)
make_handler(2)
make_handler(3)
make_handler(4)
make_handler(5)
make_handler(6)
make_handler(7)
make_error_handler(8)
make_handler(9)
make_error_handler(10)
make_error_handler(11)
make_error_handler(12)
make_error_handler(13)
make_error_handler(14)
make_handler(15)
make_handler(16)
make_error_handler(17)
make_handler(18)
make_handler(19)
make_handler(20)
make_handler(21)
make_handler(22)
make_handler(23)
make_handler(24)
make_handler(25)
make_handler(26)
make_handler(27)
make_handler(28)
make_handler(29)
make_handler(30)
make_handler(31)
make_handler(32)
make_handler(33)
make_handler(34)
make_handler(35)
make_handler(36)
make_handler(37)
make_handler(38)
make_handler(39)
make_handler(40)
make_handler(41)
make_handler(42)
make_handler(43)
make_handler(44)
make_handler(45)
make_handler(46)
make_handler(47)

addrs:
	.quad int_handler0
	.quad int_handler1
	.quad int_handler2
	.quad int_handler3
	.quad int_handler4
	.quad int_handler5
	.quad int_handler6
	.quad int_handler7
	.quad int_handler8
	.quad int_handler9
	.quad int_handler10
	.quad int_handler11
	.quad int_handler12
	.quad int_handler13
	.quad int_handler14
	.quad int_handler15
	.quad int_handler16
	.quad int_handler17
	.quad int_handler18
	.quad int_handler19
	.quad int_handler20
	.quad int_handler21
	.quad int_handler22
	.quad int_handler23
	.quad int_handler24
	.quad int_handler25
	.quad int_handler26
	.quad int_handler27
	.quad int_handler28
	.quad int_handler29
	.quad int_handler30
	.quad int_handler31
	.quad int_handler32
	.quad int_handler33
	.quad int_handler34
	.quad int_handler35
	.quad int_handler36
	.quad int_handler37
	.quad int_handler38
	.quad int_handler39
	.quad int_handler40
	.quad int_handler41
	.quad int_handler42
	.quad int_handler43
	.quad int_handler44
	.quad int_handler45
	.quad int_handler46
	.quad int_handler47
