	.file	"helloworld_linux.cc"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello World\n"
	.text
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB7415:
	.cfi_startproc
	movl	$1, %eax
	movl	$1, %edi
	movl	$.LC0, %esi
	movl	$12, %edx
#APP
# 74 "../../include/fast_io_core_impl/linux/amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
	cmpq	$-4096, %rax
	jb	.L2
	movl	$60, %eax
	movl	$-1, %edi
#APP
# 43 "../../include/fast_io_core_impl/linux/amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
.L2:
	movl	$60, %eax
	xorl	%edi, %edi
#APP
# 43 "../../include/fast_io_core_impl/linux/amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE7415:
	.size	_start, .-_start
	.ident	"GCC: (cqwrteur) 10.0.1 20200219 (experimental)"
	.section	.note.GNU-stack,"",@progbits
