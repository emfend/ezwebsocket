/* EzWebSocket
 *
 * Copyright © 2017 Clemens Kresser
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ezwebsocket_log.h>

//! mapping that is used for base64 conversion
static const char base64_table[64] =
{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'
};

/**
 * \brief encodes the given 3 bytes to wrBuffer
 *
 * \param byte0 1st byte to encode
 * \param byte1 2nd byte to encode
 * \param byte2 3rd byte to encode
 * \param [out] wrBuffer Pointer to where the data is written (size must be at least 4 bytes)
 *
 */
static inline void encode(unsigned char byte0, unsigned char byte1, unsigned char byte2, char *wrBuffer)
{
  wrBuffer[0] = base64_table[(byte0 & 0xFC) >> 2];
  wrBuffer[1] = base64_table[((byte0 & 0x03) << 4) | ((byte1 & 0xF0) >> 4)];
  wrBuffer[2] = base64_table[((byte1 & 0x0F) << 2) | ((byte2 & 0xC0) >> 6)];
  wrBuffer[3] = base64_table[(byte2 & 0x3F)];
}

/**
 * \brief encodes the given data to base64 format
 *
 * \param *data Pointer to the data that should be encoded
 * \param len The length of the data
 *
 * \return base64 encoded string or NULL
 *
 *  WARNING: return value must be freed after use!
 *
 */
char *base64_encode(unsigned char *data, size_t len)
{
  char *encString = malloc(((len + 2) / 3) * 4 + 1);
  char *ptr;
  size_t i;
  unsigned char help[3];
  int count;

  if(!encString)
  {
    ezwebsocket_log(EZLOG_ERROR, "malloc failed\n");
    return NULL;
  }
  ptr = encString;

  for(i = 0; i < (len / 3) * 3; i += 3)
  {
    encode(data[i + 0], data[i + 1], data[i + 2], ptr);
    ptr += 4;
  }

  //check if we need to pad with 0
  if(i < len)
  {
    help[0] = 0;
    help[1] = 0;
    help[2] = 0;

    count = 0;

    for(; i < len; i++)
    {
      help[count] = data[i];
      count++;
    }

    encode(help[0], help[1], help[2], ptr);
    ptr += (count + 1);

    //pad with '='
    for(; count < 3; count++)
    {
      *ptr = '=';
      ptr++;
    }
  }
  *ptr = '\0';
  return encString;
}
