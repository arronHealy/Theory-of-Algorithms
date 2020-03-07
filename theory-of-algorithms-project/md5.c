#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <byteswap.h>

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

// word as 32 bit integer
#define WORD uint32_t

// rotate bits to left method definition from md5 standard
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// hash functions
#define F(x, y, z) ((x & y) | (~x & z))

#define G(x, y, z) ((x & z) | (y & ~z))

#define H(x, y, z) (x ^ y ^ z)

#define I(x, y, z) (y ^ (x | ~z))

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
void FF(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void GG(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void HH(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void II(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

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

/*
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
*/

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

  char* messageText;

  FILE* inputFile;

  struct md5_context *context = NULL;

  messageText = (char*)malloc(30 * sizeof(char));
 
  printf("\nPlease enter the Text to be hashed: ");
  scanf("%s", messageText);

  printf("Entered Text is: %s", messageText);
  printf("\n");

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

      
   WORD H[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};


   while(nextBlock(&M, inputFile, &nobits, &status))
   {
      //printf("next block\n");
      nexthash(&M, H);
   }

   for(uint8_t i = 0; i < 64; i++)
   {
     printf("%02" PRIx8 "", M.eight[i]);
   }

   printf("\n");

   for(uint8_t i = 0; i < 64; i++)
   {
     printf("%02" PRIu8 " ", M.eight[i]);
   }

   printf("\n");

   for (int i = 0; i < 4; i++)
   {
     printf("%08" PRIx32 "", bswap_32(H[i]));
     //printf("%02x%02x%02x%02x", (H[i] >> 0) & 0xFF, (H[i] >> 8) & 0xFF, (H[i] >> 16) & 0xFF, (H[i] >> 24) & 0xFF);
   }
  
    // close file before finished
    fclose(inputFile);
  }//if
  

}//readFileInput


void nexthash(union block *M, uint32_t *H)
{
  uint32_t a = H[0], b = H[1], c = H[2], d = H[3];

  /* Round 1 */
  FF(&H[0], H[1], H[2], H[3], M->threetwo[0], S11, 0xd76aa478); /*1*/
  FF(&H[3], H[0], H[1], H[2], M->threetwo[1], S12, 0xe8c7b756); /*2*/
  FF(&H[2], H[3], H[0], H[1], M->threetwo[2], S13, 0x242070db); /*3*/
  FF(&H[1], H[2], H[3], H[0], M->threetwo[3], S14, 0xc1bdceee); /*4*/
  FF(&H[0], H[1], H[2], H[3], M->threetwo[4], S11, 0xf57c0faf); /*5*/
  FF(&H[3], H[0], H[1], H[2], M->threetwo[5], S12, 0x4787c62a); /*6*/
  FF(&H[2], H[3], H[0], H[1], M->threetwo[6], S13, 0xa8304613); /*7*/
  FF(&H[1], H[2], H[3], H[0], M->threetwo[7], S14, 0xfd469501); /*8*/
  FF(&H[0], H[1], H[2], H[3], M->threetwo[8], S11, 0x698098d8); /*9*/
  FF(&H[3], H[0], H[1], H[2], M->threetwo[9], S12, 0x8b44f7af); /*10*/
  FF(&H[2], H[3], H[0], H[1], M->threetwo[10], S13, 0xffff5bb1); /*11*/
  FF(&H[1], H[2], H[3], H[0], M->threetwo[11], S14, 0x895cd7be); /*12*/
  FF(&H[0], H[1], H[2], H[3], M->threetwo[12], S11, 0x6b901122); /*13*/
  FF(&H[3], H[0], H[1], H[2], M->threetwo[13], S12, 0xfd987193); /*14*/
  FF(&H[2], H[3], H[0], H[1], M->threetwo[14], S13, 0xa679438e); /*15*/
  FF(&H[1], H[2], H[3], H[0], M->threetwo[15], S14, 0x49b40821); /*16*/

  /* Round 2 */
  GG(&H[0], H[1], H[2], H[3], M->threetwo[1], S21, 0xf61e2562); /*17*/
  GG(&H[3], H[0], H[1], H[2], M->threetwo[6], S22, 0xc040b340); /*18*/
  GG(&H[2], H[3], H[0], H[1], M->threetwo[11], S23, 0x265e5a51); /*19*/
  GG(&H[1], H[2], H[3], H[0], M->threetwo[0], S24, 0xe9b6c7aa); /*20*/
  GG(&H[0], H[1], H[2], H[3], M->threetwo[5], S21, 0xd62f105d); /*21*/
  GG(&H[3], H[0], H[1], H[2], M->threetwo[10], S22, 0x2441453); /*22*/
  GG(&H[2], H[3], H[0], H[1], M->threetwo[15], S23, 0xd8a1e681); /*23*/
  GG(&H[1], H[2], H[3], H[0], M->threetwo[4], S24, 0xe7d3fbc8); /*24*/
  GG(&H[0], H[1], H[2], H[3], M->threetwo[9], S21, 0x21e1cde6); /*25*/
  GG(&H[3], H[0], H[1], H[2], M->threetwo[14], S22, 0xc33707d6); /*26*/
  GG(&H[2], H[3], H[0], H[1], M->threetwo[3], S23, 0xf4d50d87); /*27*/
  GG(&H[1], H[2], H[3], H[0], M->threetwo[8], S24, 0x455a14ed); /*28*/
  GG(&H[0], H[1], H[2], H[3], M->threetwo[13], S21, 0xa9e3e905); /*29*/
  GG(&H[3], H[0], H[1], H[2], M->threetwo[2], S22, 0xfcefa3f8); /*30*/
  GG(&H[2], H[3], H[0], H[1], M->threetwo[7], S23, 0x676f02d9); /*31*/
  GG(&H[1], H[2], H[3], H[0], M->threetwo[12], S24, 0x8d2a4c8a); /*32*/

  /* Round 3 */
  HH(&H[0], H[1], H[2], H[3], M->threetwo[5], S31, 0xfffa3942); /*33*/
  HH(&H[3], H[0], H[1], H[2], M->threetwo[8], S32, 0x8771f681); /*34*/
  HH(&H[2], H[3], H[0], H[1], M->threetwo[11], S33, 0x6d9d6122); /*35*/
  HH(&H[1], H[2], H[3], H[0], M->threetwo[14], S34, 0xfde5380c); /*36*/
  HH(&H[0], H[1], H[2], H[3], M->threetwo[1], S31, 0xa4beea44); /*37*/
  HH(&H[3], H[0], H[1], H[2], M->threetwo[4], S32, 0x4bdecfa9); /*38*/
  HH(&H[2], H[3], H[0], H[1], M->threetwo[7], S33, 0xf6bb4b60); /*39*/
  HH(&H[1], H[2], H[3], H[0], M->threetwo[10], S34, 0xbebfbc70); /*40*/
  HH(&H[0], H[1], H[2], H[3], M->threetwo[13], S31, 0x289b7ec6); /*41*/
  HH(&H[3], H[0], H[1], H[2], M->threetwo[0], S32, 0xeaa127fa); /*42*/
  HH(&H[2], H[3], H[0], H[1], M->threetwo[3], S33, 0xd4ef3085); /*43*/
  HH(&H[1], H[2], H[3], H[0], M->threetwo[6], S34, 0x4881d05); /*44*/
  HH(&H[0], H[1], H[2], H[3], M->threetwo[9], S31, 0xd9d4d039); /*45*/
  HH(&H[3], H[0], H[1], H[2], M->threetwo[12], S32, 0xe6db99e5); /*46*/
  HH(&H[2], H[3], H[0], H[1], M->threetwo[15], S33, 0x1fa27cf8); /*47*/
  HH(&H[1], H[2], H[3], H[0], M->threetwo[2], S34, 0xc4ac5665); /*48*/

  /* Round 4 */
  II(&H[0], H[1], H[2], H[3], M->threetwo[0], S41, 0xf4292244); /*49*/
  II(&H[3], H[0], H[1], H[2], M->threetwo[7], S42, 0x432aff97); /*50*/
  II(&H[2], H[3], H[0], H[1], M->threetwo[14], S43, 0xab9423a7); /*51*/
  II(&H[1], H[2], H[3], H[0], M->threetwo[5], S44, 0xfc93a039); /*52*/
  II(&H[0], H[1], H[2], H[3], M->threetwo[12], S41, 0x655b59c3); /*53*/
  II(&H[3], H[0], H[1], H[2], M->threetwo[3], S42, 0x8f0ccc92); /*54*/
  II(&H[2], H[3], H[0], H[1], M->threetwo[10], S43, 0xffeff47d); /*55*/
  II(&H[1], H[2], H[3], H[0], M->threetwo[1], S44, 0x85845dd1); /*56*/
  II(&H[0], H[1], H[2], H[3], M->threetwo[8], S41, 0x6fa87e4f); /*57*/
  II(&H[3], H[0], H[1], H[2], M->threetwo[15], S42, 0xfe2ce6e0); /*58*/
  II(&H[2], H[3], H[0], H[1], M->threetwo[6], S43, 0xa3014314); /*59*/
  II(&H[1], H[2], H[3], H[0], M->threetwo[13], S44, 0x4e0811a1); /*60*/
  II(&H[0], H[1], H[2], H[3], M->threetwo[4], S41, 0xf7537e82); /*61*/
  II(&H[3], H[0], H[1], H[2], M->threetwo[11], S42, 0xbd3af235); /*62*/
  II(&H[2], H[3], H[0], H[1], M->threetwo[2], S43, 0x2ad7d2bb); /*63*/
  II(&H[1], H[2], H[3], H[0], M->threetwo[9], S44, 0xeb86d391); /*64*/

  printf("a = %08" PRIx32 "\n", H[0]);
  printf("b = %08" PRIx32 "\n", H[1]);
  printf("c = %08" PRIx32 "\n", H[2]);
  printf("d = %08" PRIx32 "\n", H[3]);

  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;

    
}

int nextBlock(union block *M, FILE *inFile, uint64_t *nobits, enum flag *status)
{
  int i;

 if (*status == FINISH)
 {
  return 0;
 }

 if (*status == PAD0)
 {
  for (i = 0; i < 56; i++)
  {
    M->eight[i] = 0x00;
  }

  M->sixfour[7] = *nobits;
  *status = FINISH;
  return 1;
 }

 size_t nobytesread = fread(M->eight, 1, 64, inFile);
 *nobits += (8ULL * ((uint64_t) nobytesread));

 if (nobytesread == 64)
 {
   
   for (i = 0; i < 16; i++)
   {
     M->threetwo[i] = bswap_32(M->threetwo[i]);
   }
   
   
  
  return 1;
 }

 if (nobytesread < 56)
 {
    M->eight[nobytesread] = 0x80;
    for(i = nobytesread + 1; i < 56; i++)
    {
      M->eight[i] = 0x00;
    }

    M->sixfour[7] = *nobits;
    *status = FINISH;
    return 1;
 }

 M->eight[nobytesread] = 0x80;
 for (i = nobytesread + 1; i < 64; i++)
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

void FF(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  *a += F(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void GG(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  *a += G(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void HH(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  *a += H(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void II(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
  *a += I(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

