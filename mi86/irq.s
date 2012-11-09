.extern _irq0
.extern save_regs
.extern restore_regs
.extern push_args

.extern _maintain
.extern _scheduler
.extern _printk
.extern _printc
.extern _queue_visit
.extern _trap_handler
.extern _display_stack
.extern _current_pcb

.define _cli
.define _sti
.define _idle_cpu
.define _timer_handle
.define _shif_proc
.define _trap_int
.define _pick_proc

_cli:
	cli
	ret
_sti:
	sti
	ret
_idle_cpu:
	hlt
	ret

_timer_handle:
	call save_regs
	call _irq0
	call restore_regs

_shif_proc:
	call _maintain
	call _scheduler
	call restore_regs

_pick_proc:
	call _scheduler
	call restore_regs

sen:
	.asciz "stack %x %x %x %x %x\n"

_trap_int:
	call save_regs
	push 6(si)
	push 4(si)
	push 2(si)
	push (si)
	call _trap_handler
	call restore_regs

int80:
	.asciz "back from int 0x80\n"

.define _keyboard_int
.extern _irq1
_keyboard_int:
	call save_regs
	call _irq1
	call restore_regs
