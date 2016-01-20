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


// For Gaussian Blur Filter
int GBLUR_MAX_KERNEL_LEN = 51;
int blur_size = 3;
int max_blur_size = 61;
// For laplacian filter
int DDEPTH = CV_16S;// CV_16S;
int KERNEL_SIZE = 3;
int max_KernelSize = 15;// 2k+1
int SCALE = 1;
int DELTA = 0;
int max_Delta = 10;
int BORDER_TYPE = BORDER_DEFAULT;
// For combination of two images
double alpha = 0.50;
int alphaInt = 0;
int max_alpha = 20;
double beta;
double gamma = 0.9;
int gammaInt = 0;
int max_gamma = 20;
// For Edge detection
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

// Function Headers
void image1_processing();
void image2_processing();
void combineImages();
void Laplacian_blur_callback(int, void*);
void Laplacian_kernelsize_callback(int, void*);
void alpha_callback(int, void*);
void gamma_callback(int, void*);

int main()
{
   // Read in the images
   imgOrig2 = imread("../images/urkel_400x400.jpg", 1);
   imgOrig1 = imread("../images/2004-Clarkson-400x400.jpg", 1);
   //imgOrig1 = imread("../images/2016-Ford-Mustang-rear_640x480.jpg", 1);
   //imgOrig2 = imread("../images/modelT6_640x480.jpg", 1);

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

   createTrackbar("Blur Size", "img1", &blur_size, max_blur_size, Laplacian_blur_callback);
   createTrackbar("Kernel Size", "img2", &KERNEL_SIZE, max_KernelSize, Laplacian_kernelsize_callback);
   createTrackbar("Alpha", "Final Image", &alphaInt, max_alpha, alpha_callback);
   createTrackbar("Gamma:", "Final Image", &gammaInt, max_gamma, gamma_callback);

   waitKey(0);
   return 0;
}


// ===== Applying Gaussian Blur on image 1 =====
void image1_processing()
{
	//for (int i = 1; i < GBLUR_MAX_KERNEL_LEN; i = i + 2)
	//{
		//GaussianBlur(imgOrig1, imgLow, Size(i, i), 0, 0);
	//}
	GaussianBlur(imgOrig1, imgLow, Size(blur_size, blur_size), 0, 0); // size 45
	//cvtColor(imgLow, imgLow, CV_BGR2GRAY);  // Convert to Gray scale

	//imshow("original image 1", imgOrig1);   // Show the original image 1
	imshow("img1", imgLow);
}

// ===== Apply Laplacian filter to Image 2 =====
void image2_processing()
{
	
	// Convert the image 2 to gray
	//cvtColor(imgHigh, imgHigh_gray, CV_BGR2GRAY);
	// Gaussian blur filter
	GaussianBlur(imgOrig2, blurred, Size(7, 7), 0, 0, BORDER_DEFAULT);
	// Laplacian filter
	Laplacian(blurred, imgHigh, DDEPTH, KERNEL_SIZE, SCALE, DELTA, BORDER_TYPE);
	//addWeighted(imgOrig2, 2.0, sharpened, -1.25, 0, imgHigh);
	//blur(imgHigh, edges, Size(3, 3));
	//subtract(imgOrig2, blurred, imgHigh);
	// Edge detection
	//Canny(edges, edges, lowThreshold, lowThreshold*ratio, kernel_size);
	// Using Canny's output as a mask, we display our result
	//imgFinal = Scalar::all(0);

	//imgOrig2.copyTo(imgHigh, edges);

	//imshow("original image 2", imgOrig2);   // Show the original image 2

	//addWeighted(imgOrig2, .5, imgHigh, .5, 0, imgHigh);
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

void Laplacian_blur_callback(int, void*)
{
	blur_size = 2 * blur_size + 1;
	image1_processing();
}

void Laplacian_kernelsize_callback(int, void*)
{
	KERNEL_SIZE = 2 * KERNEL_SIZE + 1;
	image2_processing();
}

void alpha_callback(int, void*)
{
	alpha = alphaInt*0.15;
	combineImages();
}

void gamma_callback(int, void*)
{
	gamma = gammaInt*0.1;
	combineImages();
}