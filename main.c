#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>

#include "custMalloc.h"


/* GLOBAL VARIABLES */
memSession session;

void LOG (void* ptr)
{
    puts("Session");
    printf("First unit in session: [%p]\n", session.firstElem);
    printf("Last unit in session: [%p]\n", session.lastElem);
    printf("Allocated unit in session: %d\n", session.allocUnitCount);

    puts("Unit");
    printf("...Pointer = [%p] ", ptr);
}

/* FUNCTIONS */
void create_session () 
{
    session.allocUnitCount = 0;
    session.isActive = ACTIVE;

    session.firstElem = session.lastElem = NULL;
}

unit* find_free_unit (size_t size)
{
    unit* tmp = session.firstElem;
    for(int i = 0; i < session.allocUnitCount && tmp != NULL; i++)
    {
        if(tmp->free == FREE && tmp->size == size)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;

}

void* malloc_c (size_t size)
{
    if(session.isActive == NOT_ACTIVE)
    {
        create_session ();
    }

    size_t sizeAround = (((size - 1) >> 2) << 2) + 4;
    
    unit* newUnit = find_free_unit(sizeAround);
    if(newUnit == NULL)
    {
        void* ptr = sbrk(0);

        if(brk(ptr + sizeof(unit) + sizeAround) < 0)
        {
            puts("Couldn't allocate memory");
            exit(2);
        }

        newUnit = (unit* )ptr;
        newUnit->size = sizeAround;
        newUnit->free = NOT_FREE;
        newUnit->prev = session.lastElem;
        newUnit->next = NULL;

        if(session.firstElem == NULL)
        {
            session.firstElem = newUnit;
            session.lastElem = newUnit;
        }

        session.lastElem->next = newUnit;
        session.lastElem = newUnit;
        session.allocUnitCount++;

    }
    else
    {
        newUnit->free = NOT_FREE;
        
    }
    return (sizeof(unit) + (char*) newUnit);

}

void free_c (void* ptr)
{
    if(ptr < (void*)((char*) session.firstElem + sizeof(unit)) 
    || ptr > (void*)((char*) session.lastElem + sizeof(unit)))
    {
        printf("Invalid Free. Out of range!");
        exit(3);
    }
    unit* tmp = (unit*)((char*)ptr - sizeof(unit));
    tmp->free = FREE;
}

void free_all_session ()
{
    unit* tmp = (unit*)session.firstElem;
    while (tmp != NULL)
    {
        tmp->free = FREE;
        tmp = tmp->next;
    }
}

void testUnit () 
{
    int* intPtr = malloc_c(sizeof(int));
    LOG(intPtr);
    printf("Value = %i\n", *intPtr);

    double* doublePtr = malloc_c(sizeof(double));
    LOG(doublePtr);
    printf("Value = %d\n", *doublePtr);

    free_c(intPtr);

    int* oneMoreIntPtr = malloc_c(sizeof(int));
    LOG(oneMoreIntPtr);
    printf("Value = %i\n", *oneMoreIntPtr);
    
    free_all_session ();
}


int main (int argc, char ** argv) 
{
    testUnit();
    return 0;
};