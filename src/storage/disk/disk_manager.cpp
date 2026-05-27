#include "disk_manager.h"

#include <iostream>

using namespace std;

DiskManager::DiskManager(const string& fileName)
    : dbFileName_(fileName) {

    dbFile_.open(
        dbFileName_,
        ios::in |
        ios::out |
        ios::binary
    );

    if(!dbFile_.is_open()) {

        dbFile_.clear();

        dbFile_.open(
            dbFileName_,
            ios::out |
            ios::binary
        );

        dbFile_.close();

        dbFile_.open(
            dbFileName_,
            ios::in |
            ios::out |
            ios::binary
        );
    }

    if(!dbFile_.is_open()) {
        throw runtime_error("Failed to open database file");
    }
}

DiskManager::~DiskManager() {

    if(dbFile_.is_open()) {
        dbFile_.close();
    }
}

uint64_t DiskManager::pageOffset(PageId pageId) const {

    return static_cast<uint64_t>(pageId) * PAGE_SIZE;
}

void DiskManager::writePage(const Page& page) {

    uint64_t offset = pageOffset(page.getPageId());

    dbFile_.seekp(offset);

    dbFile_.write(
        reinterpret_cast<const char*>(page.data()),
        PAGE_SIZE
    );

    dbFile_.flush();

    if(!dbFile_) {
        throw runtime_error("Failed to write page");
    }
}

void DiskManager::readPage(PageId pageId, Page& page) {

    uint64_t offset = pageOffset(pageId);

    page.setPageId(pageId);

    dbFile_.seekg(offset);

    dbFile_.read(
        reinterpret_cast<char*>(page.data()),
        PAGE_SIZE
    );

    streamsize bytesRead = dbFile_.gcount();

    if(bytesRead < PAGE_SIZE) {

        byte* raw = page.data();

        for(size_t i = bytesRead; i < PAGE_SIZE; i++) {
            raw[i] = byte{0};
        }

        dbFile_.clear();
    }
}

uint64_t DiskManager::getFileSize() {

    dbFile_.seekg(0, ios::end);

    return dbFile_.tellg();
}

uint64_t DiskManager::getPageCount() {

    return getFileSize() / PAGE_SIZE;
}

PageId DiskManager::allocatePage() {

    PageId newPageId =
        static_cast<PageId>(getPageCount());

    Page newPage(newPageId);

    writePage(newPage);

    return newPageId;
}