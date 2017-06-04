/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: alg.cpp
 *     Created: 2017-06-02 19:12
 * Description:
 * ===================================================
 */
#include "alg.h"
#include "a/alga.h"

static AlgA IMPL;

bool Alg::Init()
{
    m_taskq = new TaskQueue(8);
    return IMPL.Init();
}

bool Alg::Release()
{
    delete m_taskq;
    return true;
}

void Alg::Push(Task *task)
{
    m_taskq->Push(task);
}

bool Alg::Detect(const FaceSource &src, DetectResults &results)
{
    return IMPL.Detect(src, results);
}

bool Alg::Extract(const FaceSource &src, ExtractResults &results)
{
    return IMPL.Extract(src, results);
}

float Alg::Compare(const Feature &f1, const Feature &f2)
{
    return IMPL.Compare(f1, f2);
}

