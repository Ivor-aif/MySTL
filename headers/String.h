//
// Created by Ivor_Aif on 2026/2/12.
//

#ifndef STRING_H
#define STRING_H

#include "Array.h"

namespace mySTL::containers {

class String {
private:
    Array<char> str;

    friend String operator+(const String& str, const String& other);
    friend std::ostream& operator<<(std::ostream& os, const String& str);

public:
    String() = default;

    explicit String(const char* cStr) {
        while (*cStr != '\0') {
            str.pushBack(*cStr++);
        }
    }

    String(const size_t nn, const char* subStr) {
        for (size_t i = 0; i < nn; i++) {
            const char* sPtr = subStr;
            while (*sPtr != '\0') {
                str.pushBack(*sPtr++);
            }
        }
    }

    String(const size_t nn, const char ch) {
        for (size_t i = 0; i < nn; i++) {
            str.pushBack(ch);
        }
    }

    String(const String& other) {
        for (const char& ch: other) {
            str.pushBack(ch);
        }
    }

    String(String&& other) noexcept : str(std::move(other.str)) {

    }

    String& operator=(const String&) = default;
    String& operator=(String&&) noexcept = default;
    String& operator=(const char*) = default;
    ~String() = default;

    [[nodiscard]] size_t length() const noexcept {
        return str.getSize();
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return str.capacity();
    }

    [[nodiscard]] bool empty() const noexcept {
        return str.empty();
    }

    void reserve(const size_t len) {
        str.reserve(len);
    }

    void shrink() {
        str.shrink();
    }

    char& operator[](const size_t index) {
        return str[index];
    }

    const char& operator[](const size_t index) const {
        return str[index];
    }

    char& charAt(const size_t index) {
        return str.get(index);
    }

    [[nodiscard]] const char& charAt(const size_t index) const {
        return str.get(index);
    }

    char& front() {
        return str.front();
    }

    char& back() {
        return str.back();
    }

    [[nodiscard]] const char* cStr() const noexcept {
        auto ret = static_cast<char*>(malloc(1 + str.getSize()));
        for (const char& ch: str) {
            *ret++ = ch;
        }
        *ret = '\0';
        return ret;
    }

    [[nodiscard]] const Array<char>& data() const noexcept {
        return str;
    }

    Array<char>& data() noexcept {
        return str;
    }

    char* begin() noexcept {
        return str.begin();
    }

    char* end() noexcept {
        return str.end();
    }

    [[nodiscard]] const char* begin() const noexcept {
        return str.begin();
    }

    [[nodiscard]] const char* end() const noexcept {
        return str.end();
    }

    void append(const String& subStr) {
        for (const char& ch: subStr) {
            str.pushBack(ch);
        }
    }

    void append(const char* cStr) {
        while (*cStr != '\0') {
            str.pushBack(*cStr++);
        }
    }

    void append(const size_t nn, const char ch) {
        for (size_t i = 0; i < nn; i++) {
            str.pushBack(ch);
        }
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
        str.pushBack(ch);
        return *this;
    }

    void pushBack(const char ch) {
        str.pushBack(ch);
    }

    void popBack() {
        str.popBack();
    }

    void clear() noexcept {
        str.clear();
    }

    [[nodiscard]] int compare(const String& other) const {
        for (size_t i = 0; i < length(); ++i) {
            if (other.length() < i) {
                return str[i];
            }
            if (str[i] != other[i]) {
                return str[i] - other[i];
            }
        }
        if (length() < other.length()) {
            return -other[length()];
        }
        return 0;
    }

    [[nodiscard]] String substr(const size_t pos, const size_t len) const {
        String ret;
        for (size_t i = 0; i < len; ++i) {
            ret.pushBack(str.get(pos + i));
        }
        return ret;
    }

    String cat(const String &other) {
        for (const char& ch: other) {
            str.pushBack(ch);
        }
        return *this;
    }

    [[nodiscard]] size_t find(char ch, const size_t pos) const {
        size_t ret = 0;
        for (size_t i = pos; i < length(); ++i) {
            ret += str.get[i] == ch;
        }
        return ret;
    }

    void replace(const size_t pos, const String& subStr) {
        size_t index = pos;
        const size_t len = subStr.length();
        for ( ; index < len; ++index) {
            if (index >= length()) {
                break;
            }
            str[index] = subStr[index - pos];
        }
        for ( ; index < len; ++index) {
            str.pushBack(subStr[index - pos]);
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

inline String operator+(String& str, const String& other) {
    return str.cat(other);
}

inline std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << str.data();
}

}

#endif // STRING_H
