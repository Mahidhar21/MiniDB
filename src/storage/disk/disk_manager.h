#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "../page/page.h"

using namespace std;

class DiskManager {
private:
    string dbFileName_;

    fstream dbFile_;

    uint64_t pageOffset(PageId pageId) const;

public:
    explicit DiskManager(const string& fileName);

    ~DiskManager();

    void writePage(const Page& page);

    void readPage(PageId pageId, Page& page);

    PageId allocatePage();

    uint64_t getFileSize();

    uint64_t getPageCount();
};