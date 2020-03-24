// decoding.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#include "decode.h"
void decode()
{
	cv::Mat src = cv::imread(".\\result_pic\\pic0.jpg");
	cv::Mat src3 = cv::imread(".\\result_pic\\pic1.jpg");
	cv::Mat src5 = cv::imread(".\\result_pic\\pic2.jpg");
	cv::Mat src7 = cv::imread(".\\result_pic\\pic3.jpg");
	cv::Mat src9 = cv::imread(".\\result_pic\\pic4.jpg");
	cv::Mat src11 = cv::imread(".\\result_pic\\pic5.jpg");
	cv::Mat src2, src4, src6, src8, src10, src12;
	cv::Point point[3]; cv::Point pointAdjust[3];
	
	string dname;
	cout << "����������ļ�����";
	cin >> dname;

	ofstream file_out(dname, ios::app);
	if (file_out.fail()) {
		cout << "���ļ�ʧ��" << endl;
		exit(0);
	}
	//position(src, point, src2);
	//adjusting(src2, src2, point);
	//decoding(point, src2, file_out);//imshow("1", src2);
	/*for (int i = 0; i < 3; i++) {
		cout << "��" <<  i<< "�����ĵ�����" << point[i].x << "." << point[i].y << endl;
	}*/
	//imshow("1", src2);
	//position(src3, point, src4);
	//cv::imshow("1", src4);
	//adjusting(src3, src3, point);
	//decoding(point, src4, file_out);
	//position(src5, point, src6);
	//adjusting(src6, src6, point);
	//decoding(point, src6, file_out);
	//position(src7, point, src8);
	//adjusting(src8, src8, point);
	//decoding(point, src8, file_out);
	//position(src9, point, src10);
	//adjusting(src10, src10, point);
	//decoding(point, src10, file_out);
	//position(src11, point, src12);
	//adjusting(src12, src12, point);
	//decoding(point, src12, file_out);
	file_out.close();
	//imshow("title", src2);
	cv::waitKey(0);
}

void position(cv::Mat InImage, cv::Point point[4], cv::Mat& threshold_output)
{
	cv::Point QRPoints[4];
	cv::Mat src = InImage;
	cv::Mat src_gray;
	cv::Mat src_all = src.clone();

	vector<vector<cv::Point> > contours, contours2;
	vector<cv::Vec4i> hierarchy;
	//Ԥ����
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, cv::Size(3, 3)); //ģ����ȥ��ë��
	threshold(src_gray, threshold_output, 100, 255, cv::THRESH_OTSU);//����ͼ�����ͼ
	//imshow("1", threshold_output);
	//Ѱ������ 
	//��һ������������ͼ�� 2ֵ����
	//�ڶ����������ڴ�洢����FindContours�ҵ��������ŵ��ڴ����档
	//�����������ǲ㼶��**[Next, Previous, First_Child, Parent]** ��vector
	//���ĸ����������ͣ��������ṹ
	//����������ǽڵ����ģʽ��������ȫ��Ѱ��
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));

	//����ɸѡ
	int c = 0, ic = 0, area = 0;
	int parentIdx = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		//hierarchy[i][2] != -1 ��ʾ���������������
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		//���������0
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}
		//�ҵ���λ����Ϣ
		if (ic >= 2)
		{
			contours2.push_back(contours[parentIdx]);
			ic = 0;
			parentIdx = -1;
		}
	}
	if (contours2.size() < 3)
	{
		for (int i = 0; i < 4; i++)
		{
			QRPoints[i].x = 0;
			QRPoints[i].y = 0;
		}
	}
	else
	{

		for (int i = 0; i < contours2.size(); i++)
		{
			point[i] = Center_cal(contours2, i);
		}

		//cv::line(src_all, point[0], point[1], cv::Scalar(0, 0, 255), 2);//����������������߶�,����Ϊͼ�񣬵�һ/�ڶ����˵㣬��ɫ���ߴ�ϸ
		//cv::line(src_all, point[1], point[2], cv::Scalar(0, 0, 255), 2);
		//line(src_all, point[2], point[0], Scalar(0, 0, 255), 2);
	}
	//cv::imshow("123", src_all);
}
cv::Point Center_cal(vector<vector<cv::Point> > contours, int i)//��0�����½ǣ�1�����Ͻǣ�2�����Ͻ�
{

	int centerx = 0, centery = 0, n = contours[i].size();
	//cout << i << "size" << n<<endl;
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	cv::Point point1 = cv::Point(centerx, centery);
	cout << "��" << i << "�����ĵ�����" <<centerx<<"."<<centery<< endl;
	return point1;
}
//��������ĺ���
int rounding(double x) {
	if (x - (int)x >= 0.5)return (int)x + 1;
	else return (int)x;
}

//͸�ӱ任

//ͼƬ�п�������б�ģ���б�нǿ���ͨ��pt0��pt1������ˮƽ��֮��ļн�ȷ������ά�����б�ǶȾ�������ͼƬ����б�Ƕȣ��Ӷ����Զ�����ͼƬ����ˮƽ������
double Angle(cv::Point p1, cv::Point p2, bool& clockwise) {
	if (p1.y > p2.y)clockwise = 1;//Ϊ����ʱ��ת
	else clockwise = 0;
	return cvFastArctan(p1.y - p2.y, p1.x - p2.x);
}
void adjusting(cv::Mat src, cv::Mat& src_all, cv::Point point[]) {
	cv::Point pointAdjust[3];
	pointAdjust[0].x = point[2].x - 35 * (point[1].x - point[2].x) / 730;
	pointAdjust[1].x = point[1].x + 35 * (point[0].y - point[2].y) / 380;
	pointAdjust[0].y = point[2].y - 35 * (point[0].y - point[2].y) / 380;
	pointAdjust[1].y = point[1].y - 35 * (point[1].x - point[2].x) / 730;
	//line(src, pointAdjust[0], pointAdjust[1], cv::Scalar(0, 0, 255), 2);

	//��ά����б�Ƕ�
	bool clockwise;
	double qrAngle = Angle(pointAdjust[1], pointAdjust[0], clockwise);

	//�Զ�ά�����Ͻǵ�Ϊ���� ��ת
	src_all = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
	double rotationAngle = clockwise ? -qrAngle : qrAngle;//��ֵ��ʱ�룬Ϊ��ȡ��ֵ
	cv::Mat affine_matrix = cv::getRotationMatrix2D(pointAdjust[0], rotationAngle, 1.0);//�����ת����
	cv::warpAffine(src, src_all, affine_matrix, src_all.size());
	//cv::imshow("1", src);

}
//���벿��
void decoding(cv::Point point[3], cv::Mat InImage, ofstream& file_out) {
	//���x���꣬y���꣬�����ߣ��������ؿ����Ͻ���

	double scalex = (point[1].x - point[2].x) / 730.0; // cout << scalex;                //����ʱ���߾��루790+730��/2-��0+60��/2=730.
	double scaley = (point[0].y - point[2].y) / 380.0;// cout << scaley;            //(440+380)/2-(0+60)/2=380
	double ereax = scalex * 10.0; double ereay = (scaley * 10.0);
	double detect_x = point[2].x - (35.0 * scalex);
	double detect_y = point[2].y - (35.0 * scaley);
	double totalx = (800.0 * scalex);
	double totaly = (450.0 * scaley);
	double slope_y = (point[1].y - point[2].y)/(double)(point[1].x-point[2].x);
	double slope_x = (point[0].x - point[2].x)/(double)(point[0].y-point[2].y);
	for (double hight = detect_y; hight < detect_y + totaly; hight = hight + ereay)
	{
		//cout << "1";
		int i = 0,j=0;
		for (double width = detect_x; width < detect_x + totalx; width = width + ereax)
		{
			//cout << detect_x+totalx<<endl;
			int black = 0, white = 0;
			if (hight <= detect_y + (70.0 * scaley) && (width <= detect_x + (70.0 * scalex) || width >= detect_x + (720.0 * scalex)));//��λ�㲻����
			else if (hight >= detect_y + (370.0 * scaley) && width <= detect_x + (70.0 * scalex));//��λ�㲻����
			else {
				for (int m = 0; m < ereay; m++) {
					for (int n = 0; n < ereax; n++) {
						if (InImage.at<uchar>(hight +m+i*slope_y, width + n+j*slope_x) == 0)black++;
						else if (InImage.at<uchar>(hight + m, width + n) == 255)white++;
					}
				}
				//cout << black;
				if (white > black) {
					file_out << "1"; //cout << "1";
				}
				else if (black > white) {
					file_out << "0";// cout << "0";
				}
			}
			i +=  ereay;
			j += ereax;
		}
	}

}