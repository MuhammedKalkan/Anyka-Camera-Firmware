#ifndef _AK_TW_H_
#define _AK_TW_H_

struct tw_detect {
    /* ai handle to get pcm frame */
    void *ai_handle;
    /* exit after detect time when detect failed, unit: ms */
    long detect_time;
    /* parsed string when detect success. */
    char *parse_str;

    /* write debug wave file at path appoited by "debug_file" */
    int debug_file_flag;
    /* debug_filefile name, including file path */
    char *debug_file;
};

struct tw_generate {
    /* generate string */
    char *gen_str;
    /* only support 44.1K and 48K, default is 48K */
    int sample_rate;

    /* generated data len */
    int data_len;
    /* generated data, the same as in "wav_file" */
    unsigned char *data;

    /* write wave file at path appoited by "wav_file" */
    int write_file_flag;
    /* wave file name, including file path */
    char *wav_file;
};

/**
 * ak_tw_detect - detect tone wave from ai to analyze ssid and password.
 * @detect[IN/OUT]: tone wave detection control and data
 * return: 0 success, -1 failed
 * notes: 1. open ai with sample rate 44.1K/48K, 16 bits per sample, mono channel.
 *      2. set "debug_file_flag=1" if you want to write a debug file.
 *      3. parsed string in "parse_str" when detect success.
 */
int ak_tw_get_detect(struct tw_detect *detect);

/**
 * ak_tw_release_detect - release tone wave detection info,
 *      especially the out parsed buffer.
 * @detect[IN/OUT]: tone wave detection control and data
 * return: 0 success, -1 failed
 */
int ak_tw_release_detect(struct tw_detect *detect);

/**
 * ak_tw_get_generate - get generate tone wave info according to string.
 * @gen[IN/OUT]: tone wave generate control and data
 * return: 0 success, -1 failed
 * notes: 1. sample rate 44.1K/48K, 16 bits per sample, mono channel.
 *      2. 88.2K/96K bytes per seconds.
 *      3. you can generate string according to ssid and passwd.
 *      4. you MUST call "ak_tw_release_gen" to free the out buffer.
 *      5. the out buffer data is wave format,
 *          skip the first 44 bytes if you do not use.
 */
int ak_tw_get_generate(struct tw_generate *gen);

/**
 * ak_tw_release_generate - release tone wave generation info,
 *          especially the out buffer.
 * @gen[IN/OUT]: tone wave generate control and data
 * return: 0 success, -1 failed
 */
int ak_tw_release_generate(struct tw_generate *gen);

/**
 * ak_tw_get_version - get tw version
 * return: pointer to tw version string
 */
const char *ak_tw_get_version(void);

#endif
