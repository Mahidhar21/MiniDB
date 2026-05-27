#include "page.h"

using namespace std;

Page::Page()
    : id_(0),
      dirty_(false) {

    data_.fill(byte{0});
}

Page::Page(PageId id)
    : id_(id),
      dirty_(false) {

    data_.fill(byte{0});
}

void Page::setPageId(PageId id) {
    id_ = id;
}

PageId Page::getPageId() const {
    return id_;
}

bool Page::isDirty() const {
    return dirty_;
}

void Page::markDirty() {
    dirty_ = true;
}

void Page::clearDirty() {
    dirty_ = false;
}

byte* Page::data() {
    return data_.data();
}

const byte* Page::data() const {
    return data_.data();
}