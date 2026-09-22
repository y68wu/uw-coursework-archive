/*
 * The code in this file was provided by our Student's instructor
 * A correctly implemented version of the functions declared in this
 * header are available in the pre-compiled linkedlist.o binary
 * For this question, the linkedlist.o binary was compiled using
 * the gcc compiler with the following command:
 * gcc -Wall -O0 -g -c linkedlist.c
 * Therefore, it will make sense for you to also use gcc
 * and then use Valgrind as the memory checker.
  */

struct ListNode {
  int data;
  struct ListNode *next;
};

/* Alloc a new node with given data. */
struct ListNode* createNode(int inputData);

/* Free memory used by the list. */
void freeList(struct ListNode* head);

/* Print list data on single line, separated with spaces. */
void printList(struct ListNode* head);
