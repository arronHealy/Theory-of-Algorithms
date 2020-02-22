#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// include md5 struct via header file so struct can be seen globally and not restricted to being first defined in main function
#include "md5.h"

// links that lead me to use struct in header file
// https://stackoverflow.com/questions/8915230/invalid-application-of-sizeof-to-incomplete-type-with-a-struct
// https://stackoverflow.com/questions/2576554/c-programming-dereferencing-pointer-to-incomplete-type-error


// defined md5 padding constant, append 1 to end followed by 0's
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



// C program function definitions, implementation and references towards end of file

struct md5_context* init_MD5_Context(struct md5_context *context);

uint32_t ROTATE_LEFT(uint32_t x, int n);

void readStringInput();

void readFileInput();

int getFileLineCount(char* fileName);

int writeToFile(char* input);

uint64_t numOfZeroBits(uint64_t numBits);


// main function to run program
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


// MD5 Data Structure initialization, Begins MD5 operation of writing a new context
// https://www.ietf.org/rfc/rfc1321.txt -  page 10 
struct md5_context* init_MD5_Context(struct md5_context *context)
{
  context = (struct md5_context*)malloc(sizeof(struct md5_context));

  // initialize bit counts to 0
  context->count[0] = 0;
  context->count[1] = 0;

  // initialize state constants
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;

  return context;
}

// rotate bits to left method definition from md5 standard
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
uint32_t ROTATE_LEFT(uint32_t x, int n)
{
  return ((x << n) | (x >> (32 - n)));
}

int getFileLineCount(char* fileName)
{
  FILE* file;
  int counter = 0;
  char c;

  file = fopen(fileName, "r");

  if(file == NULL)
  {
    printf("\nError reading file %s, Please Try again...", fileName);
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

  // md5_context *context;

  filePath = (char*)malloc(30 * sizeof(char));
  
  printf("\nEnter the File path you wish to read: ");
  scanf("%s", filePath);

  fileLineCount = getFileLineCount(filePath);

  printf("file line count: %d \n", fileLineCount);

  readFile = fopen(filePath, "r");

  if(readFile == NULL)
  {
    printf("\nError reading file %s, Please Try again...", filePath);
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
  int writeStatus;

  uint8_t bit;

  uint64_t numBits;

  int hexLength;

  char* messageText;

  char* hexText;
   
  char* paddedText;
  
  char* paddedFinalText;

  FILE* inputFile;

  struct md5_context *context = NULL;

  messageText = (char*)malloc(30 * sizeof(char));
 
  printf("\nPlease enter the Text to be hashed: ");
  scanf("%s", messageText);

  printf("Entered Text is: %s", messageText);
  printf("\n");

  hexText = (char*)malloc((strlen(messageText) * 2 + 1) * sizeof(char));

  writeStatus = writeToFile(messageText);
  
  if (writeStatus == 0)
  {
    return;
  }
  else
  {
    printf("string written to file!\n");
    free(messageText);  
  }//if


  inputFile = fopen("inputText.txt", "rb");

 
  if (inputFile == NULL)
  {
    printf("\nError: failed to access stored string input! Please try again.");
    return;
  }
  else
  {
    printf("file opened!\n");

    char hexBits[2];

    for(numBits = 0; fread(&bit, 1, 1, inputFile) == 1; numBits +=8)
    {
      // write byte as hex to char array
      sprintf(hexBits, "%02" PRIx8, bit);

      printf("hex bits %s\n", hexBits);
      
      // concatenate to create hex representation of string input
      strcat(hexText, hexBits);

    }//for
    
    // append 1 byte to string
    sprintf(hexBits, "%02" PRIx8, PADDING[0]);
    strcat(hexText, hexBits); 
    
    printf("hex text %s\n", hexText);
    
    printf("\nnumBits size is %" PRIu64 " \n", numBits);

    printf("\nnumBits to append to message: %" PRIu64 " \n", numOfZeroBits(numBits));
    
    // assign memory for padded message by getting current hex message and adding number of bits to be added
    paddedText = (char*)malloc(((sizeof(hexText) + numOfZeroBits(numBits))*2) * sizeof(char));  
    
    // concatenate hex string input
    strcat(paddedText, hexText);

    // loop for number of bits to be added - 1 because one byte added already so message is 64 bits less than a multiple of 512
    for(uint64_t i = 0; i < numOfZeroBits(numBits) - 1; i++)
    {
      sprintf(hexBits, "%02" PRIx8, PADDING[i + 1]);

      strcat(paddedText, hexBits);
    }
    
    // reuse messageText to store original length of message as hexidecimal to append to padded message
    messageText = (char*)malloc(sizeof(hexText) * sizeof(char));
    
    // assign memory for final version of padded message
    paddedFinalText = (char*)malloc((sizeof(paddedText) + 64) * sizeof(char));

    // write message length as 64 bits to hexidecimal
    sprintf(messageText, "%016" PRIx64 , numBits);

    printf("original message length as 64 bits: %s \n", messageText);
    
    // concatenate paddedText to paddedFinalText
    strcat(paddedFinalText, paddedText);
    //concatenate original message length as hexideciaml to paddedFinalText
    strcat(paddedFinalText, messageText);

    printf("padded text after all padding is: %s\n", paddedFinalText);

    
    // free memory once padding complete
    free(hexText);
    free(messageText);
    free(paddedText);
    
    // close file before finished
    fclose(inputFile);
  }//if
  

}//readFileInput


// write user input string to file, so itcan be read back in as binary and converted to hexidecimal
int writeToFile(char* input)
{
  FILE* textFile;
  char filePath[] = "inputText.txt";

  textFile = fopen(filePath, "w");

  if (textFile == NULL)
  {
    printf("Error: could not store string input! Please try again.");
    return 0;
  }

  fputs(input, textFile);

  fclose(textFile);

  return 1;
}


// code to check how many zero bits to append to message
// stack overflow link to help with padding
// https://crypto.stackexchange.com/questions/50836/md5-what-happens-if-message-is-exactly-64bit-block-size-message

uint64_t numOfZeroBits(uint64_t numBits)
{
  uint64_t result = 448ULL - (numBits % 512ULL);

  return (result / 8ULL);
}



