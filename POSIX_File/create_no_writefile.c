#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    const char *content = "test message!\n";

    fd = open(argv[1], O_CREAT | O_WRONLY, 0000);
    write(fd, content, strlen(content)+1);
    close(fd);

    /* open flag option:
     *
     * O_RDONLY     : read only 
     * O_WRONLY     : write only
     * O_RDWR       : open with allowing read and write
     * O_CREAT      : if the opened file does not exist, then create a new one
     * O_EXCL       : if O_CREAT is set, then will check the existence of the file, if it does not exist, then creat a new one.
     *                otherwise will return false
     * O_NOCTTY     : if the file we want to open is termainal device, then will not treat it as a process's terminal device
     * O_TRUNC      : if the file exist and opened it with writable flags, will reset the size of the file to 0, the 
     *                original data will lose
     * O_APPEND     : open the file with pointer at the tail of file, able to append data to it
     * O_NONBLOCK   : open the file with non-blocking method
     * O_NDELAY     : same as O_NONBLOCK
     * O_SYNC       : open the file with synchronous state
     * O_LARGEFILE  : support 32Bits OS to open the files larger than 2GB
     */

    return 0;
}

