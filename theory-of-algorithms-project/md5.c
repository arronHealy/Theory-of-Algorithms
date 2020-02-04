#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void readStringInput();

void readFileInput();

int getFileLineCount(char* fileName);


int main(int argc, char *argv[])
{
  int userOption;

  printf("\nMD5 Hash Function");
  printf("\nEnter -1 to exit the program!");
  printf("\nEnter 1 if you wish to enter Text to be hashed");
  printf("\nEnter 2 if you wish the contents of a file to be hashed");
 
  do{
       printf("\nEnter option: ");
       scanf("%d", &userOption);

       switch(userOption)
       {
          case 1:
            readStringInput();
           break;
    
          case 2:
            readFileInput();
           break;

          case -1:
            printf("\nExiting MD5 program!\n");
            break;

           default:
             printf("Invalid input entered! Please Try Again\n");
       }

  }while(userOption != -1);


  return 0;
}


int getFileLineCount(char* fileName)
{
  FILE* file;
  int counter = 0;
  char c;

  file = fopen(fileName, "r");

  if(file == NULL)
  {
    printf("Error reading file %s, Please Try again...", fileName);
    return 0;

  }
  else
  {

    for(c = getc(file); c != EOF; c = getc(file))
    {
      if(c == '\n')
      {
        counter = counter + 1;
      }
    }
    fclose(file);
    printf("file num lines: %d \n", counter);
    
  }

  return counter;

}

void readFileInput()
{
  char* filePath;
  FILE* readFile;

  int pos = 0;

  int fileLineCount = 0;


  filePath = (char*)malloc(30 * sizeof(char));
  
  printf("\nEnter the File path you wish to read: ");
  scanf("%s", filePath);

  fileLineCount = getFileLineCount(filePath);

  printf("file line count: %d \n", fileLineCount);

  readFile = fopen(filePath, "r");

  if(readFile == NULL)
  {
    printf("Error reading file %s, Please Try again...", filePath);
    return;

  }
  else
  {
    char testTexts[fileLineCount - 1][30];

    while(fscanf(readFile, "%s", testTexts[pos]) == 1)
    {

      printf("\nFile text at pos %d is: %s", pos, testTexts[pos]);
      printf("\n");
      

      pos++;
    }

    fclose(readFile);
  }
  
}


void readStringInput()
{
  char* messageText;

  messageText = (char*)malloc(30 * sizeof(char));
 
  printf("\nPlease enter the Text to be hashed: ");
  scanf("%s", messageText);

  printf("Entered Text is: %s", messageText);
  printf("\n");

}
