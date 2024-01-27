/******************************************************************************

  Copyright (C), 2013-2020, GuangZhou JUAN Electronics Co., Ltd.

 ******************************************************************************
  File Name    : rtplib.h
  Version       : Initial Draft
  Author        : kejiazhw@gmail.com(kaga)
  Created       : 2013/04/25
  Last Modified : 2013/04/25
  Description   : A transport protocal for real time application  utils , reference to rfc3550,rfc3984
 
  History       : 
  1.Date        : 2013/04/25
    	Author      : kaga
 	Modification: Created file	
******************************************************************************/

#ifndef __RTPLIB_H__
#define __RTPLIB_H__

#ifdef __cplusplus
extern "C" {
#endif


//#include "rtspdef.h"

/**************************************************************************
* configure mirco
**************************************************************************/
#define RTP_MTU_SIZE 			(1258)
#define RTP_MAX_FRAGMENTATION	(800) /*we must configure this manually*/
#define RTP_FRAME_INFO_MAGIC	(0x36277263)

#define RTP_VIDEO_RECV_BUFFER_SIZE	(800*1024)
#define RTP_AUDIO_RECV_BUFFER_SIZE	(10000)
#define RTP_NORTPBUF_MAGIC	(0xCACBACBC)
/*************************************************************************
* const micro relative to rtp, must not modified
**************************************************************************/
#define RTSP_INTERLEAVED_HEADER_SIZE	(4)
#define RTP_HEADER_SIZE					(12)
#define RTP_MAX_EXT_SIZE		(18)
#define RTP_PACKET_SIZE			(RTP_MTU_SIZE + 18 + 4)//max header size: 4(rtsp_interleaved_header)+
			//12(rtp_header)+1(FU_indicator)+1(FU_header) + paddingsize(1~3)(4bytes align)
#define RTP_G711_PACKET_SIZE	(8000/25 + 12 + 4) //336 //max header size: 4(rtsp_interleaved_header)+
			//12(rtp_header)

#define RTP_VERSION				(2)
#define RTP_DYNAMIC_TYPE		(96)

#define RTP_TRANSPORT_UDP	(0)
#define RTP_TRANSPORT_TCP	(1)

#define RTP_UNICAST		(0)
#define	RTP_MULTICAST		(1)

//#define RTP_UNDEFINED	0
//#define RTP_SINGLE_UNIT	1-23 //single nal unit packet per H.264
#define H264_NON_IDR				1
#define H264_SLICE_PARTITION_A		2
#define H264_SLICE_PARTITION_B		3
#define H264_SLICE_PARTITION_C		4
#define H264_IDR					5
#define H264_SEI					6
#define H264_SPS					7
#define H264_PPS					8
#define H264_ACCESS_UNIT_DELIMITER	9
#define H264_END_OF_SEQUENCE		10
#define H264_END_OF_STREAM			11
#define H264_FILLER_DATA			12
// 13...23 RESERVED
#define RTP_STAP_A	24	//single-time aggregation packet
#define RTP_STAP_B	25
#define RTP_MTAP16	26	//multi-time aggregation packet
#define RTP_MTAP24	27
#define	RTP_FU_A	28	//fragmentation unit
#define RTP_FU_B	29	//fragmentation unit

#define H265_TAIL_N		1
#define H265_TAIL_R		2
#define H265_IDR_W_RADL	19
#define H265_IDR_N_LP	20
#define H265_VPS_NUT	32
#define H265_SPS_NUT	33
#define H265_PPS_NUT	34
#define H265_AUD_NUT	35
#define H265_EOS_NUT	36
#define H265_EOB_NUT	37
#define H265_FD_NUT		38
#define H265_PREFIX_SEI_NUT	39
#define H265_SUFFIX_SEI_NUT	40
#define H265_NUT_END	47

#define RTP_H265_AP		48
#define RTP_H265_FU		49


#define PT_IS_H265_IDR(_pt)	(((_pt)  == H265_IDR_W_RADL) || ((_pt) == H265_IDR_N_LP)) 
#define PT_IS_H265_PSLICE(_pt) (((_pt)  < H265_IDR_W_RADL) && ((_pt)  >= 0))
#define PT_IS_H265_X_NUT(_pt)	(((_pt)  == H265_VPS_NUT) || ((_pt) == H265_SPS_NUT) \
	|| ((_pt) == H265_PPS_NUT) || ((_pt) == H265_PREFIX_SEI_NUT) || ((_pt) == H265_SUFFIX_SEI_NUT))

