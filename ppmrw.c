#include <stdlib.h>
#include <stdio.h>

// CONSTANTS
const int buffer = 60;

// STRUCTS
struct PMM
{
    int type;
    int width;
    int height;
    int max;
}PMM;

// General function for printing fail statment and exiting program
void fail(char *mess)
{
    printf("An error has occured:\n");
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

// Read in either kind of PPM and store in memory
void readPPM(char *file)
{
    FILE *PPMFile;
    char line[buffer];
    struct PMM PMM;
    
    if((PPMFile = fopen(file, "r"))) 
    {
        charToInt('a');
        fgets(line, buffer, PPMFile);
        PMM.type = charToInt(line[1]);
        fgets(line, buffer, PPMFile);
        removeSpaces(line);
        PMM.width = charToInt(line[0]);
        PMM.height = charToInt(line[1]);
        fgets(line, buffer, PPMFile);
        PMM.max = atoi(line);
        
        printf("type: %d\n", PMM.type);
        printf("width: %d\n", PMM.width);
        printf("height: %d\n", PMM.height);
        printf("max size: %d\n", PMM.max);
    }
    else
    {
        fail("File Does Not Exist");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fail("Wrong Number of Arguments");
    }
    
    char *PPMFile = argv[1];
    
    printf("Reading in %s\n", PPMFile);
    readPPM(PPMFile);
    
    return 0;
}



