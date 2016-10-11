/*
 * Test vectors generated from the crc-catalogue:
 * http://reveng.sourceforge.net/crc-catalogue/
 *
 *                 Copyright MartynP 2016.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/dynamic_bitset.hpp>
#include <boost/crc.hpp>
//#include <boost/timer/timer.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test the CRC engine"
#include <boost/test/unit_test.hpp>

//#define BOOST_CRC
//#define TIMER

#ifdef TIMER
boost::timer::auto_cpu_timer t;
#endif

#include "../crc.hpp"

// For dynamic bitset conversions only
#include "../bf_crc.hpp"

//typedef my_crc_basic crc_t;
uint8_t default_data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

uint32_t calculate_crc(uint32_t width, const uint8_t* data, size_t length, uint32_t polynomial, uint32_t initial, uint32_t final_xor, bool reflected_input, bool reflected_output)
{

	crc crc(width);
	crc.set_parameters(polynomial, initial, final_xor, reflected_input, reflected_output);
	crc.set_data(data, length, 0, true);
	crc.calculate_crc();

	return crc.result();

}

uint32_t calculate_crc(uint32_t width, std::string data, uint32_t polynomial, uint32_t initial, uint32_t final_xor, bool reflected_input, bool reflected_output)
{
	crc crc(width);
	crc.set_parameters(polynomial, initial, final_xor, reflected_input, reflected_output);
	crc.set_data(data, 0, true);
	crc.calculate_crc();

	return crc.result();
}

/* 
 * Test 3-bit CRC's from reveng.sourceforge.net
 * 1 Example
 * 14/09/2016
 */	
BOOST_AUTO_TEST_CASE(crcThree) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 3 bit CRC's
	uint8_t crc_width = 3;
	uint32_t calculated_crc;

	/*
	 * CRC-3/ROHC
	 * width=3 poly=0x3 init=0x7 refin=true refout=true xorout=0x0 check=0x6 name="CRC-3/ROHC"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x3, 0x7, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x6);
}

/*
 * Test 4-bit CRC's from reveng.sourceforge.net
 * 2 Examples
 * 14/09/2016
 */
BOOST_AUTO_TEST_CASE(crcFour) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 4 bit CRC's
	uint8_t crc_width = 4;
	uint32_t calculated_crc;

	/*
 	 * CRC-4/INTERLAKEN
 	 * width=4 poly=0x3 init=0xf refin=false refout=false xorout=0xf check=0xb name="CRC-4/INTERLAKEN"
 	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x3, 0xF, 0xF, false, false);
	BOOST_CHECK(calculated_crc == 0xB);

	/*
	 * CRC-4/ITU
	 * width=4 poly=0x3 init=0x0 refin=true refout=true xorout=0x0 check=0x7 name="CRC-4/ITU"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x3, 0x0, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x7);

}

/* 
 * Test 5-bit CRC's from reveng.sourceforge.net
 * 3 Examples
 * 14/09/2016
 */
