#include <stdio.h>
#include <stdlib.h>

// general concept: head node points to newest element but inst a node, its only a var
// nodes always point to their previous node (newer nodes point to older ones)

struct listelement{
        int data;
        struct listelement *next;
};
typedef struct listelement listelement;
 
listelement *list_head = NULL; 				//points to newest element


int insertElement(int value){
    if(value < 0){ return -1;} 				// only non-neg ints allowed

    listelement *node = list_head; 			// need this later to check if value already exists
    
    // allocate memory with malloc
    listelement *newElement = malloc(sizeof(listelement));
        
    if(newElement == NULL) { return -1;}				// if error in allocation process


    // fill newElement if memory allocation was successful
    newElement->data = value; 				// == (*newElement).data = value
    newElement->next = NULL;

    if(node != NULL){						// list not empty, node is initialized as list_head

        // check if new
        while(node->next != NULL){
            if(node->data == value){
                free(newElement); 			// free the allocated memory as we dont need it
                return -1; 					// if value is already in list then return
            } 
            node = node->next; 				// go to next node

        }
        
        // everything went according to plan so far, so actually do insert
        newElement->next = list_head; 		// newElement points to previous newest element        
    }

    list_head = newElement; 				// update list_Head

    return value;
}

int removeElement(void){
	int returndata = -1;
    if (list_head == NULL){return -1;} 		// if list empty 
    else{
        listelement *node = list_head;
        listelement *previous = NULL; 		// keep track of element previous to the one we are currently looking at
        
        // search oldest element by backtracking from newest one
        while(node->next != NULL){
            previous = node;
            node = node->next;
        }
        previous->next = NULL; 				// remove reference to deleted node
        
		returndata = node->data; 			// need to save data before freeing memory
		
        // free allocated memory
        free(node);
        return returndata;
    }
}

// test function, goes through the list one by one
void print_entries(){
	
    listelement *entry = list_head;
	while(entry != NULL){
		printf("%d ", (*entry).data);
		entry = (*entry).next;
	} 
	printf("\n");	
}


int main(void) {
    printf("insert 47: %d \n", insertElement(47)); 
    printf("insert 11: %d \n", insertElement(11)); 
    printf("insert 23: %d \n", insertElement(23)); 
    printf("insert 11: %d \n", insertElement(11)); 

    printf("insert -2: %d \n", insertElement(-2));
     
	print_entries();
	
	printf("remove: %d \n", removeElement());
	printf("remove: %d \n", removeElement());
	
   
    print_entries();

    return 0;
}
