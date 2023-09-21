import matplotlib.pyplot as plt
import numpy as np

filename = "points_dimension_10.txt"  # Cambia el nombre del archivo según la dimensión deseada

# Leer los números aleatorios del archivo de texto
points = np.loadtxt(filename)

# Calcular la distancia euclidiana entre todos los pares de puntos
distances = []
numPoints = len(points)
for i in range(numPoints):
    for j in range(i + 1, numPoints):
        distance = np.linalg.norm(points[i] - points[j])
        distances.append(distance)

# Generar un histograma de las distancias obtenidas
plt.hist(distances, bins=10)
plt.xlabel('Distancia')
plt.ylabel('Frecuencia')
plt.title('Histograma de Distancias')
plt.show()
