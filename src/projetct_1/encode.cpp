#include "encode.h"
int analysis(int h, int w) {
	//�Ӱױ� 10�����ص� h��50��470 w��400��820
	if ((h >= 0 && h < 10) || (h >= 460 && h < 470))return 1;
	if ((w >= 0 && w < 10) || (w >= 810 && w < 820))return 1;
	//

	if ((h == 10 || h == 70) && w >= 10 && w <= 70)return 0;//��ɫ
	else if (h >= 20 && h <= 60 && (w == 10 || w == 70))return 0;
	else if (h >= 30 && h <= 50 && w >= 30 && w <= 50)return 0;
	else if ((h == 20 || h == 60) && w >= 20 && w <= 60)return 1;//��ɫ
	else if ((w == 20 || w == 60) && h >= 30 && h <= 50)return 1;

	int h2 = h - 380;
	if ((h2 == 10 || h2 == 70) && w >= 10 && w <= 70)return 0;//��ɫ
	else if (h2 >= 20 && h2 <= 60 && (w == 10 || w == 70))return 0;
	else if (h2 >= 30 && h2 <= 50 && w >= 30 && w <= 50)return 0;
	else if ((h2 == 20 || h2 == 60) && w >= 20 && w <= 60)return 1;//��ɫ
	else if ((w == 20 || w == 60) && h2 >= 30 && h2 <= 50)return 1;

	int w2 = w - 730;
	if ((h == 10 || h == 70) && w2 >= 10 && w2 <= 70)return 0;//��ɫ
	else if (h >= 20 && h <= 60 && (w2 == 10 || w2 == 70))return 0;
	else if (h >= 30 && h <= 50 && w2 >= 30 && w2 <= 50)return 0;
	else if ((h == 20 || h == 60) && w2 >= 20 && w2 <= 60)return 1;//��ɫ
	else if ((w2 == 20 || w2 == 60) && h >= 30 && h <= 50)return 1;

	//��λ��İ�ɫ�߿�
	if (h == 80 && ((w >= 10 && w <= 80) || (w <= 800 && w >= 730)))return 1;
	else if (h == 380 && w >= 10 & w <= 80)return 1;
	else if ((w == 80 || w == 730) && h >= 10 && h <= 70)return 1;
	else if (w == 80 && h <= 450 && h >= 390)return 1;

	//���ĸ���λ��
	if ((h2 == 10 || h2 == 70) && w2 >= 10 && w2 <= 70)return 0;//��ɫ
	else if (h2 >= 20 && h2 <= 60 && (w2 == 10 || w2 == 70))return 0;
	else if (h2 >= 30 && h2 <= 50 && w2 >= 30 && w2 <= 50)return 0;
	else if ((h2 == 20 || h2 == 60) && w2 >= 20 && w2 <= 60)return 1;//��ɫ
	else if ((w2 == 20 || w2 == 60) && h2 >= 30 && h2 <= 50)return 1;

	if (h == 380 && w >= 730 && w <= 800)return 1;
	else if (w == 730 && h >= 390 && h <= 450)return 1;
	return -1;
}//
//����ʼ����֡�Ĵ���ͼ
void create_white(cv::Mat& img) {
	for (int hight = 0; hight < 470; hight = hight + 10)
	{
		for (int width = 0; width < 820; width = width + 10)
		{
			for (int m = 0; m < 10; m++) {
				for (int n = 0; n < 10; n++) {
					img.at<uchar>(hight + m, width + n) = 255;//���Ҷ�ֵ 0�Ǻ� 255�ǰ�
				}
			}
		}
	}
}
//���м�ͼ��Ӵ���д��������ļ�
void create_pic(cv::Mat img, ofstream& file_out) {
	for (int hight = 0; hight < 470; hight = hight + 10)
	{
		for (int width = 0; width < 820; width = width + 10)
		{

			//����λ�� 1:1:3:1:1
			if (analysis(hight, width) == 0) {
				//�кڽ���

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {
						img.at<uchar>(hight + m, width + n) = 0;//���Ҷ�ֵ 0�Ǻ� 255�ǰ�
					}
				}

			}
			else if (analysis(hight, width) == 1) {
				//�а׽���

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {

						img.at<uchar>(hight + m, width + n) = 255;//���Ҷ�ֵ 0�Ǻ� 255�ǰ�
					}
				}
			}
			else {
				//��������������ļ�������ͼƬ

					//at<����>(i,j)���в��������ڻҶ�ͼ ��i�е�j��
				int i = rand() % 2;
				file_out << i;
				if (i == 0) { i = 0; }
				else i = 255;

				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {

						img.at<uchar>(hight + m, width + n) = i;//���Ҷ�ֵ 0�Ǻ� 255�ǰ�
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
	//����Ƿ�򿪳ɹ�
	char data = 0;//�洢0/1
	
	string dname;//�����ļ���
	cout << "�����ļ�����";
	cin >> dname;
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ofstream file_out(dname, ios::app);
	if (file_out.fail()) {
		cout << "���ļ�ʧ��" << endl;
		return 0;
	}
	cv::Mat img(470, 820, CV_8UC1);//1�Ǻڰף�3�ǲ�ͼ
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



