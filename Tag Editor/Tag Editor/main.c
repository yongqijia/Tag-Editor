#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


struct id3tagEd {
    char identifier[3];
    char song_title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    char zero_byte_separator;
    unsigned char track;
    char genre;
};

int show_content(struct id3tagEd id3tag){
    int i = 0;
    printf("Song title: ");
    for (i = 0; i < 30; i++) {
        printf("%c", id3tag.song_title[i]);
    }
    printf("\n");
    printf("Artist: ");
    for (i = 0; i < 30; i++) {
        printf("%c", id3tag.artist[i]);
    }
    printf("\n");
    printf("Album: ");
    for (i = 0; i < 30; i++) {
        printf("%c", id3tag.album[i]);
    }
    printf("\n");
    printf("Year: ");
    for (i = 0; i < 4; i++) {
        printf("%c", id3tag.year[i]);
    }
    printf("\n");
    printf("Comment: ");
    for (i = 0; i < 28; i++) {
        printf("%c", id3tag.comment[i]);
    }
    printf("\n");
    
    printf("Zero byte separator: %c\n", id3tag.zero_byte_separator);
    printf("Track number byte: %u\n", id3tag.track);
    printf("Genre identifier byte: %c\n", id3tag.genre);
    
    
    return 0;
}

int checkreadable(char *argv[]){
    int check;
    if(access((argv[1]), F_OK) == -1){
        printf("Error: File is not found.");
        return check = 1;
    }
    return check = 0;
}

int setfield(char *argv[], int argc, struct id3tagEd id3tag, FILE *f){
    int i = 0;
    for (i = 2; i < argc; i += 2) {
        if (strcmp(argv[i], "-title") == 0) {
            strncpy(id3tag.song_title, argv[i + 1], 30);
        }
        if (strcmp(argv[i], "-artist") == 0) {
            strncpy(id3tag.artist, argv[i + 1], 30);
        }
        if (strcmp(argv[i], "-album") == 0) {
            strncpy(id3tag.album, argv[i + 1], 30);
        }
        if (strcmp(argv[i], "-year") == 0) {
            strncpy(id3tag.year, argv[i + 1], 4);
        }
        if (strcmp(argv[i], "-comment") == 0) {
            strncpy(id3tag.comment, argv[i + 1], 28);
        }
        if (strcmp(argv[i], "-track") == 0) {
            id3tag.track = atoi(argv[i + 1]);
        }
    }
    show_content(id3tag);
    fwrite(&id3tag, 128, 1, f);
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *f;
    struct id3tagEd id3tag;

    if (argc == 1) {
        printf("Error: No file import.\n");
        return 0;
    }else if(argc == 2){
        int check = checkreadable(argv);
        if(check){
            return 0;
        }
        f = fopen(argv[1], "rb");
        fseek(f, -128, SEEK_END);
        fread(&id3tag, 128, 1, f);
        int len = 3;
        if (strncmp(id3tag.identifier, "TAG", len) != 0) {
            printf("Error: Identifier string is not \"TAG\".");
            return 0;
        }
        show_content(id3tag);
        return 0;
    }else{
        int check = checkreadable(argv);
        if(check){
            return 0;
        }
        if(argc % 2 == 1){
            printf("Error: Missing field or value information.\n");
            return 0;
        }
        f = fopen(argv[1], "rb");
        fseek(f, -128, SEEK_END);
        fread(&id3tag, 128, 1, f);
        int len = 3;
        if (strncpy(id3tag.identifier, "TAG", len) != 0) {
            fseek(f, 0, SEEK_END);
            strncpy(id3tag.identifier, "TAG", 3);
            strncpy(id3tag.song_title, "0", 30);
            strncpy(id3tag.artist, "0", 30);
            strncpy(id3tag.album, "0", 30);
            strncpy(id3tag.year, "0", 4);
            strncpy(id3tag.comment, "0", 28);
            id3tag.zero_byte_separator = '0';
            id3tag.track = '0';
            id3tag.genre = '0';
        }else{
            fseek(f, -128, SEEK_END);
        }
        setfield(argv, argc, id3tag, f);
    }
    return fclose(f);
}
