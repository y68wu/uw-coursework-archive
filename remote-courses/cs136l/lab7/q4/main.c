#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

/* Return length of the list. */
// Corrected implementation
int listLength(struct ListNode* head)
{
    int i = 0;
    while (head != NULL)
    {
        i++;
        head = head->next;
    }
    return i;
}

// Created by CS136L staff
int main(void){

    struct ListNode *node = createNode(5);
    node->next = createNode(10);
    node->next->next = createNode(15);
    node->next->next->next = createNode(20);
    
    printList(node);
    printf("First List Length = %d\n",listLength(node));

    freeList(node);

    struct ListNode *bla = NULL;
    printf("Second List Length = %d\n",listLength(bla));

    return 0;
}
