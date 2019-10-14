#include <stdio.h>
#include <stdlib.h>
#include "serialize.h"

typedef struct occupation_t {
    char dept_name[30];
    int  employee_code;
} occupation_t;

typedef struct person_t {
    char         name[30];
    int          age;
    occupation_t *occ;
    int          weight;
} person_t;

static void serialize_occupation(ser_buff_t buf, any_t _occ)
{
    if (!_occ) {
        serialize_null(buf);
        return;
    }

    occupation_t *occ = (occupation_t *)_occ;

    serialize_data(buf, occ->dept_name, sizeof(char)*30);
    serialize_data(buf, &occ->employee_code, sizeof(int));
}

static void serialize_person(ser_buff_t buf, any_t _person)
{
    if (!_person) {
        serialize_null(buf);
        return;
    }

    person_t *person = (person_t *)_person;

    serialize_data(buf, person->name, sizeof(char)*30);
    serialize_data(buf, &person->age, sizeof(int));
    serialize_occupation(buf, person->occ);
    serialize_data(buf, &person->weight, sizeof(int)); 
    serialized_buffer_dump(buf);
}

static any_t deserialize_occupation(ser_buff_t buf)
{
    if (deserialize_null(buf))
        return NULL;

    occupation_t *obj = calloc(1, sizeof(occupation_t));

    deserialize_data(buf, (void *)obj->dept_name, sizeof(char)*30);
    deserialize_data(buf, (void *)&obj->employee_code, sizeof(int));

    return (void *)obj;
}

static any_t deserialize_person(ser_buff_t buf)
{
    person_t *obj = calloc(1, sizeof(person_t));

    deserialize_data(buf, (void *)obj->name, sizeof(char)*30);
    deserialize_data(buf, (void *)&obj->age, sizeof(int));
    obj->occ = (occupation_t *)deserialize_occupation(buf);
    deserialize_data(buf, (void *)&obj->weight, sizeof(int));

    return obj;
}

static void dump_person(person_t *person)
{
    if (!person) {
        fprintf(stderr, "person is NULL...\n");
        return;
    }

    printf("name   = %s\n", person->name);
    printf("age    = %d\n", person->age);
    if (person->occ)
        printf("occ    = {%s,%d}\n", person->occ->dept_name, person->occ->employee_code);
    else
        printf("occ    = (nil)\n");
    printf("weight = %d\n", person->weight);
    
}

int main(int argc, char **argv)
{
    ser_buff_t buf = serialized_buffer_create(serialize_person, deserialize_person);
    if (!buf)
        return EXIT_FAILURE;

    occupation_t jack_occ = {"software developement", 2507};

    person_t jack = {
        .name   = "jack",
        .age    = 25,
        .occ    = &jack_occ,
        .weight = 80
    };

    person_t peter = {
        .name   = "peter",
        .age    = 26,
        .occ    = NULL,
        .weight = 55
    };

    serialize(buf, &jack);
    serialize(buf, NULL);
    serialize(buf, &peter);

    serialized_buffer_reset(buf);

    person_t *jack_de = (person_t *)deserialize(buf);
    dump_person(jack_de);

    person_t *dump = (person_t *)deserialize(buf);
    dump_person(dump);

    person_t *peter_de = (person_t *)deserialize(buf);
    dump_person(peter_de);

    /*
    list_t *node_jack = calloc(1, sizeof(list_t));
    list_t *node_peter = calloc(1, sizeof(list_t));

    node_jack->value  = jack_de;
    node_jack->next   = node_peter;
    node_peter->value = peter_de;
    node_peter->next  = NULL;
    */

    return 0;
}
