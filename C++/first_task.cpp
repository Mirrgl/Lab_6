#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, vector<T> out) {
    for (auto& element : out) {
    	os << setw(4) << element;
        if (find(out.begin(), out.end(), element) == (out.end() - 1)) os << " ";
    }
    os << endl;

    return os;
}

vector<vector<int>> create_matrix(mt19937_64& engine, uniform_int_distribution<>& distribution, int M, int N = 0) {
    vector<vector<int>> matrix;
    if (N == 0) N = M;
    
    for (int i = 0; i < M; i++) {
        vector<int> matrix_line;
        for (int j = 0; j < N; j++) {
            matrix_line.push_back(distribution(engine));
        }
        matrix.push_back(matrix_line);
    }

    return matrix;
}

vector<int> find_line(const vector<vector<int>>& matrix) {
    int max_count = 0;
    vector<int> max_line;

    for (auto& line : matrix) 
    {
        int current_count = count_if(line.begin(), line.end(), [](int x){return x < 0;});
        if (current_count > max_count) {
            max_count = current_count;
            max_line = line;
        }
    }

    return max_line;
}

void first(mt19937_64& engine) {
    int m,n;
    cout << "Ввод M и N (M > 7, N > 5)" << endl;
    while (true) {
        if ((cin >> m >> n) && m > 7 && n > 5) break;
        cout << "Неправильный ввод. M > 7, N > 5" << endl;
        cin.clear();
        cin.ignore(1000,'\n');
    }

    uniform_int_distribution<> distribution(-10, 10);
    vector<vector<int>> matrix = create_matrix(engine, distribution, m, n);

    cout << matrix << endl;

    vector<int> max_line = find_line(matrix);

    cout << "Строка с наибольшим количеством отрицательных значений: " << (find(matrix.begin(), matrix.end(), max_line) - matrix.begin()) + 1 << endl;
    cout << max_line << endl;
}

void second(mt19937_64& engine) {
        int m;
    cout << "Ввод M (M > 5)" << endl;
    while (true) {
        if ((cin >> m) && m > 5) break;
        cout << "Неправильный ввод. M > 5" << endl;
        cin.clear();
        cin.ignore(1000,'\n');
    }

    uniform_int_distribution<> dist(10,100);
    vector<vector<int>> matrix = create_matrix(engine, dist, m);

    cout << matrix << endl;
    
    vector<int> diag;

    for (int i = 0; i < m; i++) {
        diag.push_back(matrix[i][i]);
    }

    vector<int> temp;
    string tempInt;
    for (int i = 0; i < diag.size(); i++) {
        temp.push_back(diag[i] / 10);
        temp.push_back(diag[i] % 10);
        sort(temp.begin(), temp.end(), [](int x, int y){return x > y;});
        tempInt = string{} + to_string(temp[0]) + to_string(temp[1]);
        diag[i] = stoi(tempInt);
        temp = {};
        tempInt = "";
    }

    cout << "Первая диагональ: " << diag << endl;

    vector<int> diag2;

    for (int i = 0; i < m; i++) {
        diag2.push_back(matrix[m-i-1][i]);
    }

    temp = {};
    tempInt = "";
    for (int i = 0; i < diag2.size(); i++) {
        temp.push_back(diag2[i] / 10);
        temp.push_back(diag2[i] % 10);
        sort(temp.begin(), temp.end(), [](int x, int y){return x > y;});
        tempInt = string{} + to_string(temp[0]) + to_string(temp[1]);
        diag2[i] = stoi(tempInt);
        temp = {};
        tempInt = "";
    }

    cout << "Вторая диагональ: " << diag2 << endl;

    int sum1 = accumulate(diag.begin(), diag.end(), 0);

    int sum2 = accumulate(diag2.begin(), diag2.end(), 0);

    cout << "Сумма первой диагонали: " << sum1 << endl;

    cout << "Сумма второй диагонали: " << sum2 << endl;

    if (sum1 > sum2) {
        cout << "Сумма первой диагонали больше второй" << endl;
    }
    else if (sum1 < sum2) {
        cout << "Сумма второй диагонали больше первой" << endl;
    }
    else {
        cout << "Суммы одинаковы" << endl;
    }
}

vector<vector<char>> inputBoard() {
    vector<vector<char>> board;
    
    while(true) {
        string inputString;
        cin >> inputString;

        if (inputString == "0") break;

        vector<char> boardLine;
        
        istringstream inStream(inputString);
        char element;

        while (inStream >> element) {
            if (element != '.' && element != 'X') throw string{"Доступны только символы \"X\" и \".\""};

            boardLine.push_back(element);
        }

        if (!board.empty() && board.back().size() != boardLine.size()) throw string{"Строки таблицы имеют разные длины"};

        board.push_back(boardLine);

        if (board.size() >= 2 && boardLine.size() >= 2) {
            for (int i = 0; i < boardLine.size(); i++) {
                string square = string() + board[board.size() - 2][i] + board[board.size() - 2][i + 1] + board[board.size() - 1][i] + board[board.size() - 1][i + 1];
                if (count(square.begin(), square.end(), 'X') >= 3) throw string{"Между линкорами нет свободной клетки"};
            }
        }
    }

    return board;
}

int countBattleships(const vector<vector<char>>& board) {
    if (board.empty() || board[0].empty()) return 0;
    
    int count = 0;
    int m = board.size();
    int n = board[0].size();
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'X') {
                bool isFirstCell = true;
                
                if (j > 0 && board[i][j-1] == 'X') {
                    isFirstCell = false;
                }
                
                if (i > 0 && board[i-1][j] == 'X') {
                    isFirstCell = false;
                }
                
                if (isFirstCell) {
                    count++;
                }
            }
        }
    }
    
    return count;
}

void third() {
    vector<vector<char>> board;

    cout << "Введите доску с линкорами для игры в морской бой. Доска вводится построчно" << endl;
    cout << "Знак \"X\" - это элемент линкора. Знак \".\" - это пустая клетка" << endl;
    cout << "Между знаками нет пробелов. Строки должны быть одинакового размера" << endl;
    cout << "Между короблями должна быть минимум одна свободная клетка" << endl;
    cout << "Доска заканчивается символом \"0\"" << endl << endl;

    while(true) {
        try {
            board = inputBoard();
            break;
        }
        catch (const string& error){
            cout << endl << "Ошибка: " << error << endl << "Введите доску заново: " << endl;
        }
    }
    
    cout << endl << "Получившаяся доска:" << endl << endl;
    cout << board;

    int result = countBattleships(board);

    cout << "Количество линкоров: " << result << endl;
}

int main() {
    random_device rand;
    mt19937_64 engine(rand());

    cout << "-----------Первое задание-----------" << endl;
    first(engine);

    /*----------------------------------------------------------------------------------------*/

    cout << "-----------Второе задание-----------" << endl;
    second(engine);

    /*----------------------------------------------------------------------------------------*/

    cout << "-----------Третье задание-----------" << endl;
    third();
    
    return 0;
}