/* =====================================================================
 * Copyright (C) 2017 mincore All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: store.cpp
 * Description:
 * =====================================================================
 */
#include <mutex>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include "store.h"

/*
 * +------------------+---------+-----------------
 * |  FID_FACEID1     | feature |
 * +------------------+---------+ key: faceID
 * |  ...             | feature | val: feature, tagIds
 * +------------------+---------+
 * |  FID_FACEIDN     | feature |
 * +------------------+---------+-----------------
 * |  TAG_NAME1       | tagattr |
 * +------------------+---------+ key: tag_name
 * |  ...             | tagattr | val: tagId
 * +------------------+---------+
 * |  TAG_NAMEN       | tagattr |
 * +------------------+---------+-----------------
 */

using namespace leveldb;

#define DB_NAME     "./db"
#define PREFIX_FID   "FID_"
#define PREFIX_TAG   "TAG_"

bool Store::Init()
{
    Options opts;
    opts.create_if_missing = true;
    assert(DB::Open(opts, DB_NAME, &m_db).ok());
    m_nextFaceID = 0;
    m_nextTagID = 0;

    std::unordered_map<TagID, Tag> tagIdMap;
    return LoadTags(tagIdMap) && LoadFaces(tagIdMap);
}

bool Store::Release()
{
    delete m_db;
    return true;
}

bool Store::LoadTags(std::unordered_map<TagID, Tag> &tagIdMap)
{
    int len = strlen(PREFIX_TAG);
    Iterator* iter = m_db->NewIterator(ReadOptions());

    for (iter->Seek(PREFIX_TAG); iter->Valid(); iter->Next()) {
        const char *key = iter->key().data();
        size_t size = iter->key().size();
        if (0 != memcmp(key, PREFIX_TAG, len))
            break;

        std::string tagName(key+len, size-len);
        Tag &tag = m_tagMap[tagName] = Tag(new _Tag);
        tag->m_name = tagName;
        tag->m_attr.ParseFromArray(iter->value().data(), iter->value().size());

        tagIdMap[tag->m_attr.tagid()] = tag;

        auto tagId = tag->m_attr.tagid();
        if (m_nextTagID < tagId+1)
            m_nextTagID = tagId+1;

        LOG_INFO("load tag: %s", tagName.c_str());
    }

    delete iter;
    return true;
}

bool Store::LoadFaces(const std::unordered_map<TagID, Tag> &tagIdMap)
{
    int len = strlen(PREFIX_FID);
    Iterator* iter = m_db->NewIterator(ReadOptions());

    for (iter->Seek(PREFIX_FID); iter->Valid(); iter->Next()) {
        const char *key = iter->key().data();
        if (0 != memcmp(key, PREFIX_FID, len))
            break;

        FaceID faceID = atoll(iter->key().data() + len);
        if (m_nextFaceID < faceID+1)
            m_nextFaceID = faceID+1;

        FaceAttr &faceAttr = m_faceMap[faceID] = FaceAttr();
        faceAttr.ParseFromArray(iter->value().data(), iter->value().size());

        for (auto &tagId : faceAttr.tagids()) {
            auto it = tagIdMap.find(tagId);
            if (it == tagIdMap.end()) {
                LOG_WARN("NO TAG:%d FOUND", tagId);
                continue;
            }
            LOG_DEBUG("push FaceID:%lld to tag:%s", (long long)faeID,
                    it->second->m_name.c_str());
            it->second->m_faceIDs.push_back(faceID);
        }
    }

    LOG_INFO("load faces: %zd", m_faceMap.size());

    delete iter;
    return true;
}

void Store::AddFaces(const std::string &tagName, const ExtractResults &results)
{
    WriteBatch writeBatch;

    bool tagIsNew = false;
    Tag tag;
    {
        std::unique_lock<std::mutex> lk(m_tagMapMutex);
        auto it = m_tagMap.find(tagName);
        if (it != m_tagMap.end()) {
            tag = it->second;
        } else {
            tag = m_tagMap[tagName] = Tag(new _Tag);
            tagIsNew = true;
        }
    }

    if (tagIsNew) {
        tag->m_attr.set_tagid(m_nextTagID++);

        LOG_DEBUG("add tag to db. tag: %s", tagName.c_str());
        std::string realTagName(PREFIX_TAG);
        realTagName += tagName;
        std::string tagAttr;
        tag->m_attr.SerializeToString(&tagAttr);
        writeBatch.Put(realTagName, tagAttr);
    }

    std::string strFaceID;

    for (auto &r : results) {
        const Feature &feat = r.feat;
        FaceID faceID = m_nextFaceID++;

        {
            std::unique_lock<std::mutex> lk(tag->m_mutex);
            tag->m_faceIDs.push_back(faceID);
        }

        FaceAttr *faceAttr;
        {
            std::unique_lock<std::mutex> lk(m_faceMapMutex);
            m_faceMap[faceID] = FaceAttr();
            faceAttr = &m_faceMap[faceID];
        }

        faceAttr->add_tagids(tag->m_attr.tagid());
        faceAttr->set_feature(feat.data, feat.size);

        LOG_DEBUG("writeBatch faceID: %lld to tag: %s", (long long)faceID, tagName.c_str());
        strFaceID = PREFIX_FID;
        strFaceID += std::to_string((long long)faceID);

        std::string tmp;
        faceAttr->SerializeToString(&tmp);
        writeBatch.Put(strFaceID, tmp);
    }

    LOG_DEBUG("add %zd faces to db. tag: %s", results.size(), tagName.c_str());
    m_db->Write(WriteOptions(), &writeBatch);
}

void Store::ListTags(std::vector<std::string> &tagNames)
{
    std::unique_lock<std::mutex> lk(m_tagMapMutex);
    for (auto &i : m_tagMap) {
        tagNames.push_back(i.first);
    }
}

void Store::ListFaces(const std::string &tagName, std::list<FaceID> &faceIDs)
{
    std::unique_lock<std::mutex> lk(m_tagMapMutex);
    auto &tag = m_tagMap[tagName];
    if (tag.get()) {
        faceIDs = tag->m_faceIDs;
    }
}

void Store::SearchTag(FaceID faceID, const std::string &tagName)
{

}
