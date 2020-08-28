#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// CONSTANTS
const int buffer = 60;
const int pixel = 2;

// STRUCTS
struct PPM
{
    int type;
    int width;
    int height;
    int max;
    uint8_t *image;
}PPM;

// General function for printing fail statment and exiting program
void fail(char *mess)
{
    printf("\nAn Error has Occured:\n");
    printf("\t%s\n", mess);
    printf("\n");
    exit(1);
}

int charToInt(char c)
{
    return c - '0';
}

//Confirm that the height width and max color is above 0
void verifyConfig(struct PPM pmm)
{   
    if(pmm.width <= 0)
    {
        fail("Invalid Width");
    }
    
    if(pmm.height <= 0)
    {
        fail("Invalid Height");
    }
    
    if(pmm.max <= 0)
    {
        fail("Invalid Max Score");
    }
    
    printf("Valid Config\n");
}

//Helper function that helps with splitting the line when multiple values are needed from the line.
void splitLine(char *line, int *values)
{
    char delim = ' ';
    char *delimPtr = &delim;
    char *token = strtok(line, delimPtr);
    int arrPos = 0;
    
    while(token != NULL)
    {
        values[arrPos] = atoi(token);
        token = strtok(NULL, delimPtr);
        arrPos++;
    }
}

//Skips any comment that starts with #
void skipComments(FILE *f)
{
    char line[buffer];
    
    while(fgetc(f) == '#')
    {
        fgets(line, buffer, f);
    }
    
    fseek(f, -sizeof(char), SEEK_CUR);
}

struct PPM readPPM3(char *file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PPM *PPM = malloc(sizeof(PPM));
    int temp[2];
    int pixNum;
    int inPix;
    
    PPMFile = fopen(file, "r");
    
    //general open and read in the header before the data.
    skipComments(PPMFile);
    fgets(line, buffer, PPMFile);
    PPM->type = charToInt(line[1]);

    skipComments(PPMFile);
    fgets(line, buffer, PPMFile);
    splitLine(line, temp);
    PPM->width = temp[0];
    PPM->height = temp[1];
    
    skipComments(PPMFile);
    fgets(line, buffer, PPMFile);
    PPM->max = atoi(line);

    verifyConfig(*PPM);
   
    skipComments(PPMFile);
    fgets(line, buffer, PPMFile);
    pixNum = 3 * PPM->width * PPM->height;
    PPM->image = (uint8_t *)malloc(sizeof(uint8_t)*pixNum);
    
    //converts each line from the lines ascii to integers.
    for(int i = 0; i < pixNum; i++)
    {
        inPix = atoi(line);
        
        if(inPix > PPM->max || inPix < 0)
        {
            fail("Incorrect Pixel Intensity");
        }
        
        PPM->image[i] = inPix;
        fgets(line, buffer, PPMFile);
        
        if(feof(PPMFile) && i < (pixNum - 1))
        {
            fail("Incorrect File Size");
        }
    }
    
    if(!feof(PPMFile))
    {
        fail("Incorret File Size");
    }
    
    printf("File Read in Correctly\n"); 
    fclose(PPMFile);
    
    return *PPM;
}

struct PPM readPPM6(char * file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PPM *PPM = malloc(sizeof(PPM));
    int temp[2];
    int pixNum;
    int inPix;
    
    //general open and read in the header before the data.
    PPMFile = fopen(file, "rb");
    skipComments(PPMFile);
    
    fgets(line, buffer, PPMFile);
    PPM->type = charToInt(line[1]);
    skipComments(PPMFile);
    
    fgets(line, buffer, PPMFile);
    splitLine(line, temp);
    PPM->width = temp[0];
    PPM->height = temp[1];
    skipComments(PPMFile);

    fgets(line, buffer, PPMFile);
    PPM->max = atoi(line);

    verifyConfig(*PPM);
    
    //Create a malloc with the appropriate size for 3 values per pixel
    pixNum = 3 * PPM->width * PPM->height;
    PPM->image = (uint8_t *)malloc(sizeof(uint8_t)*pixNum);

    //single line to read all the image data.
    fread(PPM->image,pixNum,1, PPMFile);

    fgets(line, buffer, PPMFile);

    if(!feof(PPMFile))
    {
        fail("Incorret File Size");
    }
    
    printf("File Read in Correctly\n"); 
    fclose(PPMFile);
    
    return *PPM;
}

void writePPM3(char * file, struct PPM *PPM)
{
    FILE *convFile;
    int maxPPM;
    uint8_t *ptrImage;

    maxPPM = (PPM -> height * PPM -> width*3);
    ptrImage = PPM -> image;

    convFile = fopen(file, "w");
    if(convFile == NULL)
    {
        fail("Could not open file");
    }

    fprintf(convFile,"P3\n");
    fprintf(convFile,"%d %d\n",PPM -> width,PPM ->height);
    fprintf(convFile,"%d\n",PPM -> max);

    //write the PPM array to the file 
    for(int i = 0; i < maxPPM; i++)
    {
        fprintf(convFile,"%d\n",*ptrImage);
        ptrImage++;
    }
    fclose(convFile);
}

void writePPM6(char * file, struct PPM *PPM)
{
    FILE *convFile;
    int maxPPM;
    uint8_t *ptrImage;
    maxPPM = (PPM -> height * PPM -> width)*3;
    ptrImage = PPM -> image;
    convFile = fopen(file, "wb");
    if(convFile == NULL)
    {
        fail("Could not open file");
    }

    //Write the header file for a PPM P6 File
    fprintf(convFile,"P6\n");
    fprintf(convFile,"%d %d\n",PPM -> width,PPM ->height);
    fprintf(convFile,"%d\n",PPM -> max);

    //Write the whole Array into the File
    fwrite(ptrImage,maxPPM,1,convFile);
    fclose(convFile);
}

//Get if the file is P6 or P3 formatting
int getType(FILE *f)
{
    char line[buffer];
    skipComments(f);
    fgets(line, buffer, f);
    return charToInt(line[1]);
}

int main(int argc, char *argv[])
{
    char *PPMFile;
    FILE *f;
    char *convFile;
    struct PPM *PPM;
    int type;
    int conCode;

    if(argc != 4)
    {
        fail("Wrong Number of Arguments");
    }
    
    conCode = atoi(argv[1]);
    convFile = argv[3];
    if(conCode != 3)
    {
        if(conCode != 6)
        {
            fail("Invalid convertion code");
        }
    }
    
    PPMFile = argv[2];
    
    if((f = fopen(PPMFile, "r")))
    {
        printf("Reading in %s\n", PPMFile);
        
        type = getType(f);
        fclose(f);
        
        if(type == 3)
        {
            *PPM = readPPM3(PPMFile);
        }
        
        else if(type == 6)
        {
            *PPM = readPPM6(PPMFile);
        }
        
        else
        {
            fail("Invalid PMM Type");
        }

        if(conCode == 3)
        {
            writePPM3(convFile,PPM);
        }

        else if(conCode == 6)
        {
            writePPM6(convFile,PPM);
        }
    }

    else
    {
        fail("File Does Not Exist");
    }
    
    return 0;
}



