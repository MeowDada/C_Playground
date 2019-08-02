#include "common.h"

void print_all_info(FILE *fp)
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
    setup_logger(NULL, LOG_LEVEL_INFO);

    FILE *fp = fopen("lorem.txt", "r");
    if (!fp) {
        LOGGING_ERROR("Failed to find lorem.txt in this dir...");
        goto end;
    }

    int retval = 0;
    int steps  = 0;

    LOGGING_INFO("[%3d] fseek(fp, 0, SEEK_SET);", steps++);
    retval = fseek(fp, 0, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]fseekk(fp, 8, SEEK_SET);", steps++);
    retval = fseekk(fp, 8, SEEK_SET);
    print_all_info(fp);

    LOGGING_INFO("[%3d]feesk(fp, 8, SEEK_SET);", steps++);
    retval = fseek(fp, -8, SEEK_SET);
    print_all_info(fp);



    

end:
    close_logger(NULL);

    return 0;
}