typedef enum{
	RTP_TYPE_PCMU = 0,
	RTP_TYPE_RESERVED1,
	RTP_TYPE_RESERVED2,
	RTP_TYPE_GSM,
	RTP_TYPE_G723,
	RTP_TYPE_DVI4_8000,
	RTP_TYPE_DVI4_16000,
	RTP_TYPE_LPC,
	RTP_TYPE_PCMA,
	RTP_TYPE_G722,
	RTP_TYPE_L161,//44.1khz ,2 channels  //10
	RTP_TYPE_L162,//44.1khz, 1 channel
	RTP_TYPE_QCELP,
	RTP_TYPE_CN,
	RTP_TYPE_MPA,
	RTP_TYPE_G728,
	RTP_TYPE_DVI4_11025,
	RTP_TYPE_DVI4_22050,
	RTP_TYPE_G729, //18
	//
	RTP_TYPE_CEIB =25,
	RTP_TYPE_JPEG =26,
	RTP_TYPE_H261 =31,
	RTP_TYPE_MPV =32,
	RTP_TYPE_MP2T =33,
	RTP_TYPE_H263=34,
	//
	//
	RTP_TYPE_DYNAMIC = 96,//~127
	RTP_TYPE_DYNAMIC_H265 = 97,
}RtpType_t;


#define RTP_TYPE_PS		(128)
#define RTP_TYPE_TS		(129)
#define RTP_TYPE_MPEG	(130)
#define RTP_TYPE_H264  	(131)
#define RTP_TYPE_H265	(132)
#define RTP_TYPE_AAC	(150)


#define RTP_FRAME_TYPE_H264		(96)
#define RTP_FRAME_TYPE_ULAW		(0)
#define RTP_FRAME_TYPE_ALAW		(8)

#define RTP_DEFAULT_AUDIO_TYPE	RTP_TYPE_PCMA
#define RTP_DEFAULT_VIDEO_TYPE	RTP_TYPE_DYNAMIC

typedef struct _frame_info
{
	unsigned int magic;
	unsigned int timestamp;
	unsigned int seq:16;
	unsigned int type:8;
	unsigned int start_flag:1;
	unsigned int stop_flag:1;
	unsigned int key_flag:1;
	unsigned int reserved:5;
	//
	unsigned int frame_pos;// offset to the bottom of circle buffer
	unsigned int frame_size;
}RtpFrameInfo_t;


typedef struct _rtp_header
{
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |V=2|P|X| CC|M|     PT      |       sequence number         |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                           timestamp                           |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           synchronization source (SSRC) identifier            |
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// |            contributing source (CSRC) identifiers             |
// |                             ....                              |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    unsigned char csrc_cnt:4;
    unsigned char extension:1;
    unsigned char padding:1; 
    unsigned char version : 2;
    unsigned char payload_type:7;
    unsigned char marker:1;
    unsigned short sequence_number;
    unsigned int timestamp;
    unsigned int ssrc;
    // ignore the csrc id
}RtpHeader_t;


typedef struct RtpExtHeader
{
	uint16_t d_profile; //defined profile
	uint16_t length;  // 4 *L , exclude this 4 B
	// ... data
}RtpExtHeader_t;

typedef struct _nalu
{
	union
	{
		struct
		{
			unsigned char type : 5;
			unsigned char nal_ref_idc : 2;
			unsigned char forbidden_zero_bit : 1; // must be 0
		};
		unsigned char padding;
	};
}Nalu_t;

