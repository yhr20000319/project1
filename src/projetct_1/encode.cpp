#include "encode.h"
int analysis(int h, int w) {
	//加白边 10个像素点 h加50到470 w加400到820
	if ((h >= 0 && h < 10) || (h >= 460 && h < 470))return 1;
	if ((w >= 0 && w < 10) || (w >= 810 && w < 820))return 1;
	//

	if ((h == 10 || h == 70) && w >= 10 && w <= 70)return 0;//黑色
	else if (h >= 20 && h <= 60 && (w == 10 || w == 70))return 0;
	else if (h >= 30 && h <= 50 && w >= 30 && w <= 50)return 0;
	else if ((h == 20 || h == 60) && w >= 20 && w <= 60)return 1;//白色
	else if ((w == 20 || w == 60) && h >= 30 && h <= 50)return 1;

	int h2 = h - 380;
	if ((h2 == 10 || h2 == 70) && w >= 10 && w <= 70)return 0;//黑色
	else if (h2 >= 20 && h2 <= 60 && (w == 10 || w == 70))return 0;
	else if (h2 >= 30 && h2 <= 50 && w >= 30 && w <= 50)return 0;
	else if ((h2 == 20 || h2 == 60) && w >= 20 && w <= 60)return 1;//白色
	else if ((w == 20 || w == 60) && h2 >= 30 && h2 <= 50)return 1;

	int w2 = w - 730;
	if ((h == 10 || h == 70) && w2 >= 10 && w2 <= 70)return 0;//黑色
	else if (h >= 20 && h <= 60 && (w2 == 10 || w2 == 70))return 0;
	else if (h >= 30 && h <= 50 && w2 >= 30 && w2 <= 50)return 0;
	else if ((h == 20 || h == 60) && w2 >= 20 && w2 <= 60)return 1;//白色
	else if ((w2 == 20 || w2 == 60) && h >= 30 && h <= 50)return 1;

	//定位点的白色边框
	if (h == 80 && ((w >= 10 && w <= 80) || (w <= 800 && w >= 730)))return 1;
	else if (h == 380 && w >= 10 & w <= 80)return 1;
	else if ((w == 80 || w == 730) && h >= 10 && h <= 70)return 1;
	else if (w == 80 && h <= 450 && h >= 390)return 1;

	//第四个定位点
	if ((h2 == 10 || h2 == 70) && w2 >= 10 && w2 <= 70)return 0;//黑色
	else if (h2 >= 20 && h2 <= 60 && (w2 == 10 || w2 == 70))return 0;
	else if (h2 >= 30 && h2 <= 50 && w2 >= 30 && w2 <= 50)return 0;
	else if ((h2 == 20 || h2 == 60) && w2 >= 20 && w2 <= 60)return 1;//白色
	else if ((w2 == 20 || w2 == 60) && h2 >= 30 && h2 <= 50)return 1;

	if (h == 380 && w >= 730 && w <= 800)return 1;
	else if (w == 730 && h >= 390 && h <= 450)return 1;
	return -1;
}//
//做开始结束帧的纯白图
void create_white(cv::Mat& img) {
	for (int hight = 0; hight < 470; hight = hight + 10)
	{
		for (int width = 0; width < 820; width = width + 10)
		{
			for (int m = 0; m < 10; m++) {
				for (int n = 0; n < 10; n++) {
					img.at<uchar>(hight + m, width + n) = 255;//给灰度值 0是黑 255是白
				}
			}
		}
	}
}
//做中间图像加创建写入二进制文件
void create_pic(cv::Mat img, ofstream& file_out) {
	for (int hight = 0; hight < 470; hight = hight + 10)
	{
		for (int width = 0; width < 820; width = width + 10)
		{

			//做定位点 1:1:3:1:1
			if (analysis(hight, width) == 0) {
				//判黑进入

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {
						img.at<uchar>(hight + m, width + n) = 0;//给灰度值 0是黑 255是白
					}
				}

			}
			else if (analysis(hight, width) == 1) {
				//判白进入

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {

						img.at<uchar>(hight + m, width + n) = 255;//给灰度值 0是黑 255是白
					}
				}
			}
			else {
				//生成随机二进制文件并产生图片

					//at<类型>(i,j)进行操作，对于灰度图 第i行第j列
				int i = rand() % 2;
				file_out << i;
				if (i == 0) { i = 0; }
				else i = 255;

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {

						img.at<uchar>(hight + m, width + n) = i;//给灰度值 0是黑 255是白
					}
				}
			}

		}
	}
}

int encode()
{
	srand(time(NULL));
	//ifstream file_in("code.txt", ios::in);
	//检测是否打开成功
	char data = 0;//存储0/1
	
	string dname;//保存文件名
	cout << "输入文件名：";
	cin >> dname;
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ofstream file_out(dname, ios::app);
	if (file_out.fail()) {
		cout << "打开文件失败" << endl;
		return 0;
	}
	cv::Mat img(470, 820, CV_8UC1);//1是黑白，3是彩图
	//imshow("gray_ori", img);
	create_white(img);
	cv::imwrite(".\\src_code_pic\\0.jpg", img);
	cv::imwrite(".\\src_code_pic\\1.jpg", img);
	create_pic(img, file_out);
	//imshow("result", img);
	cv::imwrite(".\\src_code_pic\\2.jpg", img);
	create_pic(img, file_out);
	cv::imwrite(".\\src_code_pic\\3.jpg", img); create_pic(img, file_out);
	cv::imwrite(".\\src_code_pic\\4.jpg", img); create_pic(img, file_out);
	cv::imwrite(".\\src_code_pic\\5.jpg", img); create_pic(img, file_out);
	cv::imwrite(".\\src_code_pic\\6.jpg", img); create_pic(img, file_out);
	cv::imwrite(".\\src_code_pic\\7.jpg", img);

	create_white(img);
	cv::imwrite(".\\src_code_pic\\8.jpg", img);
	cv::imwrite(".\\src_code_pic\\9.jpg", img);
	file_out.close();
	//waitKey(0);
	return 1;
}



