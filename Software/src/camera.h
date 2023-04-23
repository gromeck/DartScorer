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
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"

using namespace cv;

#define CAMS				2
#define CAPTURE_MAX_WIDTH	1200
#define CAPTURE_WIDTH		CAPTURE_MAX_WIDTH
#define CAPTURE_HEIGHT		400

typedef struct {
	int flip_h;
	int flip_v;
	int grey;
	int crop;
	Rect crop_area;
} CAM_CFG;

typedef struct {
	int index;				// the camera index
    VideoCapture cap;		// capture interface
	Mat frame;				//	frame buffer
	char windowTitle[100];	// title of the window when the frame is displayed
	CAM_CFG config;
} CAMERA;

bool camera_init(void);
bool camera_get_frame(int cam);

#endif

/**/
