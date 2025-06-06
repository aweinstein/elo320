// https://youtu.be/M_WVEF1i5Og?t=2132

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {
    int key;
    struct node *left;
    struct node *right;
} node;

node *make_node(int data);
node *insert_recursive(node *np, int dato);
void follow_in_order(node *np);
void follow_per_level(node *np, int level);
node *search(node *np, int val);
node *search_min(node *np);
node *remove_node(node *np, int val);
int tree_height(node *np);

void print_bfs_tree(node *root);

int main(void)
{
    node *bst = NULL;
    int i;
    
    printf("Ejemplo de arbol binario de busqueda");
    int vals[15] = {20, 10, 30, 5, 15, 25, 40, 4, 7, 12, 18, 22, 27, 35, 45};
    for(i=0; i<15; i++)
        bst = insert_recursive(bst, vals[i]);
    printf("Muestra el arbol en orden \n");
    follow_in_order(bst);
    printf("Muestra el arbol por nivel\n");
    follow_per_level(bst, 0);
    print_bfs_tree(bst);
    printf("Busca un valor en el arbol\n");
    i = 22;
    node *p = search(bst, i);
    if(p != NULL)
        printf("Encontramos %d\n", p->key);
    else
        printf("%d no esta en el arbol\n", i);

    printf("El minimo es: %d\n", search_min(bst)->key);
    print_bfs_tree(bst);

    i = 30;
    printf("Remover elemento %d \n", i);
    bst = remove_node(bst, i);
    print_bfs_tree(bst);
    return 0;
}


node *make_node(int data)
{
    node *np = malloc(sizeof(node));
    if(np == NULL)
        exit(1);
    np->key = data;
    np->left = NULL;
    np->right = NULL;
    return np;
}


node *insert_recursive(node *np, int dato)
{
    if(np == NULL) {
        return make_node(dato);
    }
    if(np->key == dato)
        return np;  // dato duplicado
    if(dato < np->key)
        np->left = insert_recursive(np->left, dato);
    else
        np->right = insert_recursive(np->right, dato);
    return np; // es necesario?
}


void follow_in_order(node *np)
{
    if(np != NULL) {
        follow_in_order(np->left);
        printf("%d \n", np->key);
        follow_in_order(np->right);
    }
}


void follow_per_level(node *np, int level){
    if(np != NULL) {
        follow_per_level(np->left, level + 1);
        printf("Nodo: %d, nivel %d\n", np->key, level);
        follow_per_level(np->right, level + 1);
    }
}


node *search(node *np, int val)
{
    while(np != NULL) {
        if(np->key == val)
            return np;
        else {
            if(val > np->key)
                np = np->right;
            else
                np = np->left;
        }
    }
    return NULL;
}


node *search_min(node *np)
{
    if(np == NULL)
        return NULL;
    if(np->left == NULL)
        return np;
    return search_min(np->left);  
}


/* https://www.youtube.com/watch?v=uKPDHZZfoCk&t=23s */
node *remove_node(node *np, int val)
{
    node *tmp;
    if(np == NULL)
        printf("Elemento no encontrado\n");
    else
        if(val < np->key) // por la izquierda
            np->left = remove_node(np->left, val);
        else if(val > np->key) // por la derecha
            np->right = remove_node(np->right, val);
        else if(np->left && np->right) { // dos hijos
            tmp = search_min(np->right);
            np->key = tmp->key;
            np->right = remove_node(np->right, tmp->key); // borrar hoja
        } else {
            tmp = np;
            if(np->left == NULL)
                np = np->right;
            else if(np->right == NULL)
                np = np->left;
            free(tmp);
        }
    return np; 
}

//////////////////////////////////////////////////////////////////////////////

void print_spaces(int n) {
    for(int i = 0; i < n; i++) 
        putchar(' ');
}

int tree_height(node *np) {
    int n_left, n_right;
    if (np == NULL) 
        return 0;
    n_left = tree_height(np->left);
    n_right = tree_height(np->right);
    if(n_left > n_right)  // Retorna max(n_left,n_right)+1
        return (n_left + 1);
    else
        return (n_right + 1);       
}

void print_bfs_tree(node *root) {
    if (!root) return;

    int h = tree_height(root);
    int max_width = (1 << h) * 2;

    node **level_nodes = malloc(sizeof(node*) * (1 << h));
    int count = 1;
    level_nodes[0] = root;

    for (int level = 0; level < h; level++) {
        int nodes_in_level = 1 << level;
        int spacing = max_width / nodes_in_level;
        int branch_offset = spacing / 4;

        // Print nodes
        for (int i = 0; i < nodes_in_level; i++) {
            if (i == 0) print_spaces(spacing / 2);
            else print_spaces(spacing);

            if (level_nodes[i] != NULL) {
                printf("%2d", level_nodes[i]->key);
            } else {
                printf("  ");
            }
        }
        printf("\n");

        // Print branches
        if (level < h - 1) {
            for (int i = 0; i < nodes_in_level; i++) {
                if (i == 0) print_spaces(spacing / 2);
                else print_spaces(spacing);

                if (level_nodes[i] != NULL) {
                    printf("%c", level_nodes[i]->left ? '/' : ' ');
                    print_spaces(1);
                    printf("%c", level_nodes[i]->right ? '\\' : ' ');
                } else {
                    print_spaces(3);
                }
            }
            printf("\n");
        }

        // Prepare next level
        node **next_level = malloc(sizeof(node*) * (1 << h));
        for (int i = 0; i < nodes_in_level; i++) {
            next_level[i * 2] = (level_nodes[i] ? level_nodes[i]->left : NULL);
            next_level[i * 2 + 1] = (level_nodes[i] ? level_nodes[i]->right : NULL);
        }
        free(level_nodes);
        level_nodes = next_level;
    }
    free(level_nodes);
}
