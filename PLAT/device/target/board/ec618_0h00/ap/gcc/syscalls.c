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
    extern char _heap_memory_start, _heap_memory_end; /* Defined by the linker */

    static char *heap_end;
    char        *prev_heap_end;

    if (heap_end == NULL) {
        heap_end = (char *)&_heap_memory_start;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr >= (char *)&_heap_memory_end) {

        return (caddr_t) - 1;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
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





