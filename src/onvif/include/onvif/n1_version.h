
/**
 * N1 Version Defination.
 */

#if !defined(NK_N1_VERSION_H_)
#define NK_N1_VERSION_H_

/**
 * Major Version Number.
 */
#if !defined(NK_N1_VER_MAJ)
# define NK_N1_VER_MAJ                (2U)
#endif

/**
 * Minor Version Number.
 */
#if !defined(NK_N1_VER_MIN)
# define NK_N1_VER_MIN                (4U)
#endif

/**
 * Revision Version Number.
 */
#if !defined(NK_N1_VER_REV)
# define NK_N1_VER_REV                (1U)
#endif

/**
 * Version Number Code.
 */
#if !defined(NK_N1_VER_NUM)
# define NK_N1_VER_NUM                (9U)
#endif

/**
 * Version Text
 */
#if !defined(NK_N1_VER_STR)
# define NK_N1_VER_STR()                "2.4.1.9"
#endif

/**
 * Version Encode
 */
#if !defined(NK_N1_VER_ENC)
# define NK_N1_VER_ENC(__maj, __min, __rev, __num) \
	((((__num) + (__rev) * 10000) + (__min) * 1000000) + (__maj) * 100000000)
#endif

/**
 * Version Decode
 */
#if !defined(NK_N1_VER_DEC)
# define NK_N1_VER_DEC(__code, __maj, __min, __rev, __num) \
	do {\
		__maj = ((__code) / 100000000) % 100;\
		__min = ((__code) / 1000000) % 100;\
		__rev = ((__code) / 10000) % 100;\
		__num = (__code) % 10000;\
	} while (0)
#endif

/**
 * Version Code
 */
#if !defined(NK_N1_VER_CODE)
# define NK_N1_VER_CODE() \
	NK_N1_VER_ENC(NK_N1_VER_MAJ, NK_N1_VER_MIN, NK_N1_VER_REV, NK_N1_VER_NUM)
#endif


/**
 * Version Release Year Value.
 */
#if !defined(NK_N1_VER_DATE_YEAR)
# define NK_N1_VER_DATE_YEAR           (2018U)
#endif

/**
 * Version Release Month Value.
 */
#if !defined(NK_N1_VER_DATE_MON)
# define NK_N1_VER_DATE_MON            (3U)
#endif

/**
 * Version Release Month Day Value.
 */
#if !defined(NK_N1_VER_DATE_MDAY)
#  define NK_N1_VER_DATE_MDAY           (20U)
#endif

/**
 * Version Release Date Text.
 */
#if !defined(NK_N1_VER_DATE_STR)
#  define NK_N1_VER_DATE_STR()           "2018.3.20"
#endif


#endif /* NK_N1_VERSION_H_ */
