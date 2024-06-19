# image_noise_level_estimation

## Introduction

This repository is a non-official C++ implementation of the following paper:

Chen G , Zhu F , Heng P A . An Efficient Statistical Method for Image Noise Level Estimation[C]// 2015 IEEE International Conference on Computer Vision (ICCV). IEEE Computer Society, 2015.

The code only supports processing 8-bit grayscale images. Please modify it accordingly if you need to process other types of images.

## Dependencies

OpenCV

## Demo

NoiseLevelEstimation nle;

float noiseLevel;

nle.estimateNoiseLevel(gray, noiseLevel);
