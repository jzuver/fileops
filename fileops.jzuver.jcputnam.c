#include "fileops.jzuver.jcputnam.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>



int checkWord(char *word) {
    int i, len;

    len = strlen(word);
    for (i=0; i<len; ++i) {
        if ( ! isalpha(word[i]) )
            return 1;
    }

    return 0;
}


int convertToLower(char *word, char *convertedWord) {
    int i, len;

    strcpy(convertedWord, word);

    len = strlen(word);
    for (i=0; i<len; ++i)
        convertedWord[i] = tolower(word[i]);

    return 0;
}


int main() {
    char word[MAXWORDLEN + 1];
    char lcword[MAXWORDLEN + 1];
    strcpy(word, "NargGLEs");
    convertToLower(word, lcword);

    printf("%s", word);
    printf("\n%s", lcword);

}