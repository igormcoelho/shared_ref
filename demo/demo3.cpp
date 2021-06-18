
#include <iostream>
#include <memory>
//
#include <nnshared/shared.hpp>

// nn::nnptr is still a zero cost abstraction (on Release)
// https://github.com/microsoft/GSL/issues/877

int
foo(int* ptr)
{
   return *ptr;
}

int
bar(nn::nnptr<int*> ptr)
{
   return *ptr;
}

int
main()
{
   // RELEASE
   /*
_Z3fooPi:
.LFB2886:
	.cfi_startproc
	endbr64
	movl	(%rdi), %eax
	ret
	.cfi_endproc

_Z3barN2nn5nnptrIPiEE:
.LFB2887:
	.cfi_startproc
	endbr64
	movl	(%rdi), %eax
	ret
	.cfi_endproc
*/

   // DEBUG
   /*
_Z3fooPi:
.LFB2886:
	.cfi_startproc
	endbr64
	movl	(%rdi), %eax
	ret
	.cfi_endproc

_Z3barN2nn5nnptrIPiEE:
.LFB2887:
	.cfi_startproc
	endbr64
	testq	%rdi, %rdi
	je	.L8
	movl	(%rdi), %eax
	ret
*/
   return 0;
}