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
    long buffer[MAXWORDLEN*8*100];
    long fileSize[1];
    strcpy(wordToConvert, word);
    convertToLower(wordToConvert, lowerCaseWord);
    //printf("%s", lowerCaseWord);
    int num = lowerCaseWord[0] - 'a';
    int offset = 8*num;
    fseek(fp, offset,SEEK_SET);
    fread(buffer, 8, 1, fp);
    //printf("\n\n%ld buffer", *buffer);

	// seek end to find filesize
	fseek(fp, 0L, SEEK_END);
	fileSize[0] = ftell(fp);

    if(*buffer == 0){

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
    // else - num in buffer is not zero, points to a location of another record.
    // go to that record, while loop check if nextpos is zero
    //upon finding zero, write the same record with nextpos = end of file
    // then go to end of file and write new record
    else{
    	Record *record = (Record*) malloc(sizeof(Record));
    	//look where buffer is pointing
    	fseek(fp, *buffer, SEEK_SET);
    	fread(record, sizeof(Record), 1, fp);
    	char originalRecordWord[MAXWORDLEN + 1];
    	strcpy(originalRecordWord, record->word);
    	printf("%s", record->word);
    	while(record->nextpos != 0){
        	fseek(fp, record->nextpos, SEEK_SET);
        	fread(record, sizeof(Record), 1, fp);
    	}
    	fseek(fp, *buffer, SEEK_SET);
    	//we are now looking at the record with nextpos = 0
    	//write record with same value, but now with pointer to filesize
    	Record *rec = (Record*) malloc(sizeof(Record));
    	strcpy(rec->word, originalRecordWord);
    	rec->nextpos = fileSize[0];
    	fwrite(rec, sizeof(Record), 1, fp);
    	fseek(fp, 0L, SEEK_END);
    	strcpy(record->word, word);
    	record->nextpos = 0;
    	fwrite(record, sizeof(Record), 1, fp);


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
            fseek(fp, record->nextpos, SEEK_SET);
            fread(record, sizeof(Record), 1, fp);
            *count = *count + 1;
        }
        *count = *count + 1;
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
    countWords(fp, 'f', &wordCount);


    printf("\n Num words: %d", wordCount);





}
