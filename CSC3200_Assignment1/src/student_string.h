#ifndef STUDENT_STRING_H
#define STUDENT_STRING_H

#define MAXLEN 256

namespace student_std {

class string {
public:
    string();
    string(const char* str);
    string(string const& other);
    ~string();

    int get_strlen() const;
    const char* get_c_str() const;

    void strcat(string const& other);
    string& operator=(string const& other);
    string& operator+=(string const& other);
    char& operator[](int index);
    const char& operator[](int index) const;

    void to_upper();
    void to_lower();
    void strcopy(string const& other);
    bool equals(string const& other) const;
    bool equals_ignore_case(string const& other) const;
    void trim(); // 去掉首尾空格
    bool is_alphabetic() const;

private:
    char c_str[MAXLEN];
    int strlen;
};

}

#endif
