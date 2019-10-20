#include "face_detection.hpp"
#include "../EventLibrary.hpp"

#include <yarp/os/all.h>
#include <yarp/sig/all.h>

FaceDetection::FaceDetection() {
	//load cascade file for face detection
	//If the load function returns false, set a break point before this call
	//and see the contents inside "haarcascade_face".
	if (!cascade_face.load(haarcascade_face)) {
		cerr << "ERROR: Could not load classifier cascade" << endl;
	} else {
		facePort.open("/facePort/out");
		event.addEvent<ImageOf<PixelRgb> *>("image right eye", [this](ImageOf<PixelRgb> *image) {
			IplImage *iplImg = static_cast<IplImage *>(image->getIplImage());
			frame = iplImg;
			if (frame.empty())
				return;
			if (iplImg->origin == IPL_ORIGIN_TL)
				frame.copyTo(frameCopy);
			else
				flip(frame, frameCopy, 0);
			detectAndDraw(image, frameCopy, cascade_face, scale);
		});
		Network::connect("/facePort/out", "/test/out");
	}
}

void FaceDetection::detectAndDraw(ImageOf<PixelRgb> *point, Mat &img, CascadeClassifier &cascade, double scale) {
	int i = 0;
	double t = 0;
	double lookXPos, lookYPos;
	cv::vector<Rect> faces;
	const static Scalar colors[] = {CV_RGB(0, 0, 255),
					CV_RGB(0, 128, 255),
					CV_RGB(0, 255, 255),
					CV_RGB(0, 255, 0),
					CV_RGB(255, 128, 0),
					CV_RGB(255, 255, 0),
					CV_RGB(255, 0, 0),
					CV_RGB(255, 0, 255)};

	// resizes and applies grey on image for methods work easier

	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);

	cvtColor(img, gray, CV_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	t = (double) cvGetTickCount();
	cascade.detectMultiScale(smallImg, faces,
				 1.1, 2, 0
					 | CV_HAAR_FIND_BIGGEST_OBJECT
					 | CV_HAAR_DO_ROUGH_SEARCH
					 | CV_HAAR_SCALE_IMAGE,
				 Size(30, 30));
	t = (double) cvGetTickCount() - t;
	// code will do a rough search and detect always the biggest face.

	//----------------
	cv::vector<cv::Vec3f> circles = detect_circle(img, &lookXPos, &lookYPos);

	//std::cout << circles.size() << " __ " << faces.size() << std::endl;

	/**
		Here is the state machine that determine which movement is done depending of the detections
		Author : Pierre-Vincent (for the logical part)
			 (for the movement call part and the drawing part)
	**/
	//State 1 : if there is no face and no circle --look in front of it
	if (circles.empty() && faces.empty()) {
		lookXPos = 320 / 2;
		lookYPos = 240 / 2;
		event.fire("look at position", (int) lookXPos, (int) lookYPos);
	}
	//State 2 : If a face is detected -- draw a circle for each face detected and look at the last face detected
	else if (!faces.empty()) {
		//Draw Circle on the output image
		Point center;
		for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++) {
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Scalar color = colors[i % 8];
			int radius;
			center.x = cvRound((r->x + r->width * 0.5) * scale);
			center.y = cvRound((r->y + r->height * 0.5) * scale);
			radius = cvRound((r->width + r->height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		event.fire("look at position", center.x, center.y);
		event.fire("saved arm left", std::string("wave"));
	//State 3 : if there is a face and a circle detected -- draw a circle around each circle center and do a specific movement
		if (!circles.empty()) {
			event.fire("saved arm right", std::string("bronchade"));
			event.fire("saved arm left", std::string("bronchade"));
			for (auto pos : circles) {
				if (pos[2] > 1.0) {
					circle(img, Point((int) pos[0], (int) pos[1]), 10, colors[0], 3, 8, 0);
				}
			}
		}
	}
	//State 4 : if there is noface but circle(s) detected -- draw circle around each center and look at the last circle detected
	else if (!circles.empty()) {
		Point center;
		for (auto pos : circles) {
			if (pos[2] > 1.0) {
				circle(img, Point((int) pos[0], (int) pos[1]), 10, colors[0], 3, 8, 0);
				center.x = pos[0];
				center.y = pos[1];
			}
		}
		event.fire("look at position", (int) center.x, (int) center.y);
		event.fire("saved arm left", std::string("catch"));
		event.fire("saved arm right", std::string("catch"));
	}
	ImageOf<PixelRgb> &tmp = facePort.prepare();
	for (int x = 0; x < 320; ++x) {
		for (int y = 0; y < 240; ++y) {
			point->pixel(x, y).b = img.at<Vec3b>(y, x)[0];
			point->pixel(x, y).g = img.at<Vec3b>(y, x)[1];
			point->pixel(x, y).r = img.at<Vec3b>(y, x)[2];
		}
	}
	tmp = *point;
	facePort.write();
}

/**
	Function of circle detection
	Author : Pierre-Vincent Gouel
	Parameters : image -the image to analyse // *xPos - pointer to a X position (optional) // *yPos - pointer to a Y position (optional)
	Return : a vector of 3 dimensionals vectors ;; each represent a circle with X of the center, Y of the center and radius

	This function apply 2 filters on the image in order to make the detection possible and optimised.
	Then call for the openCV function to detect circles based on the Hough algorithm
**/
cv::vector<cv::Vec3f> FaceDetection::detect_circle(Mat &image, double *xPos, double *yPos) {
	cv::Mat img_gray;  // Declaration of an image which will be the gray image
	cvtColor(image, img_gray, CV_BGR2GRAY); //Creation of the gray version of the image
	GaussianBlur(img_gray, img_gray, cv::Size(1, 1), 2,
		     2); //Blur the image to make the detection more easy (because in reality a circle can never be a mathematical perfect circle)
	cv::vector<cv::Vec3f> circles; //Creation of the vector in which every circles will be stored
	cv::HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, img_gray.rows / 8, 100, 50, 0,
			 0); //Detection of the circle from the filtered image 130 and 65 can be adjusted (too big no detection, to small too much detection
	if (!circles.empty()) { //modify the center of thefirst detected circle (not used anymore)
		*xPos = circles[0][0];
		*yPos = circles[0][1];
	}
	return circles;
}