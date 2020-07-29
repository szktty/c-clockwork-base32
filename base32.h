#include <stdint.h>

// return output length or -1
extern int base32_encode(const uint8_t *input, const int input_len,
                         uint8_t *output, const int output_len);
extern int base32_decode(const uint8_t *input, const int input_len,
                         uint8_t *output, const int output_len);
