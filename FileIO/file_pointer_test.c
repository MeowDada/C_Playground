#include <unistd.h>
#include "common.h"

#if __linux__
#include <sys/types.h>
#include <sys/stat.h>
#endif

static off_t get_file_size(const char *fname)
{
#if __linux__
    struct stat st;

    lstat(fname, &st);
    return st.st_size;
#endif
}

static void print_file_info(const char *fname, off_t fsize)
{
    LOGGING_INFO("File name = %20s", fname);
    LOGGING_INFO("File size = %10ld", fsize);
}

static void print_all_info(FILE *fp)
{
#if __linux__
    LOGGING_INFO("===================[START]===================")
    LOGGING_INFO("_IO_read_ptr    = %10p", fp->_IO_read_ptr);
    LOGGING_INFO("_IO_read_end    = %10p", fp->_IO_read_end);
    LOGGING_INFO("_IO_read_base   = %10p", fp->_IO_read_base);
    LOGGING_INFO("_IO_write_ptr   = %10p", fp->_IO_write_ptr);
    LOGGING_INFO("_IO_write_base  = %10p", fp->_IO_write_base);
    LOGGING_INFO("_IO_write_end   = %10p", fp->_IO_write_end);
    LOGGING_INFO("_IO_buf_base    = %10p", fp->_IO_buf_base);
    LOGGING_INFO("_IO_buf_end     = %10p", fp->_IO_buf_end);
    LOGGING_INFO("_IO_save_base   = %10p", fp->_IO_save_base);
    LOGGING_INFO("_IO_backup_base = %10p", fp->_IO_backup_base);
    LOGGING_INFO("_IO_save_end    = %10p", fp->_IO_save_end);
    LOGGING_INFO("====================[END]====================")
#endif
}

int main(int argc, char **argv)
{
    const char fname = "lorem.txt";

    setup_logger(NULL, LOG_LEVEL_INFO);

    FILE *fp = fopen(fname, "r");
    if (!fp) {
        LOGGING_ERROR("Failed to find lorem.txt in this dir...");
        goto end;
    }

    int retval = 0;
    int steps  = 0;

    off_t fsize = get_file_size(fname);

    print_file_info(fname, fsize);

    LOGGING_INFO("[%3d] fseek(fp, 0, SEEK_SET);", steps++);
    retval = fseek(fp, 0, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]fseek(fp, 8, SEEK_SET);", steps++);
    retval = fseek(fp, 8, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, -8, SEEK_SET);", steps++);
    retval = fseek(fp, -8, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, 0, SEEK_END);", steps++);
    retval = fseek(fp, 0, SEEK_END);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, -8, SEEK_END);", steps++);
    retval = fseek(fp, -8, SEEK_END);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, 4, SEEK_SET);", steps++);
    retval = fseek(fp, 4, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, 4, SEEK_CUR);", steps++);
    retval = fseek(fp, 4, SEEK_CUR);
    print_all_info(fp);
    

end:
    close_logger(NULL);

    return 0;
}