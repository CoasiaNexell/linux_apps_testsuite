//------------------------------------------------------------------------------
//
//	Copyright (C) 2016 Nexell Co. All Rights Reserved
//	Nexell Co. Proprietary & Confidential
//
//	NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module		:
//	File		:
//	Description	:
//	Author		:
//	Export		:
//	History		:
//
//------------------------------------------------------------------------------

#include <stdio.h>		// printf
#include <unistd.h>		// getopt & optarg
#include <stdlib.h>		// atoi
#include <string.h>		// strdup

#include <Util.h>

//------------------------------------------------------------------------------
extern int32_t VpuDecMain(CODEC_APP_DATA *pAppData);
extern int32_t VpuEncMain(CODEC_APP_DATA *pAppData);

enum
{
	MODE_NONE,
	DECODER_MODE,
	ENCODER_MODE,
	MODE_MAX
};

//------------------------------------------------------------------------------
void print_usage(const char *appName)
{
	printf(
		"Usage : %s [options] -i [input file], [M] = mandatory, [O] = Optional \n"
		"  common options :\n"
		"     -m [mode]                  [O]  : 1:decoder mode, 2:encoder mode (def:decoder mode)\n"
		"     -i [input file name]       [M]  : input media file name (When is camera encoder, the value set NULL\n"
		"     -o [output file name]      [O]  : output file name\n"
		"     -h : help\n"
		" -------------------------------------------------------------------------------------------------------------------\n"
		"  only decoder options :\n"
		"     -j [seek frame],           [O]  : seek frame\n"
		"     -d [frame Num],[file name] [O]  : dump frame\n"
		" -------------------------------------------------------------------------------------------------------------------\n"
		"  only encoder options :\n"
		"     -c [codec]                 [O]  : 0:H.264, 1:Mp4v, 2:H.263, 3:JPEG (def:H.264)\n"
		"     -s [width],[height]        [M]  : input image's size\n"
		"     -f [fps Num],[fps Den]     [O]  : input image's framerate(def:30/1) \n"
		"     -b [Kbitrate]              [M]  : target Kilo bitrate (0:VBR mode, other:CBR mode)\n"
		"     -g [gop size]              [O]  : gop size (def:framerate) \n"
		"     -q [quality or QP]         [O]  : Jpeg Quality or Other codec Quantization Parameter(When is VBR, it is valid) \n"
		"     -v [VBV]                   [O]  : VBV Size (def:2Sec)\n"
		"     -x [Max Qp]                [O]  : Maximum Qp \n"
		" ===================================================================================================================\n\n"
		,appName);
	printf(
		"Examples\n");
	printf(
		" Decoder Mode :\n"
		"     #> %s -i [input filename]\n", appName);
	printf(
		" Decoder Mode & Capture :\n"
		"     #> %s -i [input filename] -d [num],[dump filename] \n", appName);
	printf(
		" Encoder Camera Mode :\n"
		"     #> %s -m 2 -o [output filename]\n", appName);
	printf(
		" Encoder File Mode :(H.264, 1920x1080, 10Mbps, 30fps, 30 gop)\n"
		"     #> %s -m 2 -i [input filename] -o [output filename] -s 1920,1080 -f 30,1 -b 10000 -g 30 \n", appName);
}

//------------------------------------------------------------------------------
int32_t main(int32_t argc, char *argv[])
{
	int32_t opt;
	int32_t mode = DECODER_MODE;
	CODEC_APP_DATA appData;

	memset(&appData, 0, sizeof(CODEC_APP_DATA));

	while (-1 != (opt = getopt(argc, argv, "m:i:o:hc:d:s:f:b:g:q:v:x:j:")))
	{
		switch (opt)
		{
		case 'm':
			mode = atoi(optarg);
			if ((mode != DECODER_MODE) && (mode != ENCODER_MODE))
			{
				printf("Error : invalid mode ( %d:decoder mode, %d:encoder mode )!!!\n", DECODER_MODE, ENCODER_MODE);
				return -1;
			}
			break;
		case 'i':	appData.inFileName  = strdup(optarg);  break;
		case 'o':	appData.outFileName = strdup(optarg);  break;
		case 'h':	print_usage(argv[0]);  return 0;
		case 'c':	appData.codec = atoi(optarg);  break;
		case 'd':	appData.dumpFileName = strdup(optarg); sscanf(optarg, "%d,%s", &appData.dumpFrameNumber, appData.dumpFileName);  break;
		case 's':	sscanf(optarg, "%d,%d", &appData.width, &appData.height); break;
		case 'f':	sscanf( optarg, "%d,%d", &appData.fpsNum, &appData.fpsDen );  break;
		case 'b':	appData.kbitrate = atoi(optarg);  break;
		case 'g':	appData.gop = atoi(optarg);  break;
		case 'q':	appData.qp = atoi(optarg);  break;		/* JPEG Quality or Quantization Parameter */
		case 'v':	appData.vbv = atoi(optarg);  break;
		case 'x':	appData.maxQp = atoi(optarg);  break;
		case 'j':
			sscanf(optarg, "%d,%d", &appData.iSeekStartFrame, &appData.iSeekPos);
			printf("iSeekStartFrame = %d, iSeekPos=%d\n", appData.iSeekStartFrame, appData.iSeekPos);
			break;
		default:		break;
		}
	}

	switch (mode)
	{
	case DECODER_MODE:
		return VpuDecMain(&appData);
	case ENCODER_MODE:
		return VpuEncMain(&appData);
	}

	return 0;
}
