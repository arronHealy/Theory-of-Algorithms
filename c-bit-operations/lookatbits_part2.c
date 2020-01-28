#include <stdio.h>
#include <inttypes.h>

void print64bits(uint64_t s);

void print8bits(uint8_t s);


int main(int argc, char *argv[])
{
  uint64_t s = 65;

  uint8_t a = 73;

  uint8_t b = 99;

  uint8_t c = (a & b);

  uint8_t d = (a | b);

  uint8_t e = (a ^ b);


  printf("a = ");  print8bits(a); printf("\n");


  printf("b = "); print8bits(b); printf("\n");


  printf("c = (a & b) = "); print8bits(c); printf("\n");

  printf("d = (a | b) = "); print8bits(d); printf("\n");

  printf("e = (a ^ b) = "); print8bits(e); printf("\n");

  printf("\n");

  printf(" a = "); print8bits(a); printf("\n");

  printf("~a = "); print8bits(~a); printf("\n");

  printf("\n");
 
  printf(" 1       = "); print8bits(1); printf("\n");
  
  printf("(1 << 3) = "); print8bits((1 << 3)); printf("\n");

  printf("\n");

  printf(" 99       = "); print8bits(99); printf("\n");
  
  printf("(99 << 3) = ");
  print8bits(((uint8_t)99 << (uint8_t)3));
  printf("\n");

  
  print64bits(s);

  printf("%llx \t %lld \t \n", s, s);

  return 0;
}


void print64bits(uint64_t s)
{
  for(int i = 63; i >= 0; i--)
  {
    printf("%d", ((1ULL << i) & s) ? 1 : 0);
  }

  printf("\n");

}

void print8bits(uint8_t s)
{
  for(int i = (8 - 1); i >= 0; i--)
  {
    printf("%d", ((1ULL << i) & s) ? 1 : 0);

  }
}
