//#include <opencv\cv.h>   // This is the original code, but I couldn't get VideoCapture work correctly.
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui_c.h>

using namespace cv;
using namespace std;

// Global Vars
char window_name[] = "CV Project 1";
Mat imgOrig1;
Mat imgLow;   // Low-pass (Blkurring) filter image
Mat imgOrig2;
Mat imgHigh;   // High-pass (Sharpening) filter image

// For Gaussian Blur Filter
const int GBLUR_MAX_KERNEL_LEN = 31;
const int DELAY_BLUR = 100;
const int DELAY_CAPTION = 1500;
// For laplacian filter
const int DEPTH = 1;
const int KSIZE = 1;
const double SCALE = 1.0;
const double DELTA = 0;
const int BORDER_TYPE = 4;

// Function Headers
int display_filtered_img(int delay, Mat img);
int display_caption(char* caption);


int main()
{
   // Read in the images
   imgOrig1 = imread("../images/urkel_400x400.jpg", 1);
   imgOrig2 = imread("../images/MonroeMarilyn_400x400.jpg", 1);

   // Copy the images to filter
   imgLow = imgOrig1.clone();
   imgHigh = imgOrig2.clone();

   // Applying Gaussian Blur on image 1
   for (int i = 0; i < GBLUR_MAX_KERNEL_LEN; i = i + 2)
   {
	   GaussianBlur(imgOrig1, imgLow, Size(i, i), 0, 0);
	   int result = display_filtered_img(DELAY_BLUR, imgLow);
       if (result != 0) { return 0; }
   }

   // Apply Laplacian filter to Image 2
   Laplacian(imgOrig2, imgHigh, DEPTH, KSIZE, SCALE, DELTA, BORDER_TYPE);
   int result = display_filtered_img(DELAY_BLUR, imgHigh);
   if (result != 0) { return 0; }

   // Combine the two images
}

// To Display image in window
int display_filtered_img(int delay, Mat img)
{
	imshow(window_name, img);
	int c = waitKey(delay);
	if (c >= 0)
	{
		return -1;
	}
	return 0;
}

// For displaying the caption on the window
int display_caption(char* caption)
{
	imgHigh = Mat::zeros(imgOrig2.size(), imgOrig2.type());
	putText(imgHigh, caption,
		Point(imgOrig2.cols / 4, imgOrig2.rows / 2),
		CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

	imshow(window_name, imgHigh);
	int c = waitKey(DELAY_CAPTION);
	if (c >= 0) { return -1; }
	return 0;
}