#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <unistd.h>

typedef void   *any_t;
typedef any_t   ser_buff_t;
typedef void  (*ser_fptr)(ser_buff_t, any_t);
typedef any_t (*dser_fptr)(ser_buff_t);

extern ser_buff_t serialized_buffer_create(ser_fptr, dser_fptr);
extern void       serialized_buffer_destroy(ser_buff_t);
extern int        serialized_buffer_size(ser_buff_t);
extern int        serialized_buffer_next(ser_buff_t);
extern void       serialized_buffer_resize(ser_buff_t, int);
extern void       serialized_buffer_skip(ser_buff_t, int);
extern void      *serialized_buffer_start_pos(ser_buff_t);
extern void      *serialized_buffer_ser_pos(ser_buff_t);
extern void       serialized_buffer_ser_pos_incr(ser_buff_t, int);
extern void       serialized_buffer_do_serialize(ser_buff_t, any_t);
extern any_t      serialized_buffer_do_deserialize(ser_buff_t);
extern void       serialized_buffer_dump(ser_buff_t);
extern void       serialized_buffer_reset(ser_buff_t);

extern void  serialize(ser_buff_t, any_t);
extern void  serialize_data(ser_buff_t, any_t, size_t);
extern void  serialize_null(ser_buff_t);
extern void  serialize_list(ser_buff_t, any_t, ser_fptr);
extern void  serialize_int(ser_buff_t, int);

extern any_t deserialize(ser_buff_t);
extern void  deserialize_data(ser_buff_t, any_t, size_t);
extern int   deserialize_null(ser_buff_t);
extern int   deserialize_int(ser_buff_t);

#endif /* SERIALIZE_H */