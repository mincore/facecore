/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: alg/alg.h
 * Description:
 * =====================================================================
 */
#ifndef _ALG_ALG_H
#define _ALG_ALG_H

#include <string>
#include <vector>
#include "../common/common.h"
#include "taskq/taskq.h"

#define INVALID_FACEID -1
#define MAX_PATH 256

struct Feature {
    std::uint8_t *data;
    int size;
};

struct Rect {
    std::uint32_t left;
    std::uint32_t top;
    std::uint32_t right;
    std::uint32_t bottom;
};

struct ExtractResult {
    Feature feat;
};

struct DetectResult {
    Rect rc;
};

struct FaceSource {
    std::string imgFile;
};

typedef std::vector<DetectResult> DetectResults;
typedef std::vector<ExtractResult> ExtractResults;

class Alg: public singleton<Alg> {
public:
    bool Init();
    bool Release();

public:
    void Push(Task *);
    bool Detect(const FaceSource &src, DetectResults &results);
    bool Extract(const FaceSource &src, ExtractResults &results);
    float Compare(const Feature &f1, const Feature &f2);

private:
    TaskQueue *m_taskq;
};

#endif
