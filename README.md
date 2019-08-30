# encoder_h264_demo
ffmpeg version 3.4.4
function: encode camera video in h264 with ffmpeg
Argument: filename
将相机捕获的图像进行h264编码，通过udp发送至指定的端口
编码器参数调整可修改encoder.cpp中的InitEncoder函数
udp配置的ip 及 端口 暂不支持参数输入 
