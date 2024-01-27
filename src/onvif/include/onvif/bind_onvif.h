#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <NkUtils/assert.h>
#include <NkUtils/macro.h>
#include <NkUtils/allocator.h>
#include <NkUtils/http_utils.h>

#define SPOOK_PROBE_MATCH (0)
#define SPOOK_PROBE_MISMATCH (2)

static int
ONVIF_nvt_probe(const void* msg, ssize_t msg_sz)
{
	NK_Byte mem[1024 * 4];
	NK_Allocator *Alloctr = NK_Nil;
	NK_HTTPHeadField *HeadField = NK_Nil;

	NK_EXPECT_RETURN_VAL((msg != NK_Nil) && (msg_sz > 0), SPOOK_PROBE_MISMATCH);
	NK_EXPECT_RETURN_VAL(NK_PREFIX_STRCASECMP(msg, "POST "), SPOOK_PROBE_MISMATCH);

	Alloctr = NK_Alloc_Create(mem, sizeof(mem));
	NK_EXPECT_RETURN_VAL(NK_Nil != Alloctr, SPOOK_PROBE_MISMATCH);

	HeadField = NK_HTTPUtils_ParseHeadField(Alloctr, "HTTP", (NK_PChar)msg, (NK_Size *)(&msg_sz));
	NK_EXPECT_VERBOSE_RETURN_VAL(NK_Nil != HeadField, SPOOK_PROBE_MISMATCH);

	/**
	 * ÅÐ¶Ï Onvif Â·¾¶ÌØÕ÷¡£
	 */
	NK_EXPECT_VERBOSE_RETURN_VAL(0 == ONVIF_check_uri(HeadField->abs_path, strlen(HeadField->abs_path)), SPOOK_PROBE_MISMATCH);
	return SPOOK_PROBE_MATCH;
}

static int
ONVIF_nvt_loop(bool* trigger, int sock, time_t* read_pts)
{
	int ret;
	ret = ONVIF_SERVER_daemon(sock);
	*trigger = false;
	return ret;
}

NK_Int NK_SPOOK_add_service(char *,
        int (*probe)(const void*, ssize_t),
        int (*loop)(bool*, int , time_t*));
