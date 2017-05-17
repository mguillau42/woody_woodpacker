
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>


uint64_t	get_m(uint8_t *msg);
void		set_m(uint8_t *msg, uint64_t m);
uint32_t	rol_subkey(uint64_t nbr, uint32_t state);
uint8_t		get_block(uint64_t blocks, uint32_t i);

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
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 1), get_block(0xdeadbeef, 1));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 2), get_block(0xdeadbeef, 2));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 3), get_block(0xdeadbeef, 3));
	printf("C: %lx | ASM: %lx\n", get_block_c(0xdeadbeef, 4), get_block(0xdeadbeef, 4));
}
