/**
 * 操作系统适配层。
 */


#include <NkUtils/types.h>

#if !defined(NK_UTILS_STDOS_H_)
#define NK_UTILS_STDOS_H_
NK_CPP_EXTERN_BEGIN

#include <stdio.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32)
//#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")

//# define snprintf		_snprintf
# define strcasecmp		stricmp
# define strncasecmp	strnicmp

/**
 * man strcasestr
 */
NK_API char*
strcasestr(const char* s1, const char* s2);


/**
 * man strtok_r
 */
NK_API char *
strtok_r(char *s, const char *delim, char **save_ptr);


/**
 * man memmem
 */
#define memmem memseek

/**
* man strndup
*/
NK_API char *
strndup(const char *s, size_t n);

# if _MSC_VER < 1900
/**
 * man snprintf
 */
NK_API int
snprintf(char *buffer, size_t count, const char *format, ...);

# endif

#else
# include <string.h>
#endif

/*
 * Find the first occurrence of the byte string needle in byte string haystack.
 */
NK_API NK_PVoid
memseek(const NK_PVoid haystack, NK_Size haystacklen, const NK_PVoid needle, NK_Size needlelen);

NK_CPP_EXTERN_END
#endif /* NK_UTILS_STDOS_H_ */
