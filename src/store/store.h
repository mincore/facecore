/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: cache.h
 * Description:
 * =====================================================================
 */
#ifndef _CACHE_H
#define _CACHE_H

#include <mutex>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <list>
#include "../msg/pb/core.pb.h"
#include "../alg/alg.h"

namespace leveldb {
    class DB;
}
using namespace core;

typedef std::uint64_t FaceID;
typedef std::uint32_t TagID;

struct _Tag {
    std::string m_name;
    TagAttr m_attr;
    std::mutex m_mutex;
    std::list<FaceID> m_faceIDs;
};
typedef std::shared_ptr<_Tag> Tag;

class Store: public singleton<Store> {
public:
    bool Init();
    bool Release();

    void AddFaces(const std::string &tagName, const ExtractResults &results);
    void ListTags(std::vector<std::string> &tagNames);
    void SearchTag(FaceID faceID, const std::string &tagName);

    // just for test
    void ListFaces(const std::string &tagName, std::list<FaceID> &faceIDs);

private:
    bool LoadTags(std::unordered_map<TagID, Tag> &tagIdMap);
    bool LoadFaces(const std::unordered_map<TagID, Tag> &tagIdMap);

private:
    leveldb::DB *m_db;
    std::atomic<FaceID> m_nextFaceID;
    std::atomic<TagID> m_nextTagID;

    // TODO: change FaceAttr to FaceAttr* or FaceAttrID
    std::unordered_map<FaceID, FaceAttr> m_faceMap;
    std::mutex m_faceMapMutex;

    std::unordered_map<std::string, Tag> m_tagMap;
    std::mutex m_tagMapMutex;

    std::unordered_map<TagID, Tag> m_tagIdMap;
    std::mutex m_tagIdMapMutex;
};

#endif
