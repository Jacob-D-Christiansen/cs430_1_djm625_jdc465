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
    if(pmm.type != 3)
    {
        if(pmm.type != 6)
        {
            fail("Unknown PMM Type");
        }
    }
    
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
/**
void skipComments(FILE *f)
{
    char line[buffer];
    
    while(fgetc(f) == '#')
    {
        fgets(line, buffer, f);
    }
}
**/
// Read in PPM3 and store in memory
void readPPM3(char *file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PPM *PPM = malloc(sizeof(PPM));
    int temp[2];
    int pixNum;
    int inPix;
    
    PPMFile = fopen(file, "r");
    

    fgets(line, buffer, PPMFile);
    PPM->type = charToInt(line[1]);

    fgets(line, buffer, PPMFile);
    splitLine(line, temp);
    PPM->width = temp[0];
    PPM->height = temp[1];

    fgets(line, buffer, PPMFile);
    PPM->max = atoi(line);
        
    verifyConfig(*PPM);
   
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
    
    printf("File Read in Correctly\n");
    printf("Hello world\n"); 
    fclose(PPMFile);
}

int getType(FILE *f)
{
    char line[buffer];
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
            printf("type equal 6\n");
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



