/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: alga.cpp
 * Description:
 * =====================================================================
 */
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "alga.h"

#define FEATURE_DIM 360
#define FEATURE_SIZE (360*sizeof(float))

static std::string GetEnv(const char *key)
{
    const char *val = getenv(key);
    return val ? val : "";
}

bool AlgA::Init()
{
#if 1
    setenv("ALG_MODEL_DIR", "ALG_MODEL_DIR", 1);
    setenv("ALG_MODEL_FILE", "ALG_MODEL_FILE", 1);
    setenv("ALG_TRAINED_FILE", "ALG_TRAINED_FILE", 1);
#endif

    m_modelDir = GetEnv("ALG_MODEL_DIR");
    m_modelFile = GetEnv("ALG_MODEL_FILE");
    m_trainedFile = GetEnv("ALG_TRAINED_FILE");

    if (m_modelDir.empty() || m_modelFile.empty() || m_trainedFile.empty()) {
        LOG_ERROR("ALG ENV IS NOT SET");
        return false;
    }

    srand(time(NULL));

    return true;
}

bool AlgA::Detect(const FaceSource &src, DetectResults &result)
{
    std::vector<FaceInfo> info = detectANDlandmark(m_modelDir.c_str(), src.imgFile.c_str());
    if (info.empty()) {
        LOG_ERROR("NO FACE FOUND IN %s", src.imgFile.c_str());
        return false;
    }
    result.resize(info.size());

    DetectResult tmp;
    for (auto i: info) {
        tmp.rc.left = (uint32_t)i.bbox.x1;
        tmp.rc.top = (uint32_t)i.bbox.y1;
        tmp.rc.right = (uint32_t)i.bbox.x2;
        tmp.rc.bottom = (uint32_t)i.bbox.y2;
        result.push_back(tmp);
    }

    return true;
}

bool AlgA::Extract(const FaceSource &src, ExtractResults &result)
{
    result.resize(1);

    std::vector<float> tmp = extract(m_modelFile.c_str(), m_trainedFile.c_str(), src.imgFile.c_str());
    if (tmp.empty()) {
        LOG_ERROR("NO FEATURE EXTRACTED IN %s", src.imgFile.c_str());
        return false;
    }

    Feature &feat = result[0].feat;
    feat.size = tmp.size() * sizeof(float);
    feat.data = new std::uint8_t[feat.size];
    memcpy(feat.data, &tmp[0], feat.size);

    return true;
}

float AlgA::Compare(const Feature &f1, const Feature &f2)
{
    // FIXME: USE REAL CMP ALG
    usleep(1000*(rand()%10));
    return rand()%100 / 1000.0f;
}

//////////////////////FAKE FUNCTIONS///////////////////
const std::vector<FaceInfo> detectANDlandmark(const char* modelDir, const char* imagePath)
{
    std::vector<FaceInfo> n(1+rand()%3);

    for (auto &i : n) {
        i.bbox.x1 = rand()%100;
        i.bbox.y1 = rand()%100;
        i.bbox.x2 = rand()%100;
        i.bbox.y2 = rand()%100;
    }
    return n;
}

const std::vector<float> extract(const char* modelFile, const char* trainedFile, const char* imagePath)
{
    std::vector<float> ret;
    ret.resize(FEATURE_DIM);
    for (auto &i : ret) {
        i = rand()/100;
    }
    return ret;
}
