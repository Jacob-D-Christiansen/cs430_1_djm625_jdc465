#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// CONSTANTS
const int buffer = 60;
const int pixel = 2;

// STRUCTS
struct PMM
{
    int type;
    int width;
    int height;
    int max;
    int *image;
}PMM;

// General function for printing fail statment and exiting program
void fail(char *mess)
{
    printf("\n\nAn error has occured:\n");
    printf("\t%s\n", mess);
    printf("\n");
    exit(1);
}


// I Shamlessly stole this from the internet so we should probably write one 
// ourselves
void removeSpaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int charToInt(char c)
{
    return c - '0';
}

int strToInt(char *c)
{
    printf("what");
    return 0;
}

void verifyConfig(struct PMM pmm)
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
        printf("%s ", token);
        values[arrPos] = atoi(token);
        
        token = strtok(NULL, delimPtr);
        arrPos++;
    }
}

// Read in either kind of PPM and store in memory
void readPPM(char *file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PMM *PMM = malloc(sizeof(PMM));
    
    if((PPMFile = fopen(file, "r"))) 
    {
        fgets(line, buffer, PPMFile);
        PMM->type = charToInt(line[1]);
        fgets(line, buffer, PPMFile);
        removeSpaces(line);
        PMM->width = charToInt(line[0]);
        PMM->height = charToInt(line[1]);
        fgets(line, buffer, PPMFile);
        PMM->max = atoi(line);
        
        verifyConfig(*PMM);
        
        fgets(line, buffer, PPMFile);
        printf("%s", line);
        int temp[2];
        splitLine(line, temp);
        printf("%d %d %d\n", temp[0], temp[1], temp[2]);
    }
    else
    {
        fail("File Does Not Exist");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    char *PPMFile;
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
    
    printf("Reading in %s\n", PPMFile);
    readPPM(PPMFile);
    
    return 0;
}



