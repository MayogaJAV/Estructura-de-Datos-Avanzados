#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>
#include <array>
#include <cmath>
#include <chrono>
#include <thread>

struct Punto {
    float x;
    float y;
    float z;
};

struct NodoOctree {
    std::vector<Punto> puntos;
    Punto puntoMinimo;
    Punto puntoMaximo;
    std::array<NodoOctree*, 8> hijos;

    NodoOctree(const Punto& min, const Punto& max) : puntoMinimo(min), puntoMaximo(max) {
        hijos.fill(nullptr);
    }

    ~NodoOctree() {
        for (NodoOctree* hijo : hijos) {
            delete hijo;
        }
    }
};

class Octree {
public:
    Octree(const Punto& min, const Punto& max) : raiz(new NodoOctree(min, max)) {}

    ~Octree() {
        delete raiz;
    }

    void insertar(const Punto& punto) {
        insertarRecursivo(raiz, punto);
    }

    NodoOctree* obtenerRaiz() const {
        return raiz;
    }

private:
    NodoOctree* raiz;

    void insertarRecursivo(NodoOctree* nodo, const Punto& punto) {
        if (nodo->hijos[0] == nullptr) {
            nodo->puntos.push_back(punto);
            return;
        }

        int octante = obtenerOctante(nodo, punto);
        if (octante >= 0) {
            if (nodo->hijos[octante] == nullptr) {
                nodo->hijos[octante] = crearHijo(nodo, octante);
            }
            insertarRecursivo(nodo->hijos[octante], punto);
        }
        else {
            nodo->puntos.push_back(punto);
        }
    }

    int obtenerOctante(const NodoOctree* nodo, const Punto& punto) const {
        float centroX = (nodo->puntoMinimo.x + nodo->puntoMaximo.x) / 2;
        float centroY = (nodo->puntoMinimo.y + nodo->puntoMaximo.y) / 2;
        float centroZ = (nodo->puntoMinimo.z + nodo->puntoMaximo.z) / 2;

        int octante = 0;
        if (punto.x > centroX) octante |= 1;
        if (punto.y > centroY) octante |= 2;
        if (punto.z > centroZ) octante |= 4;

        return octante;
    }

    NodoOctree* crearHijo(NodoOctree* nodo, int octante) {
        float minX, maxX, minY, maxY, minZ, maxZ;

        float centroX = (nodo->puntoMinimo.x + nodo->puntoMaximo.x) / 2;
        float centroY = (nodo->puntoMinimo.y + nodo->puntoMaximo.y) / 2;
        float centroZ = (nodo->puntoMinimo.z + nodo->puntoMaximo.z) / 2;

        if (octante & 1) {
            minX = centroX;
            maxX = nodo->puntoMaximo.x;
        }
        else {
            minX = nodo->puntoMinimo.x;
            maxX = centroX;
        }

        if (octante & 2) {
            minY = centroY;
            maxY = nodo->puntoMaximo.y;
        }
        else {
            minY = nodo->puntoMinimo.y;
            maxY = centroY;
        }

        if (octante & 4) {
            minZ = centroZ;
            maxZ = nodo->puntoMaximo.z;
        }
        else {
            minZ = nodo->puntoMinimo.z;
            maxZ = centroZ;
        }

        NodoOctree* hijo = new NodoOctree(Punto{ minX, minY, minZ }, Punto{ maxX, maxY, maxZ });
        return hijo;
    }
};

void leerPuntosDesdeArchivo(const std::string& nombreArchivo, Octree& octree) {
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return;
    }

    Punto punto;
    char coma;
    int contadorLinea = 0;

    while (archivo >> punto.x >> coma >> punto.y >> coma >> punto.z) {
        octree.insertar(punto);
        contadorLinea++;
    }

    archivo.close();
}

void dibujarPuntosOctree(const NodoOctree* nodo, float escalaPuntosPorNodo) {
    if (nodo == nullptr) {
        return;
    }

    for (const Punto& punto : nodo->puntos) {
        glPushMatrix();
        glTranslatef(punto.x, punto.y - 170, punto.z);

        float factorGradiente = (punto.y + 150) / 300.0;
        glColor3f(0.0, 0.0, 1.0);
        glutSolidCube(20.0 / escalaPuntosPorNodo); 

        glPopMatrix();
    }

    for (const NodoOctree* hijo : nodo->hijos) {
        dibujarPuntosOctree(hijo, escalaPuntosPorNodo);
    }
}

float anguloRotacionX = 0.0;
float anguloRotacionY = 0.0;
int ultimoMouseX = 0;
int ultimoMouseY = 0;
float escalaPuntosPorNodo = 10;  // Factor de escala para el tamaño de los cubos
Octree octree(Punto{ -100, -100, -100 }, Punto{ 100, 100, 100 });

void mostrar() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(anguloRotacionX, 1.0, 0.0, 0.0);
    glRotatef(anguloRotacionY, 0.0, 1.0, 0.0);

    dibujarPuntosOctree(octree.obtenerRaiz(), escalaPuntosPorNodo);

    glFlush();
    glutSwapBuffers();
}

void cambiarTamanoVentana(int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)ancho / (float)alto, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void manejarClickMouse(int boton, int estado, int x, int y) {
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        ultimoMouseX = x;
        ultimoMouseY = y;
    }
}

void manejarMovimientoMouse(int x, int y) {
    int deltaMouseX = x - ultimoMouseX;
    int deltaMouseY = y - ultimoMouseY;

    anguloRotacionX += deltaMouseY * 0.5;
    anguloRotacionY += deltaMouseX * 0.5;

    ultimoMouseX = x;
    ultimoMouseY = y;

    glutPostRedisplay();
}

void inactividad() {
    anguloRotacionY += 0.5;

    glutPostRedisplay();
    std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1500, 800);
    glutCreateWindow("Visualización Octree");

    leerPuntosDesdeArchivo("gato.txt", octree);

    glutDisplayFunc(mostrar);
    glutReshapeFunc(cambiarTamanoVentana);

    glutMouseFunc(manejarClickMouse);
    glutMotionFunc(manejarMovimientoMouse);

    glutIdleFunc(inactividad);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glutMainLoop();

    return 0;
}
