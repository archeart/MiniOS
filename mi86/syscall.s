.define _exit
.define _yield
.define _create_proc
.define _receive
.define _invoke
.define _send

.extern _printk
.extern _printx

_exit:
	push bp
	push ds
	mov ax, #0x8000
	mov ds, ax

	mov ax, #0

	int 0x80
	pop bp
	ret

_yield:
	push bp
	mov ax, #1
	mov bx, #0
	mov cx, #0
	mov dx, #0

	int 0x80

	pop bp
	ret

_create_proc:   ! bx: seg; cs: ip; dx: sp; ah: pri 
	movb al, #2
	push bp
	mov bp, sp
	mov bx, 4(bp)
	mov cx, 6(bp)
	mov dx, 8(bp)
	movb ah, 10(bp)
	int 0x80
	pop bp
	ret

_receive:
	movb al, #3
	push bp
	mov bp, sp
	mov bx, 4(bp)
	int 0x80
	pop bp
	ret

_invoke:
	movb al, #4
	push bp
	mov bp, sp
	mov cx, 4(bp)
	mov bx, 6(bp)
	int 0x80
	pop bp
	ret

_send:
	movb al, #5
	push bp
	mov bp, sp
	mov cx, 4(bp)
	mov bx, 6(bp)
	int 0x80
	pop bp
	ret
