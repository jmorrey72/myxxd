#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
  for (int i = 0; i < size; i+=2) {
    if ((size - i) == 1) {
      printf("%02x   ", data[i]);
    } else {
    printf("%02x%02x ", data[i], data[i + 1]);
    }
  }
  if (size < 16) {
    int offset = 16 - size;
    for (int j = 0; j < (offset / 2); j++) {
      printf("     ");
    }
  }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
  for (int i = 0; i < size; i++) {
    if (data[i] < 32 || data[i] > 126) {
      printf(".");
    } else {
      printf("%c", data[i]);
    }
  }
}

void readAndPrintInputAsHex(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x: ", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}


void printDataAsBinary(unsigned char *data, size_t size) {
  unsigned int bits[8];
  for (int i = 0; i < size; i++) {
    unsigned char x = data[i];
    for (int j = 7; j >= 0; j--) {
      if (x % 2 == 1) {
        bits[j] = 1;
      } else {
        bits[j] = 0;
      }
      x = x / 2;
    }
    for (int k = 0; k < 8; k++) {
      printf("%d", bits[k]);
    }
    printf(" ");
  }
  if (size < 6) {
    int offset = 6 - size;
    for (int l = 0; l < offset; l++) {
      printf("         ");
    }
  }
}


/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x: ", offset);
    offset += numBytesRead;
    printDataAsBinary(data, numBytesRead);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }
}


int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
