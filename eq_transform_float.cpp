#if 0
#include <opencv2\opencv.hpp>
#include <string>
#include <fstream>
#define pi 3.14159265358979323846

using namespace cv;
using namespace std;

int sign(double x)
{
	if (x == 0)
	{
		return 0;
	}
	else
	{
		if (x > 0) return 1;
		else return -1;
	}
}

int eq_transform(unsigned char* src_data, unsigned char* dest_data, double ar, double ap, double ay, int pnwidth, int pnheight, int step, int channels)
{
	std::ofstream ofs("float.txt");
	double b = 2 * pi;
	double mpdistance = pnwidth / b;

	double rot[2] = { mpdistance*pi, -ay*mpdistance };

	double m[3][3] = { cos(ar), sin(ar), 0,
		-cos(ap)*sin(ar), cos(ap)*cos(ar), sin(ap),
		sin(ar)*sin(ap), -sin(ap)*cos(ar), cos(ap)
	};
	ofs << (m[0][0]) << " " << (m[0][1]) << " " << (m[0][2]) << " " << mpdistance << endl;

	{

		
		for (int x = 1; x <= pnheight; ++x)
		{
			//	int x = l1;

			for (int y = 1; y <= pnwidth; ++y)
			{

				//int y = l2;

				double src[2] = { y - pnwidth / 2, x - pnheight / 2 };
				src[0] += rot[1];

				src[0] = sign(abs(src[0] - pnwidth / 2))*((sign(src[0] - pnwidth / 2) + 1)*(src[0] - pnwidth / 2) - sign(src[0] - pnwidth / 2)*src[0]) + (1 - sign(abs(src[0] - pnwidth / 2)))*src[0];

				src[0] = sign(abs(src[0] + pnwidth / 2))*((1 - sign(src[0] + pnwidth / 2))*(src[0] + pnwidth / 2) + sign(src[0] + pnwidth / 2)*src[0]) + (1 - sign(abs(src[0] + pnwidth / 2)))*src[0];
				//cout << src[0] << "src " << src[1] << endl;

				double phi = src[0] / mpdistance;

				double theta1 = -src[1] / mpdistance + pi / 2;
				
				
					//ofs << src[0] << " " << src[1] << " " << phi << " " << theta1 << endl;

				double s1 = sin(theta1);
				double c1 = cos(theta1);
				double s2 = sin(phi);
				double c2 = cos(phi);



				double v1[2] = { s1*s2, c1 };
				double r1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
				double theta2 = mpdistance*atan2(r1, s1*c2);

				double v[3] = { (abs(sign(theta2))*s1*s2), (abs(sign(theta2))*c1), (s1*c2) };

				double u[3] = { v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0], v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1], v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] };


				double r = sqrt(u[0] * u[0] + u[1] * u[1]);
				
				//	ofs << (u[0]) << " " << (u[1]) << " " << (u[2]) << " " << (r) << endl;

				src[0] = abs(sign(r)*mpdistance*atan2(r, u[2])) / r*u[0];
				src[1] = abs(sign(r)*mpdistance*atan2(r, u[2])) / r*u[1];
				if (abs(x - 750) < 10 && y < 50)
					ofs << (src[0]) << " " << (src[1]) << " " << (u[0]) << " " << (u[1]) << " " << (u[2]) << " " << (r) << endl;
				r = sqrt(src[0] * src[0] + src[1] * src[1]);
				double theta = r / mpdistance;


				double s = abs(sign(theta))*sin(theta) / r + (1 - abs(sign(theta))) / mpdistance;

				
					//ofs << (src[0]) << " " << (src[1]) << " " << (r) << " " << (s) << " " << (theta) << " " << v[0] << " " << v[1] << endl;

				v[1] = s*src[0];
				v[0] = cos(theta);


				src[0] = mpdistance*atan2(v[1], v[0]);
				src[1] = mpdistance*atan(s*src[1] / sqrt(v[0] * v[0] + v[1] * v[1]));

				

				src[0] = src[0] + pnwidth / 2;
				src[1] = src[1] + pnheight / 2;


				if (src[1] < 1)
					src[1] = 1;
				if (src[1] > pnwidth / 2)
					src[1] = pnwidth / 2;
				if (src[0] < 1)
					src[0] = 1;
				if (src[0] > pnwidth)
					src[0] = pnwidth;
				
					//ofs << (src[0]) << " " << (src[1]) << " " << (r) << " " << (s) << " " << (theta) <<" " << v[0] << " " <<v[1] << endl;
				//最近邻插值
				int dx = int((src[1] - 0.5))*step;
				int dy = int((src[0] - 0.5))*channels;
				dest_data[(x - 1)*step + (y - 1)*channels + 0] = src_data[dx + dy + 0];
				dest_data[(x - 1)*step + (y - 1)*channels + 1] = src_data[dx + dy + 1];
				dest_data[(x - 1)*step + (y - 1)*channels + 2] = src_data[dx + dy + 2];
				//img_undist.at<cv::Vec3b>(x - 1, y - 1) = img_origin.at<cv::Vec3b>(src[1] - 1, src[0] - 1);

			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
//int eq_transform(int argc, char* argv[])
{


	string filename = "1.jpeg";
	double ar, ap, ay;
	ar = 0;
	ap = 0;
	ay = 0;

	IplImage* img = cvLoadImage(filename.c_str());
	int pnwidth = img->width;
	int pnheight = img->height;
	IplImage* img1 = cvCreateImage(cvSize(pnwidth, pnheight), img->depth, img->nChannels);;


	int step = img->widthStep / sizeof(uchar);
	int channels = img->nChannels;
	cout << step << " " << channels << endl;

	uchar* data = (uchar *)img->imageData;
	uchar* data1 = (uchar *)img1->imageData;
	//data为图像地址，ar,ap,ay代表三个旋转角，pnwidth,pnheight图像高宽，step表示通道数*列数
	eq_transform(data, data1, ar, ap, ay, pnwidth, pnheight, step, channels);

	cvSaveImage("rota.jpg", img1);
	//cv::imwrite("rota.jpg", img_undist);
	std::system("pause");
	return 0;
}
#endif