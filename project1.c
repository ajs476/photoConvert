#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *fp;
FILE *fpOut;
int tempChar;
int i;
int imageSize;
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
	if(imageDimensionColor > 255){
		fprintf(stderr, "Error: Image color not 8 bit per channel");
	}
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




int main(int argc, char *argv[]) {

	
	
    // open up our image fo reading
    fp = fopen(argv[2], "rb");

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

	// reset file pointer position to beginning of file
	rewind(fp);

	// read header info from file
	getHeaderInfo();

	int imageDataSize = sizeof(Pixel)*imageDimensionX*imageDimensionY;
	int count = 0;
	fpOut = fopen(argv[3], "wb+");
	Pixel myPixel;
	Pixel *pixelBuffer = malloc(imageDataSize);

	

	if(magicNumber[0] == 80 && magicNumber[1] == 51){

		if(strcmp(argv[1], "3") == 0){
			printf("Creating P3 from P3...\n");
			// write p3 header info
			fprintf(fpOut, "P3\n");
			fprintf(fpOut, "%d %d\n", imageDimensionX, imageDimensionY);
			fprintf(fpOut, "%d\n", imageDimensionColor);
			while(!feof(fp)){
				fscanf(fp, "%hhu ", &myPixel.red);
				fscanf(fp, "%hhu ", &myPixel.green);
				fscanf(fp, "%hhu ", &myPixel.blue);
				pixelBuffer[count] = myPixel;
				count++;
				// write p3 image
				fprintf(fpOut, "%i %i %i ", myPixel.red, myPixel.green, myPixel.blue);
			}
		}
		else if(strcmp(argv[1], "6") == 0){
			printf("Creating P6 from P3...\n");
			// we were given a p3 ppm file
			// write p6 header info
			fprintf(fpOut, "P6\n");
			fprintf(fpOut, "%d %d\n", imageDimensionX, imageDimensionY);
			fprintf(fpOut, "%d\n", imageDimensionColor);
			// read p3 image
			while(!feof(fp)){
				fscanf(fp, "%hhu ", &myPixel.red);
				fscanf(fp, "%hhu ", &myPixel.green);
				fscanf(fp, "%hhu ", &myPixel.blue);
				pixelBuffer[count] = myPixel;
				count++;
			}
			// write p6 image
			fwrite(pixelBuffer, sizeof(Pixel), count, fpOut);
		}
		
		else{
			fprintf(stderr, "Error: Expected '3' or '6' \n");
		}		
	}
	else if(magicNumber[0] == 80 && magicNumber[1] == 54){
		// we were given a p6 ppm file
		if(strcmp(argv[1], "3") == 0){
			printf("Creating P3 from P6...\n");
			// write p3 header
			fprintf(fpOut, "P3\n");
			fprintf(fpOut, "%d %d\n", imageDimensionX, imageDimensionY);
			fprintf(fpOut, "%d\n", imageDimensionColor);
			// read p6 image
			while(!feof(fp)){
				fread(&myPixel.red, 1, 1, fp);
				fread(&myPixel.green, 1, 1, fp);
				fread(&myPixel.blue, 1, 1, fp);
				pixelBuffer[count] = myPixel;
				count++;
				// write p3 image
				fprintf(fpOut, "%i %i %i ", myPixel.red, myPixel.green, myPixel.blue);
			}
		}
		else if(strcmp(argv[1], "6") == 0){
			printf("Creating P6 from P6...\n");
			// write p6 header info
			fprintf(fpOut, "P6\n");
			fprintf(fpOut, "%d %d\n", imageDimensionX, imageDimensionY);
			fprintf(fpOut, "%d\n", imageDimensionColor);
			while(!feof(fp)){
				fread(&myPixel.red, 1, 1, fp);
				fread(&myPixel.green, 1, 1, fp);
				fread(&myPixel.blue, 1, 1, fp);
				pixelBuffer[count] = myPixel;
				count++;
			}
			// write p6 image
			fwrite(pixelBuffer, sizeof(Pixel), count, fpOut);
		}
		else{
			fprintf(stderr, "Error: Expected '3' or '6' \n");
		}
		
		
			
	}
	else{
		// we were not given a p3 or a p6 ppm file
		fprintf(stderr, "Error: Invalid Filetype... \n");
		return 1;
	}


	fclose(fpOut);
	fclose(fp);

    return 0;
}