BOOST_AUTO_TEST_CASE(crcFive) {

#ifdef TIMER	
	boost::timer::auto_cpu_timer t;
#endif

	// 5 bit CRC's
	uint8_t crc_width = 5;
	uint32_t calculated_crc;

	/*
	 * CRC-5/EPC
	 * width=5 poly=0x09 init=0x09 refin=false refout=false xorout=0x00 check=0x00 name="CRC-5/EPC"
	 */

	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x9, 0x9, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x00);

	// High Security FeRAM-Based EPC C2G2 UHF (https://etrij.etri.re.kr/etrij/journal/getPublishedPaperFile.do?fileId=SPF-1228283393442)
	calculated_crc = calculate_crc(crc_width, "10001001000000100", 0x9, 0x9, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x06);

	// Computer Interfacing Forum (topic 1330, non authoritiative)
	calculated_crc = calculate_crc(crc_width, "10000001000000000", 0x9, 0x9, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x13);

	/*
	 * CRC-5/ITU
	 * width=5 poly=0x15 init=0x00 refin=true refout=true xorout=0x00 check=0x07 name="CRC-5/ITU"
	 */

	// REVENG Test Check
	uint8_t data_2_0[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; 
	calculated_crc = calculate_crc(crc_width, data_2_0, 9, 0x15, 0x0, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x07);

	/*
 	 * CRC-5/USB
	 * width=5 poly=0x05 init=0x1f refin=true refout=true xorout=0x1f check=0x19 name="CRC-5/USB"
	 */

	// REVENG Test Check
	uint8_t data_3_0[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; 
	calculated_crc = calculate_crc(crc_width, data_3_0, 9, 0x05, 0x1F, 0x1F, true, true);
	BOOST_CHECK(calculated_crc == 0x019);

	// Darft Implementation 1 TODO: These test cases do not match the exemplar / description
	calculated_crc = calculate_crc(crc_width, "10101000111", 0x05, 0x1F, 0x1F, false, false);
	BOOST_CHECK(calculated_crc == 0x017);

	// Darft Implementation 2
	calculated_crc = calculate_crc(crc_width, "01011100101", 0x05, 0x1F, 0x1F, false, false);
	BOOST_CHECK(calculated_crc == 0x01C);

	// Darft Implementation 3
	calculated_crc = calculate_crc(crc_width, "00001110010", 0x05, 0x1F, 0x1F, false, false);
	BOOST_CHECK(calculated_crc == 0x00E);

	// Darft Implementation 4
	calculated_crc = calculate_crc(crc_width, "10000000000", 0x05, 0x1F, 0x1F, false, false);
	BOOST_CHECK(calculated_crc == 0x017);

}

/*
 * Test 6-bit CRC's from reveng.sourceforge.net
 * 4 Examples
 * 14/06/2016
 */
BOOST_AUTO_TEST_CASE(crcSix) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 6 bit CRC's
	uint8_t crc_width = 6;
	uint32_t calculated_crc;

	/*
 	 * CRC-6/CDMA2000-A
 	 * width=6 poly=0x27 init=0x3f refin=false refout=false xorout=0x00 check=0x0d name="CRC-6/CDMA2000-A"
 	 */
	
	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x27, 0x3F, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x0D);

	// Test 1 - https://www.lammertbies.nl/forum/viewtopic.php?t=1702 
	// 0xC101002E - 26 bits data, 6 bits CRC
	calculated_crc = calculate_crc(crc_width, "11000001000000010000000000", 0x27, 0x3F, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x2E);

	// Test 2 - https://www.lammertbies.nl/forum/viewtopic.php?t=1702 
	// 0x0110000F - 26 bits data, 6 bits CRC
	calculated_crc = calculate_crc(crc_width, "00000001000100000000000000", 0x27, 0x3F, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x0F);

	// Test 3 - https://www.lammertbies.nl/forum/viewtopic.php?t=1702 
	// 0x05100009 - 26 bits data, 6 bits CRC
	calculated_crc = calculate_crc(crc_width, "00000101000100000000000000", 0x27, 0x3F, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x09);

	/*
	 * CRC-6/CDMA2000-B
	 * width=6 poly=0x07 init=0x3f refin=false refout=false xorout=0x00 check=0x3b name="CRC-6/CDMA2000-B"
	 */
	
	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x7, 0x3F, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x3B);

	/*
	 * CRC-6/DARC
	 * width=6 poly=0x19 init=0x00 refin=true refout=true xorout=0x00 check=0x26 name="CRC-6/DARC"
	 */

	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x19, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x26);

	// ETSI EN 300 751
	// Test Vector 1 => 1001010011000100, bits are transmitted LSB first (lsb is leftmost in this example)
	// Data: LSB 100100011 MSB
	// CRC: LSB 000100 MSB
	// Data and CRC are fed in lsb first, however the definitition stated reflected input and output
	// Data and CRC should be reversed for the check in order to work
	/*
     = CAB8
    00001100010000000010000000101101 = 300204B4*/

	calculated_crc = calculate_crc(crc_width, "1100101001", 0x19, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x08);

	// ETSI EN 300 751
	// Test Vector 2 => 0101001100 011101
	// Reversed: 0011001010 101110             0011001010
	calculated_crc = calculate_crc(crc_width, "0011001010", 0x19, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x2E);

	// ETSI EN 300 751
	// Test Vector 3 => 00001100010000000010000000 101101 
	// Reversed: 00000001000000001000110000 101101
	// TODO: This one does not work...
	/*
	calculated_crc = calculate_crc(crc_width, "00001100010000000010000000", 0x19, 0x00, 0x00, false, false);
std::cout << std::hex << calculated_crc <<std::endl;
	BOOST_CHECK(calculated_crc == 0x13);
	*/

	/*
	 * CRC-6/ITU
	 * width=6 poly=0x03 init=0x00 refin=true refout=true xorout=0x00 check=0x06 name="CRC-6/ITU"
	 */

	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x03, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x06);

}

