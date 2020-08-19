#include <stdio.h>
#include <stdlib.h>

void fail(char *mess)
{
    printf("An error has occured:\n");
    printf("\t%s\n", mess);
    printf("\n");
    exit(1);
} 

void printPPM(char *file)
{
    FILE *fptr = fopen(filename, mode);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fail("Wrong Number of Arguments\n\tPlease input a file path");
    }
    
    char *inPPM = argv[1];
    printPPM(inPPM);
}



