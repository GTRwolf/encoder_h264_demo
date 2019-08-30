#include "encoder.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {
    encoder::H264Encoder h264_encoder;
    cv::VideoCapture cap(0);
    cv::Mat img_src;
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
        h264_encoder.ConvertMat2Avframe(img_src, i);
        h264_encoder.EncodeAndSendVideo(450);
        cv::imshow("test_img", img_src);
        char key = cv::waitKey(3);
        if ( key == 27 ) {
            break;
        }
        i++;
    }
//测试代码
//    h264_encoder.InitEncoder("test.h264");
//    h264_encoder.InitAvFrame();
//    for (int i = 0; i < 200; i++) {
//       h264_encoder.GenerateTestAvFrame(i);
//       h264_encoder.EncodeVideo();
//    }
//    h264_encoder.ReleaseEncoder();
//    return 0;
}