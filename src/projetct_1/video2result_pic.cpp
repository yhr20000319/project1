#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<vector>

using namespace std;

cv::Mat  gaycolor(cv::Mat& inputimage, int flag)
{
	if (flag == 3)//三通道图像的获取方法
	{

		for (int i = 0; i < inputimage.rows; i++)
		{
			for (int j = 0; j < inputimage.cols; j++)
			{
				inputimage.at<cv::Vec3b>(i, j)[0] = (inputimage.at<cv::Vec3b>(i, j)[0] * 30 + inputimage.at<cv::Vec3b>(i, j)[1] * 59 + inputimage.at<cv::Vec3b>(i, j)[2] * 11) / 100;
				inputimage.at<cv::Vec3b>(i, j)[1] = (inputimage.at<cv::Vec3b>(i, j)[0] * 30 + inputimage.at<cv::Vec3b>(i, j)[1] * 59 + inputimage.at<cv::Vec3b>(i, j)[2] * 11) / 100;
				inputimage.at<cv::Vec3b>(i, j)[2] = (inputimage.at<cv::Vec3b>(i, j)[0] * 30 + inputimage.at<cv::Vec3b>(i, j)[1] * 59 + inputimage.at<cv::Vec3b>(i, j)[2] * 11) / 100;
			}
		}
		return inputimage;
	}
	else if (flag == 1)//单通道图像的获取方法
	{
		cv::Mat outputimage(inputimage.rows, inputimage.cols, CV_8UC1);
		for (int i = 0; i < inputimage.rows; i++)
		{
			for (int j = 0; j < inputimage.cols; j++)
			{
				outputimage.at<uchar>(i, j) = (inputimage.at<cv::Vec3b>(i, j)[0] * 30 + inputimage.at<cv::Vec3b>(i, j)[1] * 59 + inputimage.at<cv::Vec3b>(i, j)[2] * 11) / 100;
			}
		}
		return outputimage;
	}
}

void video2pic()
{
	// 获取视频文件
	cv::VideoCapture cap(".\\video\\tmp.mp4");//这里是获取视频的地方
	// 获取视频总帧数
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total frames: " << totalFrameNumber << endl;

	//通过监测每一帧最中间一行，中间部分列的像素值来判断是有效帧还是无效帧
	cv::Mat frame;
	long currentFrame = 0;
	int flag = 0;	//判断是不是开始帧
	while (flag == 0)
	{
		cap.read(frame);//取视频的一帧
		frame = gaycolor(frame, 1);

		int half_row = frame.rows / 2;	//帧图像的正中间一行
		int half_col = frame.cols / 2;		//帧图像的正中间一列

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //不是开始帧
			{
				flag = 0;
				break;
			}
		}
		currentFrame++;
		cout << "第" << currentFrame << "帧丢弃" << endl;
	}
	cout << "第" << currentFrame << "帧是开始帧" << endl;
	while (flag == 1)	//是开始帧还不是数据帧
	{
		cap.read(frame);//取视频的一帧

		frame = gaycolor(frame, 1);
		int half_row = frame.rows / 2;	//帧图像的正中间一行
		int half_col = frame.cols / 2;		//帧图像的正中间一列

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //不是开始帧
			{
				flag = 0;
				break;
			}
		}
		currentFrame++;
		cout << "第" << currentFrame << "帧丢弃" << endl;
	}
	cout << "第" << currentFrame << "帧是第一数据帧" << endl;
	int jiange = 3; int count = 0;
	while (flag == 0)	//是数据帧不是结束帧
	{
		cv::Mat k;
		cap.read(frame);//取视频的一帧
		k = frame;
		frame = gaycolor(frame, 1);
		int half_row = frame.rows / 2;	//帧图像的正中间一行
		int half_col = frame.cols / 2;		//帧图像的正中间一列

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //不是结束帧
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
		else
		{
			if (jiange % 3 == 0)
			{
				stringstream str;
				str << "pic" << count << ".jpg";
				cout << "正在处理第" << currentFrame << "帧" << endl;
				cv::imwrite(".\\result_pic\\" + str.str(), k);
				count++;
			}
		}
		jiange++;
		currentFrame++;
	}
	cout << "第" << currentFrame << "帧是结束帧" << endl;
	system("pause");
}

