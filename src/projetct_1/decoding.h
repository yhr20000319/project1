#pragma once
#include <iostream>
#include<fstream>
#include<cstring>
#include "opencv.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
void decoding();
//int FindQrPoint(cv::Mat& srcImg, vector<vector<cv::Point>>& qrPoint);
int FindQrPoint(cv::Mat& srcImg, vector<vector<cv::Point>>& qrPoint, cv::Point2f adPoint[], cv::Point2f getPoint[]);
void adjusting(vector<vector<cv::Point>> qrPoint, cv::Point adPoint[]);
bool IsQrPoint(vector<cv::Point>& contour, cv::Mat& img, int i);
cv::Mat transformCorner(cv::Mat src, cv::RotatedRect rect);
bool IsQrColorRate(cv::Mat& image, int flag);
bool IsQrColorRateX(cv::Mat& image, int flag);
bool IsQrColorRateY(cv::Mat& image, int flag);
bool IsQrRate(float rate);
void respective(cv::Mat src, cv::Mat &dst, cv::Point2f getPoint[]);
void thresd(cv::Mat& src);
void decode(cv::Mat src, ofstream& file_out);