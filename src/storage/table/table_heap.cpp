#include "table_heap.h"

#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

TableHeap::TableHeap(Pager& pager)
    : pager_(pager) {
}

vector<byte>
TableHeap::serializeRow(
    const Row& row
) {

    string data;

    for(const auto& [key, value]
        : row) {

        data += key;

        data += "=";

        data += value;

        data += ";";
    }

    vector<byte> bytes;

    for(char c : data) {

        bytes.push_back(
            byte(c)
        );
    }

    return bytes;
}

Row TableHeap::deserializeRow(
    const byte* data,
    uint16_t length
) {

    string text;

    for(uint16_t i = 0;
        i < length;
        i++) {

        text += char(data[i]);
    }

    Row row;

    stringstream ss(text);

    string pair;

    while(getline(ss, pair, ';')) {

        if(pair.empty()) {
            continue;
        }

        size_t eq =
            pair.find('=');

        string key =
            pair.substr(0, eq);

        string value =
            pair.substr(eq + 1);

        row[key] = value;
    }

    return row;
}

void TableHeap::insertRow(
    const Row& row
) {

    vector<byte> bytes =
        serializeRow(row);

    if(pages_.empty()) {

        auto page =
            pager_.newPage();

        pages_.push_back(
            page->getPageId()
        );

        SlottedPage slotted(*page);

        slotted.initialize();
    }

    PageId lastPageId =
        pages_.back();

    auto page =
        pager_.getPage(lastPageId);

    SlottedPage slotted(*page);

    bool inserted =
        slotted.insertRecord(
            bytes.data(),
            bytes.size()
        );

    if(!inserted) {

        auto newPage =
            pager_.newPage();

        pages_.push_back(
            newPage->getPageId()
        );

        SlottedPage newSlotted(
            *newPage
        );

        newSlotted.initialize();

        newSlotted.insertRecord(
            bytes.data(),
            bytes.size()
        );
    }
}

vector<Row>
TableHeap::scanTable() {

    vector<Row> rows;

    for(PageId pageId : pages_) {

        auto page =
            pager_.getPage(pageId);

        uint16_t slotCount =
            *reinterpret_cast
            <
                uint16_t*
            >(
                page->data()
            );

        Slot* slots =
            reinterpret_cast
            <
                Slot*
            >(
                page->data()
                + sizeof(uint16_t) * 2
            );

        for(uint16_t i = 0;
            i < slotCount;
            i++) {

            Slot& slot =
                slots[i];

            if(!slot.occupied) {
                continue;
            }

            Row row =
                deserializeRow(
                    page->data()
                    + slot.offset,
                    slot.length
                );

            rows.push_back(row);
        }
    }

    return rows;
}