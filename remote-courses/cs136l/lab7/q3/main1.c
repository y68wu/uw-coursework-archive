#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

/* Print list data on a single line, 
   with a space AFTER EACH integer that is printed.*/
// Untouched student implementation
void printList(struct ListNode* head)
{
    if (head != NULL)
    {
        while (head->next != NULL)
        {
            printf("%d\n", head->data);
            head = head->next;
        }
    }
}

// Created by CS136L staff
int main(void){
    struct ListNode *node = createNode(5);
    node->next = createNode(10);
    printList(node);
    freeList(node);
    printf("List has been freed.\n");
}
