#include "dict.h"

void __realoc__(t_dict *self, size_t len)
{
    t_value **tmp;
    size_t i;
    size_t new_i;
    t_value *tmp2;

    tmp = self->buffer;
    i = 0;
    self->len_buf <<= 1;
    self->buffer = calloc(self->len_buf, sizeof(t_value *));
    if (!self->buffer)
        return;
    while (i < len)
    {
        while (tmp[i])
        {
            new_i = self->hash(self, tmp[i]->key);
            if (self->buffer[new_i] == NULL)
            {
                self->buffer[new_i] = tmp[i];
            }
            else
            {
                tmp2 = self->buffer[new_i];
                while (tmp2->next)
                    tmp2 = tmp2->next;
                tmp2->next = tmp[i];
            }
            t_value *test = tmp[i];
            tmp[i] = tmp[i]->next;
            test->next = NULL;
        }
        i++;
    }
    free(tmp);
}

void set(t_dict *self, char *key, void *value, t_type type)
{
    t_value *val;
    size_t index;

    if (self->cur_use >= (self->len_buf))
        self->__realoc__(self, self->len_buf);
    val = calloc(1, sizeof(t_value));
    __init__value(val, type, value, key);
    index = self->hash(self, key);
    self->cur_use += 1;
    if (self->buffer[index] == NULL)
    {
        self->buffer[index] = val;
    }
    else
    {
        t_value *tmp = self->buffer[index];
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = val;
    }
}

// if (strcmp(key, "truc") == 0)
//     {
//         write(1, key, 4);
//         exit(1);
//     }

t_value *get(t_dict *self, char *key)
{
    size_t index;

    index = self->hash(self, key);
    t_value *tmp = self->buffer[index];
    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void print_dict(struct s_dict *self)
{
    size_t i;
    t_value *ptr;
    int flag;

    i = -1;
    printf("\ncur_size = %zu | pour dict de taille %zu:\n", self->cur_use, self->len_buf);
    while (++i < self->len_buf)
    {
        ptr = self->buffer[i];
        if (!ptr)
        {
            printf("\t[%ld]: %p -> %s\n", i, ptr, "NULL");
            continue;
        }
        flag = 0;
        while (ptr)
        {
            if (!flag)
            {
                printf("\t[%ld]: %p -> %s", i, ptr, ptr->data.s);
                flag = 1;
            }
            else
                printf(" -> %s", ptr->data.s);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

int main(void)
{
    t_dict dict;

    __init__dict(&dict, 12);
    dict.print_dict(&dict);
    dict.set(&dict, "machin", (void *)"bchdfjgcfdhi", V_STRING);
    dict.set(&dict, "chose", (void *)"str_chose", V_STRING);
    dict.set(&dict, "alpha", (void *)"str_alpha", V_STRING);
    dict.set(&dict, "beta", (void *)"str_beta", V_STRING);
    dict.set(&dict, "gamma", (void *)"str_gamma", V_STRING);
    dict.set(&dict, "delta", (void *)"str_delta", V_STRING);
    dict.set(&dict, "epsilon", (void *)"str_epsilon", V_STRING);
    dict.set(&dict, "zeta", (void *)"str_zeta", V_STRING);
    dict.set(&dict, "eta", (void *)"str_eta", V_STRING);
    dict.set(&dict, "theta", (void *)"str_theta", V_STRING);
    dict.set(&dict, "iota", (void *)"str_iota", V_STRING);
    dict.set(&dict, "kappa", (void *)"str_kappa", V_STRING);
    dict.set(&dict, "lambda", (void *)"str_lambda", V_STRING);
    dict.set(&dict, "mu", (void *)"str_mu", V_STRING);
    dict.set(&dict, "nu", (void *)"str_nu", V_STRING);
    dict.set(&dict, "xi", (void *)"str_xi", V_STRING);
    dict.set(&dict, "omicron", (void *)"str_omicron", V_STRING);
    dict.set(&dict, "pi", (void *)"str_pi", V_STRING);
    dict.set(&dict, "rho", (void *)"str_rho", V_STRING);
    dict.set(&dict, "sigma", (void *)"str_sigma", V_STRING);
    dict.set(&dict, "tau", (void *)"str_tau", V_STRING);
    dict.set(&dict, "omega", (void *)"str_omega", V_STRING);

    dict.set(&dict, "apple", (void *)"str_apple", V_STRING);
    dict.set(&dict, "banana", (void *)"str_banana", V_STRING);
    dict.set(&dict, "orange", (void *)"str_orange", V_STRING);
    dict.set(&dict, "grape", (void *)"str_grape", V_STRING);
    dict.set(&dict, "mango", (void *)"str_mango", V_STRING);
    dict.set(&dict, "peach", (void *)"str_peach", V_STRING);
    dict.set(&dict, "pear", (void *)"str_pear", V_STRING);
    dict.set(&dict, "plum", (void *)"str_plum", V_STRING);
    dict.set(&dict, "cherry", (void *)"str_cherry", V_STRING);
    dict.set(&dict, "kiwi", (void *)"str_kiwi", V_STRING);
    dict.set(&dict, "melon", (void *)"str_melon", V_STRING);
    dict.set(&dict, "lemon", (void *)"str_lemon", V_STRING);
    dict.set(&dict, "lime", (void *)"str_lime", V_STRING);
    dict.set(&dict, "strawberry", (void *)"str_strawberry", V_STRING);
    dict.set(&dict, "blueberry", (void *)"str_blueberry", V_STRING);
    dict.set(&dict, "raspberry", (void *)"str_raspberry", V_STRING);
    dict.set(&dict, "pineapple", (void *)"str_pineapple", V_STRING);
    dict.set(&dict, "coconut", (void *)"str_coconut", V_STRING);
    dict.set(&dict, "apricot", (void *)"str_apricot", V_STRING);
    dict.set(&dict, "pomegranate", (void *)"str_pomegranate", V_STRING);
    dict.print_dict(&dict);
    dict.__destructeur__dict(&dict);
}
