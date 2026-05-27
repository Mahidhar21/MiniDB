#pragma once

#include <array>
#include <cstddef>

#include "../../common/config.h"

using namespace std;

class Page {
private:
    PageId id_;

    bool dirty_;

    array<byte, PAGE_SIZE> data_;

public:
    Page();

    explicit Page(PageId id);

    void setPageId(PageId id);

    PageId getPageId() const;

    bool isDirty() const;

    void markDirty();

    void clearDirty();

    byte* data();

    const byte* data() const;
};