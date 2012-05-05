#include <stdio.h>
#include <stdlib.h>

typedef int value_t;

void
print_value(value_t val)
{
    printf("%d", val);
}

void
abort_app_malloc_failure(){
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
}

void
abort_app(char* msg){
    fprintf(stderr, "%s\n", msg);
    exit(1);
}


typedef struct CELL_T {
    struct CELL_T* next;
    value_t value;
} cell;

cell* create_cell(value_t val)
{
    cell* cel = (cell *)malloc(sizeof(cell));
    if (!cel) abort_app_malloc_failure();
    cel->next = NULL;
    cel->value = val;
    return cel;
}

void
free_list(cell* cel)
{
    if(!cel) return;
    cell* next = cel->next;
    free(cel);
    free_list(next);
}

cell*
next(cell* c)
{
    return c->next;
}

void
print_list(cell* c)
{
    if(!c) { printf("\n"); return; } 
    print_value(c->value);
    printf(" ");
    print_list(next(c));
}


cell*
push(value_t val, cell* cel)
{
    cell* new_one = create_cell(val);
    new_one->next = cel;
    return new_one;
}

cell*
pop(cell* cel)
{
    cell* remain = cel->next;
    free(cel);
    return remain;
}

static cell*
find_last(cell* cel)
{
    if(!cel) abort_app("ArgumentNullException: find_last");
    if(cel->next) 
        return find_last(cel->next);
    else
        return cel;
}

cell*
nconc(cell* first, cell* second)
{
    cell* firsts_last = find_last(first);
    firsts_last->next = second;
    return first;
}

static cell*
reverse_inner(cell* cel, cell* prev)
{
    cell* next = cel->next;
    cel->next = prev;
    if(next)
        return reverse_inner(next, cel);
    else 
        return cel;        

}

cell*
reverse(cell* cel)
{
    if(!cel || !cel->next) return cel;
    cell* next = cel->next;
    cel->next = NULL;
    return reverse_inner(next, cel);
}

int 
main(int argc, const char *argv[])
{
    cell *pl,*res;

    pl=NULL;
    pl = push(3,pl);
    pl = push(2,pl);
    pl = push(1,pl);

    print_list(pl);

    res = reverse(pl);
    print_list(res);
    return 0;
}
