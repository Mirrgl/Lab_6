import random
from typing import List

def print_vector(v):
    for element in v:
        print(f"{element:4d}", end="")
    print()

def print_matrix(matrix):
    for row in matrix:
        print_vector(row)
    print()

def create_matrix(start, end, M, N = 0):
    if N == 0:
        N = M
    
    matrix = []
    for _ in range(M):
        matrix_line = [random.randint(start,end) for _ in range(N)]
        matrix.append(matrix_line)
    
    return matrix

def find_line(matrix):
    max_count = -1
    max_line = []
    
    for line in matrix:
        current_count = sum(1 for x in line if x < 0)
        if current_count > max_count:
            max_count = current_count
            max_line = line
    
    return max_line

def first_task():
    print("-----------Первое задание-----------")
    
    print("Ввод M и N (M > 7, N > 5)")
    while True:
        try:
            m, n = map(int, input().split())
            if m > 7 and n > 5:
                break
            else:
                print("Неправильный ввод. M > 7, N > 5")
        except ValueError:
            print("Неправильный ввод. M > 7, N > 5")
    
    matrix = create_matrix(-10, 10, m, n)
    
    print_matrix(matrix)
    
    max_line = find_line(matrix)
    line_index = matrix.index(max_line) + 1
    
    print(f"Строка с наибольшим количеством отрицательных значений: {line_index}")
    print_vector(max_line)

def second_task():
    print("-----------Второе задание-----------")
    engine = random.SystemRandom()
    
    print("Ввод M (M > 5)")
    while True:
        try:
            m = int(input())
            if m > 5:
                break
            else:
                print("Неправильный ввод. M > 5")
        except ValueError:
            print("Неправильный ввод. M > 5")
    
    matrix = create_matrix(10, 100, m)
    
    print_matrix(matrix)
    
    diag1 = [matrix[i][i] for i in range(m)]
    diag1.sort(reverse=True)
    
    print("Первая диагональ: ", end="")
    print_vector(diag1)
    
    diag2 = [matrix[m - i - 1][i] for i in range(m)]
    diag2.sort(reverse=True)
    
    print("Вторая диагональ: ", end="")
    print_vector(diag2)
    
    sum1 = sum(diag1)
    sum2 = sum(diag2)
    
    print(f"Сумма первой диагонали: {sum1}")
    print(f"Сумма второй диагонали: {sum2}")
    
    if sum1 > sum2:
        print("Сумма первой диагонали больше второй")
    elif sum1 < sum2:
        print("Сумма второй диагонали больше первой")
    else:
        print("Суммы одинаковы")

def input_board():
    board = []
    
    while True:
        input_string = input().strip()
        
        if input_string == "0":
            break
        
        board_line = list(input_string)
        
        for element in board_line:
            if element not in ('.', 'X'):
                raise ValueError("Доступны только символы \"X\" и \".\"")
        
        if board and len(board[-1]) != len(board_line):
            raise ValueError("Строки таблицы имеют разные длины")
        
        board.append(board_line)
        
        if len(board) >= 2 and len(board_line) >= 2:
            for i in range(len(board_line) - 1):
                square = [
                    board[-2][i], board[-2][i + 1],
                    board[-1][i], board[-1][i + 1]
                ]
                if square.count('X') >= 3:
                    raise ValueError("Между линкорами нет свободной клетки")
    
    return board

def count_battleships(board):
    if not board or not board[0]:
        return 0
    
    count = 0
    m, n = len(board), len(board[0])
    
    for i in range(m):
        for j in range(n):
            if board[i][j] == 'X':
                is_first_cell = True
                
                if j > 0 and board[i][j - 1] == 'X':
                    is_first_cell = False
                
                if i > 0 and board[i - 1][j] == 'X':
                    is_first_cell = False
                
                if is_first_cell:
                    count += 1
    
    return count

def third_task():
    print("-----------Третье задание-----------")
    
    print("Введите доску с линкорами для игры в морской бой. Доска вводится построчно")
    print("Знак \"X\" - это элемент линкора. Знак \".\" - это пустая клетка")
    print("Между знаками нет пробелов. Строки должны быть одинакового размера")
    print("Между короблями должна быть минимум одна свободная клетка")
    print("Доска заканчивается символом \"0\"\n")
    
    while True:
        try:
            board = input_board()
            break
        except ValueError as error:
            print(f"\nОшибка: {error}\nВведите доску заново:")
    
    print("\nПолучившаяся доска:\n")
    for row in board:
        print(''.join(row))
    print()
    
    result = count_battleships(board)
    print(f"Количество линкоров: {result}")

first_task()
print("\n" + "-" * 40 + "\n")
second_task()
print("\n" + "-" * 40 + "\n")
third_task()