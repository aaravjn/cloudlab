import random

def generate_matrix(n : int) -> str:
    matrix = []
    for i in range(n):
        row = []
        for j in range(n):
            row.append(str(random.randint(1, 20)))
        matrix.append(row)
    
    result = ""

    for i in range(n):
        result += '.'.join(matrix[i])
        if i != n-1:
            result += ','

    result += '$'

    return result
