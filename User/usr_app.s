.extern _exit

start:
!	call _show_add
	call _main
	mov ax, #0
	push ax
	call _exit



