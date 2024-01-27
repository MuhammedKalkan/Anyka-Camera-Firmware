#ifndef _AK_GLOBAL_H_
#define _AK_GLOBAL_H_

#include <stdlib.h>

#define    EQ_MAX_BANDS    10

/* audio function status */
enum audio_func_status {
	AUDIO_FUNC_DISABLE = 0x00,
	AUDIO_FUNC_ENABLE,
};

/* audio channel type */
enum audio_channel_type {
	AUDIO_CHANNEL_RESERVED = 0x00,
	AUDIO_CHANNEL_MONO,
	AUDIO_CHANNEL_STEREO,
};

enum ak_audio_type {
	AK_AUDIO_TYPE_UNKNOWN,
	AK_AUDIO_TYPE_MIDI,
	AK_AUDIO_TYPE_MP3,
	AK_AUDIO_TYPE_AMR,
	AK_AUDIO_TYPE_AAC,
	AK_AUDIO_TYPE_WMA,
	AK_AUDIO_TYPE_PCM,
	AK_AUDIO_TYPE_ADPCM_IMA,
	AK_AUDIO_TYPE_ADPCM_MS,
	AK_AUDIO_TYPE_ADPCM_FLASH,
	AK_AUDIO_TYPE_APE,
	AK_AUDIO_TYPE_FLAC,
	AK_AUDIO_TYPE_OGG_FLAC,
	AK_AUDIO_TYPE_RA8LBR,
	AK_AUDIO_TYPE_DRA,
	AK_AUDIO_TYPE_OGG_VORBIS,
	AK_AUDIO_TYPE_AC3,
	AK_AUDIO_TYPE_PCM_ALAW,
	AK_AUDIO_TYPE_PCM_ULAW,
	AK_AUDIO_TYPE_SBC,
	AK_AUDIO_TYPE_MSBC,
	AK_AUDIO_TYPE_SPEEX,
	AK_AUDIO_TYPE_SPEEX_WB
};

/* video encode frame define */
enum video_frame_type {
    FRAME_TYPE_P,
    FRAME_TYPE_I,
    FRAME_TYPE_B,
	FRAME_TYPE_PI,
};

enum ak_audio_eq_type {
    TYPE_NO ,
    TYPE_HPF ,
    TYPE_LPF ,
    TYPE_HSF ,
    TYPE_LSF ,
    TYPE_PF1    //PeaKing filter
};

/* audio input/output param */
struct pcm_param {
	unsigned int sample_rate;
	unsigned int sample_bits;
	unsigned int channel_num;
};

/* audio encode/decode param */
struct audio_param {
	enum ak_audio_type type;					//encode/decode type
	unsigned int sample_rate;
	unsigned int sample_bits;
	unsigned int channel_num;
};

/* audio/video data frame */
struct frame {
	unsigned char *data;	//frame data
	unsigned int len;		//frame len in bytes
	unsigned long long ts;	//timestamp(ms)
	unsigned long seq_no;	//current frame sequence no.
};

struct audio_stream {
	unsigned char *data; 	//stream data
	unsigned int len; 		//stream len in bytes
	unsigned long long ts;	//timestamp(ms)
	unsigned long seq_no;	//current stream sequence no according to frame
};

struct video_stream {
	unsigned char *data;	//stream data
	unsigned int len; 		//stream len in bytes
	unsigned long long ts;	//timestamp(ms)
	unsigned long seq_no;	//current stream sequence no according to frame
	enum video_frame_type frame_type;	//I or P frame
};

struct ak_audio_eq_attr {
    //int eq_mode;

	///< Value in [-12.0x1024, 12.0x1024], Unit 1024xdB.
    signed short pre_gain;

    /// Number of Active Bands, Reference to @ref bandfreqs, @ref bandgains, @ref bandQ, @ref band_types.
    /// Value in [1, 10]
	unsigned long bands;

	/// Each Band Frequency, Unit Hz.
	unsigned long bandfreqs[EQ_MAX_BANDS];

	/// Each Band Gain, Value in [-32.0x1024, 32.0x1024]
	signed short bandgains[EQ_MAX_BANDS];

	/// Each Band Q-Value,
	/// Value MUST be > 0
	/// Value MUST be < 64.0x1024
	/// Value MUST be < {Sample Rate}x1024 / (2x@ref branfreqs)
	/// Default 1.22x1024 If 0 be Set.
	unsigned short bandQ[EQ_MAX_BANDS];

	/// Check @ref ak_audio_eq_type.
	unsigned short band_types[EQ_MAX_BANDS];

	/// Smoothing Eanble Indication.
	unsigned char   smoothEna;

	/// Smoothing Time, Valild ref to @ref smoothEna Active.
	/// Default 256.0x1000/{samplerate} If 0 be Set.
	/// Unit ms.
    unsigned short smoothTime;

    /// Reserved, Forbidden 0
    unsigned char     dcRmEna;
    /// Reserved, Forbidden 0
    unsigned long    dcfb;
    /// Reserved, Forbidden 0
    unsigned char   aslc_ena;
    /// Reserved, Forbidden 0
    unsigned short  aslc_level_max;
    int enable;// v300 not use
};

enum ak_audio_dev_buf_size 
{
	AK_AUDIO_DEV_BUF_SIZE_256 = 256,
	AK_AUDIO_DEV_BUF_SIZE_512 = 512,
	AK_AUDIO_DEV_BUF_SIZE_1024 = 1024,
	AK_AUDIO_DEV_BUF_SIZE_2048 = 2048,
	AK_AUDIO_DEV_BUF_SIZE_3072 = 3072,
	AK_AUDIO_DEV_BUF_SIZE_4096 = 4096
};

enum ak_audio_sample_rate 
{
	AK_AUDIO_SAMPLE_RATE_8000 = 8000,
	AK_AUDIO_SAMPLE_RATE_12000 = 12000,
	AK_AUDIO_SAMPLE_RATE_11025 = 11025,
	AK_AUDIO_SAMPLE_RATE_16000 = 16000,
	AK_AUDIO_SAMPLE_RATE_22050 = 22050,
	AK_AUDIO_SAMPLE_RATE_24000 = 24000,
	AK_AUDIO_SAMPLE_RATE_32000 = 32000,
	AK_AUDIO_SAMPLE_RATE_44100 = 44100,
	AK_AUDIO_SAMPLE_RATE_48000 = 48000,
	AK_AUDIO_SAMPLE_RATE_64000 = 64000,// now not support
};

#endif
