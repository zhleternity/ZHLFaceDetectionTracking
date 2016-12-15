#include "Camshift/Camshift.h"
#include "CMT/CMT.h"
#include "TLD/TLD.h"



class ALLTracks
{
public:
	cv::Rect initBox;
	cv::Rect box;
	bool beginInit;
	bool startTracking;
	//camshift tracking
	Camshift *camshift;
	//CMT tracker
	cmt::CMT *cmtTracker;
	

	//main function
	void trackInit();
	void processImage(cv::Mat &image, cv::Rect faceBox);


private:
	typedef enum {
		CT_TRACKER,
		TLD_TRACKER,
		CMT_TRACKER,
		COLOR_TRACKER,
		CAMSHIFT_TRACKER,
		STRUCK_TRACKER,
	}TrackType;
	TrackType track_type;

	
	void camshiftTracking(cv::Mat &frame, cv::Rect box);
	void cmtTracking(cv::Mat &frame, cv::Rect box);



};

