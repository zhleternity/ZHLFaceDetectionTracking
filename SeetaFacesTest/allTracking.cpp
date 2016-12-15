//#include "stdafx.h"
#include "allTracking.h"

void ALLTracks::trackInit(){
	beginInit = false;
	startTracking = false;
	track_type = CAMSHIFT_TRACKER;
	//Camshift *camshift;
}

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