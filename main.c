#include <stdio.h>

struct Book{
    char BookID[5];
    char Title[50];
    char Author[30];
    unsigned int status:1;
    char regno[9];
};

void init(){
    FILE *fp=fopen("record.bin","rb");
    fclose(fp);
    if(fp==NULL){
        fp=fopen("record.bin","wb");
        fclose(fp);
    }

}
int main(){
    init();
    int ch=0;

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