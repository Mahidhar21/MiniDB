#include "pager.h"

using namespace std;

Pager::Pager(const string& dbFile)
    : diskManager_(dbFile) {
}

shared_ptr<Page> Pager::getPage(PageId pageId) {

    auto it = pageTable_.find(pageId);

    if(it != pageTable_.end()) {
        return it->second;
    }

    shared_ptr<Page> page =
        make_shared<Page>();

    diskManager_.readPage(pageId, *page);

    pageTable_[pageId] = page;

    return page;
}

shared_ptr<Page> Pager::newPage() {

    PageId newPageId =
        diskManager_.allocatePage();

    shared_ptr<Page> page =
        make_shared<Page>(newPageId);

    pageTable_[newPageId] = page;

    return page;
}

void Pager::flushPage(PageId pageId) {

    auto it = pageTable_.find(pageId);

    if(it == pageTable_.end()) {
        return;
    }

    shared_ptr<Page> page = it->second;

    if(page->isDirty()) {

        diskManager_.writePage(*page);

        page->clearDirty();
    }
}

void Pager::flushAllPages() {

    for(auto& [pageId, page] : pageTable_) {

        if(page->isDirty()) {

            diskManager_.writePage(*page);

            page->clearDirty();
        }
    }
}