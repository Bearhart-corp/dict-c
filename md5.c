#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    V_INT,
    V_FLOAT,
    V_PTR,
    V_CHAR,
    V_STRING
} t_type;

typedef struct s_value {
    t_type type;

    union {
        int d;
        double f;
        void *p;
        char c;
        char *s;
    } data;
} t_value;

typedef struct s_dict
{
    t_value **buffer;
    size_t len_buf;
    size_t cap_buf;
    void (*set)(struct s_dict *self, char *key, void *value, t_type type);
    t_value *(*get)(struct s_dict *self, char *key);
    void (*hash)(uint32_t *key, uint32_t *hacher);
    void (*padding)(const char *s, uint32_t *table);
} t_dict;

void set(struct s_dict *self, char *key, void *value, t_type type)
{
    uint32_t buf_hash[16];
    uint32_t  *hacher = calloc(4, sizeof(uint32_t));
    if (type == V_CHAR)
    {
        return;
    }
    else if (type == V_INT)
    {
        self->padding(key, buf_hash);
        self->hash(buf_hash, hacher);
        t_value *val;
        val = calloc(1, sizeof(t_value));
        val->data.d = *(int*)value;
        val->type = type;
        self->buffer[hacher[0] % self->len_buf] = val;
    }
    else if (type == V_FLOAT)
    {
        return;
    }
    else if (type == V_PTR)
    {
        return;
    }
    else if (type == V_STRING)
    {
        return;
    }
    free(hacher);
}

t_value *get(t_dict *self, char *key)
{
    uint32_t buf_hash[16];
    uint32_t  *hacher = calloc(4, sizeof(uint32_t));
    self->padding(key, buf_hash);
    self->hash(buf_hash, hacher);
    free(hacher);
    return self->buffer[hacher[0] % self->len_buf];
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
    if (i > 48)
        return (C ^ (B | ~D)) + key[(7 * i) & 15];;
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

static void hash(uint32_t *key, uint32_t *hacher)
{
    uint32_t a = 0x67452301;
    uint32_t b = 0xefcdab89;
    uint32_t c = 0x98badcfe;
    uint32_t d = 0x10325476;

    const uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    int s_rotation[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

    int i = -1;
    uint32_t tmp;

    while (++i < 64)
    {
        tmp = c;
        c = b;
        b = b +  rol(a + F(b, c, d, key, i) + k[i], s_rotation[i]);
        a = d;
        d = tmp;
    }
    hacher[0] = a;
    hacher[1] = b;
    hacher[2] = c;
    hacher[3] = d;
}

void __init__dict(t_dict *self, int len, int cap)
{
    self->set = set;
    self->get = get;
    self->hash = hash;
    self->len_buf = len;
    self->cap_buf = cap;
    self->buffer = calloc(self->len_buf, sizeof(t_value *));
    self->padding = padding;
}

int main(void)
{
    // uint32_t x = 0x12345678;
    // uint32_t key[16];
    // uint32_t hacher[4];

    // padding("abcde", key);
    // hash(key, hacher);
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%x", hacher[i]);
    //     printf("_");
    // }
    t_value val;
    val.data.s = "str";
    t_dict dict = {0};
    __init__dict(&dict, 20, 15);
    int data = 42;
    dict.set(&dict, "exemple", (void *)&data, V_STRING);
    printf("%d",dict.get(&dict, "exemple")->data.d);
}
//void set(struct s_dict *self, char *key, t_value *value, t_type type)
