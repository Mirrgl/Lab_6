import random
from typing import List

def print_vector(vec: List[int]) -> None:
    for element in vec:
        print(f"{element:4}", end="")
    print()

def print_matrix(matrix: List[List[int]]) -> None:
    for row in matrix:
        print_vector(row)
    print()

def create_matrix(start, end, M: int, N: int = None) -> List[List[int]]:
    if N is None:
        N = M
    
    matrix = []
    for _ in range(M):
        matrix_line = [random.randint(start, end) for _ in range(N)]
        matrix.append(matrix_line)
    
    return matrix

def find_line(matrix: List[List[int]]) -> List[int]:
    max_count = -1
    max_line = []
    
    for line in matrix:
        current_count = sum(1 for x in line if x < 0)
        if current_count > max_count:
            max_count = current_count
            max_line = line
    
    return max_line

def first_task() -> None:
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

def process_diagonal_element(value: int) -> int:
    tens = value // 10
    units = value % 10
    digits = [tens, units]
    digits.sort(reverse=True)
    return digits[0] * 10 + digits[1]

def second_task() -> None:
    print("-----------Второе задание-----------")    
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
    
    # Process main diagonal
    diag1 = [matrix[i][i] for i in range(m)]
    diag1_processed = [process_diagonal_element(val) for val in diag1]
    print("Первая диагональ: ", end="")
    print_vector(diag1_processed)
    
    # Process anti-diagonal
    diag2 = [matrix[m - i - 1][i] for i in range(m)]
    diag2_processed = [process_diagonal_element(val) for val in diag2]
    print("Вторая диагональ: ", end="")
    print_vector(diag2_processed)
    
    sum1 = sum(diag1_processed)
    sum2 = sum(diag2_processed)
    
    print(f"Сумма первой диагонали: {sum1}")
    print(f"Сумма второй диагонали: {sum2}")
    
    if sum1 > sum2:
        print("Сумма первой диагонали больше второй")
    elif sum1 < sum2:
        print("Сумма второй диагонали больше первой")
    else:
        print("Суммы одинаковы")

def input_board() -> List[List[str]]:
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
                # Check 2x2 square: top-left, top-right, bottom-left, bottom-right
                square = [
                    board[-2][i], board[-2][i + 1],
                    board[-1][i], board[-1][i + 1]
                ]
                if square.count('X') >= 3:
                    raise ValueError("Между линкорами нет свободной клетки")
    
    return board

def count_battleships(board: List[List[str]]) -> int:
    if not board or not board[0]:
        return 0
    
    count = 0
    m, n = len(board), len(board[0])
    
    for i in range(m):
        for j in range(n):
            if board[i][j] == 'X':
                is_first_cell = True
                
                # Check left neighbor
                if j > 0 and board[i][j - 1] == 'X':
                    is_first_cell = False
                
                # Check top neighbor
                if i > 0 and board[i - 1][j] == 'X':
                    is_first_cell = False
                
                if is_first_cell:
                    count += 1
    
    return count

def third_task() -> None:
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

if __name__ == "__main__":
    first_task()
    print("\n" + "-" * 40 + "\n")
    second_task()
    print("\n" + "-" * 40 + "\n")
    third_task()