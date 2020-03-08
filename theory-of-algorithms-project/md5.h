// https://www.ietf.org/rfc/rfc1321.txt - page 10
// Constants for MD5 transform routine
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


// C preprocessor is a macro preprocessor (allows you to define macros) that transforms your program before it is compiled
// https://www.programiz.com/c-programming/c-preprocessor-macros

// word as 32 bit integer
#define WORD uint32_t

// rotate bits to left method definition from md5 standard
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// c macro hash functions
// https://www.ietf.org/rfc/rfc1321.txt -  page 10
#define F(x, y, z) ((x & y) | (~x & z))

#define G(x, y, z) ((x & z) | (y & ~z))

#define H(x, y, z) (x ^ y ^ z)

#define I(x, y, z) (y ^ (x | ~z))

// union data structure, allows access of a 64 byte block of memory with different types
// adapted to reflect md5 struct data struct
// https://www.ietf.org/rfc/rfc1321.txt - page 8
typedef union
{
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
} BLOCK;

// enum used to keep track of where the program is in padding the message
typedef enum {
  READ,
  PAD0,
  FINISH
} FLAG;

// md5.c program function definitions
void FF(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void GG(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void HH(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void II(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

void readStringInput();

void readFileInput();

int writeToFile(char* input);

int nextBlock(union BLOCK *M, FILE *inFile, uint64_t *nobits, enum FLAG *status);

void nexthash(union BLOCK *M, uint32_t *H);
