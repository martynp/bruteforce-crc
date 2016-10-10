/*
 */

#ifndef CRC_HPP
#define CRC_HPP

#include <vector>
#include <stdint.h>

class crc {

public:
  /*
   * crc class, calculates crc of inputted data
   */
  crc(unsigned int width);

  /*
   * data_t type is for storing data to be calculated
   */
  typedef class data_c {
  public:
    std::vector<uint8_t> data; // Data to be processed
    size_t unused_bits; // Number of unused bits at the END o the data array
    uint32_t crc;       // The CRC (known or unknown) for the data
  } data_t;

  /*
   * Set the CRC parameters
   */
  void set_parameters(uint32_t polynomial = 0, uint32_t initial = 0,
                      uint32_t final_xor = 0, bool reflected_input = false,
                      bool reflected_output = false);

  /*
   * Manage data
   */
  void set_data(std::vector<data_t> vectors, bool reset_list = false);
  void set_data(data_t data, bool reset_list = false);
  void set_data(std::string data, uint32_t crc = 0, bool reset_list = false);
  void set_data(const uint8_t data[], size_t length, uint32_t crc = 0,
                bool reset_list = false);

  /*
   * Main function call
   */
  void calculate_crc(void);
  void calculate_crc(data_t data);
  void calculate_crc(std::string data);
  void calculate_crc(uint8_t data[], size_t length);

  uint32_t calculate_final_bits(uint8_t data, size_t unused_bits,
                                uint32_t remainder);

private:
  /*
   * Calculate the cheat table
   */
  uint32_t crc_table[256];
  void calculate_table(void);

  std::vector<data_t> data_vectors;
  std::vector<data_t> data_vectors_reversed;

  uint8_t reverse_byte(uint8_t x);

  uint8_t binary_string_to_byte(std::string x);

  /************************
   * Properties and methods
   ************************/

private:
  unsigned int width_;
  uint32_t mask_;
  uint32_t high_bit_mask_;

  // Paramters
  uint32_t initial_;
  uint32_t final_xor_;
  uint32_t polynomial_;
  bool reflected_input_;
  bool reflected_output_;

  uint32_t result_;

public:
  void set_initial(uint32_t var) { initial_ = var; }
  uint32_t initial() const { return initial_; }
  void set_final_xor(uint32_t var) { final_xor_ = var; }
  uint32_t final_xor() const { return final_xor_; }
  void set_polynomial(uint32_t var) {
    polynomial_ = var;
    calculate_table();
  }
  uint32_t polynomial() const { return polynomial_; }
  void set_reflected_input(bool var) { reflected_input_ = var; }
  bool reflected_input() const { return reflected_input_; }
  void set_reflected_output(bool var) { reflected_output_ = var; }
  bool reflected_output() const { return reflected_output_; }

  uint32_t result() const { return result_; }
};

#endif
