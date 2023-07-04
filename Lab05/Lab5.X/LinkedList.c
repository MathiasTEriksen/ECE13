// Mathias Eriksen
// mterikse@ucsc.edu

#include <stdio.h>
#include <string.h>

#include "BOARD.h"
#include "LinkedList.h"



// Created a New List Item
ListItem *LinkedListNew(char *data)
{      
    struct ListItem *Item = malloc(sizeof(ListItem));
    // allocate memory of the list item, and save the pointer
    // to the list item in Item
    if (Item == NULL) {
        return NULL;
        // Always check for NULL address, Null address will not work
    } else {
              
        Item -> data = data; 
        // Set the data in the list item
        Item -> previousItem = NULL; 
        Item -> nextItem = NULL;  
        // Initiate the previous and next to null, since it is the only
        // item in the list
        return Item;
        // return a pointer to the item
    }
}



// create a new item after the item passed in the argument with
// the data passed
ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    struct ListItem *NewItem;
    // Initiate the new item using its structure
    NewItem = LinkedListNew(data);   
    // Use the previous function to initiate the new item, allocate
    // it to memory, and get its address, as well as giving it the
    // passed data
    
    if (NewItem == NULL){
        return NULL;
        // always check for null address
    }
    
    if (item == NULL){
        /*
        NewItem -> nextItem = (item -> nextItem);
        NewItem -> previousItem = NULL;
        
        item -> previousItem = NewItem;  
         */      
        return NULL;
        // asked to handle a passed null *item address, but do not see
        // how this would be possible since null address has no links
        // to the list
        
    } else if (item -> nextItem != NULL){ // in middle 
        // if the item is not the last item in the list
        NewItem -> nextItem = (item -> nextItem);
        NewItem -> previousItem = item;
        // Puts the new item in between the passed item, and the 
        // passed items next item
        
        item -> nextItem -> previousItem = NewItem ; 
        // fix the item in front of the passed item's previous item
        // to be the new item
        item -> nextItem = NewItem;
        // fix the original item at the passed address' next item to be
        // the new item
                
    } else if (item -> nextItem == NULL){ // insert at tail
        // check if the item we are putting the new item in front of is 
        // at the end of the list
        NewItem -> nextItem = NULL;
        // the next item will be null since it is at the end
        NewItem -> previousItem = item;
        // previous item will be passed item
        
        item -> nextItem = NewItem; 
        // fix passed items next item from null to new item, since it
        // is no longer the end of the list
        
        // don't have to fix items next items previous item like above,
        // since it is NULL
    }   
    
    return NewItem;
    // return pointer to the newly inserted item
}



// removes the item at the passed address from the linked list
char *LinkedListRemove(ListItem *item) 
{
    if (item == NULL){
        return NULL;
    }
    
    char*Data;
    Data = item -> data;
    // save the data in the item we are removing, to be returned from
    // the function
    
    if (item -> nextItem != NULL){
        // if the passed items next item is NULL we don't have to fix the next
        // items previous item
        (item -> nextItem) -> previousItem = (item -> previousItem);
        // if the passed item has a next item, we have to fix its previous
        // item to be the removed item's previous item
    }
    
    if (item -> previousItem != NULL) {
        // if the passed items previous item is NULL we don't have to fix the
        // previous items next item
        (item -> previousItem) -> nextItem = (item -> nextItem);
        // if the passed item has a previous item, its next item needs
        // to be set to the removed items next item
    }
    // this will remove the links from the removed item, and fix the items
    // next to it to have the correct links
    free(item);
    // free the memory the removed item was taking up
    return Data;
    // return the data in the removed item that we saved above
}



// returns the size of the list that the passed item address is within
int LinkedListSize(ListItem *list)
{
    int i = 0;
    // initialize a counter variable
    
    if (list == NULL){
        return 0;
        // if the address is null, there is no size
    } else { 
        list = LinkedListGetFirst(list);
        // get the address for the first item in the list (head)
        while (list != NULL){
            // stops at the end(tail) of list, because it is null
            i++;
            // iterate counter
            list = list -> nextItem;  
            // move through the list
        }
    }
    return i;  
    // return the iterated counter, which now contains the size of
    // the linked list
}



// is passed an address to an item in a list, and returns an address
// to the first item in the list
ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL){
        return NULL;
        // return null if address passed is null
    } else {
        while (list -> previousItem != NULL){
            // stop moving in list when we are at the head
            list = list -> previousItem;
            // move back through list, until we get to the head
        }
    }
    return list;
    // return the address to the list item that is at the head of the linked
    // list
}



// same as get first, just moving forward in the list in order to get
// to the last item, and return address to the tail of the linked list
ListItem *LinkedListGetLast(ListItem *list)
{
   if (list == NULL){
        return NULL;
    } else {
        while (list -> nextItem != NULL){
            list = list -> nextItem;
        }
    }
    return list; 
}



// swaps the data in two list items within a linked list
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem == NULL || secondItem == NULL){
        // if either address is null, we have an error because a null
        // address is not in the list
        return STANDARD_ERROR;
    } else {
        char * intval;
        intval = firstItem -> data;
        // save the data in the first item
        
        firstItem -> data = secondItem -> data;
        // set the data in the first item to be the data in the second item
        secondItem -> data = intval;
        // set the data in the second item to be the saved data that was
        // in the first item before it was equated to the data in the
        // second item
        
        return SUCCESS;
    }
}



// prints out every item from the list the passed address is within
int LinkedListPrint(ListItem *list)
{
    if (list == NULL){
        return STANDARD_ERROR;
    }
    printf("[");
    list = LinkedListGetFirst(list);
    // get the address of the first item in the list using our get first
    // function, we have to start printing at the head
    while (list != NULL) {
        // stops when we get to the end of the list
        printf("%s, ", list -> data);
        // print the data that is in the item we are currently in
        list = list -> nextItem;
        // moves to the next item
    }
    printf("]\n");
    // for formatting
    return SUCCESS;
}