/*
 * Test 7-bit CRC's from reveng.sourceforge.net
 * 3 Examples
 * 14/09/2016
 */
BOOST_AUTO_TEST_CASE(crcSeven) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 7 bit CRC's
	uint8_t crc_width = 7;
	uint32_t calculated_crc;

	/*
 	 * CRC-7
 	 * width=7 poly=0x09 init=0x00 refin=false refout=false xorout=0x00 check=0x75 name="CRC-7"
 	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x09, 0x0, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x75);

	/*
	 * CRC-7/ROHC
	 * width=7 poly=0x4f init=0x7f refin=true refout=true xorout=0x00 check=0x53 name="CRC-7/ROHC"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x4F, 0x7F, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x53);

	/*
	 * CRC-7/UMTS
	 * width=7 poly=0x45 init=0x00 refin=false refout=false xorout=0x00 check=0x61 name="CRC-7/UMTS"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x45, 0x0, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x61);

}

/*
 * Test 8-bit CRC's from reveng.sourceforge.net
 * 14 Examples
 * 15/09/2016
 */
BOOST_AUTO_TEST_CASE(crcEight) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 8 bit CRC's
	uint8_t crc_width = 8;
	uint32_t calculated_crc;

	/*
 	 * CRC-8
 	 * width=8 poly=0x07 init=0x00 refin=false refout=false xorout=0x00 check=0xf4 name="CRC-8"
 	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x07, 0x0, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0xF4);

	// 'Firmware CRC-8 implementations for SMBus'
	//uint8_t data_1_1[] = {0x5C};
	calculated_crc = calculate_crc(crc_width, "01011100", 0x07, 0x0, 0x0, false, false);
	//calculated_crc = calculate_crc(crc_width, data_1_1, sizeof(data_1_1), 0x07, 0x0, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x93);

	/*
	 * CRC-8/AUTOSAR
	 * width=8 poly=0x2f init=0xff refin=false refout=false xorout=0xff check=0xdf name="CRC-8/AUTOSAR"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0xDF);

	// AUTOSAR Release 4.2.2
	// Test Vector 1
	uint8_t data_2_1[] = {0x00, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_2_1, sizeof(data_2_1), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x12);

	// AUTOSAR Release 4.2.2
	// Test Vector 2
	uint8_t data_2_2[] = {0xF2, 0x01, 0x83};
	calculated_crc = calculate_crc(crc_width, data_2_2, sizeof(data_2_2), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0xC2);

 	// AUTOSAR Release 4.2.2
	// Test Vector 3
	uint8_t data_2_3[] = {0x0F, 0xAA, 0x00, 0x55};
 	calculated_crc = calculate_crc(crc_width, data_2_3, sizeof(data_2_3), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0xC6);

 	// AUTOSAR Release 4.2.2
	// Test Vector 4
	uint8_t data_2_4[] = {0x00, 0xFF, 0x55, 0x11};
	calculated_crc = calculate_crc(crc_width, data_2_4, sizeof(data_2_4), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x77);


 	// AUTOSAR Release 4.2.2
	// Test Vector 5
	uint8_t data_2_5[] = {0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	calculated_crc = calculate_crc(crc_width, data_2_5, sizeof(data_2_5), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x11);


 	// AUTOSAR Release 4.2.2
	// Test Vector 6
	uint8_t data_2_6[] = {0x92, 0x6B, 0x55};
	calculated_crc = calculate_crc(crc_width, data_2_6, sizeof(data_2_6), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x33);


 	// AUTOSAR Release 4.2.2
	// Test Vector 7
	uint8_t data_2_7[] = {0xFF, 0xFF, 0xFF, 0xFF};
	calculated_crc = calculate_crc(crc_width, data_2_7, sizeof(data_2_7), 0x2F, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x6C);

	/*
	 * CRC-8/CDMA2000
	 * width=8 poly=0x9b init=0xff refin=false refout=false xorout=0x00 check=0xda name="CRC-8/CDMA2000"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x9B, 0xFF, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0xDA);

	/*
	 * CRC-8/DARC
	 * width=8 poly=0x39 init=0x00 refin=true refout=true xorout=0x00 check=0x15 name="CRC-8/DARC"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x39, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x15);

	// ETSI EN 300 751
	// 0000000100000011 11010111 = 80C0EB
	uint8_t data_4_1[] = {0x80, 0xC0};
	calculated_crc = calculate_crc(crc_width, data_4_1, sizeof(data_4_1), 0x39, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0xEB);

	/*
 	 * CRC-8/DVB-S2
 	 * width=8 poly=0xd5 init=0x00 refin=false refout=false xorout=0x00 check=0xbc name="CRC-8/DVB-S2"
 	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0xD5, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0xBC);

	// OpenSAFETY Safety Profie Specification Working Draft Propsal Version 1.4.0
	uint8_t data_5_1[] = {0x22, 0xC8, 0x12, 0x56, 0x30, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
	calculated_crc = calculate_crc(crc_width, data_5_1, sizeof(data_5_1), 0xD5, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x4F);

	/*
	 * CRC-8/EBU
	 * width=8 poly=0x1d init=0xff refin=true refout=true xorout=0x00 check=0x97 name="CRC-8/EBU"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x1D, 0xff, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x97);

 	// Tech 3250: Specification of the audio interface, 3rd edition
 	// Test Vector 1
	uint8_t data_6_1[] = {0x3D, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
	calculated_crc = calculate_crc(crc_width, data_6_1, sizeof(data_6_1), 0x1D, 0xff, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x9B);

 	// Tech 3250: Specification of the audio interface, 3rd edition
 	// Test Vector 2
    uint8_t data_6_2[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_6_2, sizeof(data_6_2), 0x1D, 0xff, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x32);

	/*
	 * CRC-8/I-CODE
	 * width=8 poly=0x1d init=0xfd refin=false refout=false xorout=0x00 check=0x7e name="CRC-8/I-CODE"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x1D, 0xFD, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x7E);

	/*
	 * CRC-8/ITU
	 * width=8 poly=0x07 init=0x00 refin=false refout=false xorout=0x55 check=0xa1 name="CRC-8/ITU"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x07, 0x00, 0x55, false, false);
	BOOST_CHECK(calculated_crc == 0xA1);

 	// ITU-T Recommendation I.432.1
 	// Test Vector 1
	uint8_t data_9_1[] = {0x00, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_9_1, sizeof(data_9_1), 0x07, 0x00, 0x55, false, false);
	BOOST_CHECK(calculated_crc == 0x55);

 	// ITU-T Recommendation I.432.1
 	// Test Vector 2
    uint8_t data_9_2[] = {0x00, 0x00, 0x00, 0x01};
	calculated_crc = calculate_crc(crc_width, data_9_2, sizeof(data_9_2), 0x07, 0x00, 0x55, false, false);
	BOOST_CHECK(calculated_crc == 0x52);

	/*
	 * CRC-8/LTE
	 * width=8 poly=0x9b init=0x00 refin=false refout=false xorout=0x00 check=0xea name="CRC-8/LTE"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x9B, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0xEA);

	/*
	 * CRC-8/MAXIM
	 * width=8 poly=0x31 init=0x00 refin=true refout=true xorout=0x00 check=0xa1 name="CRC-8/MAXIM"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x31, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0xA1);

	// Maxim iButton datasheet (DS1904)
	uint8_t data_11_1[] = {0x24, 0x2B, 0xC5, 0xFB, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_11_1, sizeof(data_11_1), 0x31, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x40);

	// Maxim iButton datasheet (DS1920)
	uint8_t data_11_2[] = {0x10, 0x2B ,0xC5 ,0xFB ,0x00 ,0x00 ,0x00};
	calculated_crc = calculate_crc(crc_width, data_11_2, sizeof(data_11_2), 0x31, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0xA0);

	// Maxim iButton datasheet (DS1921G)
	uint8_t data_11_3[] = {0x21, 0x2B, 0xC5, 0xFB, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_11_3, sizeof(data_11_3), 0x31, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x89);

	/*
	 * CRC-8/OPENSAFETY
	 * width=8 poly=0x2f init=0x00 refin=false refout=false xorout=0x00 check=0x3e name="CRC-8/OPENSAFETY" 
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x2F, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x3E);

	// Ethernet POWERLINK Standardisation Group (13 March 2016)
	uint8_t data_12_1[] = {0x23, 0xC8, 0x08, 0x34, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
	calculated_crc = calculate_crc(crc_width, data_12_1, sizeof(data_12_1), 0x2F, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x3C);

	// openSAFETY Coommunivation using Ethernet Powerlink V2 as transport protocol (Wireshark wiki via. reveng.sourceforge.net	) Test Vector 1
	uint8_t data_12_2[] = {0x02, 0xA8, 0x06, 0x00, 0x00, 0x60, 0x65, 0x00, 0x06, 0xA1};
	calculated_crc = calculate_crc(crc_width, data_12_2, sizeof(data_12_2), 0x2F, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x1C);

	// openSAFETY Coommunivation using Ethernet Powerlink V2 as transport protocol (Wireshark wiki via. reveng.sourceforge.net	) Test Vector 2
	uint8_t data_12_3[] = {0x03, 0xA8, 0x00, 0x01, 0x00, 0x00, 0x60, 0x65, 0x00, 0x06, 0xA1};
	calculated_crc = calculate_crc(crc_width, data_12_3, sizeof(data_12_3), 0x2F, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x31);

	// openSAFETY Coommunivation using Ethernet Powerlink V2 as transport protocol (Wireshark wiki via. reveng.sourceforge.net	) Test Vector 3
	uint8_t data_12_4[] = {0x03, 0xA8, 0x06, 0x00, 0x00, 0x60, 0x65, 0x00, 0x07, 0x21};
	calculated_crc = calculate_crc(crc_width, data_12_4, sizeof(data_12_4), 0x2F, 0x00, 0x00, false, false);
	BOOST_CHECK(calculated_crc == 0x1D);

	/*
	 * CRC-8/ROHC
	 * width=8 poly=0x07 init=0xff refin=true refout=true xorout=0x00 check=0xd0 name="CRC-8/ROHC"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x07, 0xFF, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0xD0);

	/*
	 * CRC-8/SAE-J1850
	 * width=8 poly=0x1d init=0xff refin=false refout=false xorout=0xff check=0x4b name="CRC-8/SAE-J1850"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x1D, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x4B);

	// SAE Standard J1850
	// Test Vector 1
	uint8_t data_14_1[] = {0x00, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_14_1, sizeof(data_14_1), 0x1D, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x59);

	// SAE Standard J1850
	// Test Vector 2
	uint8_t data_14_2[] = {0xF2, 0x01, 0x83};
	calculated_crc = calculate_crc(crc_width, data_14_2, sizeof(data_14_2), 0x1D, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x37);

	// SAE Standard J1850
	// Test Vector 3
	uint8_t data_14_3[] = {0x0F, 0xAA, 0x00, 0x55};
	calculated_crc = calculate_crc(crc_width, data_14_3, sizeof(data_14_3), 0x1D, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x79);

	// "knivmannen" StackOverflow (via. reveng.sourceforege.net)
	// Test Vector 1
	uint8_t data_14_4[] = {0x55, 0xFF, 0x00, 0x00, 0xEC, 0xFF, 0x60};
	calculated_crc = calculate_crc(crc_width, data_14_4, sizeof(data_14_4), 0x1D, 0xFF, 0xFF, false, false);
	BOOST_CHECK(calculated_crc == 0x1F);

	/*
	 * CRC-8/WCDMA
	 * width=8 poly=0x9b init=0x00 refin=true refout=true xorout=0x00 check=0x25 name="CRC-8/WCDMA"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x9B, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x25);

	// "Max" Computer Interfacing Forum Topic 1431 (via regeng.sourceforge.net)
	// Test Vector 1
	uint8_t data_15_1[] = {0x00, 0x00, 0x11};
	calculated_crc = calculate_crc(crc_width, data_15_1, sizeof(data_15_1), 0x9B, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x48);

	// "Max" Computer Interfacing Forum Topic 1431 (via regeng.sourceforge.net)
	// Test Vector 2
	uint8_t data_15_2[] = {0x00, 0x01, 0x20};
	calculated_crc = calculate_crc(crc_width, data_15_2, sizeof(data_15_2), 0x9B, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x77);

	// "Max" Computer Interfacing Forum Topic 1431 (via regeng.sourceforge.net)
	// Test Vector 4
	uint8_t data_15_3[] = {0x00, 0x00, 0x10, 0x21, 0x01, 0x00, 0x22, 0x00, 0x0A, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0A};
	calculated_crc = calculate_crc(crc_width, data_15_3, sizeof(data_15_3), 0x9B, 0x00, 0x00, true, true);
	BOOST_CHECK(calculated_crc == 0x20);

}

BOOST_AUTO_TEST_CASE(crcTen) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 10 bit CRC's
	uint8_t crc_width = 10;
	uint32_t calculated_crc;

	crc_t crc(crc_width);

	/*
	 * CRC-10
	 * width=10 poly=0x233 init=0x000 refin=false refout=false xorout=0x000 check=0x199 name="CRC-10"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x233, 0x000, 0x000, false, false);
	BOOST_CHECK(calculated_crc == 0x199);

	// ITU-T Recommendation I.610
	// Test Vector 1
	uint8_t data_1_1[] = {0x11, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A};
	calculated_crc = calculate_crc(crc_width, data_1_1, sizeof(data_1_1), 0x233, 0x000, 0x000, false, false);
//	BOOST_CHECK(calculated_crc == 0x0AF);

	// ITU-T Recommendation I.610
	// Test Vector 2
	//uint8_t data_1_2[] = 
	
	// Generating and Checking CRC-10 ATA ALL 3/4 or OAM Cells
	// Test Vector 1
	uint8_t data_1_3[] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	calculated_crc = calculate_crc(crc_width, data_1_3, sizeof(data_1_3), 0x233, 0x000, 0x000, false, false);
//	BOOST_CHECK(calculated_crc == 0x1F6);

	/*
	 * CRC-10/CDMA2000
	 * width=10 poly=0x3d9 init=0x3ff refin=false refout=false xorout=0x000 check=0x233 name="CRC-10/CDMA2000"
	 */	
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x3D9, 0x3FF, 0x000, false, false);
	BOOST_CHECK(calculated_crc == 0x233);

}

