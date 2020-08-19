#include <stdlib.h>
#include <stdio.h>

// General function for printing fail statment and exiting program
void fail(char *mess)
{
    printf("An error has occured:\n");
    printf("\t%s\n", mess);
    printf("\n");
    exit(1);
} 

FILE *fileExists(char *file)
{
    FILE *PPMFile;
    if((PPMFile = fopen(file, "r"))) 
    {
        return PPMFile;
    }
    fail("File Does Not Exist");
    return NULL;
}

void readPPM(char *file)
{
    FILE *PPMFile = fileExists(file);
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fail("Wrong Number of Arguments\n\tPlease input a file path");
    }
    printf("reee\n");
    char *inPPM = argv[1];
    printf("aaaaa\n");
    readPPM(inPPM);
    
    return 0;
}



