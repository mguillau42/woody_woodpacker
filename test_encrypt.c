
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>


uint64_t	get_m(uint8_t *msg);
void		set_m(uint8_t *msg, uint64_t m);
uint32_t	rol_subkey(uint64_t nbr, uint32_t state);
uint8_t		get_block(uint64_t blocks, uint32_t i);
uint8_t		replace(const uint8_t block, const uint32_t table[4][16]);

const uint32_t s_box[8][4][16] = {
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
	},

	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
	},

	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
	},

	{
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
	},

	{
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
	},

	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
	},

	{
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
	},

	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
	},
};

uint8_t		replace_c(const uint8_t block, const uint32_t table[4][16])
{
	uint8_t	i = 0;
	uint8_t	j = 0;

	i = ((block >> 5) << 1) + (block & 1);
	j = (block << 3);
	j >>= 4;
	printf("i: %d | j: %d\n", i ,j);
	return (table[i][j]);
}

uint8_t		get_block_c(uint64_t blocks, uint32_t i)
{
	return ((blocks << (16 + (i * 6))) >> 58);
}

uint64_t	subkey_cat(uint64_t c, uint64_t d);
const uint32_t	subkey_rot[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

uint8_t		*empty_msg(void)
{
	uint8_t		*ret;

	ret = malloc(sizeof(uint8_t) * 8);
	ret[0] = 0x00;
	ret[1] = 0x00;
	ret[2] = 0x00;
	ret[3] = 0x00;
	ret[4] = 0x00;
	ret[5] = 0x00;
	ret[6] = 0x00;
	ret[7] = 0x00;
	return (ret);
}

uint8_t		*default_msg(void)
{
	uint8_t		*ret;

	ret = malloc(sizeof(uint8_t) * 8);
	ret[0] = 0x01;
	ret[1] = 0x23;
	ret[2] = 0x45;
	ret[3] = 0x67;
	ret[4] = 0x89;
	ret[5] = 0xAB;
	ret[6] = 0xCD;
	ret[7] = 0xEF;
	return (ret);
}

uint32_t	rol_subkey_c(uint64_t nbr, uint32_t state)
{
	uint32_t	i;

	for (i = 0; i < subkey_rot[state]; ++i)
		nbr = (((nbr << 36) >> 35) + (nbr >> 27)) & 0xfffffff;
	return (nbr);
}

uint64_t	subkeycat_c(uint64_t c, uint64_t d)
{
	uint64_t	ret = c;

	ret <<= 28;
	ret += d;
	return (ret);
}

void	print_msg(uint8_t *msg, size_t len)
{
	uint32_t	i;

	for (i = 0; i < len; ++i)
		printf("%s%02hhX", i ? " | " : "", msg[i]);
	printf("\n");
}

int		main(void)
{
	uint8_t		*msg = default_msg();
	uint64_t	mdr = get_m(msg);
	uint8_t		*msg2 = empty_msg();

	printf("Before get_m:\n");
	print_msg(msg, 8);
	printf("After get_m:\n");
	printf("%lx\n", mdr);
	printf("After set_m:\n");
	set_m(msg2, mdr);
	print_msg(msg2, 8);
	printf("After rol_subkey:\n");
	printf("C: %lx | ASM: %lx\n", rol_subkey_c(mdr, 1), rol_subkey(mdr, 1));
	printf("After subkey_cat:\n");
	printf("C: %lx | ASM: %lx\n", subkeycat_c(rol_subkey_c(mdr, 1), rol_subkey_c(mdr, 1)), subkey_cat(rol_subkey(mdr, 1), rol_subkey(mdr, 1)));
	printf("After getblock:\n");
	printf("C: %lx | ASM: %lx\n", get_block_c(0xcafebabe, 1), get_block(0xcafebabe, 1));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xcafebabe, 2), get_block(0xcafebabe, 2));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 3), get_block(0xdeadbeef, 3));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 4), get_block(0xdeadbeef, 4));
	printf("After replace:\n");
	printf("C: %x | ASM: %x\n", replace_c(get_block_c(0xcafebabe, 2), s_box[2]), replace(get_block(0xcafebabe, 2), s_box[2]));
	printf("C: %x | ASM: %x\n", replace_c(get_block_c(0xcafebabe, 3), s_box[3]), replace(get_block(0xcafebabe, 3), s_box[3]));
	printf("C: %x | ASM: %x\n", replace_c(get_block_c(0xcafebabe, 4), s_box[2]), replace(get_block(0xcafebabe, 4), s_box[2]));
}
