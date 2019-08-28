//
// Created by lewis on 2019/8/28.
//

#include "encoder.h"

void encoder::H264Encoder::InitEncoder(const char *filename) {
    int ret;
    avcodec_register_all();
    kAv_Codec_ = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!kAv_Codec_) {
        fprintf(stderr, "Codec '%s' not found\n", AV_CODEC_ID_H264);
        exit(1);
    }
    codec_context_ = avcodec_alloc_context3(kAv_Codec_);
    if (!codec_context_) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    av_packet_ = av_packet_alloc();
    if (!av_packet_) {
        exit(1);
    }
    /* put sample parameters */
    codec_context_->bit_rate = 1000000;
    /* resolution must be a multiple of two */
    codec_context_->width = 640;
    codec_context_->height = 480;
    /* frames per second */
    codec_context_->time_base = (AVRational){1, 25};
    codec_context_->framerate = (AVRational){25, 1};
    codec_context_->gop_size = 10;
    codec_context_->max_b_frames = 1;
    codec_context_->pix_fmt = AV_PIX_FMT_YUV420P;
    if (kAv_Codec_->id == AV_CODEC_ID_H264) {
        av_opt_set(codec_context_->priv_data, "preset", "slow", 0);
    }
    /* open it */
    ret = avcodec_open2(codec_context_, kAv_Codec_, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec");
        exit(1);
    }
    File_ = fopen(filename, "wb");
    if (!File_) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
}

void encoder::H264Encoder::InitAvFrame() {
    int ret;
    av_frame_ = av_frame_alloc();
    if(!av_frame_) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    av_frame_->format = codec_context_->pix_fmt;
    av_frame_->width  = codec_context_->width;
    av_frame_->height = codec_context_->height;
    ret = av_frame_get_buffer(av_frame_, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
}
//测试用代码，生成虚拟的图像
void encoder::H264Encoder::GenerateTestAvFrame(int i) {
    int ret;
    int x;
    int y;
    fflush(stdout);
    /* make sure the frame data is writable */
    ret = av_frame_make_writable(av_frame_);
    if (ret < 0)
        exit(1);
    /* prepare a dummy image */
    /* Y */
    for (y = 0; y < codec_context_->height; y++) {
        for (x = 0; x < codec_context_->width; x++) {
            av_frame_->data[0][y * av_frame_->linesize[0] + x] = x + y + i * 3;
        }
    }
    /* Cb and Cr */
    for (y = 0; y < codec_context_->height/2; y++) {
        for (x = 0; x < codec_context_->width/2; x++) {
            av_frame_->data[1][y * av_frame_->linesize[1] + x] = 128 + y + i * 2;
            av_frame_->data[2][y * av_frame_->linesize[2] + x] = 64 + x + i * 5;
        }
    }
    av_frame_->pts = i;
}

void encoder::H264Encoder::ConvertMat2Avframe(cv::Mat img, int img_sequence) {
    struct SwsContext *sws_ctx_bgr_yuv = NULL;
    sws_ctx_bgr_yuv = sws_getContext(codec_context_->width,
                                     codec_context_->height,
                                     AV_PIX_FMT_BGR24,
                                     codec_context_->width,
                                     codec_context_->height,
                                     codec_context_->pix_fmt //AV_PIX_FMT_YUV420p
                                     ,0,0,NULL,NULL);
    const int kStide[] = { (int)img.step[0] };
    sws_scale(sws_ctx_bgr_yuv, &img.data, kStide, 0, img.rows, av_frame_->data, av_frame_->linesize);
    av_frame_->pts = img_sequence;
}

void encoder::H264Encoder::EncodeVideo() {
    int ret = 0;
    /* send the frame to the encoder */
    if (av_frame_) {
        printf("Send frame %3"PRId64"\n", av_frame_->pts);
    }
    ret = avcodec_send_frame(codec_context_, av_frame_);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(codec_context_, av_packet_);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        printf("Write packet %3"PRId64" (size=%5d)\n", av_packet_->pts, av_packet_->size);
        fwrite(av_packet_->data, 1, av_packet_->size, File_);
        av_packet_unref(av_packet_);
    }
}

void encoder::H264Encoder::ReleaseEncoder() {
    uint8_t end_code[] = { 0, 0, 1, 0xb7 };
    /* flush the encoder */
    EncodeVideo();
    /* add sequence end code to have a real MPEG file */
    fwrite(end_code, 1, sizeof(end_code), File_);
    fclose(File_);
    avcodec_free_context(&codec_context_);
    av_frame_free(&av_frame_);
    av_packet_free(&av_packet_);
}
