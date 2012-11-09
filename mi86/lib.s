.define _read_disk
_read_disk:
	push bp
	mov bp, sp
	mov cx, 4(bp)
	mov dx, 6(bp)
	mov ax, 8(bp)
	mov es, ax
	mov bx, 10(bp)
	mov ax, #0x0201
	int 0x13
	pop bp
	ret

.define _write_disk
_write_disk:
	push bp
	mov bp, sp
	mov cx, 4(bp)
	mov dx, 6(bp)
	mov ax, 8(bp)
	mov es, ax
	mov bx, 10(bp)
	mov ax, #0x0301
	int 0x13
	pop bp
	ret


