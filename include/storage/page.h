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

    Page(PageID id);

    // Accessors
    PageID get_id() const;
    const uint8_t* data() const;
    uint8_t* data();

    // Metadata
    bool is_dirty() const;
    void mark_dirty(bool dirty);

    // Serialization
    void from_bytes(const uint8_t* src);
    void to_bytes(uint8_t* dest) const;

private:
    PageID id_;
    bool dirty_;
    uint8_t buffer_[PAGE_SIZE];
};

} // namespace alpha::storage
