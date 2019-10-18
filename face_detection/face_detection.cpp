#include "face_detection.hpp"
#include "../EventLibrary.hpp"

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
//#include "../include/Cv.h"

FaceDetection::FaceDetection() {
  //load cascade file for face detection
  //If the load function returns false, set a break point before this call 
  //and see the contents inside "haarcascade_face".  
  if(!cascade_face.load(haarcascade_face)){
    cerr << "ERROR: Could not load classifier cascade" << endl;
  } else {
      facePort.open("/facePort/out");
      event.addEvent<ImageOf<PixelRgb>*>("image right eye", [this](ImageOf<PixelRgb>* image){
        IplImage* iplImg = static_cast<IplImage*>(image->getIplImage());
        frame = iplImg;
        if( frame.empty() )
            return;
        if( iplImg->origin == IPL_ORIGIN_TL )
            frame.copyTo( frameCopy );
        else
            flip( frame, frameCopy, 0 );
        detectAndDraw(image, frameCopy, cascade_face, scale);
    });
    Network::connect("/facePort/out", "/test/out");
  }
}

void FaceDetection::detectAndDraw(ImageOf<PixelRgb>* point, Mat& img, CascadeClassifier& cascade, double scale) {
  int i = 0;
  double t = 0;
  double lookXPos, lookYPos;
  cv::vector<Rect> faces;
  const static Scalar colors[] =  { CV_RGB(0,0,255),
                                    CV_RGB(0,128,255),
                                    CV_RGB(0,255,255),
                                    CV_RGB(0,255,0),
                                    CV_RGB(255,128,0),
                                    CV_RGB(255,255,0),
                                    CV_RGB(255,0,0),
                                    CV_RGB(255,0,255)} ;
  
  // resizes and applies grey on imgae for methods work easier 

  Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
  
  cvtColor( img, gray, CV_BGR2GRAY );
  resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
  equalizeHist( smallImg, smallImg );
  
  t = (double)cvGetTickCount();
  cascade.detectMultiScale( smallImg, faces,
                            1.1, 2, 0
                              |CV_HAAR_FIND_BIGGEST_OBJECT
                              |CV_HAAR_DO_ROUGH_SEARCH
                              |CV_HAAR_SCALE_IMAGE
                              ,
                              Size(30, 30) );
  t = (double)cvGetTickCount() - t;
  // code will do a rough search and detect always the biggest face.
  
  //----------------
  cv::vector<cv::Vec3f> circles = detect_circle(img, &lookXPos, &lookYPos);

  //std::cout << circles.size() << " __ " << faces.size() << std::endl;

  if(circles.empty() && faces.empty()){
     lookXPos = 320 / 2;
     lookYPos = 240 / 2;
     event.fire("look at position", (int)lookXPos, (int)lookYPos); 
  }

  bool circleSure = false;
  for (auto pos : circles)
      if (pos[2] > 100.0)
        circleSure = true;

  if(circleSure){
    for (auto pos : circles) {
      if (pos[2] > 100.0) {
      std::cout << pos[2] << std::endl;
        circle( img, Point((int)pos[0], (int)pos[1]), 10, colors[0], 3, 8, 0 );
        event.fire("look at position", (int)pos[0], (int)pos[1]);
      }
    }
      
    if(!faces.empty()){
        event.fire("saved arm right", std::string("bronchade"));
        event.fire("saved arm left", std::string("bronchade"));
    }
  } else {
    //Draw Circle on the output image
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
      Mat smallImgROI;
      vector<Rect> nestedObjects;
      Point center;
      Scalar color = colors[i%8];
      int radius;
      center.x = cvRound((r->x + r->width*0.5)*scale);
      center.y = cvRound((r->y + r->height*0.5)*scale);
      radius = cvRound((r->width + r->height)*0.25*scale);
      circle( img, center, radius, color, 3, 8, 0 );
      
      event.fire("look at position", center.x, center.y);           

    }
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


cv::vector<cv::Vec3f> FaceDetection::detect_circle(Mat& image, double *xPos, double *yPos){
	cv::Mat img_gray;
//	cv::Mat src = yarp::cv::toCvMat(*image);
	cvtColor(image, img_gray, CV_BGR2GRAY);
	GaussianBlur( img_gray, img_gray, cv::Size(1, 1), 2, 2 );
	cv::vector<cv::Vec3f> circles;
	cv::HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT,1,img_gray.rows/8,100,50,0,0);
	if(!circles.empty()){
		*xPos = circles[0][0];
		*yPos = circles[0][1];
	}
  return circles;
}