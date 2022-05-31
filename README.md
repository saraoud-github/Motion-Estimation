# Motion-Estimation

## Application: Motion Estimation

Motion is estimated by drawing the optical flow within two videos of moving cars [Traffic Video 1](https://www.youtube.com/watch?v=Y1jTEyb3wiI) - [Traffic Video 2](https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4) 
using three different approaches:
1. FAST keypoint detector algorithm with the pyramid implementation of the Lucas-Kanade (KLT) 
algorithm [Optical flow, OpenCV](https://docs.opencv.org/3.4.15/d4/dee/tutorial_optical_flow.html).
2. FAST keypoint detector with the FREAK keypoint descriptor (descriptor matching).
3. FAST keypoint detector with the LUCID keypoint descriptor (descriptor matching).

## Steps to run KLTOpticalFlow_Gray:

1. Open the Code:Blocks IDE
2. Go to File --> Open...
3. Select KLTOpticalFlow_Gray.cbp located inside the folder ("KLTOpticalFlow_Gray").
Note: "slow_traffic_small.mp4" was also used to compare results. To use it, change the VideoCapture object name in the code.
4. Press the "build and run" button.
5. A console screen and a windows will appear showing the optical flow obtained from the pyramid implementation of the KLT algorithm.
6. Exit the program by pressing ESC.

## Steps to run FAST_FREAK:

1. Open the Code:Blocks IDE
2. Go to File --> Open...
3. Select FAST_FREAK.cbp located inside the folder ("FAST_FREAK").
Note: "slow_traffic_small.mp4" was also used to compare results. To use it, change the VideoCapture object name in the code.
4. Press the "build and run" button.
5. A console screen and a windows will appear showing the optical flow obtained from using FAST + FREAK algorithms.
6. Exit the program by pressing ESC.

## Steps to run FAST_LUCID:

1. Open the Code:Blocks IDE
2. Go to File --> Open...
3. Select FAST_LUCID.cbp located inside the folder ("FAST_LUCID").
Note: "slow_traffic_small.mp4" was also used to compare results. To use it, change the VideoCapture object name in the code.
4. Press the "build and run" button.
5. A console screen and a windows will appear showing the optical flow obtained from using FAST + LUCID algorithms.
6. Exit the program by pressing ESC.

