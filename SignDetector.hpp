#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

enum Sign
{
	SIGN_LEFT = 1,
	SIGN_RIGHT = 2,
	SIGN_PARKING = 3,
	SIGN_STOP = 4,
	SIGN_PEDESTRIAN = 5,
	SIGN_GIRL = 6
};

class SignDetector
{
	private:
		CascadeClassifier leftSignCascade;
		CascadeClassifier rightSignCascade;
		CascadeClassifier parkingSignCascade;
		CascadeClassifier stopSignCascade;
		CascadeClassifier pedestrianSignCascade;
//		CascadeClassifier girlSignCascade;
		bool isSignDetected(Mat image, CascadeClassifier classifier);
		
	public:
		SignDetector();
		vector<int> detectSign(Mat image);
};