typedef struct _h265_nalu
{
	union
	{
		struct
		{
			unsigned short tid : 3;
			unsigned short layer_id : 6;
			unsigned short type : 6;
			unsigned short forbidden_zero_bit : 1; // must be 0
		};
		unsigned short padding;
		unsigned char pad2[2];
	};
}H265Nalu_t;

typedef Nalu_t FUIndicator_t;
typedef H265Nalu_t H265FUIndicator_t;
typedef struct _fu_header
{
	union
	{
		struct
		{
		    unsigned char type:5;
			unsigned char reserved_bit:1;
			unsigned char stop_bit:1;    
			unsigned char start_bit:1;
		};
		unsigned char padding;
	};
}FUHeader_t;

typedef struct _h265_fu_header
{
	union
	{
		struct
		{
		    unsigned char type:6;
			unsigned char stop_bit:1;    
			unsigned char start_bit:1;
		};
		unsigned char padding;
	};
}H265FUHeader_t;


typedef struct _rtppacket
{
	int cnt;
	void *buffer;
	unsigned int malloc_size;
	unsigned int buf_size[RTP_MAX_FRAGMENTATION];
	char trunk_buf[10000];
	unsigned int trunk_readed;
	//
	int iFrameCnt; //  frame count
}RtpPacket_t;

typedef struct _rtsp_interleaved_header
{
	unsigned char magic;	//'$'
	unsigned char channel;
	unsigned short length;
}RtspInterHeader_t;


typedef struct __rtp
{
	int role;
	int buffertime;
	unsigned int buffersize;
	// transport
	int interleaved;
	int sock;
	int peer_chn_port;
	int protocal;
	char peername[20];
	//
	unsigned int ssrc;
	//int payload_type;
	int payloadType;
	int mediaType; // detailed media type; sometime is equal to payloadtype; but if payloadtype is dynamic , it is no longer
	unsigned int base_seq;
	unsigned int seq;// max seq
	unsigned int timestamp;
	unsigned int packet_cnt;// sended number of packets
	unsigned int octet_cnt;// sended number of bytes
	int cycle_cnt;// when seq exceed 65536 ,this increase
	unsigned int comulative_lost;
	int fraction_lost;
	int isDirty;
	//
	int raw_data;
	RtpHeader_t header;
	RtspInterHeader_t interHeader;
	//
	RtpPacket_t packet;
}Rtp_t;



typedef int (*fRtpParsePacket)(Rtp_t *rtp,void *payload,int size);

Rtp_t *RTP_client_new(int protocal,/* udp or tcp */
	int interleaved, /* true or false */
	int sock,
	int payloadType, 
	int mediaType,
	char *dstip,
	int dstport,
	int buffer_time);
Rtp_t *RTP_server_new(unsigned int ssrc,
	int payload_type,
	int protocal,	 /* udp or tcp */
	int interleaved, /* true or false */
	int sock,
	char *dstip,
	int dstport);
int RTP_set_bufsize(Rtp_t *rtp, int id, unsigned int size);
//
extern int RTP_destroy(Rtp_t *rtp);

// packet a video frame(h264) or audio frame to rtp packets
extern int RTP_send_packet(Rtp_t *rtp,char *src,unsigned int len,unsigned int ts,int payload_type);

int RTP_pack(Rtp_t *rtp,char *src,unsigned int len,unsigned int ts,int payload_type);
int RTP_send(Rtp_t *rtp);

// if using rtp over udp, receive a rtp packet and parse it, in this mode , we must given payload to NULL, and size to zero
// if using rtp over rtsp, only parse the packet given by parameter of [ payload ]
extern int RTP_handle_packet(Rtp_t *rtp,
	void *payload,int size);/*rtp packet(contain rtsp interleaved header) and rtp packet size,only use for interleaved mode*/
extern int RTP_handle_packet_nortpbuf(Rtp_t *rtp,void *payload,int payload_size);


#define RTP_HANDLE	RTP_handle_packet_nortpbuf


#ifdef __cplusplus
}
#endif
#endif

