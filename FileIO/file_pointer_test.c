#include <unistd.h>
#include "common.h"

#if __linux__
#include <sys/types.h>
#include <sys/stat.h>
#endif

static void print_buffer_content(char *buffer, int end)
{
    if (end < 0)
        end = 0;
    buffer[end] = '\0';
    LOGGING_INFO("buffer_content = %s", buffer);
}

static off_t get_file_size(const char *fname)
{
#if __linux__
    struct stat st;

    lstat(fname, &st);
    return st.st_size;
#endif
}

static off_t get_buffer_size(FILE *fp)
{
#ifdef __linux__
    LOGGING_INFO("Buffer size = %lu", fp->_IO_buf_end - fp->_IO_buf_base);
    off_t diff = fp->_IO_buf_end - fp->_IO_buf_base;
    diff = diff > 0 ? diff : -diff;
    return diff;
#endif
    return 0;
}

static void print_file_info(const char *fname, off_t fsize)
{
    LOGGING_INFO("File name = %-20s", fname);
    LOGGING_INFO("File size = %-10ld", fsize);
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
    size_t buf_size = 128;
    char *get_val = NULL;
    const char *fname = "lorem.txt";
    char *buffer = (char *)malloc(buf_size);
    if (!buffer) {
        fprintf(stderr, "Failed to malloc for the creating buffer");
        return EXIT_FAILURE;
    }

    setup_logger(NULL, LOG_LEVEL_INFO);

    FILE *fp = fopen(fname, "r+");
    if (!fp) {
        LOGGING_ERROR("Failed to find lorem.txt in this dir...");
        goto end;
    }

    int retval = 0;
    int steps  = 0;

    off_t fsize = get_file_size(fname);

    print_file_info(fname, fsize);
    off_t stream_buffer_size = get_buffer_size(fp);

    retval = fseek(fp, 0, SEEK_SET);
    LOGGING_INFO("[#%3d] fseek(fp, 0, SEEK_SET) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, 8, SEEK_SET);
    LOGGING_INFO("[#%3d] fseek(fp, 8, SEEK_SET) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, -8, SEEK_SET);
    LOGGING_INFO("[#%3d] fseek(fp, -8, SEEK_SET) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, 0, SEEK_END);
    LOGGING_INFO("[#%3d] fseek(fp, 0, SEEK_END) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, -8, SEEK_END);
    LOGGING_INFO("[#%3d] feesk(fp, -8, SEEK_END) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, 4, SEEK_SET);
    LOGGING_INFO("[#%3d] fseek(fp, 4, SEEK_SET) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, 4, SEEK_CUR);
    LOGGING_INFO("[#%3d] fseek(fp, 4, SEEK_CUR) = %d", steps++, retval);
    print_all_info(fp);

    retval = fseek(fp, 0, SEEK_SET);
    LOGGING_INFO("[#%3d] fseek(fp, 0, SEEK_SET) = %d", steps++, retval);
    print_all_info(fp);


    retval = fread(buffer, sizeof(char), 16, fp);
    LOGGING_INFO("[#%3d] fread(buffer, sizeof(char), 16, fp) = %d", steps++, retval);
    print_all_info(fp);
    print_buffer_content(buffer, 16);

#ifdef SEEK_HOLE
    retval = fseek(fp, 50, SEEK_HOLE);
    LOGGING_INFO("[#%3d] fseek(fp, 50, SEEK_HOLE) = %d", steps++, retval);
    print_all_info(fp);
#endif

#ifdef SEEK_DATA
    retval = fseek(fp, 50, SEEK_DATA);
    LOGGING_INFO("[#%3d] fseek(fp, 50, SEEK_DATA) = %d", steps++, retval);
    print_all_info(fp);
#endif

    retval = fread(buffer, sizeof(char), 16, fp);
    LOGGING_INFO("[#%3d] fread(buffer, sizeof(char), 16, fp) = %d", steps++, retval);
    print_all_info(fp);
    print_buffer_content(buffer, 16);

    get_val = fgets(buffer, 16, fp);
    LOGGING_INFO("[#%3d] fgets(buffer, 16, fp) = %s", steps++, get_val);
    print_all_info(fp);

end:
    close_logger(NULL);

    return 0;
}