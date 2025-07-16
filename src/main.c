#include <stdio.h>
#include "storage/page.h"

int main(void)
{
    Page page;
    page_init(&page, 1);

    /* Demo: modify page data */
    const char *msg = "hello";
    memcpy(page_data_mut(&page), msg, 6); /* include null terminator */
    page_mark_dirty(&page, true);

    uint8_t buffer[PAGE_SIZE];
    page_to_bytes(&page, buffer);

    printf("Page ID: %llu\n", (unsigned long long)page_get_id(&page));
    printf("Dirty: %d\n", page_is_dirty(&page));
    printf("Data: %s\n", (char*)buffer);

    return 0;
}
