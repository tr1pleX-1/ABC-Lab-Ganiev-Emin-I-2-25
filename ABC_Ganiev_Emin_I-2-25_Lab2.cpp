#include <iostream>
#include <cstdint>
#include <cstring>
using namespace std;

// float → IEEE754 (sign, exponent, mantissa)
void float_to_ieee754(float num, uint32_t &sign, uint32_t &exp, uint32_t &mant) {
    uint32_t bits;
    memcpy(&bits, &num, sizeof(bits));

    sign = (bits >> 31) & 1;
    exp  = (bits >> 23) & 0xFF;
    mant = bits & 0x7FFFFF;
}

// IEEE754 → float
float ieee754_to_float(uint32_t sign, uint32_t exp, uint32_t mant) {
    uint32_t bits = (sign << 31) | (exp << 23) | mant;
    float num;
    memcpy(&num, &bits, sizeof(num));
    return num;
}

// Побитовое вычитание IEEE754
float ieee754_sub(float a, float b) {
    uint32_t s1, e1, m1;
    uint32_t s2, e2, m2;

    float_to_ieee754(a, s1, e1, m1);
    float_to_ieee754(b, s2, e2, m2);

    // Добавляем скрытый бит
    m1 |= (1 << 23);
    m2 |= (1 << 23);

    // Выравнивание порядков
    uint32_t e = 0;
    if (e1 > e2) {
        m2 >>= (e1 - e2);
        e = e1;
    } else {
        m1 >>= (e2 - e1);
        e = e2;
    }

    // Меняем знак второго числа (вычитание)
    s2 ^= 1;

    uint32_t m;
    uint32_t s;

    // Складываем/вычитаем мантиссы
    if (s1 == s2) {
        m = m1 + m2;
        s = s1;
    } else {
        if (m1 >= m2) {
            m = m1 - m2;
            s = s1;
        } else {
            m = m2 - m1;
            s = s2;
        }
    }

    // Нормализация
    while (m && m < (1 << 23)) {
        m <<= 1;
        e--;
    }

    // Убираем скрытый бит
    m &= 0x7FFFFF;

    return ieee754_to_float(s, e, m);
}

int main() {
    float a = 5.75f;
    float b = 2.5f;

    float result = ieee754_sub(a, b);

    cout << "Результат: " << result << endl;
    return 0;
}
