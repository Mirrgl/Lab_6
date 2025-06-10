import math

def print_vector(vec):
    for element in vec:
        print(f"{element:8.3f}", end="")
    print()

def print_matrix(mat):
    for row in mat:
        print_vector(row)

def gauss(a, b):
    n = len(a)
    a = [row[:] for row in a]  # Create a copy of the matrix
    b = b[:]  # Create a copy of the vector

    # Forward elimination (triangularization)
    for k in range(n):
        # Partial pivoting - find row with max absolute value in current column
        max_row = k
        for i in range(k + 1, n):
            if abs(a[i][k]) > abs(a[max_row][k]):
                max_row = i

        # Swap rows for stability
        a[k], a[max_row] = a[max_row], a[k]
        b[k], b[max_row] = b[max_row], b[k]

        # Normalize the pivot row
        pivot = a[k][k]
        for j in range(k + 1, n):
            a[k][j] /= pivot
        b[k] /= pivot
        a[k][k] = 1.0

        # Eliminate variable x_k from subsequent equations
        for i in range(k + 1, n):
            factor = a[i][k]
            for j in range(k + 1, n):
                a[i][j] -= factor * a[k][j]
            b[i] -= factor * b[k]
            a[i][k] = 0.0

    print("После прямого хода:")
    print("Матрица А:")
    print_matrix(a)
    print("Матрица В:")
    print_vector(b)
    print()

    # Back substitution
    x = [0.0] * n
    for k in range(n - 1, -1, -1):
        x[k] = b[k]
        for j in range(k + 1, n):
            x[k] -= a[k][j] * x[j]

    print("После обратного хода:")
    print("Mатрица A:")
    print_matrix(a)
    print("Maтрица B:")
    print_vector(b)
    print()

    return x

def transform_system(A, b):
    n = len(A)
    C = [[0.0] * n for _ in range(n)]
    f = [0.0] * n
    
    for i in range(n):
        f[i] = b[i] / A[i][i]
        for j in range(n):
            if i != j:
                C[i][j] = -A[i][j] / A[i][i]
    
    return C, f

def vector_norm(v):
    return max(abs(val) for val in v)

def simple_iteration(A, b, epsilon):
    n = len(A)
    
    # Transform system to x = Cx + f form
    C, f = transform_system(A, b)
    
    # Initial approximation (zero vector)
    x_current = [0.0] * n
    x_next = [0.0] * n
    
    iteration = 0
    diff = float('inf')
    
    while diff >= epsilon:
        # Compute next approximation
        for i in range(n):
            x_next[i] = f[i]
            for j in range(n):
                x_next[i] += C[i][j] * x_current[j]
        
        # Check stopping condition
        delta = [abs(x_next[i] - x_current[i]) for i in range(n)]
        diff = vector_norm(delta)

        # Update current approximation
        x_current = x_next.copy()
        iteration += 1
        
        # Print iteration info
        print(f"Итерация {iteration}: ", end="")
        for val in x_current:
            print(f"{val:10.6f} ", end="")
        print()
    
    print(f"Решение было найдена за {iteration} итераций")
    return x_current



a = [
    [0.79, -0.04, 0.21, -18],
    [0.25, -1.23, 0.18, -0.09],
    [-0.21, 0.18, 0.8, -0.13],
    [0.15, -1.31, 0.06, -0.86]
]
b = [-1.24, -0.86, 2.56, 0.79]

solution = gauss(a, b)

print("Решение системы:")
for i, val in enumerate(solution):
    print(f"x[{i + 1}] = {val:.6f}")

epsilon = float(input("Введите точность: "))

solution2 = simple_iteration(a, b, epsilon)

print("Решение системы:")
for i, val in enumerate(solution2):
    print(f"x[{i + 1}] = {val:.6f}")