#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book_Entry{
    char BookID[5];
    char Title[40];
    char Author[40];
    unsigned int status:1;
    char regno[9];
};

typedef struct Book_Entry Book;
void init(){
    FILE *fp=fopen("record.bin","rb");
    fclose(fp);
    if(fp==NULL){
        fp=fopen("record.bin","wb");
        fclose(fp);
    }

}

void AddBook(){
    Book bk;char temp[40];
    printf("\n\n=====Enter Details=====\n");
    printf("Book ID  : ");scanf("%5s",bk.BookID);
    printf("Title    : ");scanf("%s",bk.Title);
    printf("Author   : ");scanf("%s%s",bk.Author,temp);strcat(bk.Author," ");strcat(bk.Author,temp);
    bk.status=0;
    strcpy(bk.regno,"");

    

}


int main(){
    init();
    int ch=0;
    AddBook();
    return 1;
    while(ch!=6){
        printf("\n\n=======================\n");
        printf("\n1.Add New Book to Library\n2.Issue Book to Student\n3.Return Book\n4.Search Book\n5.Display Library\n6.Exit\n");
        printf("\nEnter Choise: ");scanf("%d",&ch);
        switch(ch){
            case 1:printf("\n1\n");break;
            case 2:printf("\n2\n");break;
            case 3:printf("\n3\n");break;
            case 4:printf("\n4\n");break;
            case 5:printf("\n5\n");break;
            case 6:printf("\n========EXITING========\n");break;
            default:printf("\n====Invalid  Choise====\n");break;
        }
    }

    return 0;}