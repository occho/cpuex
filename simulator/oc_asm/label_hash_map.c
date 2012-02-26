#include <string.h>
#include <stdio.h>
#include "oc_asm.h"

#define HASH_MAP_MAX (LINE_MAX*4)
static label_t label_hash_map[HASH_MAP_MAX];
static inline int is_blank(unsigned index) {
    return label_hash_map[index].len == 0;
}
static inline int same_label(label_t label, unsigned index) {
    return strcmp(label.name, label_hash_map[index].name) == 0;
}
static inline unsigned rehash(unsigned index) {
    return (index + 1)%HASH_MAP_MAX;
}

static unsigned hash_func(label_t label) {
    char *name = label.name;
    int len = label.len;
    char ch0 = name[0],
    	 ch1 = name[len/2],
		 ch2 = name[len-1],
		 ch3 = 1,
		 ch4 = 1;
    if (len > 3) {
    	ch3 = name[len-2];
    	ch4 = name[len-3];
    }
    return (unsigned) ((ch0*ch1*ch2*ch3*ch4)*29)%HASH_MAP_MAX;
}

int hash_insert(label_t label) {
    unsigned index = hash_func(label);

    while (!is_blank(index)) {
    	if (same_label(label, index)) {
	    // duplicate object
	    warning("duplicate label @ hash_insert\n");
	    return -1;
	}
	// collision rehash
	warning("rehash. index : %d label: %s\n", index, label.name);
	index = rehash(index);
    }

    strncpy(label_hash_map[index].name, label.name, label.len);
    label_hash_map[index].len = label.len;
    label_hash_map[index].line = label.line;

    return 0;
}

int hash_find(label_t label) {
    int index = hash_func(label);
    int i, find_flag = 0;
    for (i = 0; i < HASH_MAP_MAX; i++) {
    	if (same_label(label, index)) {
	    find_flag = 1;
	    break;
	}
    	index = rehash(index);
    }
    if (find_flag==0) {
    	warning("Not found @ hash_find. label:%s\n", label.name);
		return -1;
    }
    return label_hash_map[index].line;
}

