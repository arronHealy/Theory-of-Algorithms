## Note: For updated instructions on running the code in this repository, refer to overview.md file


# Theory-of-Algorithms

This Repository contains the project submission for my 4th year Computing in Software Development module in GMIT, Theory of Algorithms. The following sections give an more in depth explanation about this repository and the project specification.

<hr>

# About this Repository

The main project and accompanying files for this module can be found in the theory-of-algorithms-project folder. The other folders contain code developed throughout the duration of the module through a series of video lectures/labs that our lecturer had prepared for us. The hello-programs folder contains simple hello world programs that were developed as an introduction to this module where we set up a Debian virtual machine using Google Cloud to develop our code with. The c-bit-operations folder contains C programs that were developed to give us a better understanding of Bitwise operations. The sha256 folder contains a side project that was developed to go along with our main project for the duration of the module. Over a series of video lectures, we developed the SHA256 hashing algorithm in the C programming language that would encrypt a message using a one way compression function.

<br />

The main source used to develop the SHA256 algorithm can be found here as a downloadable PDF:

[National Institute of Standards and Technology](https://www.nist.gov/publications/secure-hash-standard)

## Example SHA256 Output

### Running my local version of SHA256
```
./sha256 test.txt

b3e82811253820e3597346571e1d0e5f4fa37932232d1f3163cfe98c120c4c53
```

### Running openssl version of SHA256

```
openssl sha256 test.txt

SHA256(test.txt)= b3e82811253820e3597346571e1d0e5f4fa37932232d1f3163cfe98c120c4c53
```
<hr>

# Project Specification

Our main project for this module was to write a program in the C programming language that calculates the MD5 hash digest of an input. The algorithm is specified in the [Request For Comments 1321 document supplied by the Internet Engineering Task Force.](https://tools.ietf.org/html/rfc1321) The only pre-requisite is that the program performs the algorithm — we were free to decide what input the algorithm should be performed on.

# The MD5 Algorithm

MD5 message digest algorithm is the 5th version of the Message Digest Algorithm developed by Ron Rivest to produce 128 bit message digest. MD5 is quite faster than other versions of message digest which takes the plain text of 512 bit blocks which is further divided into 16 blocks, each of 32 bit and produces the 128 bit message digest which is a set of four blocks, each of 32 bits. This encryption of input of any size into hash values takes 5 steps and each step has its a predefined task.

## Step 1: Append Padding Bits

Padding means adding extra bits to the original message. So the original message is padded such that its length in bits is congruent to 448 modulo 512. Padding is done such that the total bits are 64 less being a multiple of 512 bits length. Padding is done even if the length of the original message is already congruent to 448 modulo 512. When padding, the first bit is 1 and the rest of the bits are 0.

## Step 2: Append Length

A 64-bit representation of the length of the message before the padding bits were added is appended to the result of the previous step. At this point, the resulting message has a length multiple of 512 bits.

## Step 3: Initialize MD buffer

A four-word buffer (A,B,C,D) is used to compute the message digest. Here each of A, B, C, D is a 32-bit register. These registers are initialized to the following values in hexadecimal, low-order bytes first.

```
word A: 01 23 45 67 - in Hex format: 0x67452301
word B: 89 ab cd ef - in Hex format: 0xefcdab89
word C: fe dc ba 98 - in Hex format: 0x98badcfe
word D: 76 54 32 10 - in Hex format: 0x10325476
```

## Step 4: Process Message in 16-Word Blocks

We first define four auxiliary functions that each take as input three 32-bit words and produce as output one 32-bit word. These functions use logical operators like AND, XOR, NOT.

```
F(X,Y,Z) = XY and not(X) Z
G(X,Y,Z) = XZ and Y not(Z)
H(X,Y,Z) = X xor Y xor Z
I(X,Y,Z) = Y xor (X and not(Z))
```
This step uses a 64-element table T[1 ... 64] constructed from the sine function. Let T[i] denote the i-th element of the table, which is equal to the integer part of 4294967296 times abs(sin(i)), where i is in radians. Each 512 bit block undergoes 4 rounds of calculations where each round has 16 steps.

```
Save a as aa, b as bb, c as cc, and d as dd.

aa = a
bb = b
cc = c
dd = d
     
/* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */
  
Then perform the following additions. (That is increment each of the four registers by the value it had before this block was started.)
   
 a = a + aa
 b = b + bb
 c = c + cc
 d = d + dd
```
# Step 5: Output the message

The message digest produced as output is A, B, C, D. That is, we begin with the low-order byte of A, and end with the high-order byte of D.

<hr>

# Running the program

To run the code in this repository the following software is required.

To clone this repository you will need the Git software installed on your machine. To install Git on your machine follow this [Git link](https://git-scm.com/) and follow the installation instructions. 

To run any of the C programs in this repository you will need a C compiler to be installed on your machine. The GCC compiler was used to develop this project and is the recommended compiler to use when running this code. To install the GCC compiler follow this [GCC link](https://gcc.gnu.org/install/index.html) and follow the installation instructions.

## Cloning the Repository

When cloning this repository, assuming you have the Git software installed. Open a terminal window and navigate to a location where you would like this repository to be cloned. Once at this location enter the following command in the terminal.

```
git clone https://github.com/arronHealy/Theory-of-Algorithms.git
```

Once the repository is cloned move into the project folder with the following command.

```
cd theory-of-algorithms-project
```
Feel free to change into any of the other directories and run those programs. The next step is to compile and run the program.

## Compiling and Running the Program

Assuming you have the GCC compiler successfully installed, To compile and run the program make sure you are in the project folder and enter the following command

```
gcc -o md5 md5.c

or you can try

make md5
```

Assuming that command executed without any error messages showing which it should have if the previous steps were completed. The final step is to run the program which will prompt the user to either enter a string of their choosing or to enter a file path. There are some test files included in the project folder, but feel free to edit or make your own if you wish. Finally it is time to run the program and view the MD5 hash output from a given input. Run the command

```
./md5
```

If the program executed correctly you should see this output in your terminal window.

```
MD5 Cryptographic Hash Function Generator
Enter -1 to exit the program!
Enter 1 if you wish to enter Text to be hashed
Enter 2 if you wish the contents of a file to be hashed
Enter option:
```

<hr>

# Testing the MD5 Hash Output

To be sure the output of my program was correct I tested it against two other MD5 Hash Generators. One was an online MD5 Hash generator which can be found [here](https://www.md5hashgenerator.com/). The other was openssl which is a software library for applications that secure communications over computer networks and provides many implementations of hashing algorithms which can be used to test against. An example of it being used with the SHA256 algorithm is provided above. On both occasions the tests passed and I am satisfied the algorithm has been implemented successfully.

## Testing my version of the MD5 Algorithm

```
MD5 Cryptographic Hash Function Generator
Enter -1 to exit the program!
Enter 1 if you wish to enter Text to be hashed
Enter 2 if you wish the contents of a file to be hashed
Enter option: 2

Enter the File path you wish to read: empty
file opened!

MD5 Generated Hash:
d41d8cd98f00b204e9800998ecf8427e
```

## Testing using opessl version of the MD5 Algorithm

```
openssl md5 empty

MD5(empty)= d41d8cd98f00b204e9800998ecf8427e
```

<hr>

# References

The following is a list of references used to help further my understanding of the MD5 Algorithm during the development of this project

- https://tools.ietf.org/html/rfc1321
- https://www.educba.com/md5-alogrithm/
- http://practicalcryptography.com/hashes/md5-hash/
- https://www.youtube.com/watch?v=53O9J2J5i14
- https://stackoverflow.com/questions/48818204/implementing-md5-inconsistent-endianness
- https://stackoverflow.com/questions/7627723/how-to-create-a-md5-hash-of-a-string-in-c
- https://crypto.stackexchange.com/questions/50836/md5-what-happens-if-message-is-exactly-64bit-block-size-message
- https://www.slideshare.net/sourav777/sourav-md5
- https://crypto.stackexchange.com/questions/41431/what-does-congruent-to-448-modulo-512-mean-for-padding-in-md5-hash-function
- https://people.csail.mit.edu/rivest/Md5.c
- https://dev.w3.org/libwww/modules/md5/md5.c
- https://www.netlib.org/crc/md5sum.c
