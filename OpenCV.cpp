//#include "OpenCV.h"
//
//double WienerFilterImpl(const cv::Mat& src, cv::Mat& dst, double noiseVariance, const cv::Size& block) {
//	assert(("Invalid block dimensions", block.width % 2 == 1 && block.height % 2 == 1 && block.width > 1 && block.height > 1));
//	assert(("src and dst must be one channel grayscale images", src.channels() == 1, dst.channels() == 1));
//
//	int h = src.rows;
//	int w = src.cols;
//
//	dst = cv::Mat1b(h, w);
//
//	cv::Mat1d means, sqrMeans, variances;
//	cv::Mat1d avgVarianceMat;
//
//	boxFilter(src, means, CV_64F, block, cv::Point(-1, -1), true, cv::BORDER_REPLICATE);
//	sqrBoxFilter(src, sqrMeans, CV_64F, block, cv::Point(-1, -1), true, cv::BORDER_REPLICATE);
//	cv::imshow("means", dst);
//	cv::waitKey(0);
//	cv::Mat1d means2 = means.mul(means);
//	variances = sqrMeans - (means.mul(means));
//
//	if (noiseVariance < 0) {
//		reduce(variances, avgVarianceMat, 1, cv::REDUCE_SUM, -1);
//		reduce(avgVarianceMat, avgVarianceMat, 0, cv::REDUCE_SUM, -1);
//		noiseVariance = avgVarianceMat(0, 0) / (h*w);
//	}
//
//	for (int r = 0; r < h; ++r) {
//		uchar const * const srcRow = src.ptr<uchar>(r);
//		uchar * const dstRow = dst.ptr<uchar>(r);
//		double * const varRow = variances.ptr<double>(r);
//		double * const meanRow = means.ptr<double>(r);
//		for (int c = 0; c < w; ++c) {
//			dstRow[c] = (uchar)(meanRow[c] + cv::max(0., varRow[c] - noiseVariance) / cv::max(varRow[c], noiseVariance) * (srcRow[c] - meanRow[c])
//				);
//		}
//	}
//
//	return noiseVariance;
//}
//void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg)
//{
//	outputImg = inputImg(cv::Rect(0, 0, inputImg.cols & -2, inputImg.rows & -2)).clone();
//	int cx = outputImg.cols / 2;
//	int cy = outputImg.rows / 2;
//	cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
//	cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
//	cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
//	cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
//	cv::Mat tmp;
//	q0.copyTo(tmp);
//	q3.copyTo(q0);
//	tmp.copyTo(q3);
//	q1.copyTo(tmp);
//	q2.copyTo(q1);
//	tmp.copyTo(q2);
//}
//void calcPSF(cv::Mat& outputImg, cv::Size filterSize, int R)
//{
//	cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
//	cv::Point point(filterSize.width / 2, filterSize.height / 2);
//	cv::circle(h, point, R, 255, -1, 8);
//	cv::Scalar summa = sum(h);
//	outputImg = h / summa[0];
//}
//void CalcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr)
//{
//	cv::Mat Hw, h;
//	cv::Rect roi = cv::Rect(0, 0, input_h_PSF.cols & -2, input_h_PSF.rows & -2);
//	calcPSF(h, roi.size(), 50);
//
//	cv::Mat h_PSF_shifted;
//
//	fftshift(h, h_PSF_shifted);
//
//	cv::Mat zeroImage(h_PSF_shifted.size(), CV_32F);
//	cv::Mat planes[] = { cv::Mat_<float>(h_PSF_shifted), zeroImage };
//	cv::Mat complexI;
//	cv::merge(planes, 2, complexI);
//	cv::dft(complexI, complexI);
//	cv::split(complexI, planes);
//	cv::Mat denom;
//	pow(abs(planes[0]), 2, denom);
//	denom += nsr;
//	divide(planes[0], denom, output_G);
//	output_G.convertTo(output_G, CV_8U);
//	cv::normalize(output_G, output_G, 0, 255, cv::NORM_MINMAX);
//}
//
//void WienerFilter(const cv::Mat& src, cv::Mat& dst, double noiseVariance, const cv::Size& block) {
//	WienerFilterImpl(src, dst, noiseVariance, block);
//	return;
//}
//
//
//double WienerFilter(const cv::Mat& src, cv::Mat& dst, const cv::Size& block) {
//	return WienerFilterImpl(src, dst, -1, block);
//}
//OpenCV::OpenCV(char* image, size_t width, size_t height)
//{
//	cv::Mat _image(height, width, CV_8UC4);
//	cv::Mat fImage;
//	cv::Mat fourierTransform;
//	cv::Mat inverseTransform;
//	cv::Mat1b dst3x3, dst5x5, dst5x5_fixedNoise;
//	memcpy(_image.data, image, width*height * 4);
//	cv::cvtColor(_image, _image, cv::COLOR_RGBA2GRAY);
//	double estimatedNoiseVariance = WienerFilter(_image, dst3x3, cv::Size(3, 3));
//	estimatedNoiseVariance = WienerFilter(_image, dst5x5, cv::Size(5, 5));
//	WienerFilter(_image, dst5x5_fixedNoise, 5000, cv::Size(5, 5));
//	estimatedNoiseVariance = WienerFilter(_image, dst3x3, cv::Size(3, 3));
//	WienerFilter(_image, _image, 1, cv::Size(5, 5));
//
//	imwrite("Filtered3X3.png", dst3x3);
//	imwrite("Filtered5X5.png", dst5x5);
//	imwrite("Filtered5X5_fixedNoise.png", dst5x5_fixedNoise);
//	cv::waitKey(0);
//}
//
//OpenCV::~OpenCV()
//{
//}