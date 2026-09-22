#include <stdio.h>
#include <stdlib.h>

// definition provided by Student's instructor
struct ListNode {
    int data;
    struct ListNode *next;
};

/* Alloc a new node with given data. */
// Corrected implementation
struct ListNode* createNode(int inputData)
{
    struct ListNode *newNodeCN = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNodeCN == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNodeCN->data = inputData;
    newNodeCN->next = NULL; // initialize next to NULL
    return newNodeCN;
}

// Created by CS136L staff
int main(void){
    struct ListNode *node = createNode(5);
    node->next = createNode(10);
    
    printf("First node's data: %d\n", node->data);
    printf("Second node's data: %d\n", node->next->data);

    free(node->next);
    free(node);
}
