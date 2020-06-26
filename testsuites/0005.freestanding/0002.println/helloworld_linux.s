	.file	"helloworld_linux.cc"
	.text
	.p2align 4
	.weak	_ZNSt7__n486122__dummy_resume_destroyEv
	.type	_ZNSt7__n486122__dummy_resume_destroyEv, @function
_ZNSt7__n486122__dummy_resume_destroyEv:
.LFB13480:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE13480:
	.size	_ZNSt7__n486122__dummy_resume_destroyEv, .-_ZNSt7__n486122__dummy_resume_destroyEv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello World\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB13629:
	.cfi_startproc
	movl	$1, %eax
	movl	$1, %edi
	movl	$.LC0, %esi
	movl	$12, %edx
#APP
# 74 "../../../include/fast_io_hosted/platforms/linux/amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
	cmpq	$-4096, %rax
	jnb	.L5
	movl	$60, %eax
	xorl	%edi, %edi
#APP
# 43 "../../../include/fast_io_hosted/platforms/linux/amd64.h" 1
	syscall
# 0 "" 2
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.type	_start.cold, @function
_start.cold:
.LFSB13629:
#NO_APP
.L5:
	ud2
	.cfi_endproc
.LFE13629:
	.text
	.size	_start, .-_start
	.section	.text.unlikely
	.size	_start.cold, .-_start.cold
.LCOLDE1:
	.text
.LHOTE1:
	.weak	_ZNSt7__n486114__noop_coro_frE
	.data
	.align 16
	.type	_ZNSt7__n486114__noop_coro_frE, @object
	.size	_ZNSt7__n486114__noop_coro_frE, 24
_ZNSt7__n486114__noop_coro_frE:
	.quad	_ZNSt7__n486122__dummy_resume_destroyEv
	.quad	_ZNSt7__n486122__dummy_resume_destroyEv
	.zero	8
	.ident	"GCC: (cqwrteur) 11.0.0 20200605 (experimental)"
	.section	.note.GNU-stack,"",@progbits
