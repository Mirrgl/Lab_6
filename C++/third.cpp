#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, vector<T>& out) {
    for (auto& element : out) {
    	os << setprecision(3) << fixed << setw(8) << element;
        if (find(out.begin(), out.end(), element) == (out.end() - 1)) os << " ";
    }
    os << endl;

    return os;
}

// Функция для решения системы линейных уравнений методом Гаусса
vector<double> gauss(vector<vector<double>> a, vector<double> b) {
    int n = a.size();

    // Прямой ход (приведение к треугольному виду)
    for (int k = 0; k < n; k++) {
        // Поиск ведущего элемента (частичный выбор)
        int max_row = k;
        for (int i = k + 1; i < n; ++i) {
            if (abs(a[i][k]) > abs(a[max_row][k])) {
                max_row = i;
            }
        }

        // Перестановка строк для устойчивости
        swap(a[k], a[max_row]);
        swap(b[k], b[max_row]);

        // Нормализация k-й строки
        for (int j = k + 1; j < n; ++j) {
            a[k][j] /= a[k][k];
        }
        b[k] /= a[k][k];
        a[k][k] = 1.0;

        // Исключение переменной x_k из последующих уравнений
        for (int i = k + 1; i < n; ++i) {
            double factor = a[i][k];
            for (int j = k + 1; j < n; ++j) {
                a[i][j] -= factor * a[k][j];
            }
            b[i] -= factor * b[k];
            a[i][k] = 0.0;
        }
    }

    cout << "После прямого хода:" << endl;
    cout << "Матрица A:" << endl;
    cout << a;
    cout << "Матрица B:" << endl;
    cout << b << endl;
    // Обратный ход (нахождение решения)
    vector<double> x(n);
    for (int k = n - 1; k >= 0; --k) {
        x[k] = b[k];
        for (int j = k + 1; j < n; ++j) {
            x[k] -= a[k][j] * x[j];
        }
    }

    cout << "После обратного хода:" << endl;
    cout << "Матрица A:" << endl;
    cout << a;
    cout << "Матрица B:" << endl;
    cout << b << endl;

    return x;
}

// Функция преобразования системы Ax=b к виду x = Cx + f
void transformSystem(const vector<vector<double>>& A, const vector<double>& b,
                    vector<vector<double>>& C, vector<double>& f) {
    int n = A.size();
    C.resize(n, vector<double>(n, 0.0));
    f.resize(n, 0.0);
    
    for (int i = 0; i < n; ++i) {
        f[i] = b[i] / A[i][i];
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                C[i][j] = -A[i][j] / A[i][i];
            }
        }
    }
}

// Функция для вычисления нормы вектора (максимальная по модулю компонента)
double vectorNorm(const vector<double>& v) {
    double norm = 0.0;
    for (double val : v) {
        if (abs(val) > norm) {
            norm = abs(val);
        }
    }
    return norm;
}

// Метод простых итераций
vector<double> simpleIteration(const vector<vector<double>>& A, const vector<double>& b,
                             double epsilon) {
    int n = A.size();
    
    // Преобразование системы к виду x = Cx + f
    vector<vector<double>> C;
    vector<double> f;
    transformSystem(A, b, C, f);
    
    // Начальное приближение (можно выбрать произвольно, здесь нулевой вектор)
    vector<double> x_current(n, 0.0);
    vector<double> x_next(n, 0.0);
    
    int iteration = 0;
    double diff;
    
    do {
        // Вычисление следующего приближения
        for (int i = 0; i < n; ++i) {
            x_next[i] = f[i];
            for (int j = 0; j < n; ++j) {
                x_next[i] += C[i][j] * x_current[j];
            }
        }
        
        // Проверка условия остановки
        vector<double> delta(n);
        for (int i = 0; i < n; ++i) {
            delta[i] = abs(x_next[i] - x_current[i]);
        }
        diff = vectorNorm(delta);

        // Обновление текущего приближения
        x_current = x_next;
        iteration++;
        
        // Вывод информации о текущей итерации (можно закомментировать)
        cout << "Итерация " << iteration << ": ";
        for (double val : x_current) {
            cout << setw(10) << val << " ";
        }    
    } while (diff >= epsilon);
    
    cout << "Решение найдено за " << iteration << " итераций" << endl;
    return x_current;
}


int main() {
    vector<vector<double>> a = {
        {0.79,-0.04,0.21,-18},
        {0.25,-1.23,0.18,-0.09},
        {-0.21,0.18,0.8,-0.13},
        {0.15,-1.31,0.06,-0.86}
    };
    vector<double> b = {-1.24,-0.86,2.56,0.79};

    vector<double> solution = gauss(a, b);

    cout << "Решение системы:" << endl;
    for (int i = 0; i < solution.size(); ++i) {
        cout << "x[" << i + 1 << "] = " << solution[i] << endl;
    }

    double epsilon;
    cout << "Введите точность вычислений: ";
    cin >> epsilon;
    
    vector<double> solution2 = simpleIteration(a, b, epsilon);
    
    cout << "Решение системы:" << endl;
    for (int i = 0; i < solution2.size(); ++i) {
        cout << "x[" << i+1 << "] = " << solution2[i] << endl;
    }

    return 0;
}