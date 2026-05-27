#include "buffer_pool.h"

#include <iostream>

using namespace std;

BufferPool::BufferPool(
    const string& dbFile,
    size_t maxPages
)
    : maxPages_(maxPages),
      pager_(dbFile) {
}

void BufferPool::touchPage(PageId pageId) {

    auto it = lruMap_.find(pageId);

    if(it != lruMap_.end()) {
        lruList_.erase(it->second);
    }

    lruList_.push_front(pageId);

    lruMap_[pageId] =
        lruList_.begin();
}

void BufferPool::evictPage() {

    if(lruList_.empty()) {
        return;
    }

    PageId victim =
        lruList_.back();

    lruList_.pop_back();

    lruMap_.erase(victim);

    auto it = pages_.find(victim);

    if(it != pages_.end()) {

        shared_ptr<Page> page =
            it->second;

        if(page->isDirty()) {

            pager_.flushPage(victim);
        }

        pages_.erase(it);

        cout << "Evicted Page "
             << victim
             << endl;
    }
}

shared_ptr<Page> BufferPool::fetchPage(
    PageId pageId
) {

    auto it = pages_.find(pageId);

    if(it != pages_.end()) {

        touchPage(pageId);

        return it->second;
    }

    if(pages_.size() >= maxPages_) {

        evictPage();
    }

    shared_ptr<Page> page =
        pager_.getPage(pageId);

    pages_[pageId] = page;

    touchPage(pageId);

    return page;
}

shared_ptr<Page> BufferPool::newPage() {

    if(pages_.size() >= maxPages_) {

        evictPage();
    }

    shared_ptr<Page> page =
        pager_.newPage();

    pages_[page->getPageId()] =
        page;

    touchPage(page->getPageId());

    return page;
}

void BufferPool::flushAllPages() {

    pager_.flushAllPages();
}