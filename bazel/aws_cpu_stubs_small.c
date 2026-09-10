/**
 * CPU feature stubs for small tier (no CPU extensions)
 * Provides non-static implementations for functions that are normally
 * in encoding_avx2.c and cpuid.c
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Always return false - no AVX2 support for small tier
bool aws_common_private_has_avx2(void) {
  return false;
}

// SSE4.1 base64 encode - should never be called since has_avx2() returns false
void aws_common_private_base64_encode_sse41(const uint8_t* input,
                                            uint8_t* output, size_t inlen) {
  (void)input;
  (void)output;
  (void)inlen;
  // This function should never be called because aws_common_private_has_avx2()
  // returns false If it is called, it's a bug
}

// SSE4.1 base64 decode - should never be called since has_avx2() returns false
size_t aws_common_private_base64_decode_sse41(const unsigned char* in,
                                              unsigned char* out, size_t len) {
  (void)in;
  (void)out;
  (void)len;
  // This function should never be called because aws_common_private_has_avx2()
  // returns false If it is called, it's a bug
  return (size_t)-1;
}
