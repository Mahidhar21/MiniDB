#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "../pager/pager.h"

using namespace std;

class BufferPool {
private:
    size_t maxPages_;

    Pager pager_;

    unordered_map<PageId, shared_ptr<Page>> pages_;

    list<PageId> lruList_;

    unordered_map<
        PageId,
        list<PageId>::iterator
    > lruMap_;

    void touchPage(PageId pageId);

    void evictPage();

public:
    BufferPool(
        const string& dbFile,
        size_t maxPages
    );

    shared_ptr<Page> fetchPage(PageId pageId);

    shared_ptr<Page> newPage();

    void flushAllPages();
};