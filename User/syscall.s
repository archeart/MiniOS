.define _yield
.define _create_proc
.define _receive
.define _invoke
.define _send


_yield:
	push bp
	push ds
	mov ax, #0x8000
	mov ds, ax

	mov ax, #1
	mov bx, #0
	mov cx, #0
	mov dx, #0
	int 0x80

	pop ds
	pop bp
	ret

_create_proc:   ! bx: seg; cs: ip; dx: sp; ah: pri 
	push ax
	add sp, #2

	push bp
	mov bp, sp

	push ds
	mov ax, #0x8000
	mov ds, ax


    movb al, #2
	mov bx, 4(bp)
	mov cx, 6(bp)
	mov dx, 8(bp)
	movb ah, 10(bp) 
	
	int 0x80

	pop ds
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


.define _cli
_cli:
	cli
	ret

.define _sti
_sti:
	sti
	ret
