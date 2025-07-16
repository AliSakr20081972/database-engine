#ifndef STORAGE_PAGE_H
#define STORAGE_PAGE_H

/**
 * @file page.h
 * @brief Fixed-size disk page abstraction used by the storage layer.
 *
 * A page represents a 4KB block of data on disk identified by a 64-bit
 * PageID. Pages track a dirty flag so that higher layers can decide whether
 * they need to be persisted back to disk. Helper functions are provided for
 * initialization and serialization to and from raw byte buffers.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define PAGE_SIZE 4096 /* 4KB fixed-size page */

typedef uint64_t PageID;

typedef struct Page {
    PageID id;
    bool dirty;
    uint8_t buffer[PAGE_SIZE];
} Page;

/* Initialize a page structure with the given identifier */
void page_init(Page *page, PageID id);

/* Accessors */
static inline PageID page_get_id(const Page *page) { return page->id; }
static inline const uint8_t *page_data(const Page *page) { return page->buffer; }
static inline uint8_t *page_data_mut(Page *page) { return page->buffer; }

/* Metadata */
static inline bool page_is_dirty(const Page *page) { return page->dirty; }
static inline void page_mark_dirty(Page *page, bool dirty) { page->dirty = dirty; }

/* Serialization */
void page_from_bytes(Page *page, const uint8_t *src);
void page_to_bytes(const Page *page, uint8_t *dest);

#endif /* STORAGE_PAGE_H */
