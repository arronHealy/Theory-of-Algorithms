#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// include md5 struct via header file so struct can be seen globally and not restricted to being first defined in main function
#include "md5.h"

// links that lead me to use struct in header file
// https://stackoverflow.com/questions/8915230/invalid-application-of-sizeof-to-incomplete-type-with-a-struct
// https://stackoverflow.com/questions/2576554/c-programming-dereferencing-pointer-to-incomplete-type-error


#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


// defined md5 padding constant, append 1 to end followed by 0's
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

union block
{
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
};

enum flag {
  READ,
  PAD0,
  FINISH
};

// C program function definitions, implementation and references towards end of file
void FF(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void GG(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void HH(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void II(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);


uint32_t F(uint32_t x, uint32_t y, uint32_t z);

uint32_t G(uint32_t x, uint32_t y, uint32_t z);

uint32_t H(uint32_t x, uint32_t y, uint32_t z);

uint32_t I(uint32_t x, uint32_t y, uint32_t z);

struct md5_context* init_MD5_Context(struct md5_context *context);

uint32_t ROTATE_LEFT(uint32_t x, int n);

void readStringInput();

void readFileInput();

int getFileLineCount(char* fileName);

int writeToFile(char* input);

uint64_t numOfZeroBits(uint64_t numBits);


int nextBlock(union block *M, FILE *inFile, uint64_t *nobits, enum flag *status);

void nexthash(union block *M, uint32_t *H);

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

  union block M;

  uint64_t nobits = 0;

  enum flag status = READ;

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

  // hexText = (char*)malloc((strlen(messageText) * 2 + 1) * sizeof(char));

  writeStatus = writeToFile(messageText);
  
  if (writeStatus == 0)
  {
    printf("Error writing string to file! Please try again.\n");
    return;
  }
  else
  {
    printf("String written to file!\n");
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

    context = init_MD5_Context(context);

    printf("context value: %x \n", context->state[0]);

    //size_t numBytesRead = fread(context->buffer, 1, 64, inputFile);

   // printf("size num bytes read: %zu \n", numBytesRead);    
   uint32_t H[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};


   while(nextBlock(&M, inputFile, &nobits, &status))
   {
      //printf("next block\n");
      nexthash(&M, H);
   }

   for (int i = 0; i < 4; i++)
   {
      printf("%02" PRIx32, H[i]);
   }
  
   //for (uint8_t i = 0; i < 64; i++)
    //printf("%" PRIu8 " ", M.eight[i]);
   
   
   //for(int i = 0; i < 64; i++)
      //printf("context buffer val %c \n", context->buffer[i]);
    /*
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
    */



    // close file before finished
    fclose(inputFile);
  }//if
  

}//readFileInput


void nexthash(union block *M, uint32_t *H)
{
  uint32_t a = H[0], b = H[1], c = H[2], d = H[3];

  /* Round 1 */
  FF(a, b, c, d, M->threetwo[0], S11, 0xd76aa478); /*1*/
  FF(d, a, b, c, M->threetwo[1], S12, 0xe8c7b756); /*2*/
  FF(c, d, a, b, M->threetwo[2], S13, 0x242070db); /*3*/
  FF(b, c, d, a, M->threetwo[3], S14, 0xc1bdceee); /*4*/
  FF(a, b, c, d, M->threetwo[4], S11, 0xf57c0faf); /*5*/
  FF(d, a, b, c, M->threetwo[5], S12, 0x4787c62a); /*6*/
  FF(c, d, a, b, M->threetwo[6], S13, 0xa8304613); /*7*/
  FF(b, c, d, a, M->threetwo[7], S14, 0xfd469501); /*8*/
  FF(a, b, c, d, M->threetwo[8], S11, 0x698098d8); /*9*/
  FF(d, a, b, c, M->threetwo[9], S12, 0x8b44f7af); /*10*/
  FF(c, d, a, b, M->threetwo[10], S13, 0xffff5bb1); /*11*/
  FF(b, c, d, a, M->threetwo[11], S14, 0x895cd7be); /*12*/
  FF(a, b, c, d, M->threetwo[12], S11, 0x6b901122); /*13*/
  FF(d, a, b, c, M->threetwo[13], S12, 0xfd987193); /*14*/
  FF(c, d, a, b, M->threetwo[14], S13, 0xa679438e); /*15*/
  FF(b, c, d, a, M->threetwo[15], S14, 0x49b40821); /*16*/

  /* Round 2 */
  GG(a, b, c, d, M->threetwo[1], S21, 0xf61e2562); /*17*/
  GG(d, a, b, c, M->threetwo[6], S22, 0xc040b340); /*18*/
  GG(c, d, a, b, M->threetwo[11], S23, 0x265e5a51); /*19*/
  GG(b, c, d, a, M->threetwo[0], S24, 0xe9b6c7aa); /*20*/
  GG(a, b, c, d, M->threetwo[5], S21, 0xd62f105d); /*21*/
  GG(d, a, b, c, M->threetwo[10], S22, 0x2441453); /*22*/
  GG(c, d, a, b, M->threetwo[15], S23, 0xd8a1e681); /*23*/
  GG(b, c, d, a, M->threetwo[4], S24, 0xe7d3fbc8); /*24*/
  GG(a, b, c, d, M->threetwo[9], S21, 0x21e1cde6); /*25*/
  GG(d, a, b, c, M->threetwo[14], S22, 0xc33707d6); /*26*/
  GG(c, d, a, b, M->threetwo[3], S23, 0xf4d50d87); /*27*/
  GG(b, c, d, a, M->threetwo[8], S24, 0x455a14ed); /*28*/
  GG(a, b, c, d, M->threetwo[13], S21, 0xa9e3e905); /*29*/
  GG(d, a, b, c, M->threetwo[2], S22, 0xfcefa3f8); /*30*/
  GG(c, d, a, b, M->threetwo[7], S23, 0x676f02d9); /*31*/
  GG(b, c, d, a, M->threetwo[12], S24, 0x8d2a4c8a); /*32*/

  /* Round 3 */
  HH(a, b, c, d, M->threetwo[5], S31, 0xfffa3942); /*33*/
  HH(d, a, b, c, M->threetwo[8], S32, 0x8771f681); /*34*/
  HH(c, d, a, b, M->threetwo[11], S33, 0x6d9d6122); /*35*/
  HH(b, c, d, a, M->threetwo[14], S34, 0xfde5380c); /*36*/
  HH(a, b, c, d, M->threetwo[1], S31, 0xa4beea44); /*37*/
  HH(d, a, b, c, M->threetwo[4], S32, 0x4bdecfa9); /*38*/
  HH(c, d, a, b, M->threetwo[7], S33, 0xf6bb4b60); /*39*/
  HH(b, c, d, a, M->threetwo[10], S34, 0xbebfbc70); /*40*/
  HH(a, b, c, d, M->threetwo[13], S31, 0x289b7ec6); /*41*/
  HH(d, a, b, c, M->threetwo[0], S32, 0xeaa127fa); /*42*/
  HH(c, d, a, b, M->threetwo[3], S33, 0xd4ef3085); /*43*/
  HH(b, c, d, a, M->threetwo[6], S34, 0x4881d05); /*44*/
  HH(a, b, c, d, M->threetwo[9], S31, 0xd9d4d039); /*45*/
  HH(d, a, b, c, M->threetwo[12], S32, 0xe6db99e5); /*46*/
  HH(c, d, a, b, M->threetwo[15], S33, 0x1fa27cf8); /*47*/
  HH(b, c, d, a, M->threetwo[2], S34, 0xc4ac5665); /*48*/

  /* Round 4 */
  II(a, b, c, d, M->threetwo[0], S41, 0xf4292244); /*49*/
  II(d, a, b, c, M->threetwo[7], S42, 0x432aff97); /*50*/
  II(c, d, a, b, M->threetwo[14], S43, 0xab9423a7); /*51*/
  II(b, c, d, a, M->threetwo[5], S44, 0xfc93a039); /*52*/
  II(a, b, c, d, M->threetwo[12], S41, 0x655b59c3); /*53*/
  II(d, a, b, c, M->threetwo[3], S42, 0x8f0ccc92); /*54*/
  II(c, d, a, b, M->threetwo[10], S43, 0xffeff47d); /*55*/
  II(b, c, d, a, M->threetwo[1], S44, 0x85845dd1); /*56*/
  II(a, b, c, d, M->threetwo[8], S41, 0x6fa87e4f); /*57*/
  II(d, a, b, c, M->threetwo[15], S42, 0xfe2ce6e0); /*58*/
  II(c, d, a, b, M->threetwo[6], S43, 0xa3014314); /*59*/
  II(b, c, d, a, M->threetwo[13], S44, 0x4e0811a1); /*60*/
  II(a, b, c, d, M->threetwo[4], S41, 0xf7537e82); /*61*/
  II(d, a, b, c, M->threetwo[11], S42, 0xbd3af235); /*62*/
  II(c, d, a, b, M->threetwo[2], S43, 0x2ad7d2bb); /*63*/
  II(b, c, d, a, M->threetwo[9], S44, 0xeb86d391); /*64*/

  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;

    
}

int nextBlock(union block *M, FILE *inFile, uint64_t *nobits, enum flag *status)
{
 if (*status == FINISH)
 {
  return 0;
 }

 if (*status == PAD0)
 {
  for (int i = 0; i < 56; i++)
  {
    M->eight[i] = 0;
  }

  M->sixfour[7] = *nobits;
  *status = FINISH;
  return 1;
 }

 size_t nobytesread = fread(M->eight, 1, 64, inFile);

 if (nobytesread == 64)
 {
  return 1;
 }

 if (nobytesread < 56)
 {
    M->eight[nobytesread] = 0x80;
    for(int i = nobytesread + 1; i < 56; i++)
    {
      M->eight[i] = 0;
    }
    M->sixfour[7] = *nobits;
    *status = FINISH;
    return 1;
 }

 M->eight[nobytesread] = 0x80;
 for (int i = nobytesread + 1; i < 64; i++)
 {
    M->eight[i] = 0;
 }
 *status = PAD0;
 return 1;
}

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

void FF(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  a += F(b, c, d) + x + ac;
  a = ROTATE_LEFT(a, s);
  a += b;
}

void GG(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  a += G(b, c, d) + x + ac;
  a = ROTATE_LEFT(a, s);
  a += b;
}

void HH(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  a += H(b, c, d) + x + ac;
  a = ROTATE_LEFT(a, s);
  a += b;
}

void II(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  a += I(b, c, d) + x + ac;
  a = ROTATE_LEFT(a, s);
  a += b;
}

uint32_t F(uint32_t x, uint32_t y, uint32_t z)
{
  return (x & y) | ((~x) & z);
}


uint32_t G(uint32_t x, uint32_t y, uint32_t z)
{
  return (x & z) | (y & (~z));
}


uint32_t H(uint32_t x, uint32_t y, uint32_t z)
{
  return ((x) ^ (y) ^ (z));
}


uint32_t I(uint32_t x, uint32_t y, uint32_t z)
{
  return y ^ (x | (~z));
}

