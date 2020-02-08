#include <stdio.h>
#include <stdint.h>

//define data structure for MD5 processing
// https://www.ietf.org/rfc/rfc1321.txt -  page 8
typedef struct {
  uint32_t state[4];  // message digest state (A,B,C,D)
  uint32_t count[2];  // number of bits, mod 2^64, least significant bit first or little endian first
  unsigned char buffer[64]; // message digest input buffer
} md5_context;


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

void init_MD5_Context(md5_context *context);

uint32_t ROTATE_LEFT(uint32_t x, int n);

void readStringInput();

void readFileInput();

int getFileLineCount(char* fileName);


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
void init_MD5_Context(md5_context *context)
{
  // initialize bit counts to 0
  context->count[0] = 0;
  context->count[1] = 0;

  // initialize state constants
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;

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
