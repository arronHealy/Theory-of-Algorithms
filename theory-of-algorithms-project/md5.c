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

  filePath = (char*)malloc(30 * sizeof(char));
  printf("\nEnter the File path you wish to read: ");
  scanf("%s", filePath);

  printf("File path entered: %s", filePath);
  printf("\n");
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
