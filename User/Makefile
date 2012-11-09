all: fileserver memserver hello shell proc2 echo cat

fileserver: start.s fileserver.c util.c clib.c \
			syscall.o lib.o io.o mem.o
	cc -Was-ncc -mi86 -com -.o -s \
		start.s \
		fileserver.c \
		util.c \
		clib.c \
		syscall.o \
		lib.o \
		io.o \
		mem.o \
		-o fileserver

memserver: start.s memserver.c util.c clib.c \
		   syscall.o lib.o io.o mem.o 
	cc -Was-ncc -com -mi86 -.o -s \
		start.s \
		memserver.c \
		util.c \
		clib.c \
		syscall.o \
		lib.o \
		io.o \
		mem.o \
		-o memserver

hello: usr_app.s hello.c io.o clib.c
	cc -Was-ncc -com -mi86 -.o -s \
		usr_app.s \
		hello.c \
		clib.c \
		io.o \
		syscall.o \
		PITFALL/fork.o \
		PITFALL/exec.o \
		PITFALL/exit.o \
		PITFALL/wait_pid.o \
		PITFALL/sleep.o \
		PITFALL/open.o \
		PITFALL/write.o \
		PITFALL/read.o \
		PITFALL/lseek.o \
		PITFALL/close.o \
		-o hello


shell: usr_app.s shell.c io.o clib.c
	cc -Was-ncc -com -mi86 -.o -s \
		usr_app.s \
		shell.c \
		clib.c \
		io.o \
		syscall.o \
		PITFALL/fork.o \
		PITFALL/exec.o \
		PITFALL/exit.o \
		PITFALL/wait_pid.o \
		PITFALL/sleep.o \
		PITFALL/open.o \
		PITFALL/write.o \
		PITFALL/read.o \
		PITFALL/lseek.o \
		PITFALL/close.o \
		PITFALL/lsdir.o \
		PITFALL/chdir.o \
		PITFALL/mkdir.o \
		PITFALL/rmdir.o \
		PITFALL/dup2.o \
		-o shell 

proc2: usr_app.s proc2.c clib.c io.o
	cc -Was-ncc -com -mi86 -.o -s \
		usr_app.s \
		proc2.c \
		clib.c \
		io.o \
		syscall.o \
		PITFALL/exit.o \
		-o proc2

echo: usr_app.s echo.c 
	cc -Was-ncc -com -mi86 -.o -s \
		usr_app.s \
		echo.c \
		syscall.o \
		PITFALL/exit.o \
		PITFALL/write.o \
		-o echo

cat: usr_app.s cat.c
	cc -Was-ncc -com -mi86 -.o -s \
		usr_app.s \
		cat.c \
		syscall.o \
		PITFALL/exit.o \
		PITFALL/write.o \
		PITFALL/read.o \
		PITFALL/open.o \
		PITFALL/close.o \
		-o cat

lib.o: lib.s
	cc -Was-ncc -mi86 -c lib.s
syscall.o: syscall.s
	cc -Was-ncc -mi86 -c syscall.s
io.o: io.s
	cc -Was-ncc -mi86 -c io.s
mem.o: mem.s
	cc -Was-ncc -mi86 -c mem.s 



		
