//#include "stdafx.h"
#include "allTracking.h"

void ALLTracks::trackInit(){
	beginInit = false;
	startTracking = false;
	//camshift
	//track_type = CAMSHIFT_TRACKER;
	//cmt
	track_type = CMT_TRACKER;

}

//camshift
void ALLTracks::camshiftTracking(cv::Mat &frame, cv::Rect box){
	if (beginInit){
		if (camshift != NULL)
			delete camshift;
	}
	camshift = new Camshift();
	initBox = box;
	camshift->initialize(frame, initBox);
	std::cout << "Camshift tracking init!" << std::endl;
	startTracking = true;
	beginInit = false;

	if (startTracking){
		std::cout << "Camshift tracking process..." << std::endl;
		camshift->processFrame(frame);
		RotatedRect rect = camshift->objectBox;
		Point2f vertices[4];
		rect.points(vertices);
		for (int i = 0; i < 4; i++)
		{
			line(frame, vertices[i], vertices[(i+1)%4],Scalar(255,0,255));
		}
	}
}

//cmt
void ALLTracks::cmtTracking(cv::Mat &frame, cv::Rect box){
	cv::Mat img_gray;
	cvtColor(frame, img_gray, COLOR_BGR2GRAY);
	if (beginInit)
	{
		if (cmtTracker != NULL)
			delete cmtTracker;
		cmtTracker = new cmt::CMT();
		initBox = box;
		cmtTracker->initialize(img_gray, initBox);
		std::cout << "cmt tracker init!" << std::endl;
		startTracking = true;
		beginInit = false;
	}

	if (startTracking)
	{
		std::cout << "cmt tracker process..." << std::endl;
		cmtTracker->processFrame(img_gray);
		for (size_t i = 0; i < cmtTracker->points_active.size(); i++)
			circle(frame, cmtTracker->points_active[i], 2, Scalar(0, 255, 0));
		RotatedRect rect = cmtTracker->bb_rot;
		Point2f vertices[4];
		rect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(frame, vertices[i], vertices[(i+1)%4], Scalar(255, 0, 255));
	}	
}

//
void ALLTracks::processImage(cv::Mat &image, cv::Rect faceBox){
	switch (track_type)
	{
	case CAMSHIFT_TRACKER:
		camshiftTracking(image, faceBox);
		break;
	case CMT_TRACKER:
		break;
	case CT_TRACKER:
		break;
	case TLD_TRACKER:
		break;
	case COLOR_TRACKER:
		break;
	case STRUCK_TRACKER:
		break;
	default:
		break;
	}
}