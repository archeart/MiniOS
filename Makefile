kernel: start.s main.c printk.o panic.o proc.o \
		tests.o util.o timer.o keyboard.o sched.o clib.o \
		queue.o procom.o trap.o tty.o floppy.o kerproc.o
		
		cd mi86 && exec make
		cc -Was-ncc -mi86 -.o -com -s \
			start.s \
			main.c \
			printk.o \
			panic.o \
			proc.o \
			tests.o \
			util.o \
			timer.o \
			keyboard.o \
			sched.o \
			clib.o \
			queue.o \
			procom.o \
			trap.o \
			tty.o \
			floppy.o \
			kerproc.o \
			mi86/io.o \
			mi86/irq.o \
			mi86/reg.o \
			mi86/mem.o \
			mi86/debug.o \
			mi86/syscall.o \
			mi86/lib.o \
			-o kernel

printk.o: printk.c
	cc -Was-ncc -mi86 -c printk.c
panic.o: panic.c
	cc -Was-ncc -mi86 -c panic.c
proc.o: proc.c
	cc -Was-ncc -mi86 -c proc.c
tests.o: tests.c
	cc -Was-ncc -mi86 -c tests.c
util.o: util.c
	cc -Was-ncc -mi86 -c util.c
timer.o: timer.c
	cc -Was-ncc -mi86 -c timer.c
keyboard.o: keyboard.c
	cc -Was-ncc -mi86 -c keyboard.c
sched.o: sched.c
	cc -Was-ncc -mi86 -c sched.c
clib.o: clib.c
	cc -Was-ncc -mi86 -c clib.c
queue.o: queue.c
	cc -Was-ncc -mi86 -c queue.c
procom.o: procom.c
	cc -Was-ncc -mi86 -c procom.c
trap.o: trap.c
	cc -Was-ncc -mi86 -c trap.c
tty.o: tty.c
	cc -Was-ncc -mi86 -c tty.c
floppy.o: floppy.c
	cc -Was-ncc -mi86 -c floppy.c
kerproc.o: kerproc.c
	cc -Was-ncc -mi86 -c kerproc.c

install:
	exec ./writeos
