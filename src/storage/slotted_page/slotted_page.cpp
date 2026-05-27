#include "slotted_page.h"

#include <cstring>
#include <iostream>

using namespace std;

SlottedPage::SlottedPage(Page& page)
    : page_(page) {
}

uint16_t* SlottedPage::slotCount() {

    return reinterpret_cast<uint16_t*>(page_.data());
}

uint16_t* SlottedPage::freeSpaceOffset() {

    return reinterpret_cast<uint16_t*>(
        page_.data() + sizeof(uint16_t)
    );
}

Slot* SlottedPage::slots() {

    return reinterpret_cast<Slot*>(
        page_.data() + sizeof(uint16_t) * 2
    );
}

void SlottedPage::initialize() {

    *slotCount() = 0;

    *freeSpaceOffset() = PAGE_SIZE;
}

bool SlottedPage::insertRecord(
    const byte* data,
    uint16_t length
) {

    uint16_t currentSlotCount =
        *slotCount();

    uint16_t currentFreeOffset =
        *freeSpaceOffset();

    uint16_t slotDirectoryEnd =
        sizeof(uint16_t) * 2 +
        sizeof(Slot) * currentSlotCount;

    if(currentFreeOffset <
       slotDirectoryEnd + length + sizeof(Slot)) {

        return false;
    }

    currentFreeOffset -= length;

    memcpy(
        page_.data() + currentFreeOffset,
        data,
        length
    );

    Slot& slot =
        slots()[currentSlotCount];

    slot.offset = currentFreeOffset;

    slot.length = length;

    slot.occupied = true;

    (*slotCount())++;

    (*freeSpaceOffset()) = currentFreeOffset;

    page_.markDirty();

    return true;
}

void SlottedPage::printLayout() {

    cout << "Slot Count: "
         << *slotCount()
         << endl;

    cout << "Free Space Offset: "
         << *freeSpaceOffset()
         << endl;

    for(uint16_t i = 0; i < *slotCount(); i++) {

        Slot& slot = slots()[i];

        cout << "Slot "
             << i
             << " Offset="
             << slot.offset
             << " Length="
             << slot.length
             << endl;
    }
}