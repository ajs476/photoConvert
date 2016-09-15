#include <stdio.h>
#include <stdlib.h>

int main() {
	// create file pointer to open file into
    FILE *fp;
    // file pointer for new output file
    FILE *fpOut;

    // initialization of variables
    int character1;
    int character2;
    int character3;
    int i;
    int sz;
    int sz2;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    // open up our files
    fp = fopen("testImageAscii.ppm", "r");
    fpOut = fopen("out.ppm", "wb");

    //get size of file inputted
    fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);


	// create char array to store file contents into
	char *buffer = malloc(sz + 1);
	char *buffer2 = malloc(sz + 1);
	printf("Size of file: %i \n", sz);

    if(fp == NULL){
   		printf("Error reading file -> Empty \n");
   		return 1;
    }
    else{
   		printf("Success: File opened \n");
   		character1 = fgetc(fp);
   		buffer[0] = character1;
   		character2 = fgetc(fp);
   		buffer[0] = character2;
   		character3 = fgetc(fp);
   	if(character1 == 80){
   		if(character2 == 51){
   			// checking if next character is on a new line
   			if(character3 == '\n'){
   				// newline was found to be the next char
   				printf("New line detected \n");
				character3 = fgetc(fp);
				printf("Current char: %d \n", character3);
   			}
   			else{
   				// char 3 was not a newline, display it for testing
   				printf("char3: %c \n", character3);
   			}
   			// file is P3
   			printf("Filetype: P3 \n");
   			// loop through file and copy contents into buffer
   			for(i = 3; i < sz-3; i++){
   				character1 = fgetc(fp);
   				buffer[i] = character1;
   			}
   			// display buffer contents in window
   			// fwrite(buffer, sz, 1, stdout);
   		}
   		else if(character2 == 54){
   			// file is P4
   			printf("Filetype: P4 \n");
   			// read p4 file into buffer
   			fread(buffer2, sz, 1, fp);
   		    // write p4 file into new out file
   			fwrite(buffer2, sz, 1, fpOut);
   		}
   	else{
   		// file is not P3 or P4, error
   		printf("Error: unsupported filetype \n");
   	}
   		
   		
   	}
   	else{
   		// first character in header was 'P', but not 3 or 4 following found
   		printf("Error: cannot continue, unsupported filetype \n");
   	}
   	
   }
   
   fclose(fp);
   fclose(fpOut);

   return 0;
}
