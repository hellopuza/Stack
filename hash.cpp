/*------------------------------------------------------------------------------
    * File:        hash.cpp                                                    *
    * Description: Implementations of hash functions                           *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------

#include "hash.h"

//------------------------------------------------------------------------------

void bit_rotate(void* buf, size_t size, int dir)
{
    assert(buf != nullptr);

    if ((size == 0) || (dir == 0))
        return;

    char* carry_bits = (char*)calloc(size, 1);

    if (dir > 0)
    {
        dir = dir % (size * 8);
        for (int i = 0; i < dir; ++i)
        {
            for (int byte_i = 0; byte_i < size; ++byte_i)
            {
                carry_bits[byte_i] = *((char*)buf + byte_i) & 1;
            }

            for (int byte_i = 0; byte_i < size - 1; ++byte_i)
            {
                char byte = *((char*)buf + byte_i);
                byte = byte >> 1;
                byte = (byte & 127) | (128 * carry_bits[byte_i + 1]);

                ((char*)buf)[byte_i] = byte;
            }

            char byte = *((char*)buf + size - 1);
            byte = byte >> 1;
            byte = (byte & 127) | (128 * carry_bits[0]);

            ((char*)buf)[size - 1] = byte;
        }
    }
    else
    {
        dir = -dir % (size * 8);

        for (int i = 0; i < dir; ++i)
        {
            for (int byte_i = 0; byte_i < size; ++byte_i)
            {
                carry_bits[byte_i] = *((char*)buf + byte_i) & 128;
            }

            for (int byte_i = size - 1; byte_i > 0; --byte_i)
            {
                char byte = *((char*)buf + byte_i);
                byte = byte << 1;
                byte = (byte & 254) | (carry_bits[byte_i - 1] / 128);

                ((char*)buf)[byte_i] = byte;
            }

            char byte = *((char*)buf);
            byte = byte << 1;
            byte = (byte & 254) | (carry_bits[size - 1] / 128);

            ((char*)buf)[0] = byte;
        }
    }

    free(carry_bits);
}

//------------------------------------------------------------------------------

hash_t hash(void* buf, size_t size)
{
    assert(buf != nullptr);

    size_t bm_size = (size / block_size) * block_size + block_size;

    char* buf_main = (char*)calloc(bm_size, 1);

    memcpy(buf_main, buf, size);

    ((size_t*)buf_main)[bm_size/sizeof(size_t) - 1] = size;

    while (bm_size > HASH_SIZE)
    {
        size_t new_size = bm_size / 2;

        for (int byte_i = 0; byte_i < new_size; ++byte_i)
        {
            char b1 = buf_main[byte_i];
            char b2 = buf_main[byte_i + bm_size / 2];

            char p1 = b1;
            char p2 = b2;

            bit_rotate(&p1, 1, -byte_i);
            bit_rotate(&p2, 1, bm_size / 2 - byte_i);

            char q1 = b2 ^ p1 ^ Keys[ byte_i      % keys_num] + b1;
            char q2 = b1 ^ p2 ^ Keys[(byte_i + 1) % keys_num] + b2;

            buf_main[byte_i] = (q1 + q2);
        }

        bm_size = new_size;
    }
    hash_t hsh1 = ((hash_t*)buf_main)[0];
    free(buf_main);


    hash_t hsh2 = (hash_t)(Keys[size % keys_num]);

    size_t hs_size = (size / HASH_SIZE) * HASH_SIZE + HASH_SIZE;

    char* hs_main = (char*)calloc(hs_size, 1);

    memcpy(hs_main, buf, size);

    for (int h = hs_size - HASH_SIZE; h >= 0; --h)
    {
        hsh2 = (hsh2 ^ *((char*)hs_main + h)) * (hsh1 ^ *((char*)hs_main + hs_size - 1 - h));
    }
    free(hs_main);


    hash_t total = hsh1 + hsh2;

    return total;
}

//------------------------------------------------------------------------------

