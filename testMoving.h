#ifndef TESTMOVING_H
#define TESTMOVING_H 1
//! Only for showing images
//#include <opencv2/opencv.hpp>
//using namespace cv;

#include <malloc.h>
#include <math.h>

//! Resize image 
#define SCALE_SAMPLE (2)
//! Original image's size
#define WIDTH_IMAGE (1920)
#define HEIGHT_IMAGE (1080)
//! After resized image's sample size
#define WIDTH_SAMPLE ((WIDTH_IMAGE/SCALE_SAMPLE)*20/100)
#define HEIGHT_SAMPLE ((HEIGHT_IMAGE/SCALE_SAMPLE)*20/100)

//! Stored frame number
#define NUMBER_STORE 10
//! Percent of pixels to judge moving or not
#define THRESHOLD_CHANGED_PERSENT (5)
//! a fixed value to remove small diff (only diff bigger than this value is stored)
#define THRESHOLD_FOREGROUND (20)
//! a fixed value to check day or night 
#define THRESHOLD_COLOR (100)
//! a fixed value to judge the complex of the image
#define THRESHOLD_COMPLEX (500)
//! a fixed value to judge the NO complex of the image
#define THRESHOLD_UNCOMPLEX (100)

//! image sampled size
#define NUMBER_BLOCK_X (20)
#define NUMBER_BLOCK_Y (20)

/**
Detect if the vehicle is moved or not using left and right sample image

@param [in] dataOriginal the big color image
@param [in] leftGroupStore left stored group
@param [in] rightGroupStore right stored group
@param [in] widthOriginal width of original image
@param [in] heightOriginal height of original image
@param [in] widthSample width of resized image(step 1 of sampling)
@param [in] heightSample height of resized image(step 1 of sampling)
@param [in] numBlockX width of down scaling image(step 2 of sampling)
@param [in] numBlockY height of down scaling image(step 2 of sampling)
@param [in] scaleSample resized scale
@param [in] numberStore stored image number of the group stored images
@param [in] thresholdForeground to judge small change
@param [in] thresholdChangedPersent to judge if move or not
@param [in] thresholdComplex to judge the variance (big)
@param [in] thresholdUncomplex to judge the variance (small)
@param [in] numFrame the current frame number to judge the updated image
@return 1: Move 0: Stop
*/
bool detectVehicleMovement(const unsigned char* dataOriginal,unsigned char** leftGroupStore,unsigned char** rightGroupStore,
	int widthOriginal,int heightOriginal,int widthSample,int heightSample,int numBlockX,int numBlockY,
	int scaleSample, int numberStore, int thresholdForeground, int thresholdChangedPersent, int thresholdComplex, int thresholdUncomplex, int numFrame);

/**
Get the sample region using 2 times down sampling
step1: Using points sample(like resize)
step2: Using average value of block (like discretization)

@param [in]  dataOriginal input data of the big image(Color)
@param [out] dataSample sampled data after 2 steps(usually a small gray image)
@param [out] valueAver the average value of this sample image
@param [out] valueVariance variance value of this image
@param [in] wOriginal input image's w size
@param [in] hOriginal input image's h size
@param [in] wSample resized image w size
@param [in] hSample resized image h size
@param [in] nBlockX downscaled image w size
@param [in] nBlockY downscaled image h size
@param [in] xStart x start point of the sample region
@param [in] yStart y point of the sample region
@param [in] sSample resize scale
@return gray summed value of the result image
*/
void getSampleRegion(const unsigned char* dataOriginal,
	unsigned char* dataSample,int & valueAver,int & valueVariance,int wOriginal,
	int hOriginal,int wSample,int hSample,int nBlockX,int nBlockY,int xStart,int yStart,int sSample);

/**
Generate the stored image space

@param [out] dataGroupStore data used for history stored (very small size)
@param [in] numberStore the stored image total number
@param [in] widthSample stored image w size
@param [in] heightSample stored image h size
@return void
*/
void generateStoreSpace(unsigned char** &dataGroupStore, int numberStore, int widthSample, int heightSample);

/**
Release the space

@param [in] dataGroupStore data used for history stored (very small size)
@param [in] numStore the stored image total number
@return void
*/
void releaseStoreSpace(unsigned char** &dataGroupStore, int numStore);

/**
1. Update the stored space( several history images)
2. Using the history image to get the average image

@param [in] dataCurrentFrame the current frame's data
@param [out] dataGroupStore stored group images
@param [out] dataStore average image of the group stored images
@param [in] wSample image w size
@param [in] hSample image h size
@param [in] idStore this is the updated image id(I used the frame number to get it)
@param [in] numStore stored image number
@return void
*/
void updateStoreValue(unsigned char* dataCurrentFrame, 
	unsigned char** dataGroupStore,
	unsigned char* dataStore,
	int wSample, 
	int hSample,
	int idStore,
	int numStore
	);

/**
Calculate the difference of the current image and the stored image

@param [in] dataCurrentFrame current sample image
@param [in] dataStore stored image
@param [out] dataDiff difference image
@param [in] wSample image w size
@param [in] hSample image h size
@param [in] thresholdForeground threshold for judging if it is changed or not
@return changed pixel percent
*/
int getDiffImage(unsigned char* dataCurrentFrame, unsigned char* dataStore,
	unsigned char* dataDiff,
	int wSample, 
	int hSample,
	unsigned char thresholdForeground
	);
/**
Using the percent of left and right to get the final result

@param [in] pLeftDiffPersent left changed percent
@param [in] pRightDiffPersent right changed percent
@param [in] thresholdF final percent of moving or not
@param [in] varianceLeft variance of left image
@param [in] varianceRight variance of right image
@param [in] thresholdC complex threshold
@param [in] thresholdUnC uncomplex threshold

@return 1: move 2: stop
*/
bool outputResult(int pLeftDiffPersent, int pRightDiffPersent, int thresholdF,
	int varianceLeft, int varianceRight, int thresholdC, int thresholdUnC);

//////////////////////////// FUNCTIONS //////////////////////////////////////////////

/**
Detect if the vehicle is moved or not using left and right sample image

@param [in] dataOriginal the big color image
@param [in] leftGroupStore left stored group
@param [in] rightGroupStore right stored group
@param [in] widthOriginal width of original image
@param [in] heightOriginal height of original image
@param [in] widthSample width of resized image(step 1 of sampling)
@param [in] heightSample height of resized image(step 1 of sampling)
@param [in] numBlockX width of down scaling image(step 2 of sampling)
@param [in] numBlockY height of down scaling image(step 2 of sampling)
@param [in] scaleSample resized scale
@param [in] numberStore stored image number of the group stored images
@param [in] thresholdForeground to judge small change
@param [in] thresholdChangedPersent to judge if move or not
@param [in] thresholdComplex to judge the variance (big)
@param [in] thresholdUncomplex to judge the variance (small)
@param [in] numFrame the current frame number to judge the updated image
@return 1: Move 0: Stop
*/
bool detectVehicleMovement(const unsigned char* dataOriginal, unsigned char** leftGroupStore, unsigned char** rightGroupStore,
	int widthOriginal, int heightOriginal, int widthSample, int heightSample, int numBlockX, int numBlockY,
	int scaleSample, int numberStore, int thresholdForeground, int thresholdChangedPersent, int thresholdComplex, int thresholdUncomplex, int numFrame)
{
	//! Final result
	bool result = 0;

	//! Left and right sample
	unsigned char* dataSampleLeft = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);
	unsigned char* dataSampleRight = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);

	//! Diff result
	unsigned char* dataSampleLeftDiff = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);
	unsigned char* dataSampleRightDiff = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);

	//! Stored result
	unsigned char* dataSampleLeftStore = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);
	unsigned char* dataSampleRightStore = (unsigned char*)malloc(sizeof(char)*numBlockX*numBlockY);

	//! Step 1
	//! Get sample for left part
	int valueAverLeft,valueVarianceLeft;
	getSampleRegion(dataOriginal, dataSampleLeft, valueAverLeft, valueVarianceLeft,
		widthOriginal, heightOriginal, widthSample, heightSample,
		numBlockX,numBlockY, 0, 0, scaleSample);
	//! Get sample for right part
	int valueAverRight, valueVarianceRight;
	getSampleRegion(dataOriginal, dataSampleRight, valueAverRight, valueVarianceRight,
		widthOriginal, heightOriginal,
		widthSample, heightSample, numBlockX, numBlockY, widthOriginal - widthSample*scaleSample, 0, scaleSample);

	//! This is the average result to judge day or night 
	int valueAverTotal = (valueAverLeft + valueAverRight) / 2;
	int valueVarianceTotal = (valueVarianceLeft + valueVarianceRight) / 2;
	printf("L: a:%d v:%d R: a:%d v:%d T: a:%d v:%d\n",
		valueAverLeft, valueVarianceLeft, valueAverRight, valueVarianceRight, valueAverTotal, valueVarianceTotal);

	//! Step 2
	//! Update the data for left right
	updateStoreValue(dataSampleLeft, leftGroupStore, dataSampleLeftStore,
		numBlockX, numBlockY,
		numFrame%numberStore,
		numberStore
		);
	updateStoreValue(dataSampleRight, rightGroupStore, dataSampleRightStore,
		numBlockX, numBlockY,
		numFrame%numberStore,
		numberStore
		);

	//! Step 3
	//! Get left and right diff
	int pLeftDiff = getDiffImage(dataSampleLeft, dataSampleLeftStore,
		dataSampleLeftDiff,
		numBlockX, 
		numBlockY,
		thresholdForeground
		);
	int pRightDiff = getDiffImage(dataSampleRight, dataSampleRightStore,
		dataSampleRightDiff,
		numBlockX,
		numBlockY,
		thresholdForeground
		);
	printf("Left Difference Percent: %d Right Difference Percent: %d\n", pLeftDiff, pRightDiff);
	
	//! Show image(Need OpenCV)
	////!¡¡This frame's up left and up right
	//Mat imageSampleUpLeft_Current = Mat::zeros(numBlockY, numBlockX, CV_8UC1);
	//Mat imageSampleUpRight_Current = Mat::zeros(numBlockY, numBlockX, CV_8UC1);
	//imageSampleUpLeft_Current.data = dataSampleLeft;
	//imageSampleUpRight_Current.data = dataSampleRight;
	//imshow("left sample", imageSampleUpLeft_Current);
	//imshow("right sample", imageSampleUpRight_Current);

	free(dataSampleLeftStore);
	dataSampleLeftStore = NULL;
	free(dataSampleRightStore);
	dataSampleRightStore = NULL;

	free(dataSampleLeft);
	dataSampleLeft = NULL;
	free(dataSampleRight);
	dataSampleRight = NULL;

	free(dataSampleLeftDiff);
	dataSampleLeftDiff = NULL;
	free(dataSampleRightDiff);
	dataSampleRightDiff = NULL;

	//! Step 4 
	//! analysis the result
	result = outputResult(pLeftDiff, pRightDiff, thresholdChangedPersent, valueVarianceLeft, valueVarianceRight,thresholdComplex,thresholdUncomplex );
	return result;
};

/**
Generate the stored image space

@param [out] dataGroupStore history stored (very small size)
@param [in] numberStore the stored image number
@param [in] widthSample stored image w size
@param [in] heightSample stored image h size
@return void
*/
void generateStoreSpace(unsigned char** &dataGroupStore, int numberStore, int widthSample, int heightSample)
{
	//! apply data space
	dataGroupStore = (unsigned char**)malloc(sizeof(char*)*numberStore);
	for (int k = 0; k < numberStore; k++)
	{
		dataGroupStore[k] = (unsigned char*)malloc(sizeof(char)*widthSample*heightSample);
	}
};

/**
Release the space

@param [in] dataGroupStore data used for history stored (very small size)
@param [in] numStore the stored image total number
@return void
*/
void releaseStoreSpace(unsigned char** &dataGroupStore, int numStore)
{
	for (int k = 0; k < numStore; k++)
	{
		free(dataGroupStore[k]);
		dataGroupStore[k] = NULL;
	}
	free(dataGroupStore);
	dataGroupStore = NULL;
};

/**
Get the sample region using 2 times down sampling
step1: Using points sample(like resize)
step2: Using average value of block (like discretization)

@param [in]  dataOriginal input data of the big image(Color)
@param [out] dataSample sampled data after 2 steps(usually a small gray image)
@param [out] valueAver the average value of this sample image
@param [out] valueVariance variance value of this image
@param [in] wOriginal input image's w size
@param [in] hOriginal input image's h size
@param [in] wSample resized image w size
@param [in] hSample resized image h size
@param [in] nBlockX downscaled image w size
@param [in] nBlockY downscaled image h size
@param [in] xStart x start point of the sample region
@param [in] yStart y point of the sample region
@param [in] sSample resize scale
@return gray summed value of the result image
*/
void getSampleRegion(const unsigned char* dataOriginal,
	unsigned char* dataSample,
	int & valueAver,
	int & valueVariance,
	int wOriginal, 
	int hOriginal, 
	int wSample, 
	int hSample, 
	int nBlockX,
	int nBlockY,
	int xStart, 
	int yStart, 
	int sSample
	)
{
	//! the total value of that region
	int valueSum = 0;

	///////////////////////////////// STEP 1 /////////////////////////////////////////
	//! The resized image from the ROI region
	unsigned char* dataSampleOriginal = (unsigned char*)malloc(sizeof(char)*wSample*hSample);
	for (int i = 0; i < wSample; i++)
	{
		for (int j = 0; j < hSample; j++)
		{
			//! The coordinate in the original image
			int x = xStart + i* sSample;
			int y = yStart + j* sSample;
			//! Check border
			//x = max(0, min(x, wOriginal - 1));
			//y = max(0, min(y, hOriginal - 1));

			//! The current gray value(Using the average value)
			unsigned char valueGray = dataOriginal[wOriginal*y * 3 + 3 * x] / 3 // B
				+ dataOriginal[wOriginal*y * 3 + 3 * x + 1] / 3 // G
				+ dataOriginal[wOriginal*y * 3 + 3 * x + 2] / 3; //R
			
			//! Give the value to the resized image
			dataSampleOriginal[wSample* j + i] = valueGray;		
		}
	}
	
	
	///////////////////////////////// STEP 2 /////////////////////////////////////////
	//! the average region's size
	int stepX = wSample / nBlockX;
	int stepY = hSample / nBlockY;
	for (int i = 0; i < nBlockX; i++)
	{
		for (int j = 0; j < nBlockY; j++)
		{
			//! the start and end point of this little block
			int x_start = i*stepX;
			int x_end = (i + 1)*stepX;
			int y_start = j*stepY;
			int y_end = (j + 1)* stepY;

			int valueBlockSum = 0;
			for (int x = x_start; x < x_end; x++)
			{
				for (int y = y_start; y < y_end; y++)
				{
					valueBlockSum += (int)dataSampleOriginal[y*wSample + x];
				}
			}
			//! Average value of this block
			int valueBlockAver = valueBlockSum / (stepX*stepY);
			//! Sample image
			//! Output 1
			dataSample[j*nBlockX + i] = valueBlockAver;
			valueSum = valueSum + valueBlockAver;
		}
	}

	free(dataSampleOriginal);
	dataSampleOriginal = NULL;

	//! The total average value of the whole image
	//! Output 2
	valueAver = valueSum / (nBlockX*nBlockY);

	//! Get the variance
	int sumVariance = 0;
	for (int i = 0; i < nBlockX; i++)
	{
		for (int j = 0; j < nBlockY; j++)
		{
			int diffValue = (dataSample[j*nBlockX + i] - valueAver);
			sumVariance = sumVariance+ diffValue*diffValue;
		}
	}
	//! The variance of this region
	//! Output 3
	valueVariance = sumVariance / (nBlockX*nBlockY);
};

/**
1. Update the stored space( several history images)
2. Using the history image to get the average image

@param [in] dataCurrentFrame the current frame's data
@param [out] dataGroupStore stored group images
@param [out] dataStore average image of the group stored images
@param [in] wSample image w size
@param [in] hSample image h size
@param [in] idStore this is the updated image id(I used the frame number to get it)
@param [in] numStore stored image number
@return void
*/
void updateStoreValue(unsigned char* dataCurrentFrame, 
	unsigned char** dataGroupStore,
	unsigned char* dataStore,
	int wSample, 
	int hSample, 
	int idStore,
	int nStore
	)
{
	//! Copy current frame's left and right sample
	for (int i = 0; i < wSample; i++)
	{
		for (int j = 0; j < hSample; j++)
		{
			//! Update the group image using the current image
			dataGroupStore[idStore][j* wSample + i] = dataCurrentFrame[j* wSample + i];

			int valueSumStore = 0;
			for (int k = 0; k < nStore; k++)
			{
				valueSumStore = valueSumStore + (int)(dataGroupStore[k][j* wSample + i]);
			}
			//! Give the average to the out image
			dataStore[j* wSample + i] = (unsigned char)(valueSumStore / nStore);
		}
	}
};

/**
Calculate the difference of the current image and the stored image

@param [in] dataCurrentFrame current sample image
@param [in] dataStore stored image
@param [out] dataDiff difference image
@param [in] wSample image w size
@param [in] hSample image h size
@param [in] thresholdForeground threshold for judging change or not
@return changed pixel percent
*/
int getDiffImage(unsigned char* dataCurrentFrame, unsigned char* dataStore,
	unsigned char* dataDiff,
	int wSample,
	int hSample,
	unsigned char thresholdForeground
	)
{
	int sumDiff = 0;
	for (int i = 0; i < wSample; i++)
	{
		for (int j = 0; j < hSample; j++)
		{
			//! The diff of current and history image
			unsigned char valueDiff = abs(dataCurrentFrame[j* wSample + i]
				- dataStore[j* wSample + i]);
			if (valueDiff < thresholdForeground)
			{
				valueDiff = 0;
			}
			else
			{
				sumDiff++;
				valueDiff = 255;
			}
			dataDiff[j* wSample + i] = valueDiff;
		}
	}
	//! The percent of no 0 pixels
	return 100 * sumDiff / (wSample*hSample);
};

/**
Using the percent of left and right to get the final result

@param [in] pLeftDiffPersent left changed percent
@param [in] pRightDiffPersent right changed percent
@param [in] thresholdF final percent of moving or not
@param [in] varianceLeft variance of left image
@param [in] varianceRight variance of right image
@param [in] thresholdC complex threshold
@param [in] thresholdUnC uncomplex threshold

@return 1: move 2: stop
*/
bool outputResult(int pLeftDiffPersent, int pRightDiffPersent, int thresholdF, 
	int varianceLeft, int varianceRight, int thresholdC, int thresholdUnC)
{
	bool resultDiff;
	bool resultComplex;
	bool resultFinal;
	//printf("Left Difference Percent: %d Right Difference Percent: %d\n", pLeftDiffPersent, pRightDiffPersent);
	if (pLeftDiffPersent > thresholdF || pRightDiffPersent > thresholdF)
	{
		//! Diff is big
		//printf("************************ M_O_V_E ***********************\n");
		resultDiff= 1;
	}
	else
	{
		//printf("------------------------ S_T_O_P ----------------------\n");
		resultDiff= 0;
	}

	if (varianceLeft < thresholdC && varianceRight < thresholdC)
	{
		//! Complex is big
		resultComplex = 0;
	}
	else
	{
		resultComplex = 1;
	}
	if (varianceLeft<thresholdUnC || varianceRight <thresholdUnC)
	{
		resultComplex = 0;
	}

	if (!resultDiff && resultComplex)
	{
		resultFinal = 0;
	}
	else
	{
		resultFinal = 1;
	}
	return resultFinal;
};

#endif