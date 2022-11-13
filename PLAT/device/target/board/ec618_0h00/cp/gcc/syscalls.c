#include <sys/stat.h>
extern int io_putc(int ch) __attribute__((weak));
extern int io_getc(void) __attribute__((weak));

int _close(int file)
{
    return 0;
}

int _fstat(int file, struct stat *st)
{
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _open(const char *name, int flags, int mode)
{
    return -1;
}

int _read(int file, char *ptr, int len)
{
    return 0;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        *ptr++ = io_getc();
    }

    return len;
}


#define __MYPID 1
int _getpid()
{
    return __MYPID;
}

/*
 * * kill -- go out via exit...
 * */
int _kill(int pid, int sig)
{
    return -1;
}

void _exit(int val)
{
    while(1);
}

caddr_t _sbrk(int incr)
{

    return (caddr_t) - 1;

}

int _write(int file, char *ptr, int len)
{
	extern int io_putchar(int ch);
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++) {
		io_putchar(*ptr++);
    }
    return len;
}





