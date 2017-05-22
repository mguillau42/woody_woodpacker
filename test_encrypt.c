#include <woody.h>

int		main(void)
{
	unsigned char buf[] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff\xde\xad\xbe\xef";
	unsigned char key[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

	printf("Before\n");
	for (int i = 0; i < 36; ++i)
	{
		printf("%hhX ", buf[i]);
	}
	printf("\n");

	encrypt(buf, 36, key);

	printf("After\n");
	for (int i = 0; i < 36; ++i)
	{
		printf("%hhX ", buf[i]);
	}
	printf("\n");
}
