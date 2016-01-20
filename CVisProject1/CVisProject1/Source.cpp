//#include <opencv\cv.h>   // This is the original code, but I couldn't get VideoCapture work correctly.
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui_c.h>

using namespace cv;
using namespace std;

// Global Vars
Mat imgOrig1;
Mat imgLow;   // Low-pass (Blkurring) filter image
Mat imgOrig2;
Mat imgHigh;   // High-pass (Sharpening) filter image
Mat imgHigh_gray;
Mat imgFinal;
Mat blurred;
Mat blurred_orig;


// For Gaussian Blur Filter
int GBLUR_MAX_KERNEL_LEN = 51;
int blur_size = 3;
int blur_size_indx = 20;
int max_blur_size = 100;
int blur_size_img2 = 3;
int blur_size_img2_indx = 20;
int max_blur_size_img2 = 99;
// For combination of two images
double alpha = 0.50;
int alphaInt = 0;
int max_alpha = 10;
double beta;
double gamma = 0.9;
int gammaInt = 0;
int max_gamma = 20;
double sigma = 0.9;
int sigmaInt = 0;
int max_sigma = 20;

// Function Headers
void image1_processing();
void image2_processing();
void combineImages();
void img1_blur_callback(int, void*);
void img2_blur_callback(int, void*);
void alpha_callback(int, void*);
void gamma_callback(int, void*);
void sigma_callback(int, void*);

int main()
{
   // Read in the images
   //imgOrig2 = imread("../images/urkel_400x400.jpg", 1);
   //imgOrig1 = imread("../images/2004-Clarkson-400x400.jpg", 1);
   imgOrig1 = imread("../images/elephant_640x480.jpg", 1);
   imgOrig2 = imread("../images/dog_640x480.jpg", 1);

   if (!imgOrig1.data || !imgOrig2.data) { printf("Error loading src1 \n"); return -1; }

   // Copy the images to filter
   imgLow = imgOrig1.clone();
   imgHigh = imgOrig2.clone();
   imgLow.convertTo(imgLow, CV_32F);
   imgHigh.convertTo(imgHigh, CV_32F);

   // Process the images
   image1_processing();
   image2_processing();
   combineImages();

   createTrackbar("Blur Size", "img1", &blur_size_indx, max_blur_size, img1_blur_callback);
   createTrackbar("Blur Size", "img2", &blur_size_img2_indx, max_blur_size_img2, img2_blur_callback);
   createTrackbar("Alpha", "Final Image", &alphaInt, max_alpha, alpha_callback);
   createTrackbar("Gamma", "Final Image", &gammaInt, max_gamma, gamma_callback);
   createTrackbar("Sigma", "img2", &sigmaInt, max_sigma, sigma_callback);

   waitKey(0);
   return 0;
}


// ===== Applying Gaussian Blur on image 1 =====
void image1_processing()
{
	GaussianBlur(imgOrig1, imgLow, Size(blur_size, blur_size), 0, 0); // size 45
	cout << "Image 1 === > Blur Size: " << blur_size << "\n";
	imshow("img1", imgLow);
}

// ===== Apply Sharpening filter to Image 2 =====
void image2_processing()
{
	Mat bw_img2 = imgOrig2.clone();

	// Convert to gray scale and back
	cvtColor(bw_img2, bw_img2, CV_BGR2GRAY);
	cvtColor(bw_img2, bw_img2, CV_GRAY2BGR);

	GaussianBlur(bw_img2, blurred, Size(blur_size_img2, blur_size_img2), sigma, sigma, BORDER_DEFAULT);
	addWeighted(bw_img2, 1, blurred, -0.75, 0, imgHigh);

	cout << "Image 2 === > Blur Size: " << blur_size_img2 << "  Sigma: " << sigma << "\n";

	imshow("img2", imgHigh);   // Show the final filtered image
}

// Combine the two images
void combineImages()
{
	//namedWindow("Final Image", 1);
	beta = (1.0 - alpha);
	addWeighted(imgLow, alpha, imgHigh, beta, gamma, imgFinal);
	imshow("Final Image", imgFinal);
}

void img1_blur_callback(int, void*)
{
	blur_size = 2 * blur_size_indx + 1;
	image1_processing();
	combineImages();
}

void img2_blur_callback(int, void*)
{
	blur_size_img2 = 2 * blur_size_img2_indx + 1;
	image2_processing();
	combineImages();
}

void alpha_callback(int, void*)
{
	alpha = alphaInt*0.1;
	combineImages();
}

void gamma_callback(int, void*)
{
	gamma = gammaInt*1.75;
	combineImages();
}

void sigma_callback(int, void*)
{
	sigma = sigmaInt*-0.15;
	image2_processing();
	combineImages();
}