#include <stdio.h>

int main() {
   FILE *fp;
   int c;

   char *buffer;
   fp = fopen("test.txt", "r");

   if(fp == NULL){
   	printf("Error reading file -> Empty \n");
   	return 1;
   }
   else{
   	printf("Success: File opened \n");
   	c = fgetc(fp);
   	if(c == 80){
   		printf("Valid file type found\n");
   	}
   	else{
   		printf("Error: File is not of type PPM, please use a PPM file!\n");
   	}
   	printf("%c \n", c);
   }
   
   fclose(fp);

   return 0;
}