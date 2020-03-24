#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<vector>

using namespace std;

cv::Mat  gaycolor(cv::Mat& inputimage, int flag)
{
	if (flag == 3)//��ͨ��ͼ��Ļ�ȡ����
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
	else if (flag == 1)//��ͨ��ͼ��Ļ�ȡ����
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
	// ��ȡ��Ƶ�ļ�
	cv::VideoCapture cap(".\\video\\tmp.mp4");//�����ǻ�ȡ��Ƶ�ĵط�
	// ��ȡ��Ƶ��֡��
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total frames: " << totalFrameNumber << endl;

	//ͨ�����ÿһ֡���м�һ�У��м䲿���е�����ֵ���ж�����Ч֡������Ч֡
	cv::Mat frame;
	long currentFrame = 0;
	int flag = 0;	//�ж��ǲ��ǿ�ʼ֡
	while (flag == 0)
	{
		cap.read(frame);//ȡ��Ƶ��һ֡
		frame = gaycolor(frame, 1);

		int half_row = frame.rows / 2;	//֡ͼ������м�һ��
		int half_col = frame.cols / 2;		//֡ͼ������м�һ��

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //���ǿ�ʼ֡
			{
				flag = 0;
				break;
			}
		}
		currentFrame++;
		cout << "��" << currentFrame << "֡����" << endl;
	}
	cout << "��" << currentFrame << "֡�ǿ�ʼ֡" << endl;
	while (flag == 1)	//�ǿ�ʼ֡����������֡
	{
		cap.read(frame);//ȡ��Ƶ��һ֡

		frame = gaycolor(frame, 1);
		int half_row = frame.rows / 2;	//֡ͼ������м�һ��
		int half_col = frame.cols / 2;		//֡ͼ������м�һ��

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //���ǿ�ʼ֡
			{
				flag = 0;
				break;
			}
		}
		currentFrame++;
		cout << "��" << currentFrame << "֡����" << endl;
	}
	cout << "��" << currentFrame << "֡�ǵ�һ����֡" << endl;
	int jiange = 3; int count = 0;
	while (flag == 0)	//������֡���ǽ���֡
	{
		cv::Mat k;
		cap.read(frame);//ȡ��Ƶ��һ֡
		k = frame;
		frame = gaycolor(frame, 1);
		int half_row = frame.rows / 2;	//֡ͼ������м�һ��
		int half_col = frame.cols / 2;		//֡ͼ������м�һ��

		for (int j = half_col - 200; j < half_col + 200; )
		{
			int a = frame.at<uchar>(half_row, j);
			j = j + 20;
			if (a < 260 && a>180)
			{
				flag = 1;
			}
			else //���ǽ���֡
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
				cout << "���ڴ����" << currentFrame << "֡" << endl;
				cv::imwrite(".\\result_pic\\" + str.str(), k);
				count++;
			}
		}
		jiange++;
		currentFrame++;
	}
	cout << "��" << currentFrame << "֡�ǽ���֡" << endl;
	system("pause");
}

