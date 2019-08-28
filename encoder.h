//
// Created by lewis on 2019/8/28.
//

#ifndef ENCODE_H264_DEMO_ENCODER_H
#define ENCODE_H264_DEMO_ENCODER_H

extern "C"
{
#include <libavutil/opt.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include <opencv2/opencv.hpp>

namespace encoder {
    class H264Encoder {
    public:
        void InitEncoder(const char *filename);
        void InitAvFrame();
        void GenerateTestAvFrame(int i);
        void ConvertMat2Avframe(cv::Mat img, int img_sequence);
        void EncodeVideo();
        void ReleaseEncoder();
    private:
        AVFrame *av_frame_;
        AVPacket *av_packet_;
        AVCodecContext *codec_context_= NULL;
        const AVCodec *kAv_Codec_;
        FILE *File_;
    };
}

#endif //ENCODE_H264_DEMO_ENCODER_H
