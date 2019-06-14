#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 30
#define SUCCESS 1
#define FAILURE 0

//6 BYTES
typedef struct{
    char title[MAX];
    char author[MAX];
    int year;
}BookType;

typedef struct NodeType{
    struct NodeType *next;
    BookType *book;
}NodeType;


typedef struct{
    NodeType *head,*tail;
}ListType;

int initBook(BookType **book);
void initList(ListType *list);
void addByTitle(ListType *list, BookType *newBook); 
void addByYear(ListType *list,  BookType *newBook);
void copyList(ListType *src,ListType *dest);
void copyByYear(ListType *src,ListType *dest);
void delAuthor(ListType *list,char *name);
void printList(ListType *list);
void cleanupList(ListType *list);
void cleanupData(ListType *list);


int main(){
    //Initilize three ListTypes
    ListType booksByTitle, booksByYear, tmpList;
    initList(&booksByTitle);
    initList(&booksByYear);
    initList(&tmpList);
    BookType book;
    BookType *b = &book;
    char author[30]; 
    int success = 1;
    while (success){
        success = initBook(&b);
        //If return value is 0 meaning the user has decided to stop entering books
        if (success == 0){
            break;
        }
        addByTitle(&booksByTitle,b);
    }

    printf("BOOKS BY TITLE\n");
    printList(&booksByTitle);
    
    copyList(&booksByTitle,&tmpList);

    printf("Type an author's name to delete :\n");
    fgets(author,sizeof(author),stdin);
    author[strlen(author)-1] = '\0';
    
    printf("\n");
    
    delAuthor(&tmpList,author);
    printf("BOOKS BY TITLE\n");
    printList(&booksByTitle);
    printf("NEW BOOKS BY TITLE (WITHOUT AUTHOR SPECIFIED ABOVE)\n");
    printList(&tmpList);

    copyByYear(&booksByTitle,&booksByYear);

    printf("BOOKS BY TITLE\n");
    printList(&booksByTitle);

    printf("BOOKS BY YEAR\n");
    printList(&booksByYear); 
    
    
    cleanupData(&booksByTitle);
    cleanupList(&booksByTitle);
    cleanupList(&tmpList);
    cleanupList(&booksByYear);
    
    return 0;
}

/*
  Function:  initList
  Purpose:   Intilizes the list
  Parameters:
    inout:      Takes in listType pointer to initalize
*/

void initList(ListType *list){
    list->head = NULL;
    list->tail = NULL;
}

/*
  Function:  addByTitle
  Purpose:   to add a new Node with newBook in alphabetically order
  Parameters:
    inout: list pointer to be modified
    in: newBook pointer to be added 
    
*/

void addByTitle(ListType *list, BookType *newBook){
  NodeType *currNode, *prevNode;
  currNode = list->head;
  //Allocating memory for the new node 
  NodeType *newNode = (NodeType*)malloc(sizeof(NodeType));
  //Setting the new book
  newNode->book = newBook;
  
   //While the current Node has not reached the tail
  while (currNode!= NULL){
      //Compare the current Node title with the new Nodes title  
      if (strcmp(currNode->book->title,newNode->book->title) > 0){
          //If it's greater break which means we have the found the position where to insert the newNode
          break;
      }
      else{

          //Otherwise move to the next position by setting the previous node to the current and the current to the next node
          prevNode = currNode;
          currNode = currNode->next;
      }
  }

  /*After finding the position if the position was found when the current Node was the head set the new Node as the head and the new Nodes next
  pointer to the current Node*/
  if (currNode == list->head){
      newNode->next = currNode;
      list->head = newNode;
  }

  else{
      //Otherwise if the position is in middle of the list or the end set the previous node equal to the new Node and the new Node to the current Node 
      //This means the node has been inserted between two nodes
      prevNode->next = newNode;
      newNode->next = currNode;
      list->tail = currNode;
  }

};

/*
  Function:  copyList
  Purpose:   copy's list into another list
  Parameters:
    in:      source list (List pointer to be copied from)
    inout:   destination list (List pointer that will be adding nodes that contain the data and pointer to the next node)
*/

void copyList(ListType *src,ListType *dest){
    NodeType*currNode,*prevNode;
    currNode = src->head;

    //While the current Node has not reached the end
    while (currNode!= NULL){
        addByTitle(dest,currNode->book);
        prevNode = currNode;
        currNode = currNode->next;
    }
};

/*
  Function:  delAuthor
  Purpose:   To delete a specific author in a list
  Parameters:
    inout:   list to delete author from 
    in:      name to be taken in

*/

void delAuthor(ListType *list,char *name){

    NodeType *currNode, *prevNode,*newNode;
    currNode = list->head;
    
    while (currNode!=NULL){
        //If the current node's author is found when comparing to the name argument in the paramaters proceed to remove that current Node
        if (strcmp(name, currNode->book->author) == 0 ){
            
            //If the authors name is found at the head set the head to the next pointer of current Node
            if (currNode == list->head){
                list->head = currNode->next;
                prevNode = currNode;
                currNode = currNode->next;
                free(prevNode);
                
        
            } 
            //Otherwise if it found at the tail set the tail to the previous node so that it skips the current Node
            else if(currNode == list->tail){
                prevNode->next = NULL;
                list->tail = prevNode;
                free(currNode);

            }
            //If its in the middle of the list set the previous pointers next to the currents next that way your skippng currentNode
            else{
                prevNode->next = currNode->next;
                newNode = currNode;
                currNode = currNode->next;
                free(newNode);
            }
        
        }
        else{
            //Start to compare using the next pointers author (Moving through the list)
            prevNode = currNode;
            currNode = currNode->next;
        }
    }
};

