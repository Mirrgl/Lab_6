while True:
    try:
        n = int(input("n: "))
        if n < 3 or n > 20000000000:
            raise ValueError("n должно быть больше 3, но меньше либо равно 2*10**9")
        m = int(input("m: "))
        if m < 3 or m > 20000000000:
            raise ValueError("m должно быть больше 3, но меньше либо равно 2*10**9")
        t = int(input("t: "))
        if t < 1 or t >= n * m:
            raise ValueError("t должно быть меньше произведения n и m, но больше 0")
        break
    except ValueError as e:
        print(f"Ошибка: {e}")
        print("Введите значения заново")

square = 0
length = 0
while True:
    # Calculate perimeter of current rectangle
    perimeter = 2 * (n + m) - 4
    
    # Add to total square
    square += perimeter
    length += 1
    
    # Prepare for next iteration
    n -= 2
    m -= 2
    
    # Check if we can add next perimeter without exceeding t
    next_perimeter = 2 * (n + m) - 4
    if next_perimeter < 0:  # Negative dimensions, stop
        break
    if square + next_perimeter > t:  # Would exceed t, stop
        break
        
print(length)