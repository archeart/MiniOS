.extern _printk
.extern _printc
.extern _printx
.define _show_add

start:
!	call _show_add
	call _main


_show_add:
	push bp
	mov bp, sp

	mov ax, 2(bp)
	push ax
	mov bx, cs
	push bx
	push #sen
	
	call _printk
	add sp, #6
	pop bp
	ret

sen:
	.asciz "cur address: [%x : %x].\n"


