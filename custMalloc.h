#pragma once
#include <stddef.h>
#include <stdint.h>


#ifndef MY_MALLOC_HEAD
#define MY_MALLOC_HEAD

typedef enum _FREE_T
{
    NOT_FREE, FREE
}free_t;

typedef enum _ACTIVE_T
{
    NOT_ACTIVE, ACTIVE
} active_t;


typedef struct _UNIT
{
    size_t size;
    free_t free;

    struct _UNIT* prev;
    struct _UNIT* next;
} unit;

typedef struct _MEM_SESSION
{
    struct _UNIT* firstElem;
    struct _UNIT* lastElem;

    size_t allocUnitCount;
    active_t isActive;

} memSession;

extern memSession session;

void create_session ();
unit* find_free_unit (size_t size);

void* malloc_c (size_t size);
void  free_c (void* ptr);
void free_all_session ();

void LOG (void* ptr);
void testUnit ();
#endif