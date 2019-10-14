#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "serialize.h"

#define SERIALIZE_BUFFER_DEFAULT_SIZE (512)
#define SER_POS ((char *)buf->b + buf->next)

typedef struct _ser_buff_t {
    void     *b;
    int       size;
    int       next;
    ser_fptr  ser_func;
    dser_fptr dser_func;
} _ser_buff_t;


ser_buff_t serialized_buffer_create(ser_fptr fptr, dser_fptr dfptr)
{
    _ser_buff_t *ser_buf = (_ser_buff_t *)calloc(1 ,sizeof(_ser_buff_t));
    if (!ser_buf)
        return NULL;
    
    ser_buf->b = calloc(1, SERIALIZE_BUFFER_DEFAULT_SIZE);
    if (!ser_buf->b) {
        free(ser_buf);
        return NULL;
    }

    ser_buf->size      = SERIALIZE_BUFFER_DEFAULT_SIZE;
    ser_buf->next      = 0;
    ser_buf->ser_func  = fptr;
    ser_buf->dser_func = dfptr;

    return (ser_buff_t)ser_buf;
}

void serialized_buffer_destroy(ser_buff_t _buf)
{
    if (!_buf)
        return;
    
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    if (buf->b)
        free(buf->b);
    buf->ser_func = NULL;
    buf->dser_func = NULL;
    free(_buf);
}

int serialized_buffer_size(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    return buf->size;
}

int serialized_buffer_next(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    return buf->next;
}

void serialized_buffer_resize(ser_buff_t _buf, int ratio)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    buf->size = buf->size * ratio;
    
    void *tmp = realloc(buf->b, buf->size);
    if (!tmp)
        return;
    
    buf->b = tmp;
}

void serialized_buffer_skip(ser_buff_t _buf, int size)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    int available_size = buf->size - buf->next;

    if (available_size >= size) {
        buf->next += size;
        return;
    }

    while (available_size < size) {
        serialized_buffer_resize(_buf, 2);
        available_size = buf->size - buf->next;
    }

    buf->next += size;
}

void *serialized_buffer_start_pos(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    return buf->b;
}

void *serialized_buffer_ser_pos(ser_buff_t _buf)
{   
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    return (void *)SER_POS;
}

void serialized_buffer_ser_pos_incr(ser_buff_t _buf, int size)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    buf->next += size;
}

void serialized_buffer_do_serialize(ser_buff_t _buf, any_t data)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    buf->ser_func(_buf, data);
}

any_t serialized_buffer_do_deserialize(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    return buf->dser_func(_buf);
}

void serialized_buffer_dump(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    printf("%-15s = %p\n", "buf->b", buf->b);
    printf("%-15s = %d\n", "buf->size", buf->size);
    printf("%-15s = %d\n", "buf->next", buf->next);
    printf("%-15s = %p\n", "buf->ser_func", buf->ser_func);
    printf("%-15s = %p\n", "buf->dser_func", buf->dser_func);
    printf("\n");
}

void serialized_buffer_reset(ser_buff_t _buf)
{
    _ser_buff_t *buf = (_ser_buff_t *)_buf;
    buf->next = 0;
}