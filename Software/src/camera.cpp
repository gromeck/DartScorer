/*

	DartScorer

	(c) 2023 by Christian.Lorenz@gromeck.de

	This file is part of DartScorer

    DartScorer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DartScorer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DartScorer.  If not, see <https://www.gnu.org/licenses/>.

*/
#include "common.h"
#include <opencv2/core/types_c.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "DartScorer.h"
#include "camera.h"

using namespace cv;
using namespace std;


//static int _cams = 0;
static CAMERA _camera[CAMS];

bool _showCamWindows = false;

/*
**	stuff to handle mouse selection
*/
bool _selectObject = false;
Point _origin;
Rect _selection;
bool _crop = false;

#if 0
static void onMouse(int event,int x,int y,int,void *)
{
	if (_selectObject) {
		_selection.x = MIN(x,_origin.x);
		_selection.y = MIN(y,_origin.y);
		_selection.width = std::abs(x - _origin.x);
		_selection.height = std::abs(y - _origin.y);
//		_selection &= Rect(0,0,image.cols,image.rows);
	}
	switch( event ) {
		case EVENT_LBUTTONDOWN:
			_origin = Point(x,y);
			_selection = Rect(x,y,0,0);
			_selectObject = true;
			break;
		case EVENT_LBUTTONUP:
			_selectObject = false;
			if (_selection.width > 0 && _selection.height > 0)
				_crop = true;
			break;
	}
}
#endif

bool camera_settings_store()
{
	FILE *fd;

	if ((fd = fopen(".dartscorer.cfg","w+"))) {
		for (int cam = 0;cam < CAMS;cam++) {
			fprintf(fd,"%d:%d:%d:%d:%d:%d:%d:%d\n",
				cam,
				_camera[cam].config.flip_h,
				_camera[cam].config.flip_v,
				_camera[cam].config.crop,
				_camera[cam].config.crop_area.x,
				_camera[cam].config.crop_area.y,
				_camera[cam].config.crop_area.width,
				_camera[cam].config.crop_area.height);
			printf("Stored settings for cam %d\n",cam);
		}
		fclose(fd);
		return true;
	}
	return false;
}

bool camera_settings_restore()
{
	FILE *fd;

	if ((fd = fopen(".dartscorer.cfg","r"))) {
		int cam;
		CAM_CFG config;
		while (fscanf(fd,"%d:%d:%d:%d:%d:%d:%d:%d\n",
				&cam,
				&config.flip_h,
				&config.flip_v,
				&config.crop,
				&config.crop_area.x,
				&config.crop_area.y,
				&config.crop_area.width,
				&config.crop_area.height) != EOF) {
			if (cam >= 0 && cam < CAMS) {
				printf("Restoring settings for cam %d\n",cam);
				_camera[cam].config = config;
			}
		}
		fclose(fd);
		return true;
	}
	return false;
}

bool camera_init(void)
{
	/*
	**	init the cameras
	*/
	for (int cam = 0;cam < CAMS;cam++) {
		/*
		* open the cameras
		*/
		printf("Opening cam[%d] width device index %d ***\n",cam,_camera[cam].index);
		//cap[cm].open(cam_idx[cam],cv::CAP_IMAGES);
		_camera[cam].cap.open(_camera[cam].index);
		if (!_camera[cam].cap.isOpened()) {
			printf("Could not open cam %d for capturing\n",cam);
			exit(-1);
		}
		printf("capturing initialized width cam %d\n",cam);

		/*
		**	create the window
		*/
		sprintf(_camera[cam].windowTitle,"%s - Cam %d",PACKAGE_NAME,cam);
		if (_showCamWindows)
			namedWindow(_camera[cam].windowTitle,true);
	}
	camera_settings_restore();
	return true;
}

bool camera_get_frame(int cam)
{
	/*
	**	get frames from the camera
	*/
	if (_camera[cam].cap.isOpened() && !_camera[cam].cap.read(_camera[cam].frame)) {
		printf("Couldn't get frame from cam %d (cam open=%d)\n",cam,_camera[cam].cap.isOpened());
		return false;
	}
	printf("Grabbed frame from cam %d\n",cam);

	/*
	**	flip the image if necessary
	*/
	if (_camera[cam].config.flip_h && _camera[cam].config.flip_v)
		cv::flip(_camera[cam].frame,_camera[cam].frame,-1);
	else if (_camera[cam].config.flip_h)
		cv::flip(_camera[cam].frame,_camera[cam].frame,1);
	else if (_camera[cam].config.flip_v)
		cv::flip(_camera[cam].frame,_camera[cam].frame,0);

	/*
	**	scale the image
	*/
	double aspect = (double) _camera[cam].frame.cols / (double) _camera[cam].frame.rows;
	cv::resize(_camera[cam].frame,_camera[cam].frame,Size(CAPTURE_MAX_WIDTH,CAPTURE_MAX_WIDTH / aspect));

	/*
	**	crop the center part
	*/
	printf("Cropping [%d] rows=%d  cols=%d  ***\n",cam,_camera[cam].frame.rows,_camera[cam].frame.cols);
	if (_camera[cam].config.crop)
		_camera[cam].frame = _camera[cam].frame(_camera[cam].config.crop_area);

	return true;
}

#if 0
/*
**	try the camera calibration
*/
bool camera_calibration(void)
{
	int cam = 0;

	int hmin =  90,hmax = 100;
	int smin = 100,smax = 255;
	int vmin = 100,vmax = 255;

    namedWindow(CALIBRATION_WINDOW_TITLE,true);
	setMouseCallback(CALIBRATION_WINDOW_TITLE, onMouse, 0 );
	//createTrackbar("H(min)", CALIBRATION_WINDOW_TITLE, &hmin, 180, 0 );
	//createTrackbar("H(max)", CALIBRATION_WINDOW_TITLE, &hmax, 180, 0 );
	//createTrackbar("S(min)", CALIBRATION_WINDOW_TITLE, &smin, 255, 0 );
	//createTrackbar("S(max)", CALIBRATION_WINDOW_TITLE, &smax, 255, 0 );
	//createTrackbar("V(min)", CALIBRATION_WINDOW_TITLE, &vmin, 255, 0 );
	//createTrackbar("V(max)", CALIBRATION_WINDOW_TITLE, &vmax, 255, 0 );

	int tmin = 230,tmax = 255;
	createTrackbar("Threashold(min)", CALIBRATION_WINDOW_TITLE, &tmin, 255, 0 );
	createTrackbar("Threashold(max)", CALIBRATION_WINDOW_TITLE, &tmax, 255, 0 );

	int minArea = 100;
	int minCircularity = 50, minConvexity = 50, minInertiaRatio = 50;
	int maxCircularity = 100, maxConvexity = 100, maxInertiaRatio = 100;
	createTrackbar("minArea", CALIBRATION_WINDOW_TITLE, &minArea, 2000, 0 );
	createTrackbar("minCircularity", CALIBRATION_WINDOW_TITLE, &minCircularity, 100, 0 );
	createTrackbar("maxCircularity", CALIBRATION_WINDOW_TITLE, &maxCircularity, 100, 0 );
	createTrackbar("minConvexity", CALIBRATION_WINDOW_TITLE, &minConvexity, 100, 0 );
	createTrackbar("maxConvexity", CALIBRATION_WINDOW_TITLE, &maxConvexity, 100, 0 );
	createTrackbar("minInertiaRatio", CALIBRATION_WINDOW_TITLE, &minInertiaRatio, 100, 0 );
	createTrackbar("maxInertiaRatio", CALIBRATION_WINDOW_TITLE, &maxInertiaRatio, 100, 0 );

	printf("Entering calibration\n");
	bool find_pins = false;

    for(;;) {
		/*
		**	get the frame
		*/
		camera_get_frame(cam);

		if (find_pins) {
			/*
			**	greyscale
			*/
			cv::cvtColor(_camera[cam].frame,_camera[cam].frame,cv::COLOR_BGR2GRAY);

			/*
			**	thresholding
			*/
			cv::threshold(_camera[cam].frame,_camera[cam].frame, tmin,tmax,cv::THRESH_BINARY_INV);

#if 0
			cv::Mat hsv;
			cv::cvtColor(_camera[cam].frame, hsv, cv::COLOR_BGR2HSV);  // Konvertiere das Bild in HSV-Farraum

			/*
			**	create mask for red
			*/
			cv::Mat mask;
			cv::inRange(hsv, cv::Scalar(hmin,smin,vmin), cv::Scalar(hmax,smax,vmax), mask);

			/*
			**	find contours matching the mask
			*/
			std::vector<std::vector<cv::Point>> contours;
			cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

			// Zeichne einen Kreis um jede gefundene Kugel
			for (const auto& contour : contours) {
				cv::Point2f center;
				float radius;

				cv::minEnclosingCircle(contour, center, radius);
				if (1 || radius > 5 && radius < 50) {
					cv::circle(_camera[cam].frame, center, static_cast<int>(radius), cv::Scalar(0,0,255), 2);
				}
			}
#else
			// Konfiguriere die Detektorparameter
			cv::SimpleBlobDetector::Params params;
			params.minThreshold = 10;
			params.maxThreshold = 100;

			params.filterByArea = true;
			params.minArea = minArea;
			params.filterByCircularity = true;
			params.minCircularity = minCircularity / 100.0;
			params.maxCircularity = maxCircularity / 100.0;
			params.filterByConvexity = true;
			params.minConvexity = minConvexity / 100.0;
			params.maxConvexity = maxConvexity / 100.0;
			params.filterByInertia = true;
			params.minInertiaRatio = minInertiaRatio / 100.0;
			params.maxInertiaRatio = maxInertiaRatio / 100.0;
			Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

			// Detect blobs.
			std::vector<KeyPoint> keypoints;
			detector->detect(_camera[cam].frame,keypoints);

			for (const cv::KeyPoint& keypoint : keypoints) {
				printf("Keypoint: x=%d  y=%d\n",keypoint.pt.x,keypoint.pt.y);
			}
			drawKeypoints(_camera[cam].frame, keypoints, _camera[cam].frame, cv::Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
#endif
		}

		/*
		**	add text to the image
		*/
		char text[1000];

		cv::rectangle(_camera[cam].frame,cv::Point(0,0),cv::Point(_camera[cam].frame.cols,25), CV_RGB(255,255,255),-1);
		sprintf(text,"CAM #%d: %dx%dpx",cam,_camera[cam].frame.cols,_camera[cam].frame.rows);
		cv::putText(_camera[cam].frame,text,Point(10,20),cv::FONT_HERSHEY_PLAIN,1,CV_RGB(0,0,0),1);

		/*
		**	display this image
		*/
		if (_selectObject && _selection.width > 0 && _selection.height > 0) {
			Mat roi(_camera[cam].frame,_selection);
			bitwise_not(roi,roi);
		}
		imshow(CALIBRATION_WINDOW_TITLE,_camera[cam].frame);
 
		/*
		**	wait for user interaction
		*/
        char c = (char) waitKey(10);
        if (c == 27 || c == 'q')
            break;
		switch (c) {
			case 'c':
				cam = (cam + 1) % CAMS;
				break;
			case 'v':
				_camera[cam].config.flip_h = !_camera[cam].config.flip_h;
				break;
			case 'h':
				_camera[cam].config.flip_v = !_camera[cam].config.flip_v;
				break;
			case 'f':
				find_pins = !find_pins;
				break;
			case 'u':
				_camera[cam].config.crop = false;
				break;
			case 'r':
				camera_settings_restore();
				break;
			case 's':
				camera_settings_store();
				break;
		}
		if (_crop) {
			/*
			**	user set a cropping area
			*/
			_camera[cam].config.crop = true;
			_camera[cam].config.crop_area = _selection;
			_crop = false;
		}
    }
    destroyWindow(CALIBRATION_WINDOW_TITLE);
	return true;
}

/**/
#endif
