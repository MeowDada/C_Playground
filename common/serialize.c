#include <stdlib.h>
#include <memory.h>
#include "serialize.h"

#define SER_BUF_SIZE serialized_buffer_size(buf)
#define SER_BUF_NEXT serialized_buffer_next(buf)
#define SER_BUF_POS  serialized_buffer_ser_pos(buf)

void serialize_data(ser_buff_t buf, any_t _data, size_t size)
{
    if (!buf)
        return;

    char *data = (char *)_data;
    
    size_t available_size = SER_BUF_SIZE - SER_BUF_NEXT;
    int is_resized = 0;

    while (available_size < size)
    {
        serialized_buffer_resize(buf, 2);
        available_size = SER_BUF_SIZE - SER_BUF_NEXT;
        is_resized = 1;
    }

    if (is_resized == 0) {
        memcpy(SER_BUF_POS, data, size);
        serialized_buffer_ser_pos_incr(buf, size);
        return;
    }

    memcpy(SER_BUF_POS, data, size);
    serialized_buffer_ser_pos_incr(buf, size);
}

void serialize(ser_buff_t _buf, any_t data)
{
    if (!data) {
        unsigned int sentinel = 0xFFFFFFFF;
        serialize_data(_buf, (any_t)&sentinel, sizeof(unsigned int));
        return;
    }

    serialized_buffer_do_serialize(_buf, data);
}

void serialize_null(ser_buff_t _buf)
{
    unsigned int sentinel = 0xFFFFFFFF;
    serialize_data(_buf, (any_t)&sentinel, sizeof(unsigned int));
}

void serialize_list(ser_buff_t _buf, any_t list, ser_fptr ser_list)
{
    (*ser_list)(_buf, list);
}

void serialize_int(ser_buff_t _buf, int _data)
{
    int data = _data;
    serialize_data(_buf, (void *)&data, sizeof(int));
}