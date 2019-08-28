#include "encoder.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {
    encoder::H264Encoder h264_encoder;
    cv::VideoCapture cap(0);
    cv::Mat img_src;
    cv::Mat img_dst;
    int i = 0;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output file> \n", argv[0]);
        exit(0);
    }
    if( !cap.isOpened() ) {
        std::cout << "cap is not opened!!!" << std::endl;
        exit(0);
    }
    h264_encoder.InitEncoder(argv[1]);
    h264_encoder.InitAvFrame();
    while(true) {
        cap >> img_src;
        if(img_src.empty()) {
            std::cout << "img_src is empty!!!" << std::endl;
            exit(0);
        }
        cv::cvtColor(img_src, img_dst, CV_BGR2RGB);
        h264_encoder.ConvertMat2Avframe(img_dst);
        h264_encoder.EncodeVideo();
        cv::imshow("test_img", img_src);
        cv::waitKey(3);
    }
//    h264_encoder.InitEncoder("test.h264");
//    h264_encoder.InitAvFrame();
//    for (int i = 0; i < 200; i++) {
//       h264_encoder.GenerateTestAvFrame(i);
//       h264_encoder.EncodeVideo();
//    }
    h264_encoder.ReleaseEncoder();

    return 0;
}