BOOST_AUTO_TEST_CASE(crcFourteen) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 14 bit CRC's
	uint8_t crc_width = 14;
	uint32_t calculated_crc;

	/*
	 * CRC-14/DARC
	 * width=14 poly=0x0805 init=0x0000 refin=true refout=true xorout=0x0000 check=0x082d name="CRC-14/DARC"
	 */

	// REVENG Test Check
	uint8_t data_1_0[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; 
	calculated_crc = calculate_crc(crc_width, data_1_0, 9, 0x805, 0x0, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x82d);

	// ETSI EN 300 751 - TODO: Needs Verification
	uint8_t data_1_1[] = {0x02, 0x00, 0x01, 0x02, 0x37, 0x20, 0x50, 0x52, 0x4F, 0x4A, 0x45, 0x43, 0x54, 0x20, 0x4D, 0x41, 0x49, 0x4E, 0x4D, 0x45, 0x4E, 0x55}; 
	calculated_crc = calculate_crc(crc_width, data_1_1, sizeof(data_1_1), 0x805, 0x0, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0x83B);

}

BOOST_AUTO_TEST_CASE(crcFifteen) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 15 bit CRC's
	uint8_t crc_width = 15;
	uint32_t calculated_crc;

	/*
	 * CRC-15
	 * width=15 poly=0x4599 init=0x0000 refin=false refout=false xorout=0x0000 check=0x059e name="CRC-15" 
	 */

	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x4599, 0x0000, 0x0000, false, false);
	BOOST_CHECK(calculated_crc == 0x059E);


}



