#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

/* Print list data on a single line, 
   with a space AFTER EACH integer that is printed.*/
// Corrected implementation
void printList(struct ListNode* head)
{
    while (head != NULL)
    {
        printf("%d", head->data);
        head = head->next;
        if (head != NULL)
        {
            printf(" ");
        }
    }
    printf("\n");
}

// Created by CS136L staff
int main(void){
    struct ListNode *node = createNode(5);
    node->next = createNode(10);
    printList(node);
    freeList(node);
    printf("List has been freed.\n");

    return 0;
}
