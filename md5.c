#include "dict.h"

void __realoc__(t_dict *self)
{
    t_value **tmp;
    tmp = self->buffer;
    self->buffer = calloc(self->len_buf << 1, sizeof(t_value *));
    self->len_buf <<= 1;
    memcpy(self->buffer, tmp, self->len_buf * sizeof(t_value));
    free(tmp);
}

void set(t_dict *self, char *key, void *value, t_type type)
{
    uint32_t buf_hash[16];
    uint32_t  hacher[4];
    t_value *val;
    size_t index;

    self->cur_use += 1;
    val = calloc(1, sizeof(t_value));
    __init__value(val, type, value, key);
    memset(buf_hash, 0, 16 * sizeof(uint32_t));
    self->padding(key, buf_hash);
    self->hash(self, buf_hash, hacher);
    if (self->cur_use > self->len_buf)
        self->__realoc__(self);
    index = hacher[0] % self->len_buf;
    if (self->buffer[index] == NULL)
    {
        write(1, "ici", 3);
        self->buffer[index] = val;
        
    }
    else
        while (self->buffer[index])
            index = (size_t)(self->buffer[index]->next - self->buffer[index]);
    self->buffer[index] = val;
}

// if (strcmp(key, "truc") == 0)
//     {
//         write(1, key, 4);
//         exit(1);
//     }

t_value *get(t_dict *self, char *key)
{
    uint32_t buf_hash[16];
    uint32_t hacher[4];
    size_t index;
    self->padding(key, buf_hash);
    self->hash(self, buf_hash, hacher);
    index = hacher[0] % self->len_buf;
    while (self->buffer[index])
    {
        if (strcmp(key, self->buffer[index]->key) == 0)
            return self->buffer[index];
        else
            index = (size_t)(self->buffer[index]->next - self->buffer[index]);
    }
    return NULL;
}

uint32_t rol(uint32_t x, uint32_t s)
{
    s &= 31;
    return (x << s) | (x >> (32 - s));
}

uint32_t F(uint32_t B, uint32_t C, uint32_t D, uint32_t *key, int i)
{
    if (i < 16)
        return ((B & C) | (~B & D)) + key[i];
    if (i >= 16 && i <= 31)
        return ((B & D) | (C & ~D)) + key[(5 * i) & 15];
    if (i >= 32 && i <= 47)
        return (B ^ C ^ D) + key[(3 * i) & 15];;
    if (i >= 48)
        return (C ^ (B | ~D)) + key[(7 * i) & 15];
    return 0;
}


void padding(const char *s, uint32_t *table)
{
    int i = 0;
    int j = 0;
    while (j < 14)
    {
        if (s[i])
        {
            j = i >> 2;
            table[j] |= s[i] << ((i & 0b11) << 3);
            i++;
        }
        else
            table[++j] = 0;
    }
    table[14] = 0x80;
    table[15] = i * 8;
}

void hash(t_dict *self, uint32_t *key, uint32_t *hacher)
{
    int i = -1;
    uint32_t tmp;
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    const uint32_t *k;

    a = self->vars->a;
    b = self->vars->b;
    c = self->vars->c;
    d = self->vars->d;
    k = self->cst->k;
    while (++i < 64)
    {
        tmp = self->vars->c;
        c = b;
        b += rol(a + F(b, c, d, key, i) + k[i], self->cst->s_rot[i]);
        a = d;
        d = tmp;
    }
    hacher[0] = a;
    hacher[1] = b;
    hacher[2] = c;
    hacher[3] = d;
}

int main(void)
{
    t_value val;
    t_value val1;
    t_value val2;
    t_value val3;
    t_dict dict;

    val.data.d = 42;
    val1.data.d = 10;
    val2.data.d = 12;
    val3.data.d = 4;
    __init__dict(&dict, 4);
    dict.set(&dict, "ecole42", (void *)&val.data.d, V_INT);
    dict.set(&dict, "truc", (void *)&val1.data.d, V_INT);
    // dict.set(&dict, "machin", (void *)&val2.data.d, V_INT);
    // dict.set(&dict, "chose", (void *)&val3.data.d, V_INT);
    t_value *test = dict.get(&dict, "ecole42");
    printf("%d\n", test->data.d);
    // test = dict.get(&dict, "truc");
    // printf("%d\n", test->data.d);
    // test = dict.get(&dict, "machin");
    // printf("%d\n", test->data.d);
    // test = dict.get(&dict, "chose");
    // printf("%d\n", test->data.d);
}
