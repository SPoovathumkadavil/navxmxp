/* ============================================
navX MXP source code is placed under the MIT license
Copyright (c) 2017 Kauai Labs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
#ifndef SPI_COMM_H_
#define SPI_COMM_H_

#define STD_SPI_MSG_LEN				   8
#define TOTAL_SPI_MSG_OVERHEAD_BYTES   4  /* Bank, Addr, Count, <DATA>, CRC */
#define MAX_SPI_MSG_LEN				 255
#define COMM_MODE_BANK				0xFF  /* Communication mode control  */
#define COMM_MODE_REG_VARIABLEWRITE	0x01  /* Variable len comm (next tx  */
										  /* only); count in Count byte. */

#endif /* SPI_COMM_H_ */
