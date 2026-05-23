#include "dict.h"

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

size_t hash(t_dict *self, const char *s)
{
    int i = 0;
    int j = 0;
    uint32_t hacher[4];
    uint32_t buf_hash[16];

    memset(buf_hash, 0, 16 * sizeof(uint32_t));
    while (j < 14)
    {
        if (s[i])
        {
            j = i >> 2;
            buf_hash[j] |= s[i] << ((i & 0b11) << 3);
            i++;
        }
        else
            buf_hash[++j] = 0;
    }
    buf_hash[14] = 0x80;
    buf_hash[15] = i * 8;
    return hash2(self, buf_hash, hacher);
}

size_t hash2(t_dict *self, uint32_t *key, uint32_t *hacher)
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
        tmp = c;
        c = b;
        b += rol(a + F(b, c, d, key, i) + k[i], self->cst->s_rot[i]);
        a = d;
        d = tmp;
    }
    hacher[0] = a;
    hacher[1] = b;
    hacher[2] = c;
    hacher[3] = d;
    return (hacher[0] % self->len_buf);
}
