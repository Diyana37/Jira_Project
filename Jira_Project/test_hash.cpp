@"
#include <iostream>
#include <string>
#include <cstdint>

constexpr uint32_t hashString(const char* str, uint32_t hash = 5381) {
    return *str ? hashString(str + 1, ((hash << 5) + hash) + *str) : hash;
}

int main() {
    std::string s = "login";
    switch (hashString(s.c_str())) {
        case hashString("login"): std::cout << "login ok\n"; break;
        default: std::cout << "fail\n"; break;
    }
    return 0;
}
"@
