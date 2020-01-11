	.file	"linux_freestanding_compile_test.cc"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello World\n"
	.text
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB6512:
	.cfi_startproc
	movl	$1, %eax
	movl	$1, %edi
	movl	$.LC0, %esi
	movl	$12, %edx
#APP
# 14 "../../include/fast_io_core_impl/asm/asm_amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
	cmpq	$-1, %rax
	jne	.L2
	movl	$60, %eax
	movl	$-1, %edi
#APP
# 29 "../../include/fast_io_core_impl/asm/asm_amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
.L2:
	movl	$60, %eax
	xorl	%edi, %edi
#APP
# 29 "../../include/fast_io_core_impl/asm/asm_amd64.h" 1
	syscall
# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE6512:
	.size	_start, .-_start
	.ident	"GCC: (GNU) 10.0.0 20191231 (experimental)"
	.section	.note.GNU-stack,"",@progbits
