#pragma once
#include "gameNode.h" // getMemDC
#include <chrono>

// FFmpeg
extern "C" {
#include "include/libavcodec/avcodec.h" // 인코딩(encoding)/디코딩(decoding)용
//#include "include/libavdevice/avdevice.h" // 특수 디바이스 멀티플레싱(muxing)/디멀티플렉싱(demuxing)용
//#include "include/libavfilter/avfilter.h" // 프레임 편집용
#include "include/libavformat/avformat.h" // 입출력용, 멀티플레싱(muxing)/디멀티플렉싱(demuxing)용
#include "include/libavutil/avutil.h" // 공통 유틸리티
#include "include/libavutil/imgutils.h" // av_image_get_buffer_size
#include "include/libswscale/swscale.h" // 색 변환, 확대/축소용
}
#pragma comment(lib, "lib/avcodec.lib")
#pragma comment(lib, "lib/avformat.lib")
#pragma comment(lib, "lib/avutil.lib")
#pragma comment(lib, "lib/swscale.lib")

class videoPlayer: public gameNode
{
	private:
	AVCodec* _videoCodec;// , * _audioCodec;
	AVCodecContext* _videoCodecContext;// , * _audioCodecContext;
	AVFrame* _videoFrame, * _videoFrameBGRA;//, * _audioFrame;
	//AVPacket _packet;
	AVPacket* _packet;
	AVFormatContext* _formatContext;
	SwsContext* _imageConvContext;
	int _videoStreamIndex;// , _audioStreamIndex;
	HANDLE _hThread;
	RECT windowRct;
	chrono::steady_clock::time_point _prevTime, _currTime;
	chrono::nanoseconds _elapsedTime;
	BOOL _shouldCloseThread, _isPlayingVideo;

	public:
	videoPlayer();
	~videoPlayer() override;
	HRESULT init() override;
	void update() override;
	void render() override;
	void release() override;
	void playOpening();

	void threadSleep(chrono::duration<long long, nano> us);
	void setFlagToCloseThread() { _shouldCloseThread = TRUE; }

	static unsigned CALLBACK threadForVideo(LPVOID params);
};
