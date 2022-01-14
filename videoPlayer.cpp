#include "stdafx.h"
#include "videoPlayer.h"

#include <process.h> // _beginthreadex

videoPlayer::videoPlayer()
{
}

videoPlayer::~videoPlayer()
{
}

HRESULT videoPlayer::init()
{
	_isPlayingVideo = FALSE;
	_shouldCloseThread = FALSE;
	_formatContext = nullptr;
	windowRct = { 0, 0, WINW, WINH };
	return S_OK;
}

void videoPlayer::update()
{
	if (!_isPlayingVideo && !_shouldCloseThread)
	{
		release();
		_shouldCloseThread = FALSE;

		playOpening();
	}
}

void videoPlayer::render()
{

}

void videoPlayer::release()
{
	if (_hThread != nullptr)
	{
		_shouldCloseThread = TRUE;
		WaitForSingleObject(_hThread, INFINITE);
		CloseHandle(_hThread);
		_hThread = nullptr;
	}

	//av_frame_free(&_audioFrame);
	avformat_close_input(&_formatContext);
	sws_freeContext(_imageConvContext);
	avcodec_free_context(&_videoCodecContext);
	//avcodec_free_context(&_audioCodecContext);
}

void videoPlayer::playOpening()
{
	// 소리 재생을 준비한다.
	SND->play("오프닝", _currMasterVolume * _currBGMVolume);
	SND->pause("오프닝");

	//==== 영상 재생을 준비한다. =======================================//
	if (avformat_open_input(&_formatContext, "res/videos/Chrono Trigger Opening.avi", nullptr, nullptr) < 0) // 특정 파일을 열고 헤더를 읽는다. 실패한다면
	{
		av_log(nullptr, AV_LOG_FATAL, "Failed to open file."); // 기록을 남긴다.
		exit(-100); // 종료한다.
	}

	if (avformat_find_stream_info(_formatContext, nullptr) < 0) // 파일 패킷을 읽어서 스트림 정보를 얻는다. 실패한다면
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to get stream information."); // 기록을 남긴다.
		exit(-100); // 종료한다.
	}

	_videoStreamIndex = av_find_best_stream(_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &_videoCodec, NULL);
	//_audioStreamIndex = av_find_best_stream(_formatContext, AVMEDIA_TYPE_AUDIO, -1, _videoStreamIndex, &_audioCodec, NULL);

	if (_videoStreamIndex < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to find the best stream.");
		exit(-100); // 종료한다.
	}

	// 처리에 필요한 공간을 할당하고 그 공간에 기본값을 채운다.
	//_audioFrame = av_frame_alloc();
	_videoCodecContext = avcodec_alloc_context3(_videoCodec);
	//_audioCodecContext = avcodec_alloc_context3(_audioCodec);

	// 스트림 정보 매개 변수 값을 기반으로 하여 codec context를 채운다.
	if (avcodec_parameters_to_context(_videoCodecContext, _formatContext->streams[_videoStreamIndex]->codecpar) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to fill the video codec context.");
		exit(-100);
	}
	//if (avcodec_parameters_to_context(_audioCodecContext, _formatContext->streams[_audioStreamIndex]->codecpar) < 0)
	//{
	//	av_log(nullptr, AV_LOG_ERROR, "Failed to fill the audio codec context.");
	//	exit(-100);
	//}

	// 지정된 코덱으로 코덱 콘텍스트를 초기화한다.
	if (avcodec_open2(_videoCodecContext, _videoCodec, nullptr) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to initialize the video codec context using the given codec.");
		exit(-100);
	}
	//if (avcodec_open2(_audioCodecContext, _audioCodec, nullptr) < 0)
	//{
	//	av_log(nullptr, AV_LOG_ERROR, "Failed to initialize the audio codec context using the given codec.");
	//	exit(-100);
	//}
	//==================================================================//

	//==== 스레드로 영상을 재생한다. ===================================//
	_hThread = (HANDLE)_beginthreadex(NULL, 0, threadForVideo, this, CREATE_SUSPENDED, NULL);
	if (!_hThread)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to begin the thread.");
		exit(-100);
	}
	_isPlayingVideo = TRUE;
	ResumeThread(_hThread);
	//==================================================================//
}

void videoPlayer::threadSleep(chrono::duration<long long, nano> ns)
{
	do {
		_currTime = chrono::high_resolution_clock::now();
		_elapsedTime = _currTime - _prevTime;
	} while (_elapsedTime < ns);
}

