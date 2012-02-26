#include <stdio.h>
#include <string.h>
#include <stdint.h>

void *mygets(char *dst, char *src, int n) {
	static char *src_cache = NULL;
	static char *src_ptr = NULL;
	char *ret;
	if (src_cache != src) {
		src_cache = src;
		src_ptr = src;
	}
	if (src_cache == NULL || src_ptr == NULL) { return NULL; }
	ret = memccpy(dst, src_ptr, '\n', n);
	if (ret == NULL) { 
	    src_ptr = src;
	    return NULL; 
	}
	*ret = '\0';
	src_ptr += (int) (ret - dst);
	return dst;
}

inline uint32_t eff_dig(unsigned dig, uint32_t num) {
	uint32_t mask = (1U << dig) - 1;
	return num & mask;
}
inline uint32_t shift_left_l(unsigned shiftwidth, uint32_t num) {
	return num << shiftwidth;
}

