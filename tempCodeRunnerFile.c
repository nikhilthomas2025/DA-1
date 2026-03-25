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