// SeetaFacesTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>  
#include <cstdint>
#include <fstream>
#include <string>
#include <io.h>
#include <conio.h>

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

#include "face_detection.h"
#include "allTracking.h"


using namespace cv;
using namespace std;


//////////////////////
void faceDetect(cv::Mat &frame, vector<cv::Rect> &face_positions);
//////////////////////////


int main()
{
	cv::Mat frame;
	vector<cv::Rect> face_positions;
	VideoCapture cap("D:/YinYueTai/sbs.mp4");
	if (!cap.isOpened()) {                                                 // if unable to open video file
		cout << "error reading video file!" << endl << endl;      // show error message
		_getch();                    // it may be necessary to change or remove this line if not using Windows
		return 0;                                                              // and exit program
	}
	int frame_count = cap.get(CAP_PROP_FRAME_COUNT);

	
	if (frame_count < 2) {
		cout << "error: video file must have at least two frames!";
		_getch();
		return 0;
	}

#if 1
	//***the first read method***
	cap.read(frame);
	char chCheckForEscKey = 0;
	int frameCount = 1;
	//tracing setup
	ALLTracks at;
	while (cap.isOpened() && chCheckForEscKey != 27)
	{
		imshow("TestFace", frame);   //窗口中显示图像
		//face datection
		faceDetect(frame, face_positions);
#if 0
		//face tracking
		//***note:in face_positions,may have the same face rect, so we must do sth***;
		at.trackInit();
		for (int i = 0; i < face_positions.size(); i++)
		{
			at.processImage(frame, face_positions[i]);
		}
		cv::imshow("Test", frame);
		face_positions.clear();
#endif
		if ((cap.get(CAP_PROP_POS_FRAMES) + 1) < cap.get(CAP_PROP_FRAME_COUNT))
			cap.read(frame);
		else 
		{
			cout << "end of video\n";
			break;
		}
		frameCount++;
		chCheckForEscKey = cv::waitKey(1);
	}
	if (chCheckForEscKey != 27)               // if the user did not press esc (i.e. we reached the end of the video)
		waitKey(0);                         // hold the windows open to allow the "end of video" message to show

#endif
#if 0
	//***the second read method***
	bool stop = false;
	double rate = cap.get(CAP_PROP_FPS);
	int delay = 1000 / rate;
	ALLTracks at;
	while (! stop ){
			if (! cap.read(frame))
			{
				cout << "can not read the frame image" << endl;
				return -1;
			}
			imshow("TestFace", frame);   //窗口中显示图像
			//face datection
			faceDetect(frame, face_positions);
			//face tracking
			//***note:in face_positions,may have the same face rect, so we must do sth***;
			at.trackInit();
			for (int i = 0; i < face_positions.size(); i++)
			{
				at.processImage(frame, face_positions[i]);
			}
			cv::imshow("Test", frame);
			char c = waitKey(delay);
			if (c == 27 )
			{
				stop = true;
			}
			face_positions.clear();		
	}
#endif

	return 0;
}

void faceDetect(cv::Mat &frame, vector<cv::Rect> &face_positions)
{
	seeta::FaceDetection detector(".\\Model\\seeta_fd_frontal_v1.0.bin");

	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);

	cv::Mat img_gray;

	if (frame.channels() != 1)
		cv::cvtColor(frame, img_gray, cv::COLOR_BGR2GRAY);
	else
		img_gray = frame;

	seeta::ImageData img_data;
	img_data.data = img_gray.data;
	img_data.width = img_gray.cols;
	img_data.height = img_gray.rows;
	img_data.num_channels = 1;

	double t0 = cv::getTickCount();
	std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
	double t1 = cv::getTickCount();
	double secs = (t1 - t0) / cv::getTickFrequency();

	cout << "Detections takes " << secs << " seconds " << endl;
	/*
#ifdef USE_OPENMP
	cout << "OpenMP is used." << endl;
#else
	cout << "OpenMP is not used. " << endl;
#endif

#ifdef USE_SSE
	cout << "SSE is used." << endl;
#else
	cout << "SSE is not used." << endl;
#endif
	*/
	cout << "Image size (wxh): " << img_data.width << "x" << img_data.height << endl;
	cv::Rect face_rect;

	int32_t num_face = static_cast<int32_t>(faces.size());
	for (int32_t i = 0; i < num_face; i++) {
		face_rect.x = faces[i].bbox.x;
		face_rect.y = faces[i].bbox.y;
		face_rect.width = faces[i].bbox.width;
		face_rect.height = faces[i].bbox.height;
		face_positions.push_back(face_rect);

		cv::rectangle(frame, face_rect, cv::Scalar(0, 0, 255), 4, 8, 0);
		cout << "face size (wxh):" << face_rect.width << "x" << face_rect.height << endl;
	}

}

//
