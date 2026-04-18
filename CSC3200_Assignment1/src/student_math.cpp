#include "student_math.h"

namespace student_std {

// 幂
static double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// 阶乘
static double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// 归一化角度到 [-π, π]，避免泰勒展开精度下降
static double normalize(double x) {
    const double PI = 3.14159265358979323846;
    const double TWO_PI = 2 * PI;
    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    return x;
}

// sin 
double sin(double x) {
    x = normalize(x);
    double term = x;  // 第一项
    double sum = x;
    for (int k = 1; k < 20; k++) { // 迭代 20 项保证精度
        term *= -1.0 * x * x / ((2 * k) * (2 * k + 1));
        sum += term;
    }
    return sum;
}

double sin_deg(double x) {
    const double PI = 3.14159265358979323846;
    return sin(x * PI / 180.0);
}

// cos
double cos(double x) {
    x = normalize(x);
    double term = 1.0; // 第一项
    double sum = 1.0;
    for (int k = 1; k < 20; k++) {
        term *= -1.0 * x * x / ((2 * k - 1) * (2 * k));
        sum += term;
    }
    return sum;
}

double cos_deg(double x) {
    const double PI = 3.14159265358979323846;
    return cos(x * PI / 180.0);
}

// tan
double tan(double x) {
    double c = cos(x);
    if (c == 0) return 1e10; // 近似无穷大
    return sin(x) / c;
}

double tan_deg(double x) {
    const double PI = 3.14159265358979323846;
    return tan(x * PI / 180.0);
}

// cot
double cot(double x) {
    double s = sin(x);
    if (s == 0) return 1e10; // 近似无穷大
    return cos(x) / s;
}

double cot_deg(double x) {
    const double PI = 3.14159265358979323846;
    return cot(x * PI / 180.0);
}

} // namespace student_std
