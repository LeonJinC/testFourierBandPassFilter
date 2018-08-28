#include"testFFT.h"

void FFT(Mat image, Mat &complexImg) {
	/*
	图像获取复数
	Mat complexImg;
	FFT(invertedImage, complexImg);
	*/

	int w = getOptimalDFTSize(image.cols);
	int h = getOptimalDFTSize(image.rows);
	Mat padded;
	copyMakeBorder(image, padded, 0, h - image.rows, 0, w - image.cols, BORDER_CONSTANT, Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);
	//imshow("padded", padded);
	for (int i = 0; i < padded.rows; i++)
	{
		float *ptr = padded.ptr<float>(i);
		for (int j = 0; j < padded.cols; j++)
		{
			ptr[j] *= pow(-1, i + j);
		}
	}
	Mat plane[] = { padded, Mat::zeros(padded.size(), CV_32F) };

	merge(plane, 2, complexImg);
	dft(complexImg, complexImg);

}
void complex2fftImage(Mat &complexImage, Mat &fftImage, int alpha)
{
	/*

	//Mat fftImage;
	//complex2fftImage(complexImage, fftImage,1);

	//Mat bandrejectfftImage;
	//complex2fftImage(bandreject, bandrejectfftImage,1);

	//Mat convolfftImage;
	//complex2fftImage(convolImage, convolfftImage, 1);

	Mat bandrejectedImage;
	complex2fftImage(shiftedfilteredImage, bandrejectedImage, 0);

	*/
	Mat padded = getPaddedImage(complexImage);
	Mat plane[] = { padded, Mat::zeros(padded.size(), CV_32F) };

	split(complexImage, plane);
	calculateSnP(plane[0], plane[1]);
	if (alpha == 1)
	{
		//log(x+1)+normalize
		logImage(plane[0], plane[0]);
	}
	else
	{
		normalize(plane[0], plane[0], 1, 0, CV_MINMAX);
	}

	fftImage = plane[0];
}


void calculateSnP(Mat &Real, Mat &Imaginary)
{ /*
  实数和虚数转化为 未归一化和log化的频谱和相位谱
  */
	magnitude(Real, Imaginary, Real);
	phase(Real, Imaginary, Imaginary);
}

void logImage(Mat input, Mat &output)
{
	/*
	log处理
	归一化处理
	*/
	input += Scalar::all(1);
	log(input, input);
	normalize(input, output, 1, 0, CV_MINMAX);

}

Mat getPaddedImage(Mat &image)
{
	/*
	Mat padded = getPaddedImage(invertedImage);
	Mat plane[] = { padded, Mat::zeros(padded.size(), CV_32F) };


	*/
	int w = getOptimalDFTSize(image.cols);
	int h = getOptimalDFTSize(image.rows);
	Mat padded;
	copyMakeBorder(image, padded, 0, h - image.rows, 0, w - image.cols, BORDER_CONSTANT, Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);
	return padded;
}

void getBandPass(Mat &padded, Mat &bandpass)
{

	/*
	Mat padded = getPaddedImage(invertedImage);
	Mat bandpass(padded.size(), CV_32FC2);//两通道
	*/
	float D0 = 50;
	float W = 20;
	for (int i = 0; i < padded.rows; i++)
	{
		float*p = bandpass.ptr<float>(i);

		for (int j = 0; j < padded.cols; j++)
		{
			float D_pow = pow(i - padded.rows / 2, 2) + pow(j - padded.cols / 2, 2);
			float D_sqrt = sqrtf(D_pow);
			float D0_pow = pow(D0, 2);
			p[2 * j] = expf(-pow(((D_pow - D0_pow) / (D_sqrt*W)), 2));
			p[2 * j + 1] = expf(-pow(((D_pow - D0_pow) / (D_sqrt*W)), 2));
		}
	}

}

void getBandreject(Mat &padded, Mat &bandpass)
{

	/*
	Mat padded = getPaddedImage(invertedImage);
	Mat bandreject(padded.size(), CV_32FC2);//两通道
	*/
	float D0 = 3;
	float W = 10;
	for (int i = 0; i < padded.rows; i++)
	{
		float*p = bandpass.ptr<float>(i);

		for (int j = 0; j < padded.cols; j++)
		{
			float D_pow = pow(i - padded.rows / 2, 2) + pow(j - padded.cols / 2, 2);
			float D_sqrt = sqrtf(D_pow);
			float D0_pow = pow(D0, 2);
			p[2 * j] = 1 - expf(-pow(((D_pow - D0_pow) / (D_sqrt*W)), 2));
			p[2 * j + 1] = 1 - expf(-pow(((D_pow - D0_pow) / (D_sqrt*W)), 2));
		}
	}

}
