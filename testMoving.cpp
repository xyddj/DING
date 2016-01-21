#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include "testMoving.h"

using namespace std;
using namespace cv;

int main()
{
	//! The test video sequence ID
	int seqId = 7;
	int f_s;
	int f_e;
	switch (seqId)
	{
	case 1:
		f_s = 300;
		f_e = 510;
		break;
	case 2:
		f_s = 300;
		f_e = 500;
		break;
	case 3:
		f_s = 300;
		f_e = 1000;
		break;
	case 4:
		f_s = 0;
		f_e = 1000;
		break;
	case 5:
		f_s = 300;
		f_e = 500;
		break;
	case 6:
		f_s = 100;
		f_e = 200;
		break;
	case 7:
		f_s = 0;
		f_e = 200;
		break;
	default:
		seqId = 1;
		f_s = 300;
		f_e = 510;
		break;
	};

	int widthOriginal = WIDTH_IMAGE;
	int heightOriginal = HEIGHT_IMAGE;
	int widthSample = WIDTH_SAMPLE;
	int heightSample = HEIGHT_SAMPLE;
	int numBlockX = NUMBER_BLOCK_X;
	int numBlockY = NUMBER_BLOCK_Y;
	int scaleSample = SCALE_SAMPLE;
	int numberStore = NUMBER_STORE;
	int thresholdForeground = THRESHOLD_FOREGROUND;
	int thresholdChangedPersent = THRESHOLD_CHANGED_PERSENT;
	int thresholdComplex = THRESHOLD_COMPLEX;
	int thresholdUncomplex = THRESHOLD_UNCOMPLEX;
	//! Image name
	char* fileInput = "D://image//moving//%d//%06d.bmp";
	char nameInput[200];
	Mat matOriginal;

	///////////////////////////// STORE SPACE ///////////////////////////////////
	unsigned char ** leftGroupStore = NULL;
	unsigned char ** rightGroupStore = NULL;
	generateStoreSpace(leftGroupStore, numberStore, numBlockX, numBlockY);
	generateStoreSpace(rightGroupStore, numberStore, numBlockX, numBlockY);
	/////////////////////////////////////////////////////////////////////////////
	
	//! Data input
	const unsigned char* dataOriginal;
	for (int numFrame = 0; numFrame < 1000; numFrame++)
	{
		int frameID = numFrame + f_s;
		if (frameID > f_e)
		{
			break;
		}
		sprintf(nameInput, fileInput, seqId, frameID);
		matOriginal = imread(nameInput, 1);
		if (NULL == matOriginal.data) break;
		imshow("Original", matOriginal);

		///////////////////////********** IMAGE INPUT *********** //////////////////////
		dataOriginal = (const unsigned char*)(matOriginal.data);

		///////////////////// MAIN_FUNCTION //////////////////////////////
		int resultMovement = detectVehicleMovement(dataOriginal,
			leftGroupStore,
			rightGroupStore, 
			widthOriginal,
			heightOriginal,
			widthSample,
			heightSample,
			numBlockX,
			numBlockY,
			scaleSample,
			numberStore,
			thresholdForeground,
			thresholdChangedPersent,
			thresholdComplex,
			thresholdUncomplex,
			numFrame
			);

		///////////////////////////// OUTPUT ///////////////////////////////
		if (numFrame>numberStore && resultMovement)
			printf("************************ M_O_V_E ***********************\n");
		else
			printf("------------------------ S_T_O_P ----------------------\n");

		if (27 == waitKey(2))
			break;
	}
	
	///////////////////////////// STORE SPACE ///////////////////////////////////
	releaseStoreSpace(leftGroupStore, numberStore);
	releaseStoreSpace(rightGroupStore, numberStore);
	/////////////////////////////////////////////////////////////////////////////

	return 1;
};
