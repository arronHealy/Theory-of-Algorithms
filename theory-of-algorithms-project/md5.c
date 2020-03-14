#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <byteswap.h>

// include md5 definitions via header file so variables can be seen globally and not restricted to being first defined in main function
#include "md5.h"

// links that lead me to use header file
// https://stackoverflow.com/questions/8915230/invalid-application-of-sizeof-to-incomplete-type-with-a-struct
// https://stackoverflow.com/questions/2576554/c-programming-dereferencing-pointer-to-incomplete-type-error

// main function to run program
int main(int argc, char *argv[])
{
  int userOption;
 
  do{
       printf("\n\nMD5 Cryptographic Hash Function Generator");
       printf("\nEnter -1 to exit the program!");
       printf("\nEnter 1 if you wish to enter Text to be hashed");
       printf("\nEnter 2 if you wish the contents of a file to be hashed");
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

void readFileInput()
{
  char* filePath;
  
  FILE* readFile;

  BLOCK M;

  uint64_t nobits = 0;

  FLAG status = READ;

  // initialization of md5 context hash constants
  // https://www.ietf.org/rfc/rfc1321.txt - page 11
  WORD H[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

  // allocate memory for string input
  filePath = (char*)malloc(50 * sizeof(char));
  
  // prompt user for string
  printf("\nEnter the File path you wish to read: ");
  scanf("%s", filePath);

  // read file contents as binary data
  readFile = fopen(filePath, "rb");

  // try and access file
  if(readFile == NULL)
  {
    printf("\nError reading file %s, Please Try again...", filePath);
    return;
  }
  else
  {
    printf("file opened!\n");
    // free memory used for string
    free(filePath);

   // loop for file contents and perform padding &hashing functions
   while(nextBlock(&M, readFile, &nobits, &status))
   {
      nexthash(&M, H);
   }

   printf("\nMD5 Generated Hash:\n");

   // loop for size of H and print MD5 Hash after transformations
   for (int i = 0; i < 4; i++)
   {
     // print MD5 hash in Big endian
     printf("%08" PRIx32 "", bswap_32(H[i]));
   }

    // close file before exiting function
    fclose(readFile);
  }
  
}


void readStringInput()
{
  int writeStatus;

  BLOCK M;

  uint64_t nobits = 0;

  FLAG status = READ;

  char* messageText;

  FILE* inputFile;

  // initialization of md5 context hash constants
  // https://www.ietf.org/rfc/rfc1321.txt - page 11
  WORD H[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

  // allocate memory for string input
  messageText = (char*)malloc(50 * sizeof(char));
 
  // prompt user for string input
  printf("\nPlease enter the Text to be hashed: ");
  scanf("%s", messageText);

  printf("Entered Text is: %s", messageText);
  printf("\n");

  // try and write string input to file
  writeStatus = writeToFile(messageText);
  
  if (writeStatus == 0)
  {
    printf("Error writing string to file! Please try again.\n");
    return;
  }
  else
  {
    // free memory of string input if written to file
    free(messageText);  
  }//if

  // read file contents as binary data
  inputFile = fopen("inputText.txt", "rb");

 
  if (inputFile == NULL)
  {
    printf("\nError: failed to access stored string input! Please try again.");
    return;
  }
  else
  {
    printf("file opened!\n");

    // loop for file contents and perform padding &hashing functions
    while(nextBlock(&M, inputFile, &nobits, &status))
    {
        nexthash(&M, H);
    }

    printf("\nMD5 Generated Hash:\n");

    // loop for size of H and print MD5 Hash after transformations
    for (int i = 0; i < 4; i++)
    {
      // print MD5 hash in Big endian
      printf("%08" PRIx32 "", bswap_32(H[i]));
    }
  
    // close file before finished
    fclose(inputFile);
  }//if
  

}//readFileInput


void nexthash(BLOCK *M, WORD *H)
{
  // perform MD5 hash transformation function
  // https://www.ietf.org/rfc/rfc1321.txt - pages 13&14

  WORD a = H[0], b = H[1], c = H[2], d = H[3];

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


  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;

    
}

int nextBlock(BLOCK *M, FILE *inFile, uint64_t *nobits, FLAG *status)
{
  int i;

  size_t nobytesread;

  switch(*status)
  {
    case FINISH:
      // padding of message is complete end loop 
      return 0;
    case PAD0:
      // pad message with 56 bytes or 448 bits with all 0's with 64 bits remaining  
      for (i = 0; i < 56; i++)
      {
        M->eight[i] = 0x00;
      }

      // https://www.ietf.org/rfc/rfc1321.txt - page 3
      // section 3.2
      // append original message length in 64 bit format to end of padded message
      // original message length added as little-endian format
      // https://stackoverflow.com/questions/48818204/implementing-md5-inconsistent-endianness
      
      M->sixfour[7] = *nobits;

      // padding complete set status to FINISH to stop anymore padding of message
      *status = FINISH;
      break;
    default:
      // try and read 64 bytes or 512 bits from the file
      nobytesread = fread(M->eight, 1, 64, inFile);
      // keep track of number of bits read from the file
      *nobits += (8ULL * ((uint64_t) nobytesread));

      // if nobytes read less than 64 we can put all padding in this block
      if (nobytesread < 56)
      {
          // append the 1 bit to message
          // https://www.ietf.org/rfc/rfc1321.txt - page 3
          // section 3.1
          M->eight[nobytesread] = 0x80;

          // pad message with 56 bytes or 448 bits with all 0's with 64 bits remaining
          // https://www.ietf.org/rfc/rfc1321.txt - page 3
          // section 3.1
          for(i = nobytesread + 1; i < 56; i++)
          {
            M->eight[i] = 0x00;
          }

          // https://www.ietf.org/rfc/rfc1321.txt - page 3
          // section 3.2
          // append original message length in 64 bit format to end of padded message
          // original message length added as little-endian format
          // https://stackoverflow.com/questions/48818204/implementing-md5-inconsistent-endianness
          
          M->sixfour[7] = *nobits;
          
          // padding complete set status to FINISH to stop anymore padding of message
          *status = FINISH;
          return 1;
      }
      else if(nobytesread < 64)
      {
        M->eight[nobytesread] = 0x80;
        for (i = nobytesread + 1; i < 64; i++)
        {
            M->eight[i] = 0x00;
        }
        
        *status = PAD0;
      }

  }//end switch

  return 1;
}

// write user input string to file, so it can be read back in as binary and converted to hexidecimal
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


// FF, GG, HH, and II transformation functions for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
// https://www.ietf.org/rfc/rfc1321.txt - pages 10 & 11


void FF(WORD *a, WORD b, WORD c, WORD d, WORD x, WORD s, WORD ac)
{
  *a += F(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void GG(WORD *a, WORD b, WORD c, WORD d, WORD x, WORD s, WORD ac)
{
  *a += G(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void HH(WORD *a, WORD b, WORD c, WORD d, WORD x, WORD s, WORD ac)
{
  *a += H(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

void II(WORD *a, WORD b, WORD c, WORD d, WORD x, WORD s, WORD ac)
{
  *a += I(b, c, d) + x + ac;
  *a = b + ROTL(*a, s);
}

