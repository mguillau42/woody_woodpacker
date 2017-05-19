#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void	*aes(void *data, void *key);
void	*unaes(void *data, void *key);

uint8_t	*set_key(void)
{
	uint8_t	*ret = malloc(16);

	/* ret[0] = 0x0;*/
	/* ret[1] = 0x1;*/
	/* ret[2] = 0x2;*/
	/* ret[3] = 0x3;*/
	/* ret[4] = 0x4;*/
	/* ret[5] = 0x5;*/
	/* ret[6] = 0x6;*/
	/* ret[7] = 0x7;*/
	/* ret[8] = 0x8;*/
	/* ret[9] = 0x9;*/
	/* ret[10] = 0xa;*/
	/* ret[11] = 0xb;*/
	/* ret[12] = 0xc;*/
	/* ret[13] = 0xd;*/
	/* ret[14] = 0xe;*/
	/* ret[15] = 0xf;*/
	ret[15] = 0x0;
	ret[14] = 0x1;
	ret[13] = 0x2;
	ret[12] = 0x3;
	ret[11] = 0x4;
	ret[10] = 0x5;
	ret[9] = 0x6;
	ret[8] = 0x7;
	ret[7] = 0x8;
	ret[6] = 0x9;
	ret[5] = 0xa;
	ret[4] = 0xb;
	ret[3] = 0xc;
	ret[2] = 0xd;
	ret[1] = 0xe;
	ret[0] = 0xf;
	return (ret);
}

uint8_t	*set_data(void)
{
	uint8_t	*ret = malloc(16);

	/* ret[0] = 0x00;*/
	/* ret[1] = 0x11;*/
	/* ret[2] = 0x22;*/
	/* ret[3] = 0x33;*/
	/* ret[4] = 0x44;*/
	/* ret[5] = 0x55;*/
	/* ret[6] = 0x66;*/
	/* ret[7] = 0x77;*/
	/* ret[8] = 0x88;*/
	/* ret[9] = 0x99;*/
	/* ret[10] = 0xaa;*/
	/* ret[11] = 0xbb;*/
	/* ret[12] = 0xcc;*/
	/* ret[13] = 0xdd;*/
	/* ret[14] = 0xee;*/
	/* ret[15] = 0xff;*/
	ret[15] = 0x00;
	ret[14] = 0x11;
	ret[13] = 0x22;
	ret[12] = 0x33;
	ret[11] = 0x44;
	ret[10] = 0x55;
	ret[9] = 0x66;
	ret[8] = 0x77;
	ret[7] = 0x88;
	ret[6] = 0x99;
	ret[5] = 0xaa;
	ret[4] = 0xbb;
	ret[3] = 0xcc;
	ret[2] = 0xdd;
	ret[1] = 0xee;
	ret[0] = 0xff;
	return (ret);
}

int		main(void)
{
	uint8_t	*data = set_data();
	uint8_t	*key = set_key();
	int		i;

	printf("key:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", key[i]);
	printf("\nBefore:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	printf("\nAfter:\n");
	data = aes(data, key);
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	/* printf("\nDECRYPTED:\n");*/
	/* data = unaes(data, key);*/
	/* for (i = 0; i < 16; ++i)*/
	/*     printf("%s%02X", i ? " | " : "", data[i]);*/
	printf("\n");
	return (0);
}
