#pragma once

#include "../page/page.h"

using namespace std;

struct Slot {

    uint16_t offset;

    uint16_t length;

    bool occupied;
};

class SlottedPage {
private:
    Page& page_;

    uint16_t* slotCount();

    uint16_t* freeSpaceOffset();

    Slot* slots();

public:
    explicit SlottedPage(Page& page);

    void initialize();

    bool insertRecord(const byte* data, uint16_t length);

    void printLayout();
};