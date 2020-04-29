#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>
#include <string.h>
#include <byteswap.h>


// cmocka include files
#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<cmocka.h>


// include md5 definitions via header file so variables can be seen globally and not restricted to being first defined in main function
#include "md5.h"

// links that lead me to use header file
// https://stackoverflow.com/questions/8915230/invalid-application-of-sizeof-to-incomplete-type-with-a-struct
// https://stackoverflow.com/questions/2576554/c-programming-dereferencing-pointer-to-incomplete-type-error

static struct option long_options[] = {
    {"help",  no_argument,  0,  'h' },
    {"test",  no_argument,  0,  't' },
    {"file",  no_argument,  0,  'f'},
    {"string", no_argument, 0,  's'}
};

// main function to run program
int main(int argc, char *argv[])
{
  // define an array of the cmocka tests to run
  const struct CMUnitTest tests[] = {
        cmocka_unit_test(TESTCASE_nexthash_equals),
        cmocka_unit_test(TESTCASE_FF_ROUND_ONE_equals),
        cmocka_unit_test(TESTCASE_GG_ROUND_TWO_equals),
        cmocka_unit_test(TESTCASE_HH_ROUND_THREE_equals),
        cmocka_unit_test(TESTCASE_II_ROUND_FOUR_equals),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_1),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_2),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_3),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_4),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_5),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_6),
        cmocka_unit_test(TESTCASE_RFC_1321_TEST_7)
    };

    int option = 0, index = 0;

     if (argc != 2)
    {
        printf("Error: Expected an argument! \n");
        return 1;
    }

    while ((option = getopt_long(argc, argv, "htfs:", long_options, &index )) != -1) {
        switch (option) {
             case 'h':
                printHelpOptions();
             break;
             
             case 't': 
                return cmocka_run_group_tests(tests, NULL, NULL);
             break;

             case 'f':
                readFileInput();
             break;

             case 's':
              readStringInput();
             break;

             default:
                printf("Error! Unrecognized command. Please Try again... \n");
        }
    }

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
      // print MD5 hash by swapping bit order
     printf("%08" PRIx32 "", bswap_32(H[i]));
   }

   printf("\n");

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

  char string[100];

  FILE* inputFile;

  // initialization of md5 context hash constants
  // https://www.ietf.org/rfc/rfc1321.txt - page 11
  WORD H[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
 
  // prompt user for string input
  printf("\nPlease enter the Text to be hashed: ");

  fgets (string, 100, stdin);
  /* Here remove  the trailing new line char; not required unless you are trying to print the string */
  string[strlen(string) - 1] = '\0';

  printf("Entered Text is: %s", string);
  printf("\n");

  // try and write string input to file
  writeStatus = writeToFile(string);
  
  if (writeStatus == 0)
  {
    printf("Error writing string to file! Please try again.\n");
    return;
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
      // print MD5 hash by swapping bit order
      printf("%08" PRIx32 "", bswap_32(H[i]));
    }

    printf("\n");
  
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


void printHelpOptions()
{
  printf("Options: \n");
  printf("\t --help \t\t Display this information.\n");
  printf("\t --test \t\t Executes the suite of cmocka unit tests defined in this file.\n");
  printf("\t --string \t\t Prompts the user for an input string and then prints out the MD5 hashed output for that string.\n");
  printf("\t --file \t\t Prompts the user for a file path and then prints out the MD5 hashed outptut for the file contents.\n");
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

// cmocka unit tests

void TESTCASE_FF_ROUND_ONE_equals(void **state){
    WORD H1[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block1, test_block2;

    for(int i = 0; i < 64; i++){
        test_block1.eight[i] = 0x00;
        test_block2.eight[i] = 0x00;
    }

    /* Round 1 */
    FF(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[0], S11, 0xd76aa478); /*1*/
    FF(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[1], S12, 0xe8c7b756); /*2*/
    FF(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[2], S13, 0x242070db); /*3*/
    FF(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[3], S14, 0xc1bdceee); /*4*/
    FF(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[4], S11, 0xf57c0faf); /*5*/
    FF(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[5], S12, 0x4787c62a); /*6*/
    FF(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[6], S13, 0xa8304613); /*7*/
    FF(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[7], S14, 0xfd469501); /*8*/
    FF(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[8], S11, 0x698098d8); /*9*/
    FF(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[9], S12, 0x8b44f7af); /*10*/
    FF(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[10], S13, 0xffff5bb1); /*11*/
    FF(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[11], S14, 0x895cd7be); /*12*/
    FF(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[12], S11, 0x6b901122); /*13*/
    FF(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[13], S12, 0xfd987193); /*14*/
    FF(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[14], S13, 0xa679438e); /*15*/
    FF(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[15], S14, 0x49b40821); /*16*/


    /* Round 1 */
    FF(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[0], S11, 0xd76aa478); /*1*/
    FF(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[1], S12, 0xe8c7b756); /*2*/
    FF(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[2], S13, 0x242070db); /*3*/
    FF(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[3], S14, 0xc1bdceee); /*4*/
    FF(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[4], S11, 0xf57c0faf); /*5*/
    FF(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[5], S12, 0x4787c62a); /*6*/
    FF(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[6], S13, 0xa8304613); /*7*/
    FF(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[7], S14, 0xfd469501); /*8*/
    FF(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[8], S11, 0x698098d8); /*9*/
    FF(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[9], S12, 0x8b44f7af); /*10*/
    FF(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[10], S13, 0xffff5bb1); /*11*/
    FF(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[11], S14, 0x895cd7be); /*12*/
    FF(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[12], S11, 0x6b901122); /*13*/
    FF(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[13], S12, 0xfd987193); /*14*/
    FF(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[14], S13, 0xa679438e); /*15*/
    FF(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[15], S14, 0x49b40821); /*16*/

    assert_int_equal(H1[0], H2[0]);
    assert_int_equal(H1[1], H2[1]);
    assert_int_equal(H1[2], H2[2]);
    assert_int_equal(H1[3], H2[3]);
}

void TESTCASE_GG_ROUND_TWO_equals(void **state){
    WORD H1[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block1, test_block2;

    for(int i = 0; i < 64; i++){
        test_block1.eight[i] = 0x00;
        test_block2.eight[i] = 0x00;
    }

    // Round 2 
    GG(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[1], S21, 0xf61e2562); //17
    GG(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[6], S22, 0xc040b340); //18
    GG(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[11], S23, 0x265e5a51); //19
    GG(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[0], S24, 0xe9b6c7aa); //20
    GG(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[5], S21, 0xd62f105d); //21
    GG(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[10], S22, 0x2441453); //22
    GG(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[15], S23, 0xd8a1e681); //23
    GG(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[4], S24, 0xe7d3fbc8); //24
    GG(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[9], S21, 0x21e1cde6); //25
    GG(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[14], S22, 0xc33707d6); //26
    GG(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[3], S23, 0xf4d50d87); //27
    GG(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[8], S24, 0x455a14ed); //28
    GG(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[13], S21, 0xa9e3e905); //29
    GG(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[2], S22, 0xfcefa3f8); //30
    GG(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[7], S23, 0x676f02d9); //31
    GG(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[12], S24, 0x8d2a4c8a); //32

    // Round 2 
    GG(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[1], S21, 0xf61e2562); //17
    GG(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[6], S22, 0xc040b340); //18
    GG(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[11], S23, 0x265e5a51); //19
    GG(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[0], S24, 0xe9b6c7aa); //20
    GG(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[5], S21, 0xd62f105d); //21
    GG(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[10], S22, 0x2441453); //22
    GG(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[15], S23, 0xd8a1e681); //23
    GG(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[4], S24, 0xe7d3fbc8); //24
    GG(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[9], S21, 0x21e1cde6); //25
    GG(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[14], S22, 0xc33707d6); //26
    GG(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[3], S23, 0xf4d50d87); //27
    GG(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[8], S24, 0x455a14ed); //28
    GG(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[13], S21, 0xa9e3e905); //29
    GG(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[2], S22, 0xfcefa3f8); //30
    GG(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[7], S23, 0x676f02d9); //31
    GG(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[12], S24, 0x8d2a4c8a); //32

    assert_int_equal(H1[0], H2[0]);
    assert_int_equal(H1[1], H2[1]);
    assert_int_equal(H1[2], H2[2]);
    assert_int_equal(H1[3], H2[3]);
}

void TESTCASE_HH_ROUND_THREE_equals(void **state){
    WORD H1[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block1, test_block2;

    for(int i = 0; i < 64; i++){
        test_block1.eight[i] = 0x00;
        test_block2.eight[i] = 0x00;
    }

    // Round 3 
    HH(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[5], S31, 0xfffa3942); //33
    HH(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[8], S32, 0x8771f681); //34
    HH(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[11], S33, 0x6d9d6122); //35
    HH(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[14], S34, 0xfde5380c); //36
    HH(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[1], S31, 0xa4beea44); //37
    HH(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[4], S32, 0x4bdecfa9); //38
    HH(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[7], S33, 0xf6bb4b60); //39
    HH(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[10], S34, 0xbebfbc70); //40
    HH(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[13], S31, 0x289b7ec6); //41
    HH(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[0], S32, 0xeaa127fa); //42
    HH(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[3], S33, 0xd4ef3085); //43
    HH(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[6], S34, 0x4881d05); //44
    HH(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[9], S31, 0xd9d4d039); //45
    HH(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[12], S32, 0xe6db99e5); //46
    HH(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[15], S33, 0x1fa27cf8); //47
    HH(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[2], S34, 0xc4ac5665); //48

    // Round 3 
    HH(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[5], S31, 0xfffa3942); //33
    HH(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[8], S32, 0x8771f681); //34
    HH(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[11], S33, 0x6d9d6122); //35
    HH(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[14], S34, 0xfde5380c); //36
    HH(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[1], S31, 0xa4beea44); //37
    HH(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[4], S32, 0x4bdecfa9); //38
    HH(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[7], S33, 0xf6bb4b60); //39
    HH(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[10], S34, 0xbebfbc70); //40
    HH(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[13], S31, 0x289b7ec6); //41
    HH(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[0], S32, 0xeaa127fa); //42
    HH(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[3], S33, 0xd4ef3085); //43
    HH(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[6], S34, 0x4881d05); //44
    HH(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[9], S31, 0xd9d4d039); //45
    HH(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[12], S32, 0xe6db99e5); //46
    HH(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[15], S33, 0x1fa27cf8); //47
    HH(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[2], S34, 0xc4ac5665); //48

    assert_int_equal(H1[0], H2[0]);
    assert_int_equal(H1[1], H2[1]);
    assert_int_equal(H1[2], H2[2]);
    assert_int_equal(H1[3], H2[3]);
}

void TESTCASE_II_ROUND_FOUR_equals(void **state){
    WORD H1[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block1, test_block2;

    for(int i = 0; i < 64; i++){
        test_block1.eight[i] = 0x00;
        test_block2.eight[i] = 0x00;
    }

    // Round 4
    II(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[0], S41, 0xf4292244); //49
    II(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[7], S42, 0x432aff97); //50
    II(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[14], S43, 0xab9423a7); //51
    II(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[5], S44, 0xfc93a039); //52
    II(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[12], S41, 0x655b59c3); //53
    II(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[3], S42, 0x8f0ccc92); //54
    II(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[10], S43, 0xffeff47d); //55
    II(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[1], S44, 0x85845dd1); //56
    II(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[8], S41, 0x6fa87e4f); //57
    II(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[15], S42, 0xfe2ce6e0); //58
    II(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[6], S43, 0xa3014314); //59
    II(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[13], S44, 0x4e0811a1); //60
    II(&H1[0], H1[1], H1[2], H1[3], test_block1.threetwo[4], S41, 0xf7537e82); //61
    II(&H1[3], H1[0], H1[1], H1[2], test_block1.threetwo[11], S42, 0xbd3af235); //62
    II(&H1[2], H1[3], H1[0], H1[1], test_block1.threetwo[2], S43, 0x2ad7d2bb); //63
    II(&H1[1], H1[2], H1[3], H1[0], test_block1.threetwo[9], S44, 0xeb86d391); //64

    // Round 4
    II(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[0], S41, 0xf4292244); //49
    II(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[7], S42, 0x432aff97); //50
    II(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[14], S43, 0xab9423a7); //51
    II(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[5], S44, 0xfc93a039); //52
    II(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[12], S41, 0x655b59c3); //53
    II(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[3], S42, 0x8f0ccc92); //54
    II(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[10], S43, 0xffeff47d); //55
    II(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[1], S44, 0x85845dd1); //56
    II(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[8], S41, 0x6fa87e4f); //57
    II(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[15], S42, 0xfe2ce6e0); //58
    II(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[6], S43, 0xa3014314); //59
    II(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[13], S44, 0x4e0811a1); //60
    II(&H2[0], H2[1], H2[2], H2[3], test_block2.threetwo[4], S41, 0xf7537e82); //61
    II(&H2[3], H2[0], H2[1], H2[2], test_block2.threetwo[11], S42, 0xbd3af235); //62
    II(&H2[2], H2[3], H2[0], H2[1], test_block2.threetwo[2], S43, 0x2ad7d2bb); //63
    II(&H2[1], H2[2], H2[3], H2[0], test_block2.threetwo[9], S44, 0xeb86d391); //64

    assert_int_equal(H1[0], H2[0]);
    assert_int_equal(H1[1], H2[1]);
    assert_int_equal(H1[2], H2[2]);
    assert_int_equal(H1[3], H2[3]);

}


void TESTCASE_nexthash_equals(void **state){
    WORD H1[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block1, test_block2;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "empty";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block1, testFile, &nobits, &status))
    {
        nexthash(&test_block1, H1);
    }

    fclose(testFile);

    status = READ;
    nobits = 0;

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block2, testFile, &nobits, &status))
    {
        nexthash(&test_block2, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], H2[0]);
    assert_int_equal(H1[1], H2[1]);
    assert_int_equal(H1[2], H2[2]);
    assert_int_equal(H1[3], H2[3]);
}

// these tests are defined in the RFC 1321 - Page 20 - A.5 Test suite
// https://tools.ietf.org/html/rfc1321

void TESTCASE_RFC_1321_TEST_1(void **state)
{
    // expected values for test
    WORD H1[] = {0xd41d8cd9, 0x8f00b204, 0xe9800998, 0xecf8427e};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "empty";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_2(void **state)
{
    // expected values for test
    WORD H1[] = {0x0cc175b9, 0xc0f1b6a8, 0x31c399e2, 0x69772661};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test2.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_3(void **state)
{
    // expected values for test
    WORD H1[] = {0x90015098, 0x3cd24fb0, 0xd6963f7d, 0x28e17f72};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test3.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_4(void **state)
{
    // expected values for test
    WORD H1[] = {0xf96b697d, 0x7cb7938d, 0x525a2f31, 0xaaf161d0};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test4.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_5(void **state)
{
    // expected values for test
    WORD H1[] = {0xc3fcd3d7, 0x6192e400, 0x7dfb496c, 0xca67e13b};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test5.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_6(void **state)
{
    // expected values for test
    WORD H1[] = {0xd174ab98, 0xd277d9f5, 0xa5611c2c, 0x9f419d9f};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test6.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}

void TESTCASE_RFC_1321_TEST_7(void **state)
{
    // expected values for test
    WORD H1[] = {0x57edf4a2, 0x2be3c955, 0xac49da2e, 0x2107b67a};

    // md5 initial starting values
    WORD H2[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    BLOCK test_block;

    FILE* testFile;

    uint64_t nobits = 0;

    FLAG status = READ;

    char filePath[] = "rfc-test7.txt";

    testFile = fopen(filePath, "rb");

    while(nextBlock(&test_block, testFile, &nobits, &status))
    {
        nexthash(&test_block, H2);
    }

    fclose(testFile);

    assert_int_equal(H1[0], bswap_32(H2[0]));
    assert_int_equal(H1[1], bswap_32(H2[1]));
    assert_int_equal(H1[2], bswap_32(H2[2]));
    assert_int_equal(H1[3], bswap_32(H2[3]));
}