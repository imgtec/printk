#ifdef DEBUG
#include <stdio.h>
void puts_ll(const char *buf)
{
	printf("%s\n", buf);
}

#define __putchar(c) printf("%c", c)

#else
void puts_ll(const char *buf)
{
	char c;
	while(c = *buf++)
		putSIO_mx28(0, c);
	putSIO_mx28(0, '\r');
	putSIO_mx28(0, '\n');
}
#endif

void printi(unsigned int i, int base, int cap)
{
	char buf[34];
	int j;
	int mod;

	buf[33] = 0;
	j = 33;

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

//	puts_ll(&buf[j]);

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
		__putchar(c);
	}
}


/*
 * safe version of printk.
 */


int printks(const char *fmt)
{
	unsigned int flag;
	int c;
	const char *seek;
	char format[8+2];
	int i, j;


	flag = 0;
	seek = fmt;

	j = 0;		/* format[] index. */
	i = 0;		/* index of fmt[]. */
	c = fmt[i];

	while(c) {
		if(j) {	/* format */
			if (c == 'd') printi(123456789, 10, 0);
			else if (c == 'x') printh(0xABCDEF00, 0);
			else if (c == 'X') printh(0xABCDEF11, 1);
			else if (j++>8) { /* else++ */
				format[j++] = c;
				format[j] = 0;
				prints(format);
				j = 0;
			}
			else {
				format[j] = fmt[i];
			}
		}
		else {
			if(c == '%') format[j++] = c;
			else __putchar(c);
		}
		c = fmt[i++];
	}
}



#ifdef DEBUG
int main(int argc, char *argv[])
{
	const char *fmt = "%d,,,,%x %11111, s%Xsfa%%, %kdfsdfsfsfx";

	printks(fmt);

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
