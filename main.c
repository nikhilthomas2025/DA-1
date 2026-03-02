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
    return 0;}