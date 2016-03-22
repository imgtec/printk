#include <stdio.h>

#if 0
extern void puts_ll(const char *buf);
#else
void puts_ll(const char *buf)
{
	printf("%s\n", buf);
}
#endif

void printi(unsigned int i, int base)
{
	const char dig[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char buf[33];
	int j;
	int c;

	unsigned div;

	div = (unsigned int)(~0)/base;

	for(j=0; j<i/base; j++) {
		c = i/div;
		buf[j] = dig[c];
		i *= base;
		
	}
	buf[j] = 0;

	puts_ll(buf);
}


void printh(unsigned int h, int cap)
{
	const char hex[16]="0123456789ABCDEF";
	char buf[16] = "0xDEADBEEF";
	unsigned int c;
	int i;

	if(cap) cap = 'a' - 'A';
	else cap = 0;

	for(i=0; i<8; i++) {
		c = h / 0x10000000;
		h <<= 4;
		c = hex[c];
		if(c > '9') c += cap;
		buf[i] = c;
	}
	buf[i] = 0;

	puts_ll(buf);
	
}


int main(int argc, char *argv[])
{
	printh(0x000dbeef, 1);
	printh(0x1234beef, 0);

	printi(0x1234ABCD, 16);
}

