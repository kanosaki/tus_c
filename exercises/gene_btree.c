#include <stdio.h>
#include <stdlib.h>

// Simplify printing.
// Ex. (3, NULL, NULL) -> [3]
#define ALTER_PRINTING 1

typedef int item_t;

typedef struct node_s {
    struct node_s *right;
    struct node_s *left;
    item_t item;
} node;

void
print_node_item(item_t* item)
{
    printf("%d", *item);
}

node* 
create_node(item_t item, node* right, node* left)
{
    node* ret = (node *)malloc(sizeof(node));
    if(!ret) { printf("malloc failed.\n"); return NULL; }
    ret->item = item;
    ret->left = left;
    ret->right = right;
    return ret;
}

void
free_nodes(node* target)
{
    if(target->left)
        free_nodes(target->left);
    if(target->right)
        free_nodes(target->right);
    free(target);
}

void
print_nodes_inner(node* target)
{
    if(!target){
        printf("NULL");
#if ALTER_PRINTING
    }else if(!target->left && !target->right){
        printf("[");
        print_node_item(&target->item);
        printf("]");
#endif
    }else{
        printf("(");
        print_node_item(&target->item);
        printf(",");
        print_nodes_inner(target->right);
        printf(",");
        print_nodes_inner(target->left);
        printf(")");
    }
}

void
print_nodes(node* target)
{
    print_nodes_inner(target);
    printf("\n");
}

int main(int argc, const char *argv[])
{
    node* root = create_node(3, NULL, NULL);
    root = create_node(8, root, NULL);
    root = create_node(7, create_node(5, NULL, NULL), root);
    print_nodes(root);
    free_nodes(root);
    return 0;
}
