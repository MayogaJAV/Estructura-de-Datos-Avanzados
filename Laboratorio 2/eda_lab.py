import random
import time
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MultipleLocator

# Generar un archivo de texto con 10,000 números aleatorios
def generate_random_numbers_file(file_name, n):
    with open(file_name, 'w') as file:
        for _ in range(n):
            number = random.randint(1, n)
            file.write(str(number) + '\n')

# Cargar los números desde el archivo de texto a una lista
def load_numbers_from_file(file_name):
    with open(file_name, 'r') as file:
        numbers = [int(line.strip()) for line in file]
    return numbers

# Algoritmo de Bubble Sort
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

# Algoritmo de Heap Sort
def heapify(arr, n, i):
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n and arr[i] < arr[left]:
        largest = left

    if right < n and arr[largest] < arr[right]:
        largest = right

    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        heapify(arr, n, largest)

def heap_sort(arr):
    n = len(arr)
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)
    for i in range(n - 1, 0, -1):
        arr[i], arr[0] = arr[0], arr[i]
        heapify(arr, i, 0)

# Algoritmo de Insertion Sort
def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and key < arr[j]:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key

# Algoritmo de Selection Sort
def selection_sort(arr):
    for i in range(len(arr)):
        min_idx = i
        for j in range(i + 1, len(arr)):
            if arr[j] < arr[min_idx]:
                min_idx = j
        arr[i], arr[min_idx] = arr[min_idx], arr[i]

# Algoritmo de Shell Sort
def shell_sort(arr):
    n = len(arr)
    gap = n // 2
    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            while j >= gap and arr[j - gap] > temp:
                arr[j] = arr[j - gap]
                j -= gap
            arr[j] = temp
        gap //= 2

# Algoritmo de Merge Sort
def merge_sort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2
        L = arr[:mid]
        R = arr[mid:]

        merge_sort(L)
        merge_sort(R)

        i = j = k = 0

        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1

        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1

        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1

# Algoritmo de Quick Sort
def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    else:
        pivot = arr[0]
        left = []
        right = []
        for x in arr[1:]:
            if x < pivot:
                left.append(x)
            else:
                right.append(x)
        return quick_sort(left) + [pivot] + quick_sort(right)

if __name__ == "__main__":
    # Generar y cargar números aleatorios
    file_name = "random_numbers.txt"
    generate_random_numbers_file(file_name, 1000000)
    numbers = load_numbers_from_file(file_name)

    # Realizar pruebas de rendimiento para cada algoritmo
    algorithms = {
        "Bubble Sort": bubble_sort,
        "Heap Sort": heap_sort,
        "Insertion Sort": insertion_sort,
        "Selection Sort": selection_sort,
        "Shell Sort": shell_sort,
        "Merge Sort": merge_sort,
        "Quick Sort": quick_sort
    }

    sizes = []
    execution_times = {algorithm_name: [] for algorithm_name in algorithms}

    for size in range(1000, 11000, 1000):  # Diferentes tamaños de lista
        sizes.append(size)
        arr = random.sample(range(1, 10001), size)  # Generar lista aleatoria de tamaño 'size'

        for algorithm_name, algorithm in algorithms.items():
            arr_copy = arr.copy()
            start_time = time.time()
            algorithm(arr_copy)
            execution_time = time.time() - start_time
            execution_times[algorithm_name].append(execution_time)

    # Crear gráficas individuales por algoritmo
    for algorithm_name in algorithms:
        plt.figure(figsize=(10, 6))
        plt.plot(sizes, execution_times[algorithm_name])
        plt.xlabel('Tamaño de la lista')
        plt.ylabel('Tiempo de ejecución (segundos)')
        plt.title(f'Rendimiento de {algorithm_name}')
        plt.grid(True)
        plt.show()

    # Crear gráfica comparativa de todos los algoritmos
    plt.figure(figsize=(10, 6))
    for algorithm_name in algorithms:
        plt.plot(sizes, execution_times[algorithm_name], label=algorithm_name)

    plt.xlabel('Tamaño de la lista')
    plt.ylabel('Tiempo de ejecución (segundos)')
    plt.title('Comparación de Rendimiento de Algoritmos de Ordenamiento')
    plt.legend()
    plt.grid(True)
    plt.show()
