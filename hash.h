/*------------------------------------------------------------------------------
    * File:        hash.h                                                      *
    * Description: Hash functions                                              *
    * Created:     1 dec 2020                                                  *
    * Copyright:   (C) 2020 MIPT                                               *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
*///----------------------------------------------------------------------------

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef unsigned long long hash_t;

#define HASH_SIZE sizeof(hash_t)
#define MAX_HASH  ULLONG_MAX
#define HASH_PRINT_FORMAT "%p%p"

static const size_t block_size = 64;
static const size_t keys_num   = 16;

static const size_t Keys[keys_num] =
{
    0x26964da6, 0x69b25a6d, 0x9b4d9693, 0x64d26d2c,
    0x4b65a6c9, 0x9a592d36, 0xa4da6cb4, 0x4b2696c9,
    0xd36934b6, 0x369b2d92, 0x6cb4da59, 0x4b65a6d2,
    0xda592d93, 0x2696c964, 0xb26d365b, 0x25936934,
};

//------------------------------------------------------------------------------
/*! @brief   Circular shift of bits anywhere in any length
 *
 *  @param   buf  Start of memory for turning round
 *  @param   size Size of memory for turning round
 *  @param   dir  Direction of turning, if >0 - right, if <0 - left
 */

void bit_rotate(void* buf, size_t size, int dir);

//------------------------------------------------------------------------------
/*! @brief   Hash counting
 *
 *  @param   buf  Start of memory to be hashable
 *  @param   size Size of memory to be hashable
 *
 *  @return  hash
 */

hash_t hash(void* buf, size_t size);

//------------------------------------------------------------------------------

#endif // HASH_H_INCLUDED

