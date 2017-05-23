#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void	*aes(void *data, void *key);
void	*unaes(void *data, void *key);

uint8_t	*set_key(void)
{
	uint8_t	*ret = malloc(16);

	ret[0] = 0x0;
	ret[1] = 0x1;
	ret[2] = 0x2;
	ret[3] = 0x3;
	ret[4] = 0x4;
	ret[5] = 0x5;
	ret[6] = 0x6;
	ret[7] = 0x7;
	ret[8] = 0x8;
	ret[9] = 0x9;
	ret[10] = 0xa;
	ret[11] = 0xb;
	ret[12] = 0xc;
	ret[13] = 0xd;
	ret[14] = 0xe;
	ret[15] = 0xf;
	return (ret);
}

uint8_t	*set_data(void)
{
	uint8_t	*ret = malloc(16);

	ret[0] = 0x00;
	ret[1] = 0x11;
	ret[2] = 0x22;
	ret[3] = 0x33;
	ret[4] = 0x44;
	ret[5] = 0x55;
	ret[6] = 0x66;
	ret[7] = 0x77;
	ret[8] = 0x88;
	ret[9] = 0x99;
	ret[10] = 0xaa;
	ret[11] = 0xbb;
	ret[12] = 0xcc;
	ret[13] = 0xdd;
	ret[14] = 0xee;
	ret[15] = 0xff;
	return (ret);
}

int		main(void)
{
	uint8_t	*data = set_data();
	uint8_t	*key = set_key();
	int		i;

	printf("ENCRYPTION:\nkey:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", key[i]);
	printf("\nBefore:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	printf("\nAfter:\n");
	data = aes(data, key);
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	printf("\n\nDECRYPTION:\nkey:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", key[i]);
	printf("\nBefore:\n");
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	printf("\nAfter:\n");
	data = unaes(data, key);
	for (i = 0; i < 16; ++i)
		printf("%s%02X", i ? " | " : "", data[i]);
	printf("\n");
	return (0);
}
