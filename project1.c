#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *fp;

int tempChar;
int i;
int imageSize;
int sz2;
int imageDimensionX;
int imageDimensionY;
int imageDimensionColor;
char magicNumber[2];

typedef struct Pixel{
	unsigned char red;
    unsigned char green;
    unsigned char blue;
    } Pixel;





void skipComments(){
	if(tempChar == '#'){
		// comment exists, lets get to the end of it
		printf("Comment found.. \n");
		while(tempChar != '\n'){
			tempChar = fgetc(fp);
		}
		// is the next line another comment?
		tempChar = fgetc(fp);
		skipComments();
	}
	else{
		// get image dimensions
		printf("End of comments.. \n");
		// reset our file pointer to the correct position
		fseek(fp, -2, SEEK_CUR);
	}
	
}

void getDimensions(){
	// get image width
	fscanf(fp, "%d",  &imageDimensionX);
	printf("Image Dimensions X: %d\n", imageDimensionX);

	// get image height
	fscanf(fp, "%d\n",  &imageDimensionY);
	printf("Image Dimensions Y: %d\n", imageDimensionY);
}

void getColorRange(){
	// get image color range
	fscanf(fp, "%d\n",  &imageDimensionColor);
	printf("Image Color Range: %d\n", imageDimensionColor);
}

void getFileType(){
	char tempBuffer[64];
	fscanf(fp, "%s\n",  tempBuffer);
	strcpy(magicNumber, tempBuffer);
	printf("Magic Number: %s\n", magicNumber);
	tempChar = fgetc(fp);
}

void getHeaderInfo(){
	getFileType();
	skipComments();
	getDimensions();
	getColorRange();	
}

int main() {
	
    // open up our image fo reading
    fp = fopen("testImageAscii1.ppm", "r");

	// make sure file opened exists
	if(fp == NULL ){
   		fprintf(stderr, "Error reading file... \n");
   		return 1;
    }

  	printf("Success: File opened \n");

	//get size of image
    fseek(fp, 0L, SEEK_END);
	imageSize = ftell(fp);
	rewind(fp);

	printf("Size of file: %i \n", imageSize);
   
 
	// create char array to store file contents into
	char *imageBuffer = malloc(imageSize + 1);
	int i = 0;
	while(!feof(fp)){
		imageBuffer[i] = fgetc(fp);
		i++;
	}
	imageBuffer[i] = 0;

	// printing contents of imageBuffer
	/*int j = 0;
	while(j < .0005*i){
		printf("%c", imageBuffer[j]);
		j++;
	}
	*/
	
	// reset file pointer position to beginning of file
	rewind(fp);

	getHeaderInfo();


	
	





	


	fclose(fp);

    return 0;
}
