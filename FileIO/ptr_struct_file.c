#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define print(content, msg) printf("%-20s:%3p\n",content, msg)
#define RBUF_SIZE  10
#define WBUF_SIZE 128

int main(int argc, char **argv)
{
    FILE *fp_src = NULL;
    FILE *fp_des = NULL;
    char rbuffer[RBUF_SIZE];
    char wbuffer[WBUF_SIZE];

    int i = 0;

    // Open the source file
    if ((fp_src = fopen(argv[1], "r+")) == NULL) {
        perror("Failed to open source file!\n");
        exit(EXIT_FAILURE);
    }

    // Open the target file
    if ((fp_des = fopen(argv[2], "w+")) == NULL) {
        perror("Failed to open target file!\n");
        exit(EXIT_FAILURE);
    }

    // [Optional] Display position and type of the buffer
    setvbuf(fp_src, rbuffer, _IOLBF, WBUF_SIZE);

    do {
#if defined(LINUX)
        // Read position of the file
        print("src_IO_read_ptr", fp_src->_IO_read_ptr);
        print("_IO_read_end", fp_src->_IO_read_end);
        print("_IO_read_base", fp_src->_IO_read_base);

        // Write position of the file
        print("src_IO_write_ptr", fp_src->_IO_write_ptr);
        print("_IO_write_end", fp_src->_IO_write_end);
        print("_IO_write_base", fp_src->_IO_write_base);

        // Buffer position of the file
        print("_IO_buf_base\t", fp_src->_IO_buf_base);
        print("_IO_buf_end\t", fp_src->_IO_buf_end);

        /* 
        *   Read from the file and put the data into read buffer,
        *   Then write the data from read buffer to the destination. 
        */
        memset(rbuffer, '\0', RBUF_SIZE);
        i = fread(rbuffer, sizeof(char), RBUF_SIZE, fp_src);
        fwrite(rbuffer, sizeof(char), i, fp_des);

        print("des_IO_read_ptr", fp_des->_IO_read_ptr);
        print("des_IO_write_ptr", fp_des->_IO_write_ptr);
#elif defined(_WIN32)
    // Read position of the file
        print("src_IO_read_ptr", fp_src->_ptr);
        print("_IO_read_end", fp_src->_IO_read_end);
        print("_IO_read_base", fp_src->_base);

        // Write position of the file
        print("src_IO_write_ptr", fp_src->_IO_write_ptr);
        print("_IO_write_end", fp_src->_IO_write_end);
        print("_IO_write_base", fp_src->_IO_write_base);

        // Buffer position of the file
        print("_IO_buf_base\t", fp_src->_IO_buf_base);
        print("_IO_buf_end\t", fp_src->_IO_buf_end);

        /* 
        *   Read from the file and put the data into read buffer,
        *   Then write the data from read buffer to the destination. 
        */
        memset(rbuffer, '\0', RBUF_SIZE);
        i = fread(rbuffer, sizeof(char), RBUF_SIZE, fp_src);
        fwrite(rbuffer, sizeof(char), i, fp_des);

        print("des_IO_read_ptr", fp_des->_IO_read_ptr);
        print("des_IO_write_ptr", fp_des->_IO_write_ptr);
#endif
    }while (i == RBUF_SIZE);

    fclose(fp_src);
    fclose(fp_des);
}