#include "BplusTree.h"
#include <chrono>
#include <fstream>
#include <iostream>

void test() {
    // Read file
    std::ifstream texto;
    texto.open("./output.txt");

    int datos[1000000];
    int element;

    if (!texto.is_open()) {
        std::cout << "No se pudo abrir el archivo output.txt, pruebe desde otra ruta (./output.txt)" << std::endl;
        return;
    }
    if (texto.is_open()) {
        int i = 0;
        while (texto >> element) {
            datos[i++] = element;
        }
    }

    int64_t tiempoInseccion = 0;
    for (int t = 0; t < 10; ++t) {
        auto tree = new BplusTree();

        std::chrono::steady_clock::time_point begin =
                std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            tree->insert(datos[i]);
        }
        std::chrono::steady_clock::time_point end =
                std::chrono::steady_clock::now();

        // Calcular tiempo
        auto tiempo =
                std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                        .count();
        tiempoInseccion += tiempo;
        delete tree;
    }

    std::cout << "Tiempo de insercion: " << tiempoInseccion << std::endl;
}

int main() {
    test();
    return 0;
}
