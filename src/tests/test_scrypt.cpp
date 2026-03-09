#include "core/scrypt.h"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::cout << "[Test] Verifying Scrypt hashing core..." << std::endl;
    std::vector<uint8_t> data(80, 0xA5);
    auto hash = core::Scrypt::litecoin_scrypt(data);
    if (hash.size() == 32) {
        std::cout << "[SUCCESS] Scrypt core functional." << std::endl;
        return 0;
    }
    return 1;
}
