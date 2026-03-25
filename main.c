#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#define FILE_NAME "record.bin"

struct Book_Entry{
    char BookID[11];
    char Title[40];
    char Author[40];
    int status;          // safer than bit-field
    char regno[9];
};

typedef struct Book_Entry Book;

void save_to_indexeddb() {
#ifdef __EMSCRIPTEN__
    // This tells Emscripten to sync the virtual FS to IndexedDB
    EM_ASM(
        FS.syncfs(false, function (err) {
            if (err) console.error('Error syncing to IndexedDB', err);
        });
    );
#endif
}

void init(){
    FILE *fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        fp = fopen(FILE_NAME, "wb");
    }
    if(fp != NULL) fclose(fp);
}

void AddBook() {
    Book bk;

    printf("\n\n===== Enter Details =====\n");

    getchar(); // clear buffer

    printf("Book ID  : ");
    fgets(bk.BookID, sizeof(bk.BookID), stdin);
    bk.BookID[strcspn(bk.BookID, "\n")] = 0;

    printf("Title    : ");
    fgets(bk.Title, sizeof(bk.Title), stdin);
    bk.Title[strcspn(bk.Title, "\n")] = 0;

    printf("Author   : ");
    fgets(bk.Author, sizeof(bk.Author), stdin);
    bk.Author[strcspn(bk.Author, "\n")] = 0;

    bk.status = 0;
    bk.regno[0] = '\0';

    FILE* fp = fopen(FILE_NAME, "ab");
    if(fp == NULL){
        printf("\nError opening file.\n");
        return;
    }

    if(fwrite(&bk, sizeof(Book), 1, fp) == 1){
        printf("\n====== Book Saved Successfully ======\n");
    } else {
        printf("\n===== ERROR: Book NOT Saved =====\n");
    }

    fclose(fp);
}

void IssueBook(){
    Book bk;
    char ID[11], regno[9];

    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%10s", ID);
    printf("RegNo   : "); scanf("%8s", regno);

    FILE* fp = fopen(FILE_NAME, "rb+");
    if(fp == NULL){
        printf("\nError opening file.\n");
        return;
    }

    int flag = 0;
    while(fread(&bk, sizeof(Book), 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;
            if(bk.status){
                printf("\nBook is already issued.\n");
            } else {
                bk.status = 1;
                strcpy(bk.regno, regno);
                fseek(fp, -sizeof(Book), SEEK_CUR);
                fwrite(&bk, sizeof(Book), 1, fp);
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
    char ID[11];

    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%10s", ID);

    FILE* fp = fopen(FILE_NAME, "rb+");
    if(fp == NULL){
        printf("\nError opening file.\n");
        return;
    }

    int flag = 0;
    while(fread(&bk, sizeof(Book), 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;
            if(bk.status){
                bk.status = 0;
                bk.regno[0] = '\0';
                fseek(fp, -sizeof(Book), SEEK_CUR);
                fwrite(&bk, sizeof(Book), 1, fp);
                printf("\n=====Book Returned=====\n");
            } else {
                printf("\nBook is Not issued.\n");
            }
            break;
        }
    }

    if(!flag) printf("\n====Book Not Found=====\n");

    fclose(fp);
}

void SearchBook(){
    Book bk;
    char ID[11];

    printf("\n\n=====Enter Details=====\n");
    printf("Book ID : "); scanf("%10s", ID);

    FILE* fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        printf("\nError opening file.\n");
        return;
    }

    int flag = 0;
    while(fread(&bk, sizeof(Book), 1, fp)){
        if(strcmp(bk.BookID, ID) == 0){
            flag = 1;

            printf("\n======Book Found=======\n");
            printf("\nBook ID: %s", bk.BookID);
            printf("\nTitle  : %s", bk.Title);
            printf("\nAuthor : %s", bk.Author);

            if(bk.status)
                printf("\nIssued to: %s\n", bk.regno);
            else
                printf("\nNot Issued.\n");

            break;
        }
    }

    if(!flag) printf("\n====Book Not Found=====\n");

    fclose(fp);
}

void Display(){
    Book bk;

    printf("\n\n========Library========\n");

    FILE* fp = fopen(FILE_NAME, "rb");
    if(fp == NULL){
        printf("\nError opening file.\n");
        return;
    }

    while(fread(&bk, sizeof(Book), 1, fp)){
        printf("\nBook ID: %s", bk.BookID);
        printf("\nTitle  : %s", bk.Title);
        printf("\nAuthor : %s", bk.Author);

        if(bk.status)
            printf("\nIssued to: %s", bk.regno);
        else
            printf("\nNot Issued.");

        printf("\n======================\n");
    }

    fclose(fp);
}

int ch = 0;
void menu_tick(){
    printf("\n\n=======================\n");
        printf("1.Add New Book\n2.Issue Book\n3.Return Book\n4.Search Book\n5.Display Library\n6.Exit\n");
        printf("Enter Choice: ");

        if(scanf("%d", &ch) != 1){
            printf("Invalid input.\n");
            while(getchar() != '\n'); // clear buffer
            return;
        }

        switch(ch){
            case 1: AddBook(); break;
            case 2: IssueBook(); break;
            case 3: ReturnBook(); break;
            case 4: SearchBook(); break;
            case 5: Display(); break;
            case 6:
    printf("\n========EXITING========\n");
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#else
    exit(0);
#endif
    break;
            default: printf("\n====Invalid Choice====\n");
        }
}

int main(){
#ifdef __EMSCRIPTEN__
    EM_ASM(
        if (!FS.analyzePath('/data').exists) {
            FS.mkdir('/data');
        }
        FS.mount(IDBFS, {}, '/data');
        FS.syncfs(true, function (err) {
            if (err) console.error("Error loading IDBFS:", err);
        });
    );
#endif

    init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(menu_tick, 0, 1);
#else
    while (ch != 6) {
        menu_tick();
    }
#endif

    return 0;
}