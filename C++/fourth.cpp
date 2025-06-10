#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, m, t;

    while (true) {
        try {
            if (!(cin >> n) || n > 2e10 || n < 3) throw string{"n должно быть больше 3, но меньше либо равно 2*10**9"};
            if (!(cin >> m) || m > 2e10 || m < 3) throw string{"m должно быть больше 3, но меньше либо равно 2*10**9"};
            if (!(cin >> t) || t < 1 || t >= n * m) throw string{"t должно быть меньше произведения n и m, но больше 0"};
            break;
        }
        catch (const string& e) {
            cout << "Ошибка: " << e << endl << "Введите значения заново" << endl;
        }
    }

    int square = 0;
    int length = 0;
    do {
        square += 2 * (n + m) - 4;
        length++;
        n -= 2;
        m -= 2;
    } while (square + (2 * (n + m) - 4) <= t);

    cout << length << endl;

    return 0;
}