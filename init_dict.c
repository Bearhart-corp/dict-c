#include "dict.h"

void    __init__const(t_const *self)
{
    *self = (t_const)
    {
        .s_rot = {7, 12, 17, 22,  7, 12, 17, 22,
                7, 12, 17, 22,  7, 12, 17, 22,
                5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21},
        .k = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf,
            0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
            0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
            0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6,
            0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
            0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
            0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
            0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
            0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
            0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}
    };
}

void __init__vars(t_abcd *self)
{
    *self = (t_abcd)
    {
        .a = 0x67452301,
        .b = 0xefcdab89,
        .c = 0x98badcfe,
        .d = 0x10325476
    };
}
void __init__dict(t_dict *self, int len)
{
    t_abcd *vars;
    t_const *cst;
    cst = malloc(1 * sizeof(t_const));
    vars = malloc(1 * sizeof(t_abcd));
    __init__vars(vars);
    __init__const(cst);
    self->set = set;
    self->get = get;
    self->print_dict = print_dict;
    self->__realoc__ = __realoc__;
    self->__destructeur__dict = __destructeur__dict;
    self->hash = hash;
    self->len_buf = len;
    self->cur_use = 0;
    self->buffer = calloc(len, sizeof(t_value *));
    self->vars = vars;
    self->cst = cst;
}

void __destructeur__dict(t_dict *self)
{
    int i;
    t_value *ptr;
    t_value *ptr2;

    i = -1;
    while (++i < self->len_buf)
    {
        ptr = self->buffer[i];
        while (ptr)
        {
            ptr2 = ptr->next;
            free(ptr);
            ptr = ptr2;
        }
    }
    free(self->buffer);
    free(self->vars);
    free(self->cst);
}

void __init__value(t_value *self, t_type type, void *value, char *key)
{
    self->type = type;
    self->key = key;
    if (type == V_CHAR)
        self->data.c = *(char *)value;
    else if (type == V_INT)
        self->data.d = *(int*)value;
    else if (type == V_FLOAT)
        self->data.f = *(double*)value;
    else if (type == V_PTR)
        self->data.p = value;
    else if (type == V_STRING)
        self->data.s = (char*)value;
    self->next = NULL;
}
