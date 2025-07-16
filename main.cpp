#include "include/storage/page.h"
#include <iostream>

int main() {
    alpha::storage::Page page(1);
    uint8_t buffer[alpha::storage::PAGE_SIZE];
    for (size_t i = 0; i < alpha::storage::PAGE_SIZE; ++i) {
        buffer[i] = static_cast<uint8_t>(i % 256);
    }

    page.from_bytes(buffer);

    uint8_t out[alpha::storage::PAGE_SIZE];
    page.to_bytes(out);

    bool ok = std::memcmp(buffer, out, alpha::storage::PAGE_SIZE) == 0;
    std::cout << (ok ? "OK" : "FAIL") << std::endl;
    return ok ? 0 : 1;
}
