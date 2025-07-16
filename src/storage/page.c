#include "storage/page.h"

/*
 * Basic operations for managing fixed-size pages.
 *
 * The storage layer deals with raw 4KB pages. These helper functions
 * initialize a page structure and convert between the in-memory representation
 * and raw byte buffers used for I/O.
 */

/* Initialize a page with the specified identifier */
void page_init(Page *page, PageID id)
{
    if (!page) return;
    page->id = id;
    page->dirty = false;
    memset(page->buffer, 0, PAGE_SIZE);
}

/* Load page contents from a raw byte buffer */
void page_from_bytes(Page *page, const uint8_t *src)
{
    if (!page || !src) return;
    memcpy(page->buffer, src, PAGE_SIZE);
}

/* Serialize page contents to a raw byte buffer */
void page_to_bytes(const Page *page, uint8_t *dest)
{
    if (!page || !dest) return;
    memcpy(dest, page->buffer, PAGE_SIZE);
}
