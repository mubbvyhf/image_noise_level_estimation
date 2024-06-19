#include <vector>

#include "opencv2/opencv.hpp"

class NoiseLevelEstimation
{
public:

	NoiseLevelEstimation();
	~NoiseLevelEstimation();

	void estimateNoiseLevel(const cv::Mat& src, float& noiseLevel);

private:
	void img2patch(int patchSize, int step);

	int d;
	int step;
	int patchSize;

	std::vector<int> mPatchesPtrOffset;
	std::vector<int> mPatchPixelsOffset;
	std::vector<int> mPatchesColPixelsOffset;
	cv::Mat mSrcFloat;
	cv::Mat mPatches;
	cv::Mat mX;
	cv::Mat mSigmaX;
	cv::Mat mEigenValues;
};
