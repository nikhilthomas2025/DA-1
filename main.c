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

// Helper function to clear the input buffer
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void init(){
    FILE *fp = fopen("record.bin", "rb");
    if(fp == NULL){
        // File doesn't exist, create it
        fp = fopen("record.bin", "wb");
        if (fp != NULL) {
            fclose(fp);
        }
    } else {
        // File exists, just close it
        fclose(fp);
    }
}

void AddBook() {
    Book bk;
    printf("\n\n===== Enter Details =====\n");

    clearBuffer(); // Clear the newline left by previous scanf

    printf("Book ID  : ");
    fgets(bk.BookID, 5, stdin);
    bk.BookID[strcspn(bk.BookID, "\n")] = 0;
    clearBuffer(); // Clear remaining characters if input exceeded buffer

    printf("Title    : ");
    fgets(bk.Title, 40, stdin);
    bk.Title[strcspn(bk.Title, "\n")] = 0; 

    printf("Author   : ");
    fgets(bk.Author, 40, stdin);
    bk.Author[strcspn(bk.Author, "\n")] = 0; 

    bk.status = 0;
    strcpy(bk.regno, "");

    FILE* fp = fopen("record.bin", "ab");
    if (fp == NULL) {
        printf("\n===== ERROR: Could not open file =====\n");
        return;
    }
    
    size_t written = fwrite(&bk, sizeof(Book), 1, fp);
    
    if (fclose(fp) == 0 && written == 1) {
        printf("\n====== Book Saved Successfully ======\n");
    } else {
        printf("\n===== ERROR: Book NOT Saved =====\n");
    }
}

void IssueBook(){
    Book bk;
    char ID[5], regno[9];
    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%s", ID);
    printf("RegNo   : "); scanf("%s", regno);

    int size = sizeof(Book), flag = 0;
    FILE* fp = fopen("record.bin", "rb+");
    if (fp == NULL) {
        printf("\nError: Could not open record file.\n");
        return;
    }

    while(fread(&bk, size, 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;
            if(bk.status) {
                printf("\nBook is already issued.\n");
            } else { 
                bk.status = 1;
                strcpy(bk.regno, regno);
                fseek(fp, -size, SEEK_CUR);
                fwrite(&bk, size, 1, fp);
                printf("\n======Book Issued======\n");
            }
            break;
        }
    }
    if(!flag) printf("\n====Book Not Found=====\n");

    fclose(fp);
}

void ReturnBook(){
    Book bk;
    char ID[5];
    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%s", ID);

    int size = sizeof(Book), flag = 0;
    FILE* fp = fopen("record.bin", "rb+");
    if (fp == NULL) {
        printf("\nError: Could not open record file.\n");
        return; // Removed fclose(fp) here
    }

    while(fread(&bk, size, 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;
            if(bk.status){
                bk.status = 0;
                strcpy(bk.regno, "");
                fseek(fp, -size, SEEK_CUR);
                fwrite(&bk, size, 1, fp);
                printf("\n=====Book Returned=====\n");
            }
            else printf("\nBook is Not issued.\n");
            break;
        }
    }
    if(!flag) printf("\n====Book Not Found=====\n");

    fclose(fp);
}

void SearchBook(){
    Book bk;
    char ID[5];
    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%s", ID);

    int size = sizeof(Book), flag = 0;
    FILE* fp = fopen("record.bin", "rb");
    if (fp == NULL) {
        printf("\nError: Could not open record file.\n");
        return;
    }

    while(fread(&bk, size, 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;
            printf("\n======Book Found=======\n");
            // Fixed missing %s format specifiers
            printf("Book ID: %s\n", bk.BookID);
            printf("Title  : %s\n", bk.Title);
            printf("Author : %s\n", bk.Author);
            if(bk.status) printf("Issued to: %s\n", bk.regno);
            else printf("Status : Not Issued\n");

            break;
        }
    }
    if(!flag) printf("\n====Book Not Found=====\n");

    fclose(fp);
}

void Display(){
    Book bk;
    printf("\n\n========Library========\n");

    int size = sizeof(Book);
    FILE* fp = fopen("record.bin", "rb");
    if (fp == NULL) {
        printf("\nError: Could not open record file.\n");
        return;
    }
    
    int count = 0;
    while(fread(&bk, size, 1, fp)){
        count++;
        // Fixed missing %s format specifiers
        printf("\nBook ID: %s", bk.BookID);
        printf("\nTitle  : %s", bk.Title);
        printf("\nAuthor : %s", bk.Author);
        if(bk.status) printf("\nIssued to: %s", bk.regno);
        else printf("\nStatus : Not Issued");
        printf("\n-----------------------");
    }
    
    if (count == 0) {
        printf("\nThe library is currently empty.\n");
    }
    printf("\n=======================\n");
    fclose(fp);
}

int main(){
    init();
    int ch = 0;

    while(ch != 6){
        printf("\n\n=======================\n");
        printf("1. Add New Book to Library\n2. Issue Book to Student\n3. Return Book\n4. Search Book\n5. Display Library\n6. Exit\n");
        printf("Enter Choice: ");
        
        // Prevent infinite loop if user enters a letter instead of a number
        if (scanf("%d", &ch) != 1) {
            clearBuffer();
            printf("\n====Invalid Choice====\n");
            continue;
        }
        
        switch(ch){
            case 1: AddBook(); break;
            case 2: IssueBook(); break;
            case 3: ReturnBook(); break;
            case 4: SearchBook(); break;
            case 5: Display(); break;
            case 6: printf("\n========EXITING========\n"); break;
            default: printf("\n====Invalid Choice====\n"); break;
        }
    }

    return 0;
}