
#define TTY 1
#define TIMER 2
#define FLOPPY 3
#define MM 4
#define FM 5
#define PM 6

#define MM_SEGMENT 0x7000
#define FM_SEGMENT 0x7400
#define PM_SEGMENT 0x8000

#define LOAD_PROCESS 1
#define DONE 2
#define FAILURE 3
#define FORK 4
#define EXEC 5
#define EXIT 6
#define FORK_DONE 7
#define FORK_FAILURE 8
#define LOADPROC_DONE 9
#define WAIT_PID 10
#define SLEEP 11

#define DEV_READ 16 
#define DEV_WRITE 17
#define DEV_DONE 18
#define HARDWARE_INT 19

#define RELEASE_MEMORY 33
#define LOAD_BUF 34
#define PUSH_BUF 35

#define OPEN 40
#define CLOSE 41
#define READ 42
#define WRITE 43
#define LSEEK 44
#define DUP 45
#define MKDIR 46
#define RMDIR 47
#define CHDIR 48
#define LSDIR 49
#define DUP2 50



#define DAEMON 7
/* idle tty floppy timer kerproc memserver fileserver*/
