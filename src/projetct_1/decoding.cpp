#include"decoding.h"
void decoding() {
	//getPoint�洢͸�ӱ任���ĸ�����
	cv::Mat src = cv::imread(".\\result_pic\\pic0.jpg");
	cv::Mat src2 = cv::imread(".\\result_pic\\pic1.jpg");
	cv::Mat src3 = cv::imread(".\\result_pic\\pic2.jpg");
	cv::Mat src4 = cv::imread(".\\result_pic\\pic3.jpg");
	cv::Mat src5 = cv::imread(".\\result_pic\\pic4.jpg");
	cv::Mat src6 = cv::imread(".\\result_pic\\pic5.jpg");
	cv::Mat dst;
	vector<vector<cv::Point>> qrPoint;
	cv::Point2f adPoint[4];
	cv::Point2f getPoint[4];
	FindQrPoint(src, qrPoint,adPoint,getPoint);
	respective(src, dst, getPoint);
	//imshow("output", dst);
	//imshow("1", src);
	thresd(dst);
	//imshow("output", dst);
	//��������
	string dname;
	cout << "����������ļ�����";
	cin >> dname;
	ofstream file_out(dname, ios::app);

	decode(dst,file_out);

	//ʣ������ͼͬ��
	FindQrPoint(src2, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src3, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src4, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src5, qrPoint, adPoint, getPoint);
	respective(src2, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	FindQrPoint(src6, qrPoint, adPoint, getPoint);
	respective(src6, dst, getPoint);
	thresd(dst);
	decode(dst, file_out);

	file_out.close();
	cv::waitKey(0);
}

//��ֵ��
void thresd(cv::Mat& src) 
{
	cv::Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	cv::Mat threshold_output;
	threshold(src_gray, src, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}
int FindQrPoint(cv::Mat& srcImg, vector<vector<cv::Point>>& qrPoint, cv::Point2f adPoint[],cv::Point2f getPoint[])
{
	//��ɫͼת�Ҷ�ͼ
	cv::Mat src_gray;
	cvtColor(srcImg, src_gray, CV_BGR2GRAY);
	//cv::namedWindow("src_gray");
	//imshow("src_gray", src_gray);

	//��ֵ��
	cv::Mat threshold_output;
	threshold(src_gray, threshold_output, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::Mat threshold_output_copy = threshold_output.clone();
	//cv::namedWindow("Threshold_output");
	//cv::imshow("Threshold_output", threshold_output);

	//���ò�����������
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

	//ͨ����ɫ��λ����Ϊ�����������������������ص㣬ɸѡ��������λ��
	int parentIdx = -1;
	int ic = 0; int count = 0;

	for (int i = 0; i < contours.size(); i++)
	{
	
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}

		// ���������������Ƕ�ά��Ķ���
		if (ic >= 2)
		{
			bool isQr = IsQrPoint(contours[parentIdx], threshold_output_copy, i);

			//�����ҵ���������ɫ��λ��
			if (isQr)
			{
				qrPoint.push_back(contours[parentIdx]);
				cv::RotatedRect rotatedRect = cv::minAreaRect(contours[parentIdx]);
				rotatedRect.points(adPoint);
				
				//����qrPoint�ı�adPoint˳��
				//�����ĸ��ǵĲ�ͬ��ȡ�ĸ���
				//���½�
				if (count == 0) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x + 10 > adPoint[0].x&&adPoint[m].x + 10 > adPoint[1].x&&adPoint[m].x + 10 > adPoint[2].x&&adPoint[m].x + 10 > adPoint[3].x) {
							if (adPoint[m].y + 10 > adPoint[0].y&&adPoint[m].y + 10 > adPoint[1].y&&adPoint[m].y + 10 > adPoint[2].y&&adPoint[m].y + 10 > adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//���½�
				else if (count == 1) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x - 10 < adPoint[0].x&&adPoint[m].x - 10 < adPoint[1].x&&adPoint[m].x - 10 < adPoint[2].x&&adPoint[m].x - 10 < adPoint[3].x) {
							if (adPoint[m].y + 10 > adPoint[0].y&&adPoint[m].y + 10 > adPoint[1].y&&adPoint[m].y + 10 > adPoint[2].y&&adPoint[m].y + 10 > adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//���Ͻ�
				else if (count == 2) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x + 10 > adPoint[0].x&&adPoint[m].x + 10 > adPoint[1].x&&adPoint[m].x + 10 > adPoint[2].x&&adPoint[m].x + 10 > adPoint[3].x) {
							if (adPoint[m].y - 10 < adPoint[0].y&&adPoint[m].y - 10 < adPoint[1].y&&adPoint[m].y - 10 < adPoint[2].y&&adPoint[m].y - 10 < adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}
				//���Ͻ�
				else if (count == 3) {
					for (int m = 0; m < 4; m++) {

						if (adPoint[m].x - 10 < adPoint[0].x&&adPoint[m].x - 10 < adPoint[1].x&&adPoint[m].x - 10 < adPoint[2].x&&adPoint[m].x - 10 < adPoint[3].x) {
							if (adPoint[m].y - 10 < adPoint[0].y&&adPoint[m].y - 10 < adPoint[1].y&&adPoint[m].y - 10 < adPoint[2].y&&adPoint[m].y - 10 < adPoint[3].y)
							{
								getPoint[count].x = adPoint[m].x;
								getPoint[count].y = adPoint[m].y;
								count++;
							}
						}

					}
				}

			
				//buxu
			}
			ic = 0;
			parentIdx = -1;
		}
	}

	return 0;
}

//������λ��
void adjusting(vector<vector<cv::Point>> qrPoint,cv::Point adPoint[]) {
	/*adPoint[3].x = qrPoint[3][0].x;
	adPoint[3].y = qrPoint[3][0].y;//���Ͻ�
		int centerx = 0, centery = 0, n = qrPoint.size();
		centerx = (qrPoint[2][n / 4].x + qrPoint[2][n * 2 / 4].x + qrPoint[2][3 * n / 4].x + qrPoint[2][n - 1].x) / 4;
		centery = (qrPoint[2][n / 4].y + qrPoint[2][n * 2 / 4].y + qrPoint[2][3 * n / 4].y + qrPoint[2][n - 1].y) / 4;
		adPoint[2].x =centerx+ (centery-qrPoint[2][0].y);
		adPoint[2].y = centery- (centerx-qrPoint[2][0].x);//���Ͻ�

		centerx = (qrPoint[1][n / 4].x + qrPoint[1][n * 2 / 4].x + qrPoint[1][3 * n / 4].x + qrPoint[1][n - 1].x) / 4;
		centery = (qrPoint[1][n / 4].y + qrPoint[1][n * 2 / 4].y + qrPoint[1][3 * n / 4].y + qrPoint[1][n - 1].y) / 4;
		adPoint[1].x = centerx - (centery - qrPoint[1][0].y);
		adPoint[1].y = centery + (centerx-qrPoint[1][0].x);//���½�

		centerx = (qrPoint[0][n / 4].x + qrPoint[0][n * 2 / 4].x + qrPoint[0][3 * n / 4].x + qrPoint[0][n - 1].x) / 4;
		centery = (qrPoint[0][n / 4].y + qrPoint[0][n * 2 / 4].y + qrPoint[0][3 * n / 4].y + qrPoint[0][n - 1].y) / 4;
		adPoint[0].x = centerx + (centery - qrPoint[0][0].y);
		adPoint[0].y = centery + (centerx - qrPoint[0][0].x);//���½�

		cout <<qrPoint.size();*/

}
bool IsQrPoint(vector<cv::Point>& contour, cv::Mat& img,int i)
{
	//��С��С�޶�
	cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
	if (rotatedRect.size.height < 10 || rotatedRect.size.width < 10)
		return false;

	//����ά�������ͼ�Ͽٳ���
	cv::Mat cropImg = transformCorner(img, rotatedRect);
	int flag = i++;

	//����ڰױ���1:1:3:1:1
	bool result = IsQrColorRate(cropImg, flag);
	return result;
}

cv::Mat transformCorner(cv::Mat src, cv::RotatedRect rect)
{
	cv::Point center = rect.center;   //��ת����
	//circle(src, center, 2, Scalar(0, 0, 255), 2);
	//Size sz = Size(rect.size.width, rect.size.height);
	cv::Point TopLeft = cv::Point(cvRound(center.x), cvRound(center.y)) - cv::Point(rect.size.height / 2, rect.size.width / 2);  //��ת���Ŀ��λ��
	TopLeft.x = TopLeft.x > src.cols ? src.cols : TopLeft.x;
	TopLeft.x = TopLeft.x < 0 ? 0 : TopLeft.x;
	TopLeft.y = TopLeft.y > src.rows ? src.rows : TopLeft.y;
	TopLeft.y = TopLeft.y < 0 ? 0 : TopLeft.y;

	//Point ButtonRight = (Point)center - Point(rect.size.width, rect.size.height);
	cv::Rect RoiRect = cv::Rect(TopLeft.x, TopLeft.y, rect.size.width, rect.size.height);   //��ͼ�ر�����
	double angle = rect.angle;        //��ת�Ƕ�
	cv::Mat mask, roi, dst;                //dst�Ǳ���ת��ͼƬ roiΪ���ͼƬ maskΪ��ģ
	cv::Mat image;						 //����ת���ͼƬ
	cv::Size sz = src.size();             //��ת��ĳߴ�
	mask = cv::Mat::zeros(src.size(), CV_8U);

	/************************************
	Ϊ��ģ��ɫ һ��Ϊ��ɫ
	��ΪRotatedRect ���͵ľ��β����׵�ȡ������ 
	����ҰѾ��ε��ĸ����㵱������ ����drawContours���
	************************************/
	vector<cv::Point> contour;
	cv::Point2f points[4];
	rect.points(points);
	for (int i = 0; i < 4; i++)
		contour.push_back(points[i]);
	vector<vector<cv::Point>> contours;
	contours.push_back(contour);
	drawContours(mask, contours, 0, cv::Scalar(1), -1);

	/*��ͼ��Ȼ��Χ�����ľ���������ת*/
	src.copyTo(dst, mask);
	//roi = dst(RoiRect);
	cv::Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(dst, image, M, sz);
	roi = image(RoiRect);

	//imshow("image", image);
	return roi;
}


//���������ڰױ����ж�
bool IsQrColorRate(cv::Mat& image, int flag)
{
	bool x = IsQrColorRateX(image, flag);
	if (!x)
		return false;
	bool y = IsQrColorRateY(image, flag);
	return y;
}
//����ڰױ����ж�
bool IsQrColorRateX(cv::Mat& image, int flag)
{
	int nr = image.rows / 2;//��
	int nc = image.cols * image.channels();//��

	vector<int> vValueCount;
	vector<uchar> vColor;
	int count = 0;
	uchar lastColor = 0;

	uchar* data = image.ptr<uchar>(nr);
	for (int i = 0; i < nc; i++)
	{
		vColor.push_back(data[i]);
		uchar color = data[i];
		if (color > 0)
			color = 255;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5)
		return false;

	//����ڰױ���1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//��� �ұ� ��������ֵ������
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//�ڰױ���1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	//cout << "flag:" << flag << " ";

	float rate2 = vValueCount[index - 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index - 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	return true;
}
//����ڰױ����ж� ʡ��
bool IsQrColorRateY(cv::Mat& image, int flag){
	int nc = image.cols / 2;
	int nr = image.rows * image.channels();

	vector<int> vValueCount;
	vector<uchar> vColor;
	int count = 0;
	uchar lastColor = 0;

	uchar* data = image.ptr<uchar>(nc);
	for (int i = 0; i < nr; i++)
	{
		vColor.push_back(data[i]);
		uchar color = data[i];
		if (color > 0)
			color = 255;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5)
		return false;

	//����ڰױ���1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//��� �ұ� ��������ֵ������
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//�ڰױ���1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	//cout << "flag:" << flag << " ";

	float rate2 = vValueCount[index - 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index - 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 1] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	rate2 = vValueCount[index + 2] / rate;
	//cout << rate2 << " ";
	if (!IsQrRate(rate2))
		return false;

	return true;
}

bool IsQrRate(float rate)
{
	//��ű��� ����̫�ϸ�
	return rate > 0.6 && rate < 1.9;
}
 // pointDest��ŵ�����������㣬�������˳������
 // pt0----pt1
 // 
 // pt2

//͸�ӱ任
void respective(cv::Mat src,cv::Mat &dst,cv::Point2f getPoint[]) {
	vector<cv::Point2f> dstTri(4);
	vector<cv::Point2f> srcTri(4);
	//��͸�ӱ任���ĸ��������srcTri
	srcTri[0].x = getPoint[3].x;
	srcTri[0].y = getPoint[3].y;
	srcTri[1].x = getPoint[2].x;
	srcTri[1].y = getPoint[2].y;
	srcTri[2].x = getPoint[0].x;
	srcTri[2].y = getPoint[0].y;
	srcTri[3].x = getPoint[1].x;
	srcTri[3].y = getPoint[1].y;

	//�涨Ŀ�ı任�ĸ�������ƴ�С
	dstTri[0].x = 0;
	dstTri[0].y = 0;
	dstTri[1].x = 800;
	dstTri[1].y = 0;
	dstTri[2].x = 800;
	dstTri[2].y = 450;
	dstTri[3].x = 0;
	dstTri[3].y = 450;


	//adjusting(qrPoint,adPoint );
	/*cv::line(src, getPoint[0], getPoint[1], cv::Scalar(0, 0, 255), 2);
	cv::line(src, getPoint[1], getPoint[3], cv::Scalar(0, 0, 255), 2);//����getPoint�Ƿ��Ƕ�ά���ĸ�����
	cv::line(src, getPoint[2], getPoint[3], cv::Scalar(0, 0, 255), 2);
	cv::line(src, getPoint[2], getPoint[0], cv::Scalar(0, 0, 255), 2);*/
	cv::Mat H = cv::findHomography(srcTri, dstTri, cv::RANSAC);//����͸�Ӿ���
	cv::warpPerspective(src, dst, H, cv::Size(800, 450));//ͼ��͸�ӱ任
}

void decode(cv::Mat src,ofstream& file_out) {
	for (int hight = 0; hight < 450; hight = hight + 10) {
		for (int width = 0; width < 800; width=width+10) {
			int black = 0, white = 0;
			if (hight <= 70 && (width <= 70.0 || width >= 720.0));//��λ�㲻����
			else if (hight >= 370  && width <= 70);//��λ�㲻����
			else {
				for (int m = 0; m <10; m++) {
					for (int n = 0; n < 10; n++) {
						if (src.at<uchar>(hight + m , width + n ) == 0)black++;
						else if (src.at<uchar>(hight + m, width + n) == 255)white++;
					}
				}
				//cout << black;
				if (white >= black) {
					file_out << "1"; //cout << "1";
				}
				else if (black > white) {
					file_out << "0";// cout << "0";
				}
			}
		}
	}
}
