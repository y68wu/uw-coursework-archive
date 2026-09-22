#include "linkedlist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Free memory used by the list. */
// Corrected implementation
void freeList(struct ListNode* head)
{
    struct ListNode* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Created by CS136L staff
int main(void){
    struct ListNode *node = createNode(5);
    node->next = createNode(10);
    
    printf("First node's data: %d\n",node->data);
    printf("Second node's data: %d\n",node->next->data);

    freeList(node);
    printf("List has been freed.\n");
}
