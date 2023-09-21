import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import normaltest

filename = "points_dimension_50.txt"  

points = np.loadtxt(filename)

distances = []
numPoints = len(points)
for i in range(numPoints):
    for j in range(i + 1, numPoints):
        distance = np.linalg.norm(points[i] - points[j])
        distances.append(distance)

plt.hist(distances, bins=10)
plt.xlabel('Distancia')
plt.ylabel('Frecuencia')
plt.title('Histograma de Distancias')
plt.show()
mean = np.mean(distances)
median = np.median(distances)
std_dev = np.std(distances)

print("Media:", mean)
print("Mediana:", median)
print("Desviación estándar:", std_dev)

_, p_value = normaltest(distances)

if p_value < 0.05:
    print("Los datos no siguen una distribución normal.")
else:
    print("Los datos siguen una distribución normal.")
