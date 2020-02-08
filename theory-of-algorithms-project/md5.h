//define data structure for MD5 processing
// https://www.ietf.org/rfc/rfc1321.txt -  page 8

struct md5_context {
  uint32_t state[4];  // message digest state (A,B,C,D)
  uint32_t count[2];  // number of bits, mod 2^64, least significant bit first or little endian first
  unsigned char buffer[64]; // message digest input buffer
};

