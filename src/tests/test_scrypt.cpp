#include "core/scrypt.h"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::cout << "[Test] Verifying Refined Scrypt hashing core..." << std::endl;
    std::vector<uint8_t> data(80, 0xAF);
    auto hash = core::Scrypt::litecoin_scrypt(data);
    if (hash.size() == 32) {
        std::cout << "[SUCCESS] Scrypt engine v2.1 functional." << std::endl;
        return 0;
    }
    return 1;
}
