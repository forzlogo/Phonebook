#include "validation.h"
#include <cctype>

bool IsValidEmail(const std::string& email) {
    if (email.empty()) return true;

    size_t at_pos = email.find('@');
    if (at_pos == std::string::npos || at_pos == 0 ||
        at_pos == email.length() - 1) {
        return false;
    }

    size_t dot_pos = email.find('.', at_pos);
    if (dot_pos == std::string::npos || dot_pos == at_pos + 1 ||
        dot_pos == email.length() - 1) {
        return false;
    }

    return true;
}

bool IsValidPhone(const std::string& phone) {
    if (phone.empty()) return true;

    int digit_count = 0;
    for (char c : phone) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            ++digit_count;
        }
    }
    return digit_count >= 7;
}