unsigned videoPlayer::threadForVideo(LPVOID params)
{
	videoPlayer* ps = (videoPlayer*)params;

	LONG scrWidth = ps->windowRct.right - ps->windowRct.left;
	LONG scrHeight = ps->windowRct.bottom - ps->windowRct.top;
	size_t totPixels = static_cast<size_t>(scrWidth * scrHeight * 4);

	BITMAPINFO bI{ 0 };
	bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bI.bmiHeader.biBitCount = 32;
	bI.bmiHeader.biWidth = scrWidth;
	bI.bmiHeader.biHeight = -scrHeight; // 음수로 쓴다.
	bI.bmiHeader.biCompression = BI_RGB;
	bI.bmiHeader.biPlanes = 1;

	HDC hDC = GetDC(_hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC); // 소스(source)로 사용할 DC의 핸들

	vector<BYTE> bitmapData;
	bitmapData.reserve(totPixels);
	BYTE* bitmapDataPtr = bitmapData.data();

	HBITMAP hBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapDataPtr), NULL, 0);
	HBITMAP hOBitmap;
	if (hBitmap != nullptr) hOBitmap = (HBITMAP)SelectObject(hTempDC, hBitmap);
	else
	{
		DeleteDC(hTempDC);
		ReleaseDC(_hWnd, hDC);
		return -999;
	}

	bitmapData.reserve(totPixels);

	const double avgFrameRate = av_q2d(ps->_formatContext->streams[ps->_videoStreamIndex]->avg_frame_rate);
	const chrono::nanoseconds sleepTime = static_cast<chrono::duration<long long, nano>>(static_cast<long long>(pow(10, 9) / avgFrameRate));
	
	// ARGB 데이터 저장 공간
	BYTE* bufBGRA = static_cast<BYTE*>(av_malloc(static_cast<size_t>(av_image_get_buffer_size(AV_PIX_FMT_RGB32,
									   scrWidth, scrHeight, 1))));

	ps->_packet = av_packet_alloc();
	ps->_videoFrame = av_frame_alloc();
	ps->_videoFrameBGRA = av_frame_alloc();

	ps->_imageConvContext = sws_getContext(ps->_videoCodecContext->width, ps->_videoCodecContext->height,
		ps->_videoCodecContext->pix_fmt, // 픽셀 형식
		scrWidth, scrHeight,
		AV_PIX_FMT_RGB32,
		/*SWS_BICUBIC*/SWS_BILINEAR,
		nullptr, nullptr, // 필터 적용을 하지 않는다.
		nullptr); // SWS_GAUSS나 SWS_LANCZOS를 사용할 때 지정한다.

	
	// 데이터 포인터들과 라인 크기들을 지정된 매개 변수에 맞추어 준비한다.
	av_image_fill_arrays(ps->_videoFrameBGRA->data, ps->_videoFrameBGRA->linesize, bufBGRA,
		AV_PIX_FMT_RGB32, scrWidth, scrHeight, 1);

	ps->_currTime = chrono::high_resolution_clock::now();
	ps->_elapsedTime = 0ns;

	FMOD::Channel* audioCh = SND->findChannel("오프닝");

	while (ps->_isPlayingVideo)
	{

		if (ps->_shouldCloseThread)
		{
			avformat_close_input(&ps->_formatContext);
			DeleteObject(SelectObject(hTempDC, hOBitmap));
			DeleteDC(hTempDC);
			ReleaseDC(_hWnd, hDC);
			ps->_shouldCloseThread = FALSE;
			av_packet_unref(ps->_packet);
			av_packet_free(&ps->_packet);
			av_frame_free(&ps->_videoFrame);
			av_frame_free(&ps->_videoFrameBGRA);
			if (!bitmapData.empty()) bitmapData.clear();
			av_free(bufBGRA);
			ps->_isPlayingVideo = FALSE;

			SND->stop("오프닝");

			return 0;
		}

		av_packet_unref(ps->_packet);
		if (av_read_frame(ps->_formatContext, ps->_packet) == 0)
		{
			if (ps->_packet->stream_index == ps->_videoStreamIndex)
			{
				//av_free(bufBGRA);
				//bufBGRA = static_cast<BYTE*>(av_malloc(static_cast<size_t>(av_image_get_buffer_size(AV_PIX_FMT_RGB32,
				//							 scrWidth, scrHeight, 1))));
				//av_frame_unref(ps->_videoFrameBGRA);

				av_image_fill_arrays(ps->_videoFrameBGRA->data, ps->_videoFrameBGRA->linesize, bufBGRA,
					AV_PIX_FMT_RGB32, scrWidth, scrHeight, 1);

				if (avcodec_send_packet(ps->_videoCodecContext, ps->_packet) == 0)  // 화상 패킷 데이터를 디코더에 보낸다. 성공한다면
				{
					ps->_elapsedTime -= sleepTime;

					//while (true)
					{
						if (avcodec_receive_frame(ps->_videoCodecContext, ps->_videoFrame) == 0) // 디코더가 반환한 데이터를 받는다. 성공한다면
						{
							sws_scale(ps->_imageConvContext,
										ps->_videoFrame->data, ps->_videoFrame->linesize,
										0, ps->_videoCodecContext->height,
										ps->_videoFrameBGRA->data, ps->_videoFrameBGRA->linesize);

							copy_n(bufBGRA, totPixels, bitmapDataPtr);
							// 또는 memcpy(bitmapDataPtr, bufBGRA, totPixels);

							if (ps->_videoCodecContext->frame_number == 1)
							{
								audioCh->setPosition(17, FMOD_TIMEUNIT_MS);
								SND->resume("오프닝");
							}
							BitBlt(ps->getMemDC(), 0, 0, scrWidth, scrHeight, hTempDC, 0, 0, SRCCOPY);
						}
						//else break;
					}
					if (ps->_elapsedTime > sleepTime) continue;
					ps->_prevTime = ps->_currTime;
					ps->threadSleep(sleepTime);
				}
				else break;
			}
		}
		else
		{
			ps->_isPlayingVideo = FALSE;
			break;
		}
	}
	if (!bitmapData.empty()) bitmapData.clear();
	av_free(bufBGRA);
	av_packet_free(&ps->_packet);
	av_frame_free(&ps->_videoFrame);
	av_frame_free(&ps->_videoFrameBGRA);
	avformat_close_input(&ps->_formatContext);
	DeleteObject(SelectObject(hTempDC, hOBitmap));
	DeleteDC(hTempDC);
	ReleaseDC(_hWnd, hDC);
	ps->_shouldCloseThread = FALSE;

	SND->stop("오프닝");

	return 0;
}



// (참고)
//if (av_read_frame(_formatContext, &_packet) == 0) // 스트림의 다음 프레임을 읽는 데에 성공한다면
//{
//	if (_packet.stream_index == _videoStreamIndex)
//	{
//		avcodec_send_packet(_videoCodecContext, &_packet); // 화상 패킷 데이터를 디코더에 보낸다.
//		if (avcodec_receive_frame(_videoCodecContext, _videoFrame) == 0) // 디코더가 반환한 데이터를 받는다. 성공한다면
//		{
//			// _videoFrame 사용
//			uint8_t* videoData = _videoFrame->data[0];
//			//char pictureType = av_get_picture_type_char(_videoFrame->pict_type);
//			//int frameNum = _videoCodecContext->frame_number;
//			//int keyFrame = _videoFrame->key_frame;
//			//int codedPictureNum = _videoFrame->coded_picture_number;
//			//int displayPictureNum = _videoFrame->display_picture_number;
//			//...
//		}
//	}
//	else if (_packet.stream_index == _audioStreamIndex)
//	{
//		avcodec_send_packet(_audioCodecContext, &_packet); // 소리 패킷 데이터를 디코더에 보낸다.
//		if (avcodec_receive_frame(_audioCodecContext, _audioFrame) == 0) // 디코더가 반환한 데이터를 받는다. 성공한다면
//		{
//			// _audioFrame 사용
//			uint8_t* audioData = _audioFrame->data[0];
//			// ...
//		}
//	}
//	av_packet_unref(&_packet); // 패킷 재사용을 하려면 이 함수를 호출하여야 한다.
//}

// (참고)
//int videoWidth = _formatContext->streams[_videoStreamIndex]->codecpar->width;
//int videoHeight = _formatContext->streams[_videoStreamIndex]->codecpar->height;
//int audioChannels = _formatContext->streams[_audioStreamIndex]->codecpar->channels;
//int audioSampleRate = _formatContext->streams[_audioStreamIndex]->codecpar->sample_rate; // Hz
//int64_t audioBitRate = _formatContext->streams[_audioStreamIndex]->codecpar->bit_rate; // bps
//int videoCodecID = _videoCodec->id;
//int audioCodecID = _audioCodec->id;

// (참고)
//_videoCodecContext->width = 1024;
//_videoCodecContext->height = 896;
//_videoCodecContext->flags2 |= AV_CODEC_FLAG2_FAST;

// (참고)
// 재생 시간 계산(double): av_q2d(_formatContext->streams[_videoStreamIndex]->time_base) * _formatContext->streams[_videoStreamIndex]->duration;
