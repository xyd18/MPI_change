	.text
	.file	"my_send.cpp"
	.globl	_Z9my_malloci           # -- Begin function _Z9my_malloci
	.p2align	4, 0x90
	.type	_Z9my_malloci,@function
_Z9my_malloci:                          # @_Z9my_malloci
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	callq	_Z9getNewMemi
	movl	%eax, -8(%rbp)
	movslq	-4(%rbp), %rsi
	movq	%rsi, -24(%rbp)         # 8-byte Spill
	callq	_Z5getFdv
	xorl	%ecx, %ecx
	movl	%ecx, %edi
	movslq	-8(%rbp), %r9
	movq	-24(%rbp), %rsi         # 8-byte Reload
	movl	$3, %edx
	movl	$1, %ecx
	movl	%eax, %r8d
	callq	mmap
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rdi
	movl	-8(%rbp), %esi
	movl	-4(%rbp), %edx
	callq	_Z10set_offsetPvii
	movq	-16(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	_Z9my_malloci, .Lfunc_end0-_Z9my_malloci
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function __cxx_global_var_init
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movabsq	$sh, %rdi
	callq	_ZN16sigaction_helperC2Ev
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	__cxx_global_var_init, .Lfunc_end1-__cxx_global_var_init
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN16sigaction_helperC2Ev,"axG",@progbits,_ZN16sigaction_helperC2Ev,comdat
	.weak	_ZN16sigaction_helperC2Ev # -- Begin function _ZN16sigaction_helperC2Ev
	.p2align	4, 0x90
	.type	_ZN16sigaction_helperC2Ev,@function
_ZN16sigaction_helperC2Ev:              # @_ZN16sigaction_helperC2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$176, %rsp
	movq	%rdi, -8(%rbp)
	movl	$4, -24(%rbp)
	leaq	-160(%rbp), %rax
	addq	$8, %rax
	movq	%rax, %rdi
	callq	sigemptyset
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movabsq	$_ZL7handleriP9siginfo_tPv, %rsi
	movq	%rsi, -160(%rbp)
	movl	$11, %edi
	leaq	-160(%rbp), %rsi
	movl	%eax, -164(%rbp)        # 4-byte Spill
	callq	sigaction
	cmpl	$-1, %eax
	jne	.LBB2_2
# %bb.1:
	movabsq	$.L.str.2, %rdi
	movb	$0, %al
	callq	printf
.LBB2_2:
	addq	$176, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	_ZN16sigaction_helperC2Ev, .Lfunc_end2-_ZN16sigaction_helperC2Ev
	.cfi_endproc
                                        # -- End function
	.text
	.globl	_Z7my_sendiiPvi         # -- Begin function _Z7my_sendiiPvi
	.p2align	4, 0x90
	.type	_Z7my_sendiiPvi,@function
_Z7my_sendiiPvi:                        # @_Z7my_sendiiPvi
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movq	%rdx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movl	$12, %edi
	callq	malloc
	movq	%rax, -32(%rbp)
	movq	-16(%rbp), %rdi
	movl	-20(%rbp), %esi
	callq	_Z13manage_dividePvi
	movq	-16(%rbp), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, -36(%rbp)
	cmpl	$-1, -36(%rbp)
	jne	.LBB3_2
# %bb.1:
	movabsq	$.L.str, %rdi
	movb	$0, %al
	callq	printf
	xorl	%edi, %edi
	movl	%eax, -48(%rbp)         # 4-byte Spill
	callq	exit
.LBB3_2:
	movl	-20(%rbp), %eax
	movq	-32(%rbp), %rcx
	movl	%eax, (%rcx)
	movl	-36(%rbp), %eax
	movq	-32(%rbp), %rcx
	movl	%eax, 4(%rcx)
	movq	-32(%rbp), %rcx
	movl	-4(%rbp), %eax
	movl	-8(%rbp), %r8d
	movq	%rcx, %rdi
	movl	$2, %esi
	movl	$1275069445, %edx       # imm = 0x4C000405
	movl	%eax, %ecx
	movl	$1140850688, %r9d       # imm = 0x44000000
	callq	MPI_Send
	movl	%eax, -40(%rbp)
	movq	-16(%rbp), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, -44(%rbp)
	movl	-44(%rbp), %edi
	callq	_Z8mapToMemi
	movq	-32(%rbp), %r10
	movq	%r10, %rdi
	callq	free
	movq	-16(%rbp), %rdi
	movslq	-20(%rbp), %rsi
	movl	$1, %edx
	callq	mprotect
	movl	-40(%rbp), %ecx
	movl	%eax, -52(%rbp)         # 4-byte Spill
	movl	%ecx, %eax
	addq	$64, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end3:
	.size	_Z7my_sendiiPvi, .Lfunc_end3-_Z7my_sendiiPvi
	.cfi_endproc
                                        # -- End function
	.globl	_Z7my_recviiPvi         # -- Begin function _Z7my_recviiPvi
	.p2align	4, 0x90
	.type	_Z7my_recviiPvi,@function
_Z7my_recviiPvi:                        # @_Z7my_recviiPvi
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$112, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movq	%rdx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movl	$12, %edi
	callq	malloc
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movl	-4(%rbp), %ecx
	movl	-8(%rbp), %r8d
	movq	%rax, %rdi
	movl	$2, %esi
	movl	$1275069445, %edx       # imm = 0x4C000405
	movl	$1140850688, %r9d       # imm = 0x44000000
	leaq	-40(%rbp), %rax
	movq	%rax, (%rsp)
	callq	MPI_Recv
	movl	%eax, -52(%rbp)
	movq	-48(%rbp), %rdi
	movl	(%rdi), %eax
	movl	%eax, -56(%rbp)
	movq	-48(%rbp), %rdi
	movl	4(%rdi), %eax
	movl	%eax, -60(%rbp)
	movq	-16(%rbp), %rdi
	movl	-56(%rbp), %esi
	callq	_Z13manage_dividePvi
	movq	-16(%rbp), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, -64(%rbp)
	movl	-64(%rbp), %edi
	callq	_Z7freeMemi
	movq	-16(%rbp), %rdi
	movslq	-56(%rbp), %rsi
	callq	munmap
	movq	-16(%rbp), %rdi
	movslq	-56(%rbp), %rsi
	movl	%eax, -76(%rbp)         # 4-byte Spill
	movq	%rdi, -88(%rbp)         # 8-byte Spill
	movq	%rsi, -96(%rbp)         # 8-byte Spill
	callq	_Z5getFdv
	movslq	-60(%rbp), %r9
	movq	-88(%rbp), %rdi         # 8-byte Reload
	movq	-96(%rbp), %rsi         # 8-byte Reload
	movl	$3, %edx
	movl	$17, %ecx
	movl	%eax, %r8d
	callq	mmap
	movq	%rax, -72(%rbp)
	movq	-16(%rbp), %rdi
	movl	-60(%rbp), %esi
	movl	-56(%rbp), %edx
	callq	_Z10set_offsetPvii
	movq	$-1, %rax
	cmpq	%rax, -72(%rbp)
	jne	.LBB4_2
# %bb.1:
	movabsq	$.L.str.1, %rdi
	movb	$0, %al
	callq	printf
	xorl	%edi, %edi
	movl	%eax, -100(%rbp)        # 4-byte Spill
	callq	exit
.LBB4_2:
	movq	-16(%rbp), %rdi
	movslq	-56(%rbp), %rsi
	movl	$1, %edx
	callq	mprotect
	movq	-48(%rbp), %rcx
	movq	%rcx, %rdi
	movl	%eax, -104(%rbp)        # 4-byte Spill
	callq	free
	movl	-52(%rbp), %eax
	addq	$112, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end4:
	.size	_Z7my_recviiPvi, .Lfunc_end4-_Z7my_recviiPvi
	.cfi_endproc
                                        # -- End function
	.globl	_Z7my_freePv            # -- Begin function _Z7my_freePv
	.p2align	4, 0x90
	.type	_Z7my_freePv,@function
_Z7my_freePv:                           # @_Z7my_freePv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	$0, -12(%rbp)
.LBB5_1:                                # =>This Inner Loop Header: Depth=1
	cmpl	$10, -12(%rbp)
	jge	.LBB5_13
# %bb.2:                                #   in Loop: Header=BB5_1 Depth=1
	movq	-8(%rbp), %rax
	movslq	-12(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	addq	%rcx, %rdx
	cmpq	8(%rdx), %rax
	jne	.LBB5_11
# %bb.3:
	movslq	-12(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movl	20(%rcx), %edx
	movl	%edx, -16(%rbp)
	movl	$0, -20(%rbp)
.LBB5_4:                                # =>This Inner Loop Header: Depth=1
	cmpl	$10, -20(%rbp)
	jge	.LBB5_10
# %bb.5:                                #   in Loop: Header=BB5_4 Depth=1
	movq	-8(%rbp), %rax
	movslq	-20(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	addq	%rcx, %rdx
	cmpq	8(%rdx), %rax
	ja	.LBB5_8
# %bb.6:                                #   in Loop: Header=BB5_4 Depth=1
	movslq	-20(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rax
	movq	-8(%rbp), %rcx
	movslq	-16(%rbp), %rdx
	addq	%rdx, %rcx
	cmpq	%rcx, %rax
	jae	.LBB5_8
# %bb.7:                                #   in Loop: Header=BB5_4 Depth=1
	movslq	-20(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movl	(%rcx), %edi
	callq	_Z7freeMemi
	movslq	-12(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	%rcx, %rdi
	callq	_ZN6Myform5clearEv
.LBB5_8:                                #   in Loop: Header=BB5_4 Depth=1
	jmp	.LBB5_9
.LBB5_9:                                #   in Loop: Header=BB5_4 Depth=1
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -20(%rbp)
	jmp	.LBB5_4
.LBB5_10:
	jmp	.LBB5_14
.LBB5_11:                               #   in Loop: Header=BB5_1 Depth=1
	jmp	.LBB5_12
.LBB5_12:                               #   in Loop: Header=BB5_1 Depth=1
	movl	-12(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -12(%rbp)
	jmp	.LBB5_1
.LBB5_13:
	movq	-8(%rbp), %rdi
	callq	free
.LBB5_14:
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end5:
	.size	_Z7my_freePv, .Lfunc_end5-_Z7my_freePv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN6Myform5clearEv,"axG",@progbits,_ZN6Myform5clearEv,comdat
	.weak	_ZN6Myform5clearEv      # -- Begin function _ZN6Myform5clearEv
	.p2align	4, 0x90
	.type	_ZN6Myform5clearEv,@function
_ZN6Myform5clearEv:                     # @_ZN6Myform5clearEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
	movl	$0, 16(%rax)
	movl	$0, 20(%rax)
	movl	$0, (%rax)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end6:
	.size	_ZN6Myform5clearEv, .Lfunc_end6-_ZN6Myform5clearEv
	.cfi_endproc
                                        # -- End function
	.text
	.p2align	4, 0x90         # -- Begin function _ZL7handleriP9siginfo_tPv
	.type	_ZL7handleriP9siginfo_tPv,@function
_ZL7handleriP9siginfo_tPv:              # @_ZL7handleriP9siginfo_tPv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$112, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rdi
	callq	_Z10get_lengthPv
	movl	%eax, -36(%rbp)
	movl	$0, -40(%rbp)
.LBB7_1:                                # =>This Inner Loop Header: Depth=1
	cmpl	$10, -40(%rbp)
	jge	.LBB7_7
# %bb.2:                                #   in Loop: Header=BB7_1 Depth=1
	movslq	-40(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rax
	cmpq	-32(%rbp), %rax
	jae	.LBB7_5
# %bb.3:                                #   in Loop: Header=BB7_1 Depth=1
	movq	-32(%rbp), %rax
	movslq	-40(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	movq	%rdx, %rsi
	addq	%rcx, %rsi
	movq	8(%rsi), %rcx
	movslq	-40(%rbp), %rsi
	imulq	$24, %rsi, %rsi
	addq	%rsi, %rdx
	movslq	20(%rdx), %rdx
	addq	%rdx, %rcx
	cmpq	%rcx, %rax
	jae	.LBB7_5
# %bb.4:                                #   in Loop: Header=BB7_1 Depth=1
	movslq	-40(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rax
	movq	%rax, -32(%rbp)
.LBB7_5:                                #   in Loop: Header=BB7_1 Depth=1
	jmp	.LBB7_6
.LBB7_6:                                #   in Loop: Header=BB7_1 Depth=1
	movl	-40(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -40(%rbp)
	jmp	.LBB7_1
.LBB7_7:
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, -44(%rbp)
	cmpl	$-1, -44(%rbp)
	jne	.LBB7_9
# %bb.8:
	movabsq	$.L.str.3, %rdi
	movb	$0, %al
	callq	printf
	xorl	%edi, %edi
	movl	%eax, -88(%rbp)         # 4-byte Spill
	callq	exit
.LBB7_9:
	movq	-32(%rbp), %rdi
	callq	_Z10get_lengthPv
	movl	%eax, -48(%rbp)
	movl	-44(%rbp), %edi
	movq	-16(%rbp), %rcx
	movq	16(%rcx), %rsi
	movl	-36(%rbp), %edx
	callq	_Z11setWritableiPvi
	andb	$1, %al
	movb	%al, -49(%rbp)
	testb	$1, -49(%rbp)
	je	.LBB7_11
# %bb.10:
	jmp	.LBB7_28
.LBB7_11:
	movl	$0, -56(%rbp)
.LBB7_12:                               # =>This Inner Loop Header: Depth=1
	cmpl	$10, -56(%rbp)
	jge	.LBB7_18
# %bb.13:                               #   in Loop: Header=BB7_12 Depth=1
	movq	-32(%rbp), %rax
	movslq	-56(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	addq	%rcx, %rdx
	cmpq	8(%rdx), %rax
	ja	.LBB7_16
# %bb.14:                               #   in Loop: Header=BB7_12 Depth=1
	movslq	-56(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rax
	movq	-32(%rbp), %rcx
	movslq	-48(%rbp), %rdx
	addq	%rdx, %rcx
	cmpq	%rcx, %rax
	jae	.LBB7_16
# %bb.15:                               #   in Loop: Header=BB7_12 Depth=1
	movslq	-56(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, %edi
	callq	_Z7freeMemi
.LBB7_16:                               #   in Loop: Header=BB7_12 Depth=1
	jmp	.LBB7_17
.LBB7_17:                               #   in Loop: Header=BB7_12 Depth=1
	movl	-56(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -56(%rbp)
	jmp	.LBB7_12
.LBB7_18:
	movl	-48(%rbp), %edi
	callq	_Z9my_malloci
	movq	%rax, -64(%rbp)
	movq	-64(%rbp), %rdi
	movq	-32(%rbp), %rsi
	movslq	-48(%rbp), %rdx
	callq	memcpy
	movq	-64(%rbp), %rdi
	callq	_Z10get_offsetPv
	movl	%eax, -68(%rbp)
	movl	-68(%rbp), %edi
	movslq	-68(%rbp), %rcx
	movq	-16(%rbp), %rdx
	movq	16(%rdx), %rdx
	movq	-32(%rbp), %rsi
	subq	%rsi, %rdx
	addq	%rdx, %rcx
                                        # kill: def $ecx killed $ecx killed $rcx
	movl	-36(%rbp), %edx
	movl	%ecx, %esi
	callq	_Z13locked_divideiii
	movq	-32(%rbp), %rdi
	movslq	-48(%rbp), %rsi
	movq	%rdi, -96(%rbp)         # 8-byte Spill
	movq	%rsi, -104(%rbp)        # 8-byte Spill
	callq	_Z5getFdv
	movslq	-68(%rbp), %r9
	movq	-96(%rbp), %rdi         # 8-byte Reload
	movq	-104(%rbp), %rsi        # 8-byte Reload
	movl	$3, %edx
	movl	$17, %ecx
	movl	%eax, %r8d
	callq	mmap
	movq	%rax, -80(%rbp)
	movq	$-1, %rax
	cmpq	%rax, -80(%rbp)
	jne	.LBB7_20
# %bb.19:
	movabsq	$.L.str.1, %rdi
	movb	$0, %al
	callq	printf
	xorl	%edi, %edi
	movl	%eax, -108(%rbp)        # 4-byte Spill
	callq	exit
.LBB7_20:
	movl	$0, -84(%rbp)
.LBB7_21:                               # =>This Inner Loop Header: Depth=1
	cmpl	$10, -84(%rbp)
	jge	.LBB7_27
# %bb.22:                               #   in Loop: Header=BB7_21 Depth=1
	movq	-32(%rbp), %rax
	movslq	-84(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	addq	%rcx, %rdx
	cmpq	8(%rdx), %rax
	ja	.LBB7_25
# %bb.23:                               #   in Loop: Header=BB7_21 Depth=1
	movslq	-84(%rbp), %rax
	imulq	$24, %rax, %rax
	movabsq	$myform, %rcx
	addq	%rax, %rcx
	movq	8(%rcx), %rax
	movq	-32(%rbp), %rcx
	movslq	-48(%rbp), %rdx
	addq	%rdx, %rcx
	cmpq	%rcx, %rax
	jae	.LBB7_25
# %bb.24:                               #   in Loop: Header=BB7_21 Depth=1
	movl	-68(%rbp), %eax
	movslq	-84(%rbp), %rcx
	imulq	$24, %rcx, %rcx
	movabsq	$myform, %rdx
	movq	%rdx, %rsi
	addq	%rcx, %rsi
	movq	8(%rsi), %rcx
	movslq	%eax, %rsi
	addq	%rsi, %rcx
	movq	-32(%rbp), %rsi
	subq	%rsi, %rcx
                                        # kill: def $ecx killed $ecx killed $rcx
	movslq	-84(%rbp), %rsi
	imulq	$24, %rsi, %rsi
	addq	%rsi, %rdx
	movl	%ecx, (%rdx)
.LBB7_25:                               #   in Loop: Header=BB7_21 Depth=1
	jmp	.LBB7_26
.LBB7_26:                               #   in Loop: Header=BB7_21 Depth=1
	movl	-84(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -84(%rbp)
	jmp	.LBB7_21
.LBB7_27:
	movq	-64(%rbp), %rdi
	movslq	-48(%rbp), %rsi
	callq	munmap
.LBB7_28:
	addq	$112, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end7:
	.size	_ZL7handleriP9siginfo_tPv, .Lfunc_end7-_ZL7handleriP9siginfo_tPv
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function _GLOBAL__sub_I_my_send.cpp
	.type	_GLOBAL__sub_I_my_send.cpp,@function
_GLOBAL__sub_I_my_send.cpp:             # @_GLOBAL__sub_I_my_send.cpp
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end8:
	.size	_GLOBAL__sub_I_my_send.cpp, .Lfunc_end8-_GLOBAL__sub_I_my_send.cpp
	.cfi_endproc
                                        # -- End function
	.type	sh,@object              # @sh
	.bss
	.globl	sh
sh:
	.zero	1
	.size	sh, 1

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"error\n"
	.size	.L.str, 7

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"map failed! \n"
	.size	.L.str.1, 14

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"sigaction error"
	.size	.L.str.2, 16

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"segmentation fault \n"
	.size	.L.str.3, 21

	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_my_send.cpp
	.ident	"clang version 10.0.0-4ubuntu1 "
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym _Z9my_malloci
	.addrsig_sym _Z9getNewMemi
	.addrsig_sym mmap
	.addrsig_sym _Z5getFdv
	.addrsig_sym _Z10set_offsetPvii
	.addrsig_sym __cxx_global_var_init
	.addrsig_sym malloc
	.addrsig_sym _Z13manage_dividePvi
	.addrsig_sym _Z10get_offsetPv
	.addrsig_sym printf
	.addrsig_sym exit
	.addrsig_sym MPI_Send
	.addrsig_sym _Z8mapToMemi
	.addrsig_sym free
	.addrsig_sym mprotect
	.addrsig_sym MPI_Recv
	.addrsig_sym _Z7freeMemi
	.addrsig_sym munmap
	.addrsig_sym _ZN6Myform5clearEv
	.addrsig_sym sigemptyset
	.addrsig_sym _ZL7handleriP9siginfo_tPv
	.addrsig_sym sigaction
	.addrsig_sym _Z10get_lengthPv
	.addrsig_sym _Z11setWritableiPvi
	.addrsig_sym _Z13locked_divideiii
	.addrsig_sym _GLOBAL__sub_I_my_send.cpp
	.addrsig_sym sh
	.addrsig_sym myform
