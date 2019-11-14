	.file	"include_dbl_table.cc"
	.text
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB491:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE491:
	.size	main, .-main
	.ident	"GCC: (cqwrteur) 10.0.0 20191113 (experimental)"
	.section	.note.GNU-stack,"",@progbits
