#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc < 2) return 0;
    char* result = NULL;
    for(int i = 1; i < argc; i++){
        int length = result == NULL ? 0 : strlen(result);
        char* new_string = realloc(result, (length + strlen(argv[i])));
        if (!new_string){
            perror("realloc");
            free(result);
            return 1;
        }
        result = new_string;
        strcpy(result + length, argv[i]);
    }
    printf("%s\n", result);
    free(result);
}