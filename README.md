# Motion-Estimation

## Approaches:

This repository contains three approaches to estimate motion in a video:

1. FAST is coupled with KLT to obtain the optical flow within a video.
2. FAST is used to detect keypoints and then is coupled with FREAK, which detects the acquired keypoints’ descriptors, and then uses them for
descriptor matching
3. Approach 2 is repeated with FAST and LUCID to obtain the optical flow. 
