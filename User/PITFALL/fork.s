.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _fork
.sect .text
_fork:
	push	bp
	mov	bp,sp
	sub	sp,14
mov -12(bp),4
lea ax,-12(bp)
push ax
mov ax,6
push ax
call _invoke
pop bx
pop bx
mov ax,-2(bp)
jmp .cret
