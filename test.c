#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base32.h"

void try_encode(const char *input, const char *expected) {
    printf("test:\n");

    printf("  encode: %s\n", input);
    char encoded[100];
    int encoded_len = base32_encode((uint8_t *)input, strlen(input),
            (uint8_t *)encoded, 100);
    if (encoded_len < 0) {
        printf("    error: encode failed\n");
    } else {
        encoded[encoded_len] = '\0';
        if (strcmp(encoded, expected) != 0) {
            printf("    error: invalid encode => %s\n", encoded);
        }
    }

    printf("  decode: %s\n", expected);
    char decoded[100];
    int decoded_len = base32_decode((uint8_t *)expected, strlen(expected),
            (uint8_t *)decoded, 100);
    if (decoded_len < 0) {
        printf("    error: decode failed\n");
    } else {
        decoded[decoded_len] = '\0';
        if (strcmp(input, decoded) != 0) {
            printf("    error: invalid decode => %s\n", decoded);
        }
    }
}

int main(int argc, char **argv) {
    try_encode("f", "CR");
    try_encode("foobar", "CSQPYRK1E8");
    try_encode("Hello, world!", "91JPRV3F5GG7EVVJDHJ22");
    try_encode("The quick brown fox jumps over the lazy dog.", "AHM6A83HENMP6TS0C9S6YXVE41K6YY10D9TPTW3K41QQCSBJ41T6GS90DHGQMY90CHQPEBG");
    return 0;
}
