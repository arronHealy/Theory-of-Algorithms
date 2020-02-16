// Arron Healy
// Padding for sha256

#include <stdio.h>
#include <inttypes.h>

union block
{
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
};


uint64_t numOfZeroBytes(uint64_t nobits);


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Error: Expected a single filename as argument! \n");
    return 1;
  }

  FILE *inFile = fopen(argv[1], "rb");

  if (!inFile)
  {
    printf("Error: Couldn't open file %s \n", argv[1]);
    return 1;
  }
  
  uint64_t nobits;
  union block M;
  uint8_t i;

  for(nobits = 0, i = 0; fread(&M.eight[i], 1, 1, inFile) == 1; nobits += 8)
  {
    printf("%02" PRIx8, M.eight[i]);
  }
  
  //printf(" ");

  printf("%02" PRIx8, 0x80); // Bits: 1000 0000

  //printf(" ");

  for (uint64_t i = numOfZeroBytes(nobits); i > 0; i--)
  {
    printf("%02" PRIx8, 0x00);
  }
  
  //printf(" ");

  printf("%016" PRIx64 "\n", nobits);

  printf("\n");

  //printf("nobits val is %" PRIu64 " \n", nobits);
  
  //printf("numOfZeroBytes is %" PRIu64 " \n", numOfZeroBytes(nobits));

  fclose(inFile);

  return 0;
}

uint64_t numOfZeroBytes(uint64_t nobits)
{
  uint64_t result = 512ULL - (nobits % 512ULL); // ULL: Unsigned Long Long 
  
  if (result < 65)
  {
    result += 512;
  }

  result -= 72;

  return (result / 8ULL);
}
