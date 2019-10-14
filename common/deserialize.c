#include <memory.h>
#include "serialize.h"

#define SER_BUF_SIZE serialized_buffer_size(buf)
#define SER_BUF_NEXT serialized_buffer_next(buf)
#define SER_BUF_POS  serialized_buffer_ser_pos(buf)

void deserialize_data(ser_buff_t buf, any_t dest, size_t size)
{
    if (size == 0 || !buf)
        return;
    
    if (SER_BUF_SIZE < ((int)size + SER_BUF_NEXT))
        return;

    memcpy(dest, SER_BUF_POS, size);
    serialized_buffer_ser_pos_incr(buf, size);
}

any_t deserialize(ser_buff_t buf)
{
    if (deserialize_null(buf))
        return NULL;

    return serialized_buffer_do_deserialize(buf);
}

int deserialize_null(ser_buff_t buf)
{
    unsigned int sentinel = 0;
    deserialize_data(buf, (void *)&sentinel, sizeof(unsigned int));
    if (sentinel == 0xFFFFFFFF)
        return -1;
    serialized_buffer_skip(buf, -1*(int)sizeof(unsigned int));
    return 0;
}

int deserialize_int(ser_buff_t buf)
{   
    if (SER_BUF_SIZE < ((int)sizeof(int) + SER_BUF_NEXT))
        return 0;

    int temp = 0;
    
    memcpy((void *)&temp, SER_BUF_POS, sizeof(int));
    serialized_buffer_ser_pos_incr(buf, sizeof(int));

    return temp;
}