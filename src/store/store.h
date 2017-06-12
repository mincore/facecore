/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: store.h
 * Description:
 * =====================================================================
 */
#ifndef _STORE_H
#define _STORE_H

#include <mutex>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <list>
#include "../msg/pb/fastore.pb.h"
#include "../alg/alg.h"

namespace leveldb {
    class DB;
}
using namespace fastore;

typedef std::uint64_t FaceID;
typedef std::uint32_t TagID;

struct _Tag {
    std::string m_name;
    TagAttr m_attr;
    std::mutex m_mutex;
    std::list<FaceAttr*> m_faceIDs;
};
typedef std::shared_ptr<_Tag> Tag;

class Store: public singleton<Store> {
public:
    bool Init();
    bool Release();

    void AddFaces(const std::string &tagName, const ExtractResults &results);
    void ListTags(std::vector<std::string> &tagNames);
    size_t GetTagFaceCount(const std::string &tagName);

private:
    bool LoadTags(std::unordered_map<TagID, Tag> &tagIdMap);
    bool LoadFaces(const std::unordered_map<TagID, Tag> &tagIdMap);

private:
    leveldb::DB *m_db;
    std::atomic<FaceID> m_nextFaceID;
    std::atomic<TagID> m_nextTagID;

    std::unordered_map<FaceID, FaceAttr> m_faceMap;
    std::mutex m_faceMapMutex;

    std::unordered_map<std::string, Tag> m_tagMap;
    std::mutex m_tagMapMutex;
};

#endif
