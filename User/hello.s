.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _main
.sect .text
_main:
	push	bp
	mov	bp,sp
	push	ax
mov -2(bp),0
I1_6:
cmp -2(bp),2
jge I1_3
lea ax,-2(bp)
push ax
mov ax,I_1
push ax
call _printk
pop bx
pop bx
call _fork
inc -2(bp)
jmp I1_6
I1_3:
xor ax,ax
push ax
call _exit
pop bx
jmp .cret
.sect .rom
I_1:
.data2	25637
.data2	0
.sect .text
