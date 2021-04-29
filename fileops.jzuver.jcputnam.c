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
    char buffer[MAXWORDLEN*8];
    strcpy(wordToConvert, word);
    convertToLower(wordToConvert, lowerCaseWord);
    printf("%s", lowerCaseWord);
    int num = lowerCaseWord[0] - 'a';
    int offset = 8*num;
    fseek(fp, offset,SEEK_SET);
    //fread(buffer, 8)
    return 0;

}


int main() {

    FILE *fp;
    fp = fopen("word.dat", "w");
    long lp[26];
	long n = 0;
   	for(int i=0;i<26;++i){
   		lp[i] = n;
    }
   	fwrite(lp, sizeof(long), 26, fp);
   	int filesize = ftell(fp);
   	printf("%d", filesize);
    //insertWord(fp, "PigWidGeon");



}
