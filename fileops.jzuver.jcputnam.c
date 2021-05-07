#include "fileops.jzuver.jcputnam.h"
#include <stdio.h>
#include <stdlib.h>
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



int insertWord(FILE *fp, char *word){ //Still need to use checkword
    char wordToConvert[MAXWORDLEN + 1];
    char lowerCaseWord[MAXWORDLEN + 1];
    long buffer[sizeof(Record)];
    long fileSize[1];
    strcpy(wordToConvert, word);
    convertToLower(wordToConvert, lowerCaseWord);
    //printf("%s", lowerCaseWord);
    int num = lowerCaseWord[0] - 'a';
    int offset = 8*num;
    fseek(fp, offset, SEEK_SET);
    fread(buffer, 8, 1, fp);
    //printf("\n\n%ld buffer", *buffer);


    if(*buffer == 0){
    	// seek end to find filesize
    	fseek(fp, 0L, SEEK_END);
    	fileSize[0] = ftell(fp);

    	// seek to position of index to write (offset is index position)
    	//printf("\n%i filesize", fileSize[0]); //debug print stmnt
    	fseek(fp, offset, SEEK_SET);
    	fwrite(fileSize, sizeof(long), 1, fp);
    	fseek(fp, 0L, SEEK_END); // go back to end to write record

    	// create record to be written to end
    	Record *rec = (Record*) malloc(sizeof(Record));
    	strcpy(rec->word, lowerCaseWord);
    	rec->nextpos = 0;
    	fwrite(rec, sizeof(Record), 1, fp);
    }
    // already a word with that letter. search until you get to the one that has nextpos = 0
    else{
    	int rc;
    	char originalRecordWord[64];
    	rc = fseek(fp, 0L, SEEK_END);
    	*fileSize = ftell(fp);
    	int v = *buffer;
    	Record *record = (Record*) malloc(sizeof(Record));
    	rc = fseek(fp, v, SEEK_SET);
    	fread(record, sizeof(Record), 1, fp);
    	strcpy(originalRecordWord, record->word);
    	while(record->nextpos != 0){
        	rc = fseek(fp, record->nextpos, SEEK_SET);
        	rc = fread(record, sizeof(Record), 1, fp);
    	}
    	fseek(fp, -sizeof(Record), SEEK_CUR);
    	//we're at the end of the thing

    	//make this record the same but pointing to the end of the file
    	Record *record1 = (Record*) malloc(sizeof(Record));
    	strcpy(record1->word, record->word);
    	record1->nextpos = *fileSize;
    	rc = fwrite(record1, sizeof(Record), 1, fp);
    	rc = fseek(fp, 0L, SEEK_END);

    	//jump to the end of the file, add new record
    	Record *record2 = (Record*) malloc(sizeof(Record));
    	strcpy(record2->word, word);
    	record2->nextpos = 0;
    	rc = fwrite(record2, sizeof(Record), 1, fp);


    }
    return 0;

}


int countWords(FILE *fp, char letter, int *count){
    int numWords = 0;
    int num = letter - 'a';
    int offset = 8*num;
    long buffer[MAXWORDLEN*8*100];

    printf("%d", offset);
    if (fp == NULL){
        return *count;
    }

    // fseek to where letter first word should be
    fseek(fp, offset,SEEK_SET);
    fread(buffer, 8, 1, fp);

    if (*buffer == 0){
        return *count;
    }
    else{
        fseek(fp, *buffer, SEEK_SET);
        Record *record = (Record*) malloc(sizeof(Record));
        fread(record, sizeof(Record), 1, fp);
        *count = *count + 1;
        while(record->nextpos != 0){
            *count = *count + 1;
            fseek(fp, record->nextpos, SEEK_SET);
            fread(record, sizeof(Record), 1, fp);
        }
        return *count;
    }
}


int main() {
    int wordCount = 0;
    FILE *fp;
    fp = fopen("word.dat", "w+");
    long lp[26];
	long n = 0;
   	for(int i=0;i<26;++i){
   		lp[i] = n;
    }
   	fwrite(lp, sizeof(long), 26, fp);
   	int filesize = ftell(fp);
   	//printf("%d", filesize);
    insertWord(fp, "nargles");
    insertWord(fp, "hermione");
    insertWord(fp, "niffler");
    insertWord(fp, "fawkes");
    insertWord(fp, "nagini");
    insertWord(fp, "firenze");
    countWords(fp, 'h', &wordCount);
    printf("num of character n: %d", wordCount);

}
