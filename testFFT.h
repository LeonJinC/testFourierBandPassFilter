#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void FFT(Mat image, Mat &complexImg);
void complex2fftImage(Mat &complexImage, Mat &fftImage, int alpha);
void calculateSnP(Mat &Real, Mat &Imaginary);
void logImage(Mat input, Mat &output);
Mat getPaddedImage(Mat &image);
void getBandPass(Mat &padded, Mat &bandpass);
void getBandreject(Mat &padded, Mat &bandpass);