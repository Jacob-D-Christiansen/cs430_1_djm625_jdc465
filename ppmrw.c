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

void skipComments(FILE *f)
{
    char line[buffer];
    
    while(fgetc(f) == '#')
    {
        fgets(line, buffer, f);
    }
    
    fseek(f, -sizeof(char), SEEK_CUR);
}

// Read in PPM3 and store in memory
struct PPM readPPM3(char *file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PPM *PPM = malloc(sizeof(PPM));
    int temp[2];
    int pixNum;
    int inPix;
    
    PPMFile = fopen(file, "r");
    
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
    
    PPMFile = fopen(file, "r");
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
   
    pixNum = 3 * PPM->width * PPM->height;
    PPM->image = (uint8_t *)malloc(sizeof(uint8_t)*pixNum);
        
    for(int i = 0; i < pixNum; i++)
    {
        fread(&PPM->image[i], 1, 1, PPMFile);
        
        inPix = PPM->image[i];
        
        if(inPix > PPM->max || inPix < 0)
        {
            fail("Incorrect Pixel Intensity");
        }
        
        if(feof(PPMFile) && i == pixNum)
        {
            fail("Incorrect File Size");
        }
    }

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

    maxPPM = PPM -> height * PPM -> width;
    ptrImage = PPM -> image;

    convFile = fopen(file, "w");
    if(convFile == NULL)
    {
        fail("Could not open file");
    }

    fprintf(convFile,"P3\n");
    fprintf(convFile,"%d %d\n",PPM -> width,PPM ->height);
    fprintf(convFile,"%d\n",PPM -> max);

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
    maxPPM = PPM -> height * PPM -> width;
    ptrImage = PPM -> image;

    convFile = fopen(file, "w");
    if(convFile == NULL)
    {
        fail("Could not open file");
    }

    fprintf(convFile,"P6\n");
    fprintf(convFile,"%d %d\n",PPM -> width,PPM ->height);
    fprintf(convFile,"%d\n",PPM -> max);
    fwrite(ptrImage,maxPPM,1,convFile);
    fclose(convFile);
}

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
    int type;
    int conCode;

    if(argc != 4)
    {
        fail("Wrong Number of Arguments");
    }
    
    conCode = atoi(argv[1]);
    
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
            readPPM3(PPMFile);
        }
        
        else if(type == 6)
        {
            readPPM6(PPMFile);
        }
        
        else
        {
            fail("Invalid PMM Type");
        }
    }
    else
    {
        fail("File Does Not Exist");
    }
    
    return 0;
}



