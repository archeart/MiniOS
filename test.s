.extern _printk
.extern _prints
.define _proc_shifter
_proc_shifter:
	mov ax, #hint2
	push ax
	call _prints
	pop ax
	int 0x08
	mov ax, #hint3
	push ax
	call _prints
	pop ax
	ret


hint1:
	.asciz "entered\n\r"
hint2:
	.asciz "killed\n\r"
hint3:
	.asciz "back\n\r"
