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


int insertWord(FILE *fp, char *word){
    char wordToConvert[MAXWORDLEN + 1];
    char lowerCaseWord[MAXWORDLEN + 1];
    strcpy(wordToConvert, word);
    convertToLower(wordToConvert, lowerCaseWord);
    printf("%s", lowerCaseWord);

}


int main() {

    FILE *fp;
    fp = fopen("word.dat", "w");

    insertWord(fp, "PigWidGeon");



}