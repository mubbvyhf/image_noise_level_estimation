#include "NoiseLevelEstimation.h"

NoiseLevelEstimation::NoiseLevelEstimation()
{
	d = 4;
	step = 15;
	patchSize = d * d;
}

NoiseLevelEstimation::~NoiseLevelEstimation()
{

}

void NoiseLevelEstimation::estimateNoiseLevel(const cv::Mat& src, float& noiseLevel)
{
	if (src.type() == CV_8UC1)
		src.convertTo(mSrcFloat, CV_32FC1, 1.f / 255.f);
	else if (src.type() == CV_16UC1)
		src.convertTo(mSrcFloat, CV_32FC1, 1.f / 16383.f);

	img2patch(d, step);

	if (mX.empty()) mX.create(mPatches.size(), CV_32FC1);
	for (int row = 0; row < mPatches.rows; row++)
	{
		float curRowMeans = cv::mean(mPatches.row(row))[0];
		mX.row(row) = mPatches.row(row) - curRowMeans;
	}
	mSigmaX = mX * mX.t() / numPatches;
	cv::eigen(mSigmaX, mEigenValues);
	
	float* pEigenValues = mEigenValues.ptr<float>(0);
	for (int i = 1; i < mEigenValues.total(); i++)
	{
		float sum = 0;
		for (int j = i; j < mEigenValues.total(); j++)
		{
			sum += pEigenValues[j];
		}
		int len = mEigenValues.total() - i;
		float mean = sum / len;
		int nLarger = 0;
		for (int j = i; j < mEigenValues.total(); j++)
		{
			if (mean < pEigenValues[j])
			{
				nLarger++;
			}
		}
		int nSmaller = len - nLarger;
		if (nLarger == nSmaller)
		{
			noiseLevel = sqrt(mean);
			if (src.type() == CV_8UC1)
				noiseLevel *= 255.f;
			else if (src.type() == CV_16UC1)
				noiseLevel *= 16383.f;
			return;
		}
	}
}

void NoiseLevelEstimation::img2patch(int d, int step)
{
	int width = mSrcFloat.cols;
	int height = mSrcFloat.rows;

	int numPatchesX = (width + step - d) / step;
	int numPatchesY = (height + step - d) / step;
	numPatches = numPatchesX * numPatchesY;
	int patchSize = d * d;
	
	if (mPatches.empty()) mPatches.create(patchSize, numPatches, CV_32FC1);
	if (mPatchPixelsOffset.size() == 0)
	{
		mPatchPixelsOffset.resize(patchSize);
		int k = 0;
		for (int i = 0; i < d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				mPatchPixelsOffset[k] = i * width + j;
				k++;
			}
		}
	}
	if (mPatchesPtrOffset.size() == 0)
	{
		mPatchesPtrOffset.resize(numPatches);
		int k = 0;
		for (int i = 0; i < numPatchesY; i++)
		{
			for (int j = 0; j < numPatchesX; j++)
			{
				mPatchesPtrOffset[k] = step * i * width + step * j;
				k++;
			}
		}
	}
	if (mPatchesColPixelsOffset.size() == 0)
	{
		mPatchesColPixelsOffset.resize(patchSize);
		for (int i = 0; i < patchSize; i++)
		{
			mPatchesColPixelsOffset[i] = i * numPatches;
		}
	}

	float* pSrcFloat = mSrcFloat.ptr<float>(0);
	float* pPatches = mPatches.ptr<float>(0);
	for (int i = 0; i < numPatches; i++)
	{
		float* pPatch = &pSrcFloat[mPatchesPtrOffset[i]];
		for (int j = 0; j < patchSize; j++)
		{
			pPatches[mPatchesColPixelsOffset[j]] = pPatch[mPatchPixelsOffset[j]];
		}
		pPatches++;
	}
}
