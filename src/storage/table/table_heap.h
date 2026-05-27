#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../pager/pager.h"
#include "../slotted_page/slotted_page.h"

using namespace std;

using Row =
    unordered_map<string, string>;

class TableHeap {
private:
    Pager& pager_;

    vector<PageId> pages_;

    vector<byte> serializeRow(
        const Row& row
    );

    Row deserializeRow(
        const byte* data,
        uint16_t length
    );

public:
    explicit TableHeap(Pager& pager);

    void insertRow(const Row& row);

    vector<Row> scanTable();
};