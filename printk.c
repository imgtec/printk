/*
 * Copyright (c) 2016 Du Huanpeng<u74147@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef DEBUG

#include <stdio.h>
#define printch(c) putchar(c);

#else

int printch(int ch)
{
	if (ch == '\n') putSIO_mx28(0, '\r');
	putSIO_mx28(0, ch);
}

#endif

#include <stdarg.h>


void printi(unsigned int i, int base, int cap)
{
	char buf[32+1];
	int j;
	int mod;

	buf[32] = 0;
	j = 32;

	if(cap) cap = 'A' - 10;
	else cap = 'a' - 10;

	do {
		mod = i % base;
		i /= base;
		if(mod < 10) {
			buf[--j] = '0' + mod;
		} else {
			buf[--j] = mod + cap;
		}
	} while(i);

	prints(&buf[j]);
}


void printh(unsigned int h, int cap)
{
	char buf[9];
	unsigned int c;
	int i;

	if(cap) cap = 'A' - 10;
	else cap = 'a' - 10;

	for(i=0; i<8; i++) {
		c = h >> 28;
		#ifdef DEBUG
//		printf("c[%u] ", c);
		#endif
		if(c < 10) buf[i] = c +'0';
		else buf[i] = c + cap;

		h <<= 4;
	}
	buf[i] = 0;

	prints(buf);
}


int prints(const char *str)
{
	int i;
	int c;

	for(i=0; str[i]; i++) {
		c = str[i];
		printch(c);
	}
}

int printki(const char *fmt, int num)
{
//	prints("[");
	int i = 0;

	while(fmt[i]) i++;
	i = fmt[i-1];

	if(i=='b') printi(num, 2, 0);
	else if(i=='d') printi(num, 10, 0);
	else if(i=='o') printi(num, 8, 0);
	else printi(num, 16, 1);

//	prints("]");
}

int printkh(const char *fmt, int num)
{
//	prints(fmt);
//	prints("[");
	printh(num, 0);
//	prints("]");
}

int printkc(const char *fmt, int ch)
{
	prints(fmt);
	prints("[");
	printch(ch);
	prints("]");
}





/*
 * second version of vprintk.
 */


int vprintks(const char *fmt, va_list args)
{
	unsigned int flag;
	int c;
	char format[16];
	int i, j;


	flag = 0;

	j = 0;		/* format[] index. */
	i = 0;		/* index of fmt[]. */

	while(c = fmt[i++]) {
		if(j) {	/* format */
			format[j++] = c;
			format[j] = 0;
			if      (c == 'c')
				printkc(format, va_arg(args, int)), j=0;
			else if (c == '%')
				printkc(format, c),                 j=0;
			else if (c == 'd')
				printki(format, va_arg(args, int)), j=0;
			else if (c == 'u')
				printki(format, va_arg(args, int)), j=0;
			else if (c == 'x')
				printkh(format, va_arg(args, int)), j=0;
			else if (c == 'X')
				printki(format, va_arg(args, int)), j=0;
			else if (c == 'p')
				printkh(format, va_arg(args, int)), j=0;
			else if (c == 'P')
				printkh(format, va_arg(args, int)), j=0;
			else if (c == 'b')
				printki(format, va_arg(args, int)), j=0;
			else if (c == 'B')
				printki(format, va_arg(args, int)), j=0;
			else if (c == 's') {
				prints(format);
				prints("[");
				prints(va_arg(args, const char *));
				j=0;
				prints("]");
			}
			else if (j>8)
				prints(format),                     j=0;
		}
		else {
			if(c == '%') format[j++] = c;
			else printch(c);
		}
	}
	va_end(args);
}

int printks(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	return vprintks(fmt, args);
}

#ifdef DEBUG
int main(int argc, char *argv[])
{
	char name[]="Du Huanpeng";
	const char *fmt = 
"%d,,,,%x %1234567890%123$% %c1234X %12345d\n"
"%b  %23B , s%Xsfa%%, %kdfsdf\n";

	prints(fmt);
	printks(fmt, 1, 2, 3, 4, 5, 6, 7, 8);

	printks("%d", 123456789, 1234);
	printks("My name is %s.\n", name);
	printks("0x8421 in binary:[%b]\n", 0x8421, 11);

	return 0;

	printh(0x1234ABCD,  0); 
	printh(0x1234ABCD,  1);
	printh(~0,  1);
	printh(0,  1);

	printi(0, 2, 1);
	printi(~0, 2, 1);

	printi(0, 10, 1);
	printi(~0, 10, 1);

	printi(012345670, 8, 1);
	printi(012345670, 8, 1);
	printi(123456789, 10, 1);
	printi(0, 2, 1);
}
#endif
