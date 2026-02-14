//
// Created by Ivor_Aif on 2026/2/12.
//

#ifndef STRING_H
#define STRING_H

#include "Array.h"

namespace mySTL::containers {

class String {
private:
    Array<char> str = {'\0'};

    friend String operator+(const String& str, const String& other);
    friend std::ostream& operator<<(std::ostream& os, const String& str);
    friend void swap(String& str1, String& str2) noexcept;

    static size_t cStrLen(const char* cStr) {
        if (!cStr) {
            throw std::invalid_argument("null pointer");
        }
        size_t len = 0;
        while (cStr[len] != '\0') {
            ++len;
        }
        return len;
    }

    void ensureEnding() {
        if (str.empty()) {
            str.pushBack('\0');
            return;
        }
        if (str.back() != '\0') {
            str.reserve(length() + 1);
            str.pushBack('\0');
        }
    }

    [[nodiscard]] bool isEnough(const char* ptr) const noexcept {
        if (!ptr) {
            return false;
        }
        const char* buf = cStr();
        const char* end = buf + capacity();
        return ptr >= buf && ptr < end;
    }

public:
    String() = default;

    explicit String(const char* cStr) {
        assign(cStr);
    }

    String(const size_t nn, const char* subStr) {
        const size_t subLen = cStrLen(subStr);
        if (subLen != 0 && nn > static_cast<size_t>(2147483647) / subLen) {
            throw std::length_error("String too long");
        }
        const size_t totalLen = nn * subLen;
        reserve(totalLen);
        str.clear();
        for (size_t i = 0; i < totalLen; ++i) {
            str.pushBack(subStr[i % subLen]);
        }
        str.pushBack('\0');
    }

    String(const size_t nn, const char ch) {
        reserve(nn);
        str.clear();
        for (size_t i = 0; i < nn; ++i) {
            str.pushBack(ch);
        }
        str.pushBack('\0');
    }

    String(const String&) = default;
    String(String&&) noexcept = default;
    String& operator=(const String&) = default;
    String& operator=(String&&) noexcept = default;

    String& operator=(const char* cStr) {
        assign(cStr);
        return *this;
    }

    ~String() = default;

    void assign(const char* cStr) {
        const size_t len = cStrLen(cStr);
        reserve(len);
        str.clear();
        for (size_t i = 0; i < len; ++i) {
            str.pushBack(cStr[i]);
        }
        str.pushBack('\0');
    }

    [[nodiscard]] size_t length() const noexcept {
        return str.getSize() ? str.getSize()-1 : 0;
    }

    [[nodiscard]] size_t capacity() const noexcept {
        const size_t cap = str.capacity();
        return cap ? cap-1 : 0;
    }

    [[nodiscard]] bool empty() const noexcept {
        return length() == 0;
    }

    void reserve(const size_t len) {
        if (len > static_cast<size_t>(2147483647)) {
            throw std::length_error("String too long");
        }
        str.reserve(len + 1);
    }

    void shrink() {
        ensureEnding();
        str.shrink();
    }

    char& operator[](const size_t index) {
        return str[index];
    }

    const char& operator[](const size_t index) const {
        return str[index];
    }

    char& charAt(const size_t index) {
        if (index >= length()) {
            throw std::out_of_range("String::charAt");
        }
        return (*this)[index];
    }

    [[nodiscard]] const char& charAt(const size_t index) const {
        if (index >= length()) {
            throw std::out_of_range("String::charAt");
        }
        return (*this)[index];
    }

    char& front() {
        if (empty()) {
            throw std::out_of_range("String::front");
        }
        return str.front();
    }

    char& back() {
        if (empty()) {
            throw std::out_of_range("String::back");
        }
        return str[length() - 1];
    }

    [[nodiscard]] const char& front() const {
        if (empty()) {
            throw std::out_of_range("String::front");
        }
        return str.front();
    }

    [[nodiscard]] const char& back() const {
        if (empty()) {
            throw std::out_of_range("String::back");
        }
        return str[length() - 1];
    }

    [[nodiscard]] const char* cStr() const noexcept {
        return str.begin();
    }

    [[nodiscard]] const char* dataRaw() const noexcept {
        return cStr();
    }

    char* dataRaw() noexcept {
        return str.getData();
    }

    [[nodiscard]] const Array<char>& data() const noexcept {
        return str;
    }

    char* begin() noexcept {
        return str.begin();
    }

    char* end() noexcept {
        return str.getData() + length();
    }

    [[nodiscard]] const char* begin() const noexcept {
        return str.begin();
    }

    [[nodiscard]] const char* end() const noexcept {
        return str.getData() + length();
    }

    [[nodiscard]] const char* cBegin() const noexcept {
        return begin();
    }

    [[nodiscard]] const char* cEnd() const noexcept {
        return end();
    }

    void append(const String& subStr) {
        if (subStr.empty()) {
            return;
        }
        if (this == &subStr) {
            const String& tmp(subStr);
            append(tmp);
            return;
        }
        const size_t oldLen = length();
        reserve(oldLen + subStr.length());
        str.popBack();
        for (size_t i = 0; i < subStr.length(); ++i) {
            str.pushBack(subStr[i]);
        }
        str.pushBack('\0');
    }

    void append(const char* cStr) {
        if (isEnough(cStr)) {
            const String tmp(cStr);
            append(tmp);
            return;
        }
        const size_t subLen = cStrLen(cStr);
        if (subLen == 0) {
            return;
        }
        const size_t oldLen = length();
        reserve(oldLen + subLen);
        str.popBack();
        for (size_t i = 0; i < subLen; ++i) {
            str.pushBack(cStr[i]);
        }
        str.pushBack('\0');
    }

    void append(const size_t nn, const char ch) {
        if (nn == 0) {
            return;
        }
        const size_t oldLen = length();
        reserve(oldLen + nn);
        str.popBack();
        for (size_t i = 0; i < nn; ++i) {
            str.pushBack(ch);
        }
        str.pushBack('\0');
    }

    String& operator+=(const String& subStr) {
        append(subStr);
        return *this;
    }

    String& operator+=(const char* cStr) {
        append(cStr);
        return *this;
    }

    String& operator+=(const char ch) {
        append(1, ch);
        return *this;
    }

    void insert(const size_t pos, const String& subStr) {
        if (pos > length()) {
            throw std::out_of_range("String::insert");
        }
        if (subStr.empty()) {
            return;
        }
        if (this == &subStr) {
            const String& tmp(subStr);
            insert(pos, tmp);
            return;
        }
        const size_t oldLen = length();
        const size_t insLen = subStr.length();
        reserve(oldLen + insLen);
        str.popBack();
        for (size_t i = 0; i < insLen; ++i) {
            str.pushBack('\0');
        }
        for (size_t i = oldLen; i > pos; --i) {
            str[i + insLen - 1] = str[i - 1];
        }
        for (size_t i = 0; i < insLen; ++i) {
            str[pos + i] = subStr[i];
        }
        str.pushBack('\0');
    }

    void insert(const size_t pos, const char* cStr) {
        if (pos > length()) {
            throw std::out_of_range("String::insert");
        }
        if (isEnough(cStr)) {
            const String tmp(cStr);
            insert(pos, tmp);
            return;
        }
        const size_t insLen = cStrLen(cStr);
        if (insLen == 0) {
            return;
        }
        const size_t oldLen = length();
        reserve(oldLen + insLen);
        str.popBack();
        for (size_t i = 0; i < insLen; ++i) {
            str.pushBack('\0');
        }
        for (size_t i = oldLen; i > pos; --i) {
            str[i + insLen - 1] = str[i - 1];
        }
        for (size_t i = 0; i < insLen; ++i) {
            str[pos + i] = cStr[i];
        }
        str.pushBack('\0');
    }

    void insert(const size_t pos, const size_t nn, const char ch) {
        if (pos > length()) {
            throw std::out_of_range("String::insert");
        }
        if (nn == 0) {
            return;
        }
        const size_t oldLen = length();
        reserve(oldLen + nn);
        str.popBack();
        for (size_t i = 0; i < nn; ++i) {
            str.pushBack('\0');
        }
        for (size_t i = oldLen; i > pos; --i) {
            str[i + nn - 1] = str[i - 1];
        }
        for (size_t i = 0; i < nn; ++i) {
            str[pos + i] = ch;
        }
        str.pushBack('\0');
    }

    void resize(const size_t len) {
        resize(len, '\0');
    }

    void resize(const size_t len, const char ch) {
        const size_t oldLen = length();
        if (len == oldLen) {
            return;
        }
        if (len < oldLen) {
            while (length() > len) {
                popBack();
            }
            return;
        }
        append(len - oldLen, ch);
    }

    String& erase(const size_t pos, size_t len = static_cast<size_t>(-1)) {
        const size_t oldLen = length();
        if (pos > oldLen) {
            throw std::out_of_range("String::erase");
        }
        if (len == static_cast<size_t>(-1) || pos + len > oldLen) {
            len = oldLen - pos;
        }
        if (len == 0) {
            return *this;
        }
        for (size_t i = pos; i + len < oldLen; ++i) {
            str[i] = str[i + len];
        }
        for (size_t i = 0; i < len; ++i) {
            popBack();
        }
        return *this;
    }

    void pushBack(const char ch) {
        reserve(length() + 1);
        str.popBack();
        str.pushBack(ch);
        str.pushBack('\0');
    }

    void popBack() {
        if (empty()) {
            return;
        }
        str.popBack();
        str.popBack();
        str.pushBack('\0');
    }

    void clear() noexcept {
        str.clear();
        str.pushBack('\0');
    }

    [[nodiscard]] int compare(const String& other) const {
        const size_t nn = length() > other.length() ? other.length() : length();
        for (size_t i = 0; i < nn; ++i) {
            if (str[i] != other[i]) {
                return str[i] - other[i];
            }
        }
        if (length() == other.length()) {
            return 0;
        }
        return length() < other.length() ? -1 : 1;
    }

    [[nodiscard]] String substr(const size_t pos, size_t len = static_cast<size_t>(-1)) const {
        const size_t oldLen = length();
        if (pos > oldLen) {
            throw std::out_of_range("String::substr");
        }
        if (len == static_cast<size_t>(-1) || pos + len > oldLen) {
            len = oldLen - pos;
        }
        String ret;
        ret.reserve(len);
        ret.str.clear();
        for (size_t i = 0; i < len; ++i) {
            ret.str.pushBack(str[pos + i]);
        }
        ret.str.pushBack('\0');
        return ret;
    }

    [[nodiscard]] String cat(const String& other) const {
        String ret(*this);
        ret.append(other);
        return ret;
    }

    [[nodiscard]] size_t find(const char ch, const size_t pos = 0) const {
        for (size_t i = pos; i < length(); ++i) {
            if (str[i] == ch) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }

    [[nodiscard]] size_t find(const String& subStr, const size_t pos = 0) const {
        if (subStr.length() == 0) {
            return pos <= length() ? pos : static_cast<size_t>(-1);
        }
        if (subStr.length() > length()) {
            return static_cast<size_t>(-1);
        }
        for (size_t i = pos; i + subStr.length() <= length(); ++i) {
            size_t j = 0;
            while (j < subStr.length() && str[i + j] == subStr[j]) {
                ++j;
            }
            if (j == subStr.length()) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }

    void swap(String& other) noexcept {
        str.swap(other.str);
    }

    void replace(const size_t pos, const String& subStr) {
        if (pos > length()) {
            throw std::out_of_range("String::replace");
        }
        if (subStr.empty()) {
            return;
        }
        if (this == &subStr) {
            const String& tmp(subStr);
            replace(pos, tmp);
            return;
        }
        const size_t oldLen = length();
        const size_t subLen = subStr.length();
        if (pos + subLen > oldLen) {
            resize(pos + subLen, '\0');
        }
        for (size_t i = 0; i < subLen; ++i) {
            str[pos + i] = subStr[i];
        }
    }

    bool operator==(const String& other) const {
        return compare(other) == 0;
    }

    bool operator!=(const String& other) const {
        return compare(other) != 0;
    }

    bool operator<(const String& other) const {
        return compare(other) < 0;
    }

    bool operator>(const String& other) const {
        return compare(other) > 0;
    }

    bool operator<=(const String& other) const {
        return compare(other) <= 0;
    }

    bool operator>=(const String& other) const {
        return compare(other) >= 0;
    }
};

inline String operator+(const String& str, const String& other) {
    return str.cat(other);
}

inline std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << str.cStr();
}

inline void swap(String& str1, String& str2) noexcept {
    str1.swap(str2);
}

}

#endif // STRING_H
