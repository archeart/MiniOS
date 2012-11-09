.extern _printd
.extern _prints
.extern _printk
.extern _printx


.define _ret_add
_ret_add:
	push bp
	mov bp, sp

	push 4(bp)
	call _printd
	add sp, #2
	pop bp
	ret

.define _call_add
_call_add:
	push bp
	mov bp, sp
	push 'a'
	call _printc
	push 4(bp)
	call _printx
	push 'a'
	call _printc
	push 6(bp)
	call _printx
	push 'a'
	call _printc
	push 8(bp)
	call _printx
	add sp, #12
	pop bp
	ret

.define _trap_showAdd
_trap_showAdd:
	push bp
	mov bp, sp
	push ax
	mov ax, 8(bp)
	push ax
	mov ax, 4(bp)
	push ax
	push #hint 
	call _printk
	add sp, #6
	pop ax
	pop bp
	iret

.define _showCurAdd
_showCurAdd:
	int 0x0E
	ret

.define _display_stack
_display_stack:
	push #sen
	call _printk
	add sp, #2
	ret

.define _get_ss
_get_ss:
	push ax
	mov ax, ss
	push ax
	call _printk
	pop ax
	pop ax
	ret
	
hint:
	.asciz "the current address is [%x:%x]. \n\r"
sen:
	.asciz "%x %x %x %x %x\n"