/*
  Function:  printList
  Purpose:   Print a list's books title, author and year
  Parameters:
    out:     List to print out 
    
*/

void printList(ListType *list){

    NodeType *currNode, *prevNode;
    printf("BOOK LIST:\n");

    currNode = list->head;
    //If current Node is NULL which means there are no nodes in the list return nothing
    if (currNode == NULL){
        return;
    }
    
    if (currNode->next == NULL){
            printf("-- %30s by %s, Yr: %d\n",currNode->book->title,currNode->book->author,currNode->book->year);

        }
    //Otherwise iterate through the entire thing and print out the values for each node
    while(currNode->next!=NULL){
        printf("-- %30s by %s, Yr: %d\n",currNode->book->title,currNode->book->author,currNode->book->year);
        currNode = currNode->next;
        //TO print out the last element therefore the tail
        if (currNode->next == NULL){
            printf("-- %30s by %s, Yr: %d\n",currNode->book->title,currNode->book->author,currNode->book->year);

        }
    }



    //AFter it is done printing all the elements it will print out the head and the tail
   printf("--> Head is: --  %30s by %s, Yr: %d\n", list->head->book->title, list->head->book->author, list->head->book->year);
   printf("--> Tail is: --  %30s by %s, Yr: %d\n", currNode->book->title,currNode->book->author,currNode->book->year);
};

/*
  Function:  cleanupList
  Purpose:   To deallocate all the nodes in a list
  Parameters:
    inout:    List to deallocate all nodes  
    
*/

void cleanupList(ListType *list){
    NodeType *currNode, *newNode;
    currNode = list->head;

    //Iterate and get rid of all the nodes
    while(currNode!=NULL){
        newNode = currNode;
        currNode = currNode->next;
        free(newNode);
    }
};
/*
  Function:  cleanupData
  Purpose:   Deallocates book pointer that contain data from a list
  Parameters:
    inout:   List to deallocate books from    
*/

void cleanupData(ListType *list){
    NodeType *currNode,*newNode;
    currNode = list->head;
    //Iterate and get rid of all the nodes book
    while(currNode!=NULL){
        free(currNode->book);
        currNode = currNode->next;
    }
};
/*
  Function:  initBook
  Purpose:   To initilize a book 
  Parameters:
    inout:   Double pointer book to be initlized (With title,author,year) 
*/

int initBook(BookType **book){
    int year;
    char title[30], author[30],year_str[30];

    //Allocating a new book
    (*book) = (BookType*)malloc(sizeof(BookType));
    printf("What is the title of your book?\n");
    //Get user input for title
    fgets(title,sizeof(title),stdin);
    title[strlen(title)-1] = '\0';
    //Copying that into title
    strcpy((*book)->title,title);

    //If the title that was entered equaled "end" exit loop and return failure value which is 0
    if (strcmp(title,"end") == 0){
        strcpy(title,"");
        free(*book);
        return FAILURE;
    }
    //Otherwise prompt data from user for author
        printf("What is the author of your book?\n");
        fgets(author,sizeof(author),stdin);
        author[strlen(author)-1] = '\0';
        strcpy((*book)->author,author);
    //And prompt data from user for year
        printf("Year that it was published?\n");
        fgets(year_str,sizeof(year_str),stdin);
        sscanf(year_str,"%d",&year);
        (*book)->year = year;
        //return success value which is 1
        return SUCCESS;
    }

/*
  Function:  addByYear
  Purpose:   allocated new node to the list and sorts them by year in descending order
  Parameters:
    inout:   list pointer to be modified 
    in:      book to be added
    
*/
//Same functionailty as addByTitle (ONLY DIFFERENCE is your comparing integer values instead of comparing strings)
void addByYear(ListType *list,  BookType *newBook){
  NodeType *currNode, *prevNode;
  NodeType *newNode = (NodeType*)malloc(sizeof(NodeType));
  newNode->book = newBook;
  currNode = list->head;

  while (currNode!= NULL){
      if (currNode->book->year < newNode->book->year){
          break;
      }
      else{
          prevNode = currNode;
          currNode = currNode->next;
      }
  }

  if (currNode == list->head){
      newNode->next = currNode;
      list->head = newNode;
  }

  else{
      prevNode->next = newNode;
      newNode->next = currNode;
      list->tail = currNode;
  }

};


/*
  Function:  copyByYear
  Purpose:   declares winner of the race between the turtle and the hare
  Parameters:
    in:      map pointer array of characters 
    in:      position of turtle
    in:      position of hare
    
*/

void copyByYear(ListType *src,ListType *dest){
    NodeType *currNode,*prevNode;
    currNode = src->head;

    while (currNode!=NULL){
        addByYear(dest,currNode->book);
        prevNode = currNode;
        currNode = currNode->next;
    }
};
    
