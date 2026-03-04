#ifndef __UTILS_H__
#define __UTILS_H__

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef struct string {
	char *begin;
	char *end;
} string;

#if defined(__GNUC__) && !defined(__clang__) && !defined(_MSVC_VER)
#define MACRO ({
#define ENDM  })
#else // __GNUC__ && !__clang__ && !_MSVC_VER
#define MACRO do {
#define ENDM  } while (0)
#endif // __GNUC__ && !__clang__ && !_MSVC_VER

#ifndef AEL_DA_MIN_CAP
#define AEL_DA_MIN_CAP (512)
#endif // AEL_DA_MIN_CAP

#define EMPTY_DA(da) ((da){.capacity=0, .length=0, .items=NULL})

#define AEL_DA_ALLOCATE(da, size)													\
MACRO																				\
	size_t __cap = (da).capacity;													\
	if ((da).capacity == 0) {														\
		(da).capacity = AEL_DA_MIN_CAP;												\
	}																				\
	while ((da).capacity < (size)) {												\
		(da).capacity *= 1.5;														\
	}																				\
	if ((da).capacity != __cap) {													\
		(da).items = realloc((da).items, (da).capacity * sizeof(*((da).items)));	\
		assert((da).items && "DynamicArray failed to allocate item buffer!");		\
	}																				\
ENDM

#define AEL_DA_PUSH(da, item)														\
MACRO																				\
	AEL_DA_ALLOCATE((da), (da).length + 1);											\
	(da).items[(da).length++] = (item);												\
ENDM

#define AEL_DA_APPEND(da, its, len)													\
MACRO																				\
	AEL_DA_ALLOCATE((da), (da).length + (len));										\
	for (size_t i = 0; i < (len); i++) {											\
		(da).items[(da).length++] = (its)[(len)];									\
	}																				\
ENDM

#ifndef NO_STRIP_AEL_PREFIX
#define DA_MIN_CAP AEL_MIN_CAP
#define DA_ALLOCATE AEL_DA_ALLOCATE
#define DA_PUSH AEL_DA_PUSH
#endif // STRIP_AEL_PREFIX

string new_string(char *begin, char *end);

#endif // __UTILS_H__
