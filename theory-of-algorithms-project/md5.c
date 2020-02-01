#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void readStringInput();

void readFileInput();



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
             printf("Invalid input entered! Please Try Again");
       }

  }while(userOption != -1);


  return 0;
}


void readFileInput()
{
  char* filePath;
  FILE* readFile;

  int pos = 0;

  char testTexts[3][30];

  filePath = (char*)malloc(30 * sizeof(char));
  
  printf("\nEnter the File path you wish to read: ");
  scanf("%s", filePath);

  printf("File path entered: %s", filePath);
  printf("\n");

  readFile = fopen(filePath, "r");

  if(readFile == NULL)
  {
    printf("Error reading file %s, Please Try again...", filePath);
    return;

  }
  else
  {
        
    while(fscanf(readFile, "%s", testTexts[pos]) == 1)
    {

     // fscanf(readFile, "%s", testTexts[pos]);

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
