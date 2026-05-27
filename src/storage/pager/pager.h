#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../disk/disk_manager.h"

using namespace std;

class Pager {
private:
    DiskManager diskManager_;

    unordered_map<PageId, shared_ptr<Page>> pageTable_;

public:
    explicit Pager(const string& dbFile);

    shared_ptr<Page> getPage(PageId pageId);

    shared_ptr<Page> newPage();

    void flushPage(PageId pageId);

    void flushAllPages();
};