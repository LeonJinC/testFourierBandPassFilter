#include"testFFT.h"

int main()
{
	Mat temp;
	Mat oriImage = imread("peace.jpg", 1);//原始图像
	Mat grayImage;//转灰度图
	cvtColor(oriImage, grayImage, CV_RGB2GRAY);
	imshow("grayImage", grayImage);

	/*傅里叶变换*/
	Mat complexImage;
	FFT(grayImage, complexImage);

	/*complex2fftImage复数图像 转 频谱图*/
	Mat fftImage;
	complex2fftImage(complexImage, fftImage, 1);
	imshow("fftImage", fftImage);
	fftImage.convertTo(temp, CV_8UC1,255.0);
	imwrite("fftImage.jpg", temp);

	/*getBandPass获取带通滤波器*/
	Mat padded = getPaddedImage(grayImage);
	Mat bandpass(padded.size(), CV_32FC2);//两通道
	getBandPass(padded, bandpass);

	Mat bandpassImagefft;
	complex2fftImage(bandpass, bandpassImagefft, 1);
	//imshow("bandrejectfftImage", bandpassImagefft);
	
	bandpassImagefft.convertTo(temp, CV_8UC1, 255.0);
	imwrite("bandpassfftImage.jpg", temp);

	Mat	convolImage;
	multiply(complexImage, bandpass, convolImage);

	Mat convolImagefft;
	complex2fftImage(convolImage, convolImagefft, 1);
	imshow("convolImagefft", convolImagefft);
	convolImagefft.convertTo(temp, CV_8UC1, 255.0);
	imwrite("convolImagefft.jpg", temp);

	Mat shiftedfilteredImage;
	idft(convolImage, shiftedfilteredImage);

	Mat bandpass_ed_Image;//带阻滤波处理后的图像
	complex2fftImage(shiftedfilteredImage, bandpass_ed_Image, 0);
	bandpass_ed_Image = bandpass_ed_Image(Rect(0, 0, grayImage.cols, grayImage.rows));
	imshow("bandrejectedImage", bandpass_ed_Image);
	bandpass_ed_Image.convertTo(bandpass_ed_Image, CV_8UC1, 255.0);
	imwrite("bandpass_ed_Image.jpg", bandpass_ed_Image);



	waitKey(0);

	return 0;
}