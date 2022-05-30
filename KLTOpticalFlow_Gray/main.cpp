#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
int main(int argc, char** argv){

    VideoCapture capture(samples::findFile("cars.mp4"));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for (int i = 0; i < 10500; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r, g, b));
    }
	//Create matrices to hold the two frames we are to compare + grayscale versions
    Mat rgb_old_frame, gray_old_frame, rgb_new_frame, gray_new_frame;

	//Creating 2 vectors to hold the keypoints of each of the 2 frames
    vector<KeyPoint> keypoints_1, keypoints_2;

	//Creat a vector of point2f for fast keypoints
	vector<Point2f> fastp0, fastp1;

	//Initializing our FAST feature detector
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create();

    //Taking the first frame & converting it to grayscale
    capture >> rgb_old_frame;
    cvtColor(rgb_old_frame, gray_old_frame, COLOR_BGR2GRAY);

	//Detecting key features and storing them in keypoints_1
	detector -> detect(gray_old_frame, keypoints_1);

	//Creating an empty mask of size eqaul to our first frame (video window size)
	Mat mask = Mat::zeros(rgb_old_frame.size(), rgb_old_frame.type());

    //Storing keypoints as point2f in vector
    for (uint i = 0; i < keypoints_1.size(); i++) {
        fastp0.push_back(keypoints_1[i].pt);
    }

    while (true) {
        capture >> rgb_new_frame;
        if (rgb_new_frame.empty())
            break;
        cvtColor(rgb_new_frame, gray_new_frame, COLOR_BGR2GRAY);

        // calculate optical flow using KLT built in method
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(gray_old_frame, gray_new_frame, fastp0, fastp1, status, err, Size(15, 15), 2, criteria);

		vector<Point2f> good_new;

		//All points who are good enough will be stored in good_new
        for (uint i = 0; i < fastp0.size(); i++)
        {
            // Select good points
            if (status[i] == 1) {
                good_new.push_back(fastp1[i]);
                // Draw the optical flow
                line(mask, fastp1[i], fastp0[i], colors[i], 2);
                circle(rgb_new_frame, fastp1[i], 5, colors[i], -1);
            }
        }
        Mat img;
        add(rgb_new_frame, mask, img);
        imshow("Frame", img);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        gray_old_frame = gray_new_frame.clone();
        fastp0 = good_new;

    }
    }
