/* EzWebSocket
 *
 * Copyright © 2017 Clemens Kresser
 * Copyright © 2021 Stefan Ursella
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

#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_

#include <stdbool.h>
#include <stdarg.h>

enum ezwebsocket_log_level
{
  EZLOG_ERROR,
  EZLOG_WARNING,
  EZLOG_INFO,
  EZLOG_DEBUG,
};

typedef int (*ezwebsocket_log_func_t)(enum ezwebsocket_log_level log_level, const char *fmt, va_list argp);

void ezwebsocket_log_set_handler(ezwebsocket_log_func_t log, ezwebsocket_log_func_t cont);
void ezwebsocket_set_level(enum ezwebsocket_log_level level);
int ezwebsocket_vlog(enum ezwebsocket_log_level log_level, const char *fmt, va_list ap);
int ezwebsocket_vlog_continue(enum ezwebsocket_log_level log_level, const char *fmt, va_list argp);
int ezwebsocket_log(enum ezwebsocket_log_level log_level, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
int ezwebsocket_log_continue(enum ezwebsocket_log_level log_level, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

#endif /* UTILS_LOG_H_ */
