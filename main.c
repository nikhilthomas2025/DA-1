#include <stdio.h>

struct Book{
    char BookID[5];
    char Title[50];
    char Author[30];
    unsigned int status:1;
    char regno[9];
};

void init(){
    FILE *fp=fopen("record.txt","r");
    fclose(fp);
    if(fp==NULL){
        fp=fopen("record.txt","w");
        fclose(fp);
    }

}
int main(){
    init();
    int ch=0;

    while(ch!=6){
        printf("\n\n\n======================");
        printf("\n1.Add New Book to Library\n2.Issue Book to Student\n3.Return Book\n4.Search Book\n5.Display Library\n6.Exit\n");
        printf("\nEnter Choise: ");
        scanf("%d",&ch);
    }

    return 0;}