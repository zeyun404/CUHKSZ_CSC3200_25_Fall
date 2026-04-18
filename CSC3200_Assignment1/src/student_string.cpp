#include "student_string.h"
#include <cctype>   // for toupper, tolower, isalpha
#include <cstring>  // for strcpy, strlen

namespace student_std {

// 默认构造函数
string::string() {
    c_str[0] = '\0';
    strlen = 0;
}

// 从 const char* 构造
string::string(const char* str) {
    int i = 0;
    while (str[i] != '\0' && i < MAXLEN - 1) {
        c_str[i] = str[i];
        i++;
    }
    c_str[i] = '\0';
    strlen = i;
}

// 拷贝构造
string::string(string const& other) {
    int i = 0;
    while (other.c_str[i] != '\0' && i < MAXLEN - 1) {
        c_str[i] = other.c_str[i];
        i++;
    }
    c_str[i] = '\0';
    strlen = other.strlen;
}

// 析构函数
string::~string() {
    // nothing to free (static array)
}

// 获取长度
int string::get_strlen() const {
    return strlen;
}

// 获取 C 风格字符串
const char* string::get_c_str() const {
    return c_str;
}

// 拼接
void string::strcat(string const& other) {
    int i = strlen;
    int j = 0;
    while (other.c_str[j] != '\0' && i < MAXLEN - 1) {
        c_str[i] = other.c_str[j];
        i++; j++;
    }
    c_str[i] = '\0';
    strlen = i;
}

// 赋值运算符
string& string::operator=(string const& other) {
    if (this != &other) {
        int i = 0;
        while (other.c_str[i] != '\0' && i < MAXLEN - 1) {
            c_str[i] = other.c_str[i];
            i++;
        }
        c_str[i] = '\0';
        strlen = other.strlen;
    }
    return *this;
}

// += 运算符
string& string::operator+=(string const& other) {
    strcat(other);
    return *this;
}

// 下标运算符
char& string::operator[](int index) {
    return c_str[index];
}

const char& string::operator[](int index) const {
    return c_str[index];
}

// 转大写
void string::to_upper() {
    for (int i = 0; i < strlen; i++) {
        if (c_str[i] >= 'a' && c_str[i] <= 'z')
            c_str[i] = c_str[i] - 'a' + 'A';
    }
}

// 转小写
void string::to_lower() {
    for (int i = 0; i < strlen; i++) {
        if (c_str[i] >= 'A' && c_str[i] <= 'Z')
            c_str[i] = c_str[i] - 'A' + 'a';
    }
}

// 拷贝另一个字符串
void string::strcopy(string const& other) {
    *this = other;
}

// 判断相等
bool string::equals(string const& other) const {
    if (strlen != other.strlen) return false;
    for (int i = 0; i < strlen; i++) {
        if (c_str[i] != other.c_str[i]) return false;
    }
    return true;
}

// 忽略大小写比较
bool string::equals_ignore_case(string const& other) const {
    if (strlen != other.strlen) return false;
    for (int i = 0; i < strlen; i++) {
        char a = c_str[i], b = other.c_str[i];
        if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
        if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
        if (a != b) return false;
    }
    return true;
}

// 去掉首尾空格
void string::trim() {
    int start = 0, end = strlen - 1;
    while (start <= end && c_str[start] == ' ') start++;
    while (end >= start && c_str[end] == ' ') end--;

    int j = 0;
    for (int i = start; i <= end; i++) {
        c_str[j++] = c_str[i];
    }
    c_str[j] = '\0';
    strlen = j;
}

// 判断是否全是字母
bool string::is_alphabetic() const {
    for (int i = 0; i < strlen; i++) {
        if (!((c_str[i] >= 'a' && c_str[i] <= 'z') || (c_str[i] >= 'A' && c_str[i] <= 'Z'))) {
            return false;
        }
    }
    return true;
}

} // namespace student_std
