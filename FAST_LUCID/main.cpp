#include <iostream>
#include <string>
#include <vector>
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

int main(int argc, char** argv) {

    VideoCapture capture(samples::findFile("cars.mp4"));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }

    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for (int i = 0; i < 300; i++)
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

	//Creating 2 matrices to hold our descriptors
	Mat desc1, desc2;

    //Initializing our FAST feature detector
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(105, false, FastFeatureDetector::TYPE_9_16);


    // Initializing our LUCID feature descriptor
    Ptr<LUCID> descriptor = LUCID::create(10,10);


    // Taking the first frame & converting it to grayscale
    capture >> rgb_old_frame;
    cvtColor(rgb_old_frame, gray_old_frame, COLOR_BGR2GRAY);

	//Detecting key features and storing them in keypoints_1
	detector -> detect(gray_old_frame, keypoints_1);

	//Filter out some excess keypoints that are not as significant
	KeyPointsFilter::retainBest(keypoints_1, 500);

	//Using our descriptor to get feature descriptors
	descriptor -> compute(rgb_old_frame, keypoints_1, desc1);

	//Create DescriptorMatcher
	FlannBasedMatcher matcher;

	//Create a vector of DMatches to hold the result of the matcher
	vector<DMatch> matches;

	//Creating an empty mask of size eqaul to our first frame (video window size)
	Mat mask = Mat::zeros(rgb_old_frame.size(), rgb_old_frame.type());

	//Loop to go through the entire video and process it
    while (true) {

		//Create 2 vectors of matched points between 2 frames
        std::vector<Point2f> matched_point1, matched_point2;

        capture >> rgb_new_frame;
		if (rgb_new_frame.empty())
            break;
        cvtColor(rgb_new_frame, gray_new_frame, COLOR_BGR2GRAY);


        detector->detect(gray_new_frame, keypoints_2);
        KeyPointsFilter::retainBest(keypoints_2, 500);
        descriptor->compute(rgb_new_frame, keypoints_2, desc2);


		//converting the type of descriptors to CV_32F
		desc1.convertTo(desc1, CV_32F);
		desc2.convertTo(desc2, CV_32F);


        matcher.match(desc1, desc2, matches);

        double min_dist = 1000;

		for (uint i = 0; i < desc1.rows; i++){
			double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
		}

		//Creating vector of DMatches to hold the ones we want to retain
		std::vector< DMatch > good_matches;
		for(uint i = 0; i <desc1.rows; i++)
			if (matches[i].distance < max(4*min_dist, 3.9) )
				good_matches.push_back(matches[i]);


        for (uint i = 0; i < good_matches.size(); i++) {

            // this is how the DMatch structure stores the matching information
            int idx2 = good_matches[i].trainIdx;
            int idx1 = good_matches[i].queryIdx;

            //now use those match indices to get the keypoints, add to your two lists of points
            matched_point1.push_back(keypoints_1[idx1].pt);
            matched_point2.push_back(keypoints_2[idx2].pt);
        }


        for (uint i = 0; i < matched_point1.size(); i++){

			//circle(rgb_new_frame, matched_point2[i], 5, colors[i], -1);
			arrowedLine(mask,matched_point1[i],matched_point2[i],colors[i],1,8,0,0.5);

        }
        Mat img;
        add(rgb_new_frame, mask, img);
        imshow("Frame", img);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        // Now update the previous frame and previous points
        gray_old_frame = gray_new_frame.clone();
        keypoints_1 = keypoints_2;
		desc1 = desc2;

		//waitKey(fps);
    }

    }

