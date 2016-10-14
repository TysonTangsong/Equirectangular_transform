#if 1
#include <opencv2\opencv.hpp>
#include <string>
#include <iqmath.h>
#include <fstream>
//#define pi 3.14159265358979323846
//
using namespace cv;
using namespace std;
typedef int FIX;
int sign(int x)
{
	if (x == _IQ19(0))
	{
		return 0;
	}
	else
	{
		if (x > _IQ19(0)) return 1;
		else return -1;
	}
}

int eq_transform(unsigned char* src_data, unsigned char* dest_data, float ar0, float ap0, float ay0, int pnwidth0, int pnheight0, int step, int channels)
{
	std::ofstream ofs("fix.txt");
	//#define GLOBAL_Q 24
	//#define Q 8
	FIX ar, ap, ay;
	FIX pnwidth, pnheight;
	ar = _IQ19(ar0);
	ap = _IQ19(ap0);
	ay = _IQ19(ay0);
	pnwidth = _IQ19(pnwidth0);
	pnheight = _IQ19(pnheight0);
	FIX pi = _IQ19(3.14159265358979323846);
	FIX halfofwidth = _IQ19(pnwidth0 >> 1);
	FIX halfofheight = _IQ19(pnheight0 >> 1);
	//cout << _IQ19toF(halfofwidth) << endl;;
	FIX b = _IQ19mpy(_IQ19(2), pi);
	FIX mpdistance = _IQ19div(pnwidth, b);

	FIX rot[2] = { _IQ19mpy(mpdistance, pi), _IQ19mpy(-ay, mpdistance) };

	FIX m[3][3] = { _IQ19cos(ar), _IQ19sin(ar), _IQ19(0),
		_IQ19mpy(-_IQ19cos(ap), _IQ19sin(ar)), _IQ19mpy(_IQ19cos(ap), _IQ19cos(ar)), _IQ19sin(ap),
		_IQ19mpy(_IQ19sin(ar), _IQ19sin(ap)), _IQ19mpy(-_IQ19sin(ap), _IQ19cos(ar)), _IQ19cos(ap)
	};

	ofs << _IQ19toF(m[0][0]) << " " << _IQ19toF(m[0][1]) << " " << _IQ19toF(m[0][2]) << " " << _IQ19toF(mpdistance) << _IQ19toF(pnwidth) << endl;
	{

		for (int x = 1; x <= pnheight0; ++x)
		{

			for (int y = 1; y <= pnwidth0; ++y)
			{


				FIX src[2] = { _IQ19(y) - halfofwidth, _IQ19(x) - halfofheight };

				src[0] = src[0] + rot[1];


				{
					/*src[0] = sign(abs(src[0] - pnwidth / 2))
					*( (sign(src[0] - pnwidth / 2) + 1) * (src[0] - pnwidth / 2) -  sign(src[0] - pnwidth / 2) *src[0] )
					+ (1 - sign(abs(src[0] - pnwidth / 2)))*src[0];*/

					FIX gs1 = _IQ19(sign(_IQ19abs(src[0] - halfofwidth)));
					FIX gs21 = _IQ19(sign(src[0] - halfofwidth)) + _IQ19(1);
					FIX gs22 = _IQ19(src[0] - halfofwidth);
					FIX gs23 = _IQ19mpy(_IQ19(sign(src[0] - halfofwidth)), src[0]);
					FIX gs2 = _IQ19mpy(gs21, gs22) - gs23;

					FIX gs3 = _IQ19mpy(_IQ19(1) - _IQ19(sign(_IQ19abs(src[0] - halfofwidth))), src[0]);

					src[0] = _IQ19mpy(gs1, gs2) + gs3;

					/*src[0] = sign(abs(src[0] + pnwidth / 2))
					*((1 - sign(src[0] + pnwidth / 2))*(src[0] + pnwidth / 2) + sign(src[0] + pnwidth / 2)*src[0])
					+ (1 - sign(abs(src[0] + pnwidth / 2)))*src[0];*/

					gs1 = _IQ19(sign(_IQ19abs(src[0] + halfofwidth)));
					gs21 = _IQ19(1) - _IQ19(sign(src[0] + halfofwidth));
					gs22 = _IQ19(src[0] + halfofwidth);
					gs23 = _IQ19mpy(_IQ19(sign(src[0] + halfofwidth)), src[0]);
					gs2 = _IQ19mpy(gs21, gs22) + gs23;

					gs3 = _IQ19mpy(_IQ19(1) - _IQ19(sign(_IQ19abs(src[0] + halfofwidth))), src[0]);
					src[0] = _IQ19mpy(gs1, gs2) + gs3;


				}

				FIX phi = _IQ19div(src[0], mpdistance);

				FIX theta1 = _IQ19div(-src[1], mpdistance) + _IQ19div(pi, _IQ19(2));


				//	ofs << _IQ19toF(src[0]) << " " << _IQ19toF(src[1]) << " " << _IQ19toF(phi) << " " << _IQ19toF(theta1) << endl;

				FIX s1 = _IQ19sin(theta1);
				FIX c1 = _IQ19cos(theta1);
				FIX s2 = _IQ19sin(phi);
				FIX c2 = _IQ19cos(phi);



				FIX v1[2] = { _IQ19mpy(s1, s2), c1 };
				FIX r1 = _IQ19sqrt(_IQ19mpy(v1[0], v1[0]) + _IQ19mpy(v1[1], v1[1]));

				FIX theta2 = _IQ19mpy(mpdistance, _IQ19atan2(r1, _IQ19mpy(s1, c2)));

				FIX v[3] = { (_IQ19mpy(_IQ19abs(_IQ19(sign(theta2))), _IQ19mpy(s1, s2))), (_IQ19mpy(_IQ19abs(_IQ19(sign(theta2))), c1)), _IQ19mpy(s1, c2) };

				FIX u[3] = { _IQ19mpy(v[0], m[0][0]) + _IQ19mpy(v[1], m[1][0]) + _IQ19mpy(v[2], m[2][0]),
					_IQ19mpy(v[0], m[0][1]) + _IQ19mpy(v[1], m[1][1]) + _IQ19mpy(v[2], m[2][1]),
					_IQ19mpy(v[0], m[0][2]) + _IQ19mpy(v[1], m[1][2]) + _IQ19mpy(v[2], m[2][2]) };

				FIX r = _IQ19sqrt(_IQ19mpy(u[0], u[0]) + _IQ19mpy(u[1], u[1]));

				//	ofs << _IQ19toF(u[0]) << " " << _IQ19toF(u[1]) << " " << _IQ19toF(u[2]) << " " << _IQ19toF(r) << endl;

				src[0] = _IQ19div(_IQ19mpy(_IQ19abs(_IQ19mpy(_IQ19(sign(r)), _IQ19mpy(mpdistance, _IQ19atan2(r, u[2])))), u[0]), r);
				src[1] = _IQ19div(_IQ19mpy(_IQ19abs(_IQ19mpy(_IQ19(sign(r)), _IQ19mpy(mpdistance, _IQ19atan2(r, u[2])))), u[1]), r);

			//	if (abs(x - 750) < 2 && y < 10)
			//		ofs << _IQ19toF(src[0]) << " " << _IQ19toF(src[1]) << " " << _IQ19toF(u[0]) << " " << _IQ19toF(u[1]) << " " << _IQ19toF(u[2]) << " " << _IQ19toF(r) << endl;
				/****************溢出**********************/
				//r = sqrt(src[0] * src[0] + src[1] * src[1]);
				//r = _IQ19sqrt(_IQ19mpy(src[0], src[0]) + _IQ19mpy(src[1], src[1]));
				if (_IQ19abs(src[1]) < _IQ19div(_IQ19abs(src[0]), _IQ19(4090)) && _IQ19abs(src[1]) < _IQ19abs(src[0]))
				{
					r = _IQ19abs(src[0]);
				}
				else
				{
					if (_IQ19abs(src[0]) < _IQ19div(_IQ19abs(src[1]), _IQ19(4090)) && (_IQ19abs(src[0])) <(_IQ19abs(src[1])))
					{
						r = _IQ19abs(src[1]);
					}
					else
					{
						r = _IQ19mpy(_IQ19sqrt(_IQ19abs(src[0])), _IQ19mpy(_IQ19sqrt(_IQ19abs(src[1])), _IQ19sqrt(_IQ19abs(_IQ19div(src[0], src[1])) + _IQ19abs(_IQ19div(src[1], src[0])))));
					}
				}
				
				/*float f1 = _IQ19toF(src[0]);
				float f2 = _IQ19toF(src[1]);
				r = _IQ19(sqrt(f1 * f1 + f2 * f2));*/

				FIX theta = _IQ19div(r, mpdistance);


				FIX s = _IQ19mpy(_IQ19abs(_IQ19(sign(theta))), _IQ19div(_IQ19sin(theta), r)) + _IQ19div((_IQ19(1) - _IQ19abs(_IQ19(sign(theta)))), mpdistance);


				//ofs << _IQ19toF(src[0]) << " " << _IQ19toF(src[1]) << " " << _IQ19toF(r) << " " << _IQ19toF(s) << " " << _IQ19toF(theta) << endl;

				v[1] = _IQ19mpy(s, src[0]);
				v[0] = _IQ19cos(theta);

				src[0] = _IQ19mpy(mpdistance, _IQ19atan2(v[1], v[0]));


				/****************溢出**********************/
				/*float fmpd = _IQ19toF(mpdistance);
				float fs = _IQ19toF(s);
				float fsrc1 = _IQ19toF(src[1]);
				float fv0 = _IQ19toF(v[0]);
				float fv1 = _IQ19toF(v[1]);
				src[1] = _IQ19(fmpd*atan(fs*fsrc1 / sqrt(fv0 * fv0 + fv1 * fv1)));*/
				FIX vr;
				if (_IQ19abs(v[1]) < _IQ19div(_IQ19abs(v[0]), _IQ19(4090)) && _IQ19abs(v[1]) < _IQ19abs(v[0]))
				{
					vr = _IQ19abs(v[0]);
				}
				else
				{
					if (_IQ19abs(v[0]) < _IQ19div(_IQ19abs(v[1]), _IQ19(4090)) && (_IQ19abs(v[0])) <(_IQ19abs(v[1])))
					{
						vr = _IQ19abs(v[1]);
					}
					else
					{
						vr = _IQ19mpy(_IQ19sqrt(_IQ19abs(v[0])), _IQ19mpy(_IQ19sqrt(_IQ19abs(v[1])), _IQ19sqrt(_IQ19abs(_IQ19div(v[0], v[1])) + _IQ19abs(_IQ19div(v[1], v[0])))));
					}
				}
				src[1] = _IQ19mpy(mpdistance, _IQ19atan2(_IQ19mpy(src[1], _IQ19div(s, vr)), _IQ19(1)));
				//src[1] = _IQ19mpy(mpdistance, _IQ19atan2(_IQ19mpy(s, _IQ19div(src[1], _IQ19sqrt(_IQ19mpy(v[0], v[0]) + _IQ19mpy(v[1], v[1])))), _IQ19(1)));

				//ofs << _IQ19toF(src[0]) << " " << _IQ19toF(src[1]) << " " << _IQ19toF(vr) << " " << _IQ19toF(s) << endl;

				src[0] = src[0] + halfofwidth;
				src[1] = src[1] + halfofheight;

				if (_IQ19toF(src[1]) < 1)
					src[1] = _IQ19(1);
				if (_IQ19toF(src[1]) > pnwidth0 / 2)
					src[1] = halfofwidth;
				if (_IQ19toF(src[0]) < 1)
					src[0] = _IQ19(1);
				if (_IQ19toF(src[0]) > pnwidth0)
					src[0] = pnwidth;

				//ofs << _IQ19toF(src[0]) << " " << _IQ19toF(src[1]) << " " << _IQ19toF(r) << " " << _IQ19toF(s) << " " << _IQ19toF(theta)
				//	<< " " << _IQ19toF(v[0]) << " " << _IQ19toF(v[1]) << endl;
				//最近邻插值
				int dx = int((_IQ19toF(src[1]) - 0.5))*step;
				int dy = int((_IQ19toF(src[0]) - 0.5))*channels;

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

	cout << sizeof(long) << " " << sizeof(int) << " " << sizeof(long long) << endl;
	string filename = "1.jpeg";
	float ar, ap, ay;
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