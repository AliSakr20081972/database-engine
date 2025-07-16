// Page.h
#pragma once

#include <cstdint>
#include <vector>
#include <cstring>
#include <stdexcept>

namespace alpha::storage {

constexpr size_t PAGE_SIZE = 4096; // 4KB fixed-size page

class Page {
public:
    using PageID = uint64_t;

    Page(PageID id) : id_(id), dirty_(false) {
        std::memset(buffer_, 0, PAGE_SIZE);
    }

    // Accessors
    PageID get_id() const { return id_; }
    const uint8_t* data() const { return buffer_; }
    uint8_t* data() { return buffer_; }

    // Metadata
    bool is_dirty() const { return dirty_; }
    void mark_dirty(bool dirty) { dirty_ = dirty; }

    // Serialization
    void from_bytes(const uint8_t* src) {
        if (src == nullptr) {
            throw std::invalid_argument("src buffer is null");
        }
        std::memcpy(buffer_, src, PAGE_SIZE);
        dirty_ = false;
    }
    void to_bytes(uint8_t* dest) const {
        if (dest == nullptr) {
            throw std::invalid_argument("dest buffer is null");
        }
        std::memcpy(dest, buffer_, PAGE_SIZE);
    }

private:
    PageID id_;
    bool dirty_;
    uint8_t buffer_[PAGE_SIZE];
};

} // namespace alpha::storage
