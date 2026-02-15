#include <iostream>

int main()
{
    setlocale(LC_ALL, "Russian");
    unsigned long long n;
    std::cout << "Введите число: ";
    std::cin >> n;

    unsigned long long result = 0;
    unsigned long long bit = 1ULL << 62;

    while (bit > n) {
        bit >>= 2;
    }

    while (bit != 0)
    {
        if (result + bit <= n / (result + bit))
        {
            result += bit;
        }

        bit >>= 2;

        result <<= 1;
    }

    result >>= 1;

    std::cout << "Целая часть квадратного корня = " << result << "\n";
    std::cout << "Проверка: " << result << "² = " << result * result;

    return 0;
}