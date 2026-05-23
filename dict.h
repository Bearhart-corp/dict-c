#ifndef DICT_H
# define DICT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_abcd
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
} t_abcd;

typedef struct s_const {
    int s_rot[64];
    uint32_t k[64];
} t_const;

typedef enum {
    V_INT,
    V_FLOAT,
    V_PTR,
    V_CHAR,
    V_STRING
} t_type;

typedef struct s_value {
    t_type type;
    char *key;
    union {
        int d;
        double f;
        void *p;
        char c;
        char *s;
    } data;
    struct s_value* next;
} t_value;

typedef struct s_dict
{
    t_value **buffer;
    size_t len_buf;
    size_t cur_use;
    void (*set)(struct s_dict *self, char *key, void *value, t_type type);
    t_value *(*get)(struct s_dict *self, char *key);
    size_t (*hash2)(struct s_dict *self, uint32_t *key, uint32_t *hacher);
    size_t (*hash)(struct s_dict *self, const char *s);
    void (*__realoc__)(struct s_dict *self, size_t len);
    void (*__destructeur__dict)(struct s_dict *self);
    void (*print_dict)(struct s_dict *self);
    t_abcd *vars;
    t_const *cst;
} t_dict;

void __init__dict(t_dict *self, int len);
t_value *get(t_dict *self, char *key);
void set(struct s_dict *self, char *key, void *value, t_type type);
size_t hash(t_dict *self, const char *s);
size_t hash2(t_dict *self, uint32_t *key, uint32_t *hacher);
void __realoc__(t_dict *self, size_t len);
void __init__value(t_value *self, t_type type, void *value, char *key);
void __destructeur__dict(t_dict *self);
///// hash functions
uint32_t rol(uint32_t x, uint32_t s);
uint32_t F(uint32_t B, uint32_t C, uint32_t D, uint32_t *key, int i);

//////////dict

void print_dict(struct s_dict *self);

#endif
