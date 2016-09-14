#include <stdio.h>
#include <stdlib.h>

int main() {
	// create file pointer to open file into
    FILE *fp;
    FILE *fpOut;

    // create character to hold chars (ascii)
    int character1;
    int character2;
    int i;
    int sz;
    int sz2;

    
    
    fp = fopen("test.txt", "r");
    fpOut = fopen("out.ppm", "wb");

    //get size of file
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
   	if(character1 == 80){
   		if(character2 == 51){
   			// file is P3
   			printf("Filetype: P3");
   			for(i = 2; i < sz-2; i++){
   				character1 = fgetc(fp);
   				buffer[i] = character1;
   			}

   		}
   		else if(character2 == 54){
   			// file is P4
   			printf("Filetype: P4");

   		}
   		else{
   			// file is not P3 or P4, error
   			printf("Error: unsupported filetype \n");
   		}
   		
   		fread(buffer2, sz, 1, fp);
   		
   		//fwrite(buffer, sz, 1, stdout);
   		fwrite(buffer2, sz, 1, fpOut);
   	}
   	else{
   		printf("Error: cannot continue, unsupported filetype \n");
   	}
   	
   }
   
   fclose(fp);
   fclose(fpOut);

   return 0;
}