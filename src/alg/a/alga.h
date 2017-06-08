/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: alga.h
 * Description:
 * =====================================================================
 */
#ifndef _ALGA_H
#define _ALGA_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../alg.h"

typedef struct FaceRect {
  float x1;
  float y1;
  float x2;
  float y2;
  float score; /**< Larger score should mean higher confidence. */
} FaceRect;

typedef struct FacePts {
  float x[5],y[5];
} FacePts;

typedef struct FaceInfo {
  FaceRect bbox;
  cv::Vec4f regression;
  FacePts facePts;
  double roll;
  double pitch;
  double yaw;
} FaceInfo;

const std::vector<FaceInfo> detectANDlandmark(const char* modelDir, const char* imagePath);
const std::vector<FaceInfo> detectANDlandmark(const char* modelDir, const char* imagePath, const int device);

const std::vector<float> extract(const char* modelFile, const char* trainedFile, const char* imagePath);
const std::vector<float> extract(const char* modelFile, const char* trainedFile, const char* imagePath, const int device);

class AlgA {
public:
    bool Init();
    bool Detect(const FaceSource &src, DetectResults &result);
    bool Extract(const FaceSource &src, ExtractResults &result);
    float Compare(const Feature &f1, const Feature &f2);

private:
    std::string m_modelDir;
    std::string m_modelFile;
    std::string m_trainedFile;
};

int AlgCreate();
int AlgDestroy();

int AlgDetect();
int AlgExtract();
float AlgCompare();


#endif
