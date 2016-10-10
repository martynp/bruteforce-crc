
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include "crc.hpp"

crc::crc(unsigned int width) {

  assert(width <= 32);
  width_ = width;

  // Create a bitmask for the fixed bit width
  mask_ = 0;
  for (size_t i = 0; i < width; i++)
    mask_ = (mask_ << 1) | 1;

  // Store just the high bit mask
  high_bit_mask_ = (uint32_t)1 << (width - (uint32_t)1);
}

void crc::set_parameters(uint32_t polynomial, uint32_t initial,
                         uint32_t final_xor, bool reflected_input,
                         bool reflected_output) {

  // Set local variables
  polynomial_ = polynomial;
  calculate_table();
  initial_ = initial;
  final_xor_ = final_xor;
  reflected_input_ = reflected_input;
  reflected_output_ = reflected_output;
}

// TODO: inverse feeding of strings is a problem
// bit alignment does not work when looking at data backwards

/*
 * Set string data, if known crc and boolean to reset the rest of the data in
 * the object
 */
void crc::set_data(std::string data, uint32_t crc, bool reset_list) {

  // We require a forward and reversed version of the data
  data_t data_vector;
  data_t data_vector_reversed;

  if (reset_list) {
    // TODO
  }

  // Get the total number of whole bytes in string
  size_t bytes = data.length() / 8;
  size_t remaining_bits = data.length() % 8;

  // As strings are read continiously from left to right, extra care is
  // required when reversing to preserve 'byte' order
  std::string data_reversed = data;
  if (remaining_bits > 0) {
    data_vector.unused_bits = data_vector_reversed.unused_bits =
        8 - remaining_bits;
    data.insert(data.length(), data_vector.unused_bits, '0');
    data_reversed.insert(0, data_vector_reversed.unused_bits, '0');
    bytes++; // There is an 'extra' byte
  } else {
    data_vector.unused_bits = data_vector_reversed.unused_bits = 0;
  }

  // Reverse the 'front' padded string
  std::reverse(data_reversed.begin(), data_reversed.end());

  // Convert the string to binary
  for (size_t i = 0; i < bytes; i++) {
    std::string str_byte = data.substr(8 * i, 8);
    std::reverse(str_byte.begin(), str_byte.end());
    uint8_t bin_byte = binary_string_to_byte(str_byte);
    data_vector.data.push_back(bin_byte);
  }

  // Convert the reversed string to binary
  for (size_t i = 0; i < bytes; i++) {
    std::string str_byte = data_reversed.substr(8 * i, 8);
    std::reverse(str_byte.begin(), str_byte.end());
    uint8_t bin_byte = binary_string_to_byte(str_byte);
    data_vector_reversed.data.push_back(bin_byte);
  }

  data_vector.crc = data_vector_reversed.crc = crc;
  data_vectors.push_back(data_vector);
  data_vectors_reversed.push_back(data_vector_reversed);
}

/*
 * Set binary data from uint8_t array of length length, crc if known and
 * optional reset_list option
 */
void crc::set_data(const uint8_t data[], size_t length, uint32_t crc,
                   bool reset_list) {

  data_t data_vector;
  data_t data_vector_reversed;

  if (reset_list) {
    // TODO
  }

  for (size_t i = 0; i < length; i++) {
    data_vector.data.push_back(data[i]);
    data_vector_reversed.data.push_back(reverse_byte(data[i]));
  }

  data_vector.crc = data_vector_reversed.crc = crc;
  data_vector.unused_bits = data_vector_reversed.unused_bits = 0;
  data_vectors.push_back(data_vector);
  data_vectors_reversed.push_back(data_vector_reversed);
}

/*
 * Calculate the CRC based on data vector[0]
 */
void crc::calculate_crc(void) {

  uint8_t data;
  uint32_t remainder = initial_;

  data_t *vector;

  if (reflected_input_)
    vector = &data_vectors_reversed[0];
  else
    vector = &data_vectors[0];

  size_t complete_bytes =
      vector->data.size() - (vector->unused_bits > 0 ? 1 : 0);

  if (width_ >= 8) {
    // Divide the message by the polynomial, a byte at a time.
    for (size_t byte = 0; byte < complete_bytes; ++byte) {
      data = vector->data[byte] ^ (remainder >> (width_ - 8));
      remainder = (crc_table[data] ^ (remainder << 8));
    }

  } else {

    // Small CRC, bitwise calculation
    for (size_t byte = 0; byte < complete_bytes; ++byte) {

      for (size_t b = 0; b < 8; b++) {
        // compare the new bit with the remainder's highest
        remainder ^=
            (((vector->data[byte] >> (7 - b)) & 0x1) ? high_bit_mask_ : 0);

        // a full polynominal division step is done when the highest bit is one
        bool const do_poly_div = (bool)(remainder & high_bit_mask_);

        // shift out the highest bit
        remainder <<= 1;

        // carry out the division, if needed
        if (do_poly_div)
          remainder ^= polynomial_;
      }
    }
  }

  if (vector->unused_bits > 0)
    remainder = calculate_final_bits(vector->data[vector->data.size() - 1],
                                     vector->unused_bits, remainder);

  // The final remainder is the CRC - reverse if required
  if (reflected_output_) {
    remainder <<= 32 - width_;
    remainder =
        ((remainder >> 1) & 0x55555555u) | ((remainder & 0x55555555u) << 1);
    remainder =
        ((remainder >> 2) & 0x33333333u) | ((remainder & 0x33333333u) << 2);
    remainder =
        ((remainder >> 4) & 0x0f0f0f0fu) | ((remainder & 0x0f0f0f0fu) << 4);
    remainder =
        ((remainder >> 8) & 0x00ff00ffu) | ((remainder & 0x00ff00ffu) << 8);
    remainder = ((remainder >> 16) & 0xffffu) | ((remainder & 0xffffu) << 16);
  }

  result_ = (remainder & mask_) ^ final_xor_;
}

/*
 * Bitwise CRC to calculate left over bits
 */
uint32_t crc::calculate_final_bits(uint8_t data, size_t unused_bits,
                                   uint32_t remainder) {

  for (size_t b = unused_bits; b < 8; b++) {

    // compare the new bit with the remainder's highest
    remainder ^= (((data >> (b)) & 0x1) ? high_bit_mask_ : 0);

    // a full polynominal division step is done when the highest bit is one
    bool const do_poly_div = static_cast<bool>(remainder & high_bit_mask_);

    // shift out the highest bit
    remainder <<= 1;

    // carry out the division, if needed
    if (do_poly_div) {
      remainder ^= polynomial_;
    }
  }

  return remainder & mask_;
}

/*
 * Create a CRC lookup table
 * From
 * https://svn.mcs.anl.gov/repos/a1/branches/scafacos/sl/sl_back_q_pg/src/core/back_q_pg_checksum_crc.c
 */
void crc::calculate_table(void) {

  uint32_t remainder;

  // Compute the remainder of each possible dividend.
  for (int dividend = 0; dividend < 256; ++dividend) {
    // Start with the dividend followed by zeros.
    remainder = dividend << (width_ - 8);

    // Perform modulo-2 division, a bit at a time.
    for (uint8_t bit = 8; bit > 0; --bit) {

      // Try to divide the current data bit.:
      if (remainder & (1 << (width_ - 1))) {
        remainder = (remainder << 1) ^ polynomial_;
      } else {
        remainder = (remainder << 1);
      }
    }

    // Store the result into the table.
    crc_table[dividend] = remainder & mask_;
  }
}

/*
 * Reverse byte lookup table for 8 bit input
 * From http://stackoverflow.com/questions/2602823/ (fredoverflow)
 */
uint8_t crc::reverse_byte(uint8_t x) {
  static const uint8_t table[] = {
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0,
    0x30, 0xb0, 0x70, 0xf0, 0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 0x04, 0x84, 0x44, 0xc4,
    0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc,
    0x3c, 0xbc, 0x7c, 0xfc, 0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 0x0a, 0x8a, 0x4a, 0xca,
    0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6,
    0x36, 0xb6, 0x76, 0xf6, 0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 0x01, 0x81, 0x41, 0xc1,
    0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9,
    0x39, 0xb9, 0x79, 0xf9, 0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 0x0d, 0x8d, 0x4d, 0xcd,
    0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3,
    0x33, 0xb3, 0x73, 0xf3, 0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 0x07, 0x87, 0x47, 0xc7,
    0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf,
    0x3f, 0xbf, 0x7f, 0xff,
  };
  return table[x];
}

uint8_t crc::binary_string_to_byte(std::string x) {

  assert(x.length() == 8);
  uint8_t result = 0;

  for (size_t i = 0; i < 8; i++)
    result = (result << 1) | (x[7 - i] == '1');

  return result;
}