BOOST_AUTO_TEST_CASE(crcSixteen) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 16 bit CRC's
	uint8_t crc_width = 16;
	uint32_t calculated_crc;

	/*
	 * ARC
	 * width=16 poly=0x8005 init=0x0000 refin=true refout=true xorout=0x0000 check=0xbb3d name="ARC"
	 */

	// REVENG Test Check
	uint8_t data_1_0[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};  
	calculated_crc = calculate_crc(crc_width, data_1_0, sizeof(data_1_0), 0x8005, 0x0, 0x0, true, true);
	BOOST_CHECK(calculated_crc == 0xBB3D);

	/* 
	 * CRC-16/CCITT-FALSE
	 * width=16 poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1 name="CRC-16/CCITT-FALSE"
	 */

	// Autosar Release 4.2.2 p.25 : 00 00 00 00 : 84C0
	uint8_t data_3_0[] = {0x00, 0x00, 0x00, 0x00};  
	calculated_crc = calculate_crc(crc_width, data_3_0, sizeof(data_3_0), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x84C0);

	// Autosar Release 4.2.2 p.25 : F2 01 83 : D374
	uint8_t data_3_1[] = {0xF2, 0x01, 0x83};  
	calculated_crc = calculate_crc(crc_width, data_3_1, sizeof(data_3_1), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0xD374);

	// Autosar Release 4.2.2 p.25 : 0F AA 00 55 : 2023
	uint8_t data_3_2[] = {0x0F, 0xAA, 0x00, 0x55};  
	calculated_crc = calculate_crc(crc_width, data_3_2, sizeof(data_3_2), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x2023);

	// Autosar Release 4.2.2 p.25 : 00 FF 55 11 : B8F9
	uint8_t data_3_3[] = {0x00, 0xFF, 0x55, 0x11};  
	calculated_crc = calculate_crc(crc_width, data_3_3, sizeof(data_3_3), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0xB8F9);

	// Autosar Release 4.2.2 p.25 : 33 22 55 AA BB CC DD EE FF : F53F
	uint8_t data_3_4[] = {0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};  
	calculated_crc = calculate_crc(crc_width, data_3_4, sizeof(data_3_4), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0xF53F);

	// Autosar Release 4.2.2 p.25 : 92 6B 55 : 0745
	uint8_t data_3_5[] = {0x92, 0x6B, 0x55}; 
	calculated_crc = calculate_crc(crc_width, data_3_5, sizeof(data_3_5), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x0745); 

	// Autosar Release 4.2.2 p.25 : FF FF FF FF : 1D0F
	uint8_t data_3_6[] = {0xFF, 0xFF, 0xFF, 0xFF};  
	calculated_crc = calculate_crc(crc_width, data_3_6, sizeof(data_3_6), 0x1021, 0xFFFF, 0x0, false, false);
	BOOST_CHECK(calculated_crc == 0x1D0F);

}

BOOST_AUTO_TEST_CASE(crcThirtyTwo) {

#ifdef TIMER
	boost::timer::auto_cpu_timer t;
#endif

	// 32 bit CRC's
	uint8_t crc_width = 32;
	uint32_t calculated_crc;

	/*
 	 * CRC-32/AUTOSAR
 	 * width=32 poly=0xf4acfb13 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0x1697d06a name="CRC-32/AUTOSAR"
 	 */
	
	// REVENG Test Check
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0xF4ACFB13, 0xFFFFFFFF, 0xFFFFFFFF, true, true);
	BOOST_CHECK(calculated_crc == 0x1697d06a);

	/*
	 * CRC-32/BZIP2
	 * width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false xorout=0xffffffff check=0xfc891918 name="CRC-32/BZIP2"
	 */
	calculated_crc = calculate_crc(crc_width, default_data, sizeof(default_data), 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false, false);
	BOOST_CHECK(calculated_crc == 0xFC891918);

}
