#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// This function creates a dictionary struct and returns a pointer to it
dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    if (dict == NULL) {
        return NULL;
    }
    dict->root = NULL;
    dict->size = 0;
    return dict;
}

// Helper to prepare a new node. Notice the
// lack of a malloc() call in this function.
// Usage:
//   node_t *n = malloc(sizeof(node));
//   new_node(n, word);
// You don't need to use this function, but it is a nice helper.
// This code is correct and should not be modified.
void new_node(node_t* node, const char *key) {
    strcpy(node->word, key);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
}

// Helper: Larger of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// Helper to perform the "right" rotation.
// This code is correct and you should not modify it.
node_t *rotate_right(node_t *y)
{
    node_t *x = y->left;
    node_t *t = x->right;
    x->right = y;
    y->left = t;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    return x;
}

// Helper to perform the "left" rotation.
// This code is correct and you should not modify it.
node_t *rotate_left(node_t *x)
{
    node_t *y = x->right;
    node_t *t = y->left;
    y->left = x;
    x->right = t;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    return y;
}

// The balance factor of AVL tree (left - right)
int calculate_balance_factor(node_t *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height;
    int right_height;
    if (root->left == NULL) {
        left_height = 0; //handle null node
    } else {
        left_height = root->left->height;
    }
    if (root->right == NULL) {
        right_height = 0; //handle null node
    } else {
        right_height = root->right->height;
    }
    return left_height - right_height;
}

/*
 * This function is a way to insert a node in AVL tree
 * Only used in dict_insert function
 * root: A pointer to the dictionary's root
 * word: The word to inser, as a null-terminated string
 * num: The integer 0 if the word was successfully added
 *      or -1 if the word could not be added
 */
node_t *node_insert(node_t *root, const char *word, int *num) {
    if (root == NULL) {
        node_t *n = malloc(sizeof(node_t));
        //Check whether the malloc succeed
        if (n == NULL) {
            *num = -1;
            return NULL;
        }
        new_node(n, word);
        return n;
    }
    if (strcmp(root->word, word) > 0) {
        root->left = node_insert(root->left, word, num);
    }
    if (strcmp(root->word, word) < 0) {
        root->right = node_insert(root->right, word, num);
    }
    if (strcmp(root->word, word) == 0) {
        *num = -1;
        return root;
    }
    
    //Calculate the height of each root in iteration.
    root->height = max(root->left != NULL ? root->left->height : 0,
                      (root->right != NULL) ? root->right->height : 0) + 1;
    
    //Modify the unbalanced height.
    int bf_root = calculate_balance_factor(root);
    int bf_right = calculate_balance_factor(root->right);
    int bf_left = calculate_balance_factor(root->left);
    //Right rotation
    if (bf_root == 2 && bf_left == 1) {
        return rotate_right(root);
    }
    //Left rotation
    if (bf_root == -2 && bf_right == -1) {
        return rotate_left(root);
    }
    //Left-Right rotaion
    if (bf_root == 2 && bf_left == -1) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    //Right-Left rotation
    if (bf_root == -2 && bf_right == 1) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

int dict_insert(dictionary_t *dict, const char *word) {
    int num = 0;
    dict->root = node_insert(dict->root, word, &num);
    if (num == 0) {
        dict->size += 1;
        return 0;
    } else {
        return -1;
    }
}

/*
 * This function is a way to find a word in BST
 * Only used in dict_find function
 * root: A pointer to the dictionary's root
 * query: The word to search for, as a null-terminated string
 */
int node_find(node_t *root, const char *query) {
    if (root == NULL) {
        return 0;
    } else {
        if (strcmp(query, root->word) < 0) {
            return node_find(root->left, query);
        } else if (strcmp(query, root->word) > 0) {
            return node_find(root->right, query);
        } else {
            return 1;
        }
    }
}

int dict_find(const dictionary_t *dict, const char *query) {
    return node_find(dict->root, query);
}

/*
 * This function print all the words in BST in order
 * Only used in dict_print function
 * root: A pointer to the dictionary's root
 */
void node_print(node_t *root) {
    if (root == NULL) {
        return;
    }
    node_print(root->left);
    printf("%s\n", root->word);
    node_print(root->right);
}

void dict_print(const dictionary_t *dict) {
    node_print(dict->root);
}

/*
 * This function frees all memory in BST
 * Only used in dict_free function
 * root: A pointer to the dictionary's root
 */
void node_free(node_t *root) {
    if (root == NULL) {
        return;
    }
    //postorder to free nodes memory
    node_free(root->left); 
    node_free(root->right);
    free(root);
}

void dict_free(dictionary_t *dict) {
    node_free(dict->root);
    free(dict);}

dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *fh = fopen(file_name, "r");
    //Check if the file opened
    if (fh == NULL) {
        return NULL;
    }
    dictionary_t *dict = create_dictionary();
    //Check if the dictionary created
    if (dict == NULL) {
        fclose(fh);
        return NULL;
    }
    char word[128];
    while (fscanf(fh, "%s", word) != EOF) {
        dict_insert(dict, word);
    }
    
    fclose(fh);
    return dict;
}

/*
 * This function write all words in BST in a file
 * Only used in write_dict_to_text_file function
 * root: A pointer to the dictionary's root
 */
void write_node_to_file(node_t *node, FILE *fh) {
    if (node == NULL) {
        return;
    }
    write_node_to_file(node->left, fh);
    fprintf(fh, "%s\n", node->word);
    write_node_to_file(node->right, fh);
}
int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    FILE *fh = fopen(file_name, "w");
    if (fh == NULL) {
        printf("Failed to open file.\n");
        return -1;
    }
    
    write_node_to_file(dict->root, fh);
    fclose(fh);
    return 0;
}
