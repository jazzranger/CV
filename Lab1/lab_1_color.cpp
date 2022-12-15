#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include<conio.h> 

using namespace std;
using namespace cv;


template <class T>
int findValue(const cv::Mat& mat, T value) {
	int count = 0;
	for (int i = 0; i < mat.rows; i++) {
		const T* row = mat.ptr<T>(i);
		if (std::find(row, row + mat.cols, value) != row + mat.cols) { count++; }
	}
	return count;
}

int main()
{
	VideoCapture vid_capture("C:/Users/whr1t/Desktop/Repos/CV/videoplayback.mp4");

	int x = 50;
	int y = 50;
	int width = 150;
	int height = 150;
	int thickness = 8;

	Scalar lower_red(0, 70, 50);
	Scalar upper_red(10, 255, 255);

    Scalar lower_green(40,40,40);
	Scalar upper_green(70,255,255);

	Scalar lower_blue(100, 150, 0);
	Scalar upper_blue(140, 255, 255);

    Scalar lower_yellow(25,50,50);
	Scalar upper_yellow(32,255,255);

	if (!vid_capture.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
	}

	else
	{
		int fps = vid_capture.get(5);
		cout << "Frames per second :" << fps;

		int frame_count = vid_capture.get(7);
		cout << "  Frame count :" << frame_count;
	}

	while (vid_capture.isOpened())
	{
		int key_arr = waitKeyEx(20);
		Rect rect(x, y, width, height);
		Mat frame, frame_copy,roi_image, roi_image_hsv;
		Mat mask_blue, mask_green, mask_red, mask_yellow;
		Mat clean_blue;
		
		int count_blue, count_red, count_green, count_yellow;
		bool isSuccess = vid_capture.read(frame);

		frame.copyTo(frame_copy);

		if (isSuccess == true)
		{
			roi_image = frame_copy(rect);

			cvtColor(roi_image, roi_image_hsv, COLOR_BGR2HSV);

			inRange(roi_image_hsv, lower_blue, upper_blue, mask_blue);
			inRange(roi_image_hsv, lower_green, upper_green, mask_green);
			inRange(roi_image_hsv, lower_red, upper_red, mask_red);
			inRange(roi_image_hsv, lower_yellow, upper_yellow, mask_yellow);

			count_blue = findValue(mask_blue, Vec3b(255, 255, 255));
			count_red = findValue(mask_red, Vec3b(255, 255, 255));
			count_green = findValue(mask_green, Vec3b(255, 255, 255));
			count_yellow = findValue(mask_yellow, Vec3b(255, 255, 255));

			int i, n;
            int c[4] = {count_blue,count_red,count_green,count_yellow};
            string col[4] = {"Blue","Red","Green","Yellow"};
            string res = "Unknown";
            for(i = 1;i < 4; ++i) {
                if(c[0] < c[i]) {
                    c[0] = c[i];
                    res = col[i];
                }
            }
            putText(frame, res, Point(x + width, y + height), FONT_HERSHEY_SIMPLEX, 1, Scalar(124, 252, 0));

			rectangle(frame, rect, cv::Scalar(0, 0, 255), thickness);
			imshow("Frame", frame);

			if (key_arr == 2424832) //left
			    { x -= 10;}
			if (key_arr == 2555904) //right
			    { x += 10; }
			if (key_arr == 2490368) //up
			    { y -= 10;}
			if (key_arr == 2621440) //down
			    { y += 10; }
			if (key_arr == 27) //esc
			    { break; }
		}
	}
	vid_capture.release();
	destroyAllWindows();
	return 0;
}
