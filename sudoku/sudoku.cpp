#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "nlohmann/json.hpp"

#define MAX_SIZE 25

using namespace std;
using json = nlohmann::json;

// Estructura auxiliar para rastrear números usados
struct Tracker {
    vector<vector<bool>> rows, cols, blocks;
    int sqrtN;

    Tracker(int n) : rows(n, vector<bool>(n + 1, false)),
                     cols(n, vector<bool>(n + 1, false)),
                     blocks(n, vector<bool>(n + 1, false)),
                     sqrtN(sqrt(n)) {}

    bool isValid(int row, int col, int num) {
        int blockIndex = (row / sqrtN) * sqrtN + (col / sqrtN);
        return !rows[row][num] && !cols[col][num] && !blocks[blockIndex][num];
    }

    void placeNumber(int row, int col, int num) {
        int blockIndex = (row / sqrtN) * sqrtN + (col / sqrtN);
        rows[row][num] = cols[col][num] = blocks[blockIndex][num] = true;
    }

    void removeNumber(int row, int col, int num) {
        int blockIndex = (row / sqrtN) * sqrtN + (col / sqrtN);
        rows[row][num] = cols[col][num] = blocks[blockIndex][num] = false;
    }
};

// Función recursiva para resolver el Sudoku
bool solveSudoku(vector<vector<int>>& board, int n, Tracker& tracker) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (board[row][col] == 0) { // Si la celda está vacía
                for (int num = 1; num <= n; num++) {
                    if (tracker.isValid(row, col, num)) {
                        board[row][col] = num;
                        tracker.placeNumber(row, col, num);

                        if (solveSudoku(board, n, tracker)) return true;

                        board[row][col] = 0;
                        tracker.removeNumber(row, col, num);
                    }
                }
                return false; // No se encontró un número válido
            }
        }
    }
    return true; // Tablero resuelto
}

// Inicializar el Tracker con los valores iniciales del tablero
void initializeTracker(const vector<vector<int>>& board, int n, Tracker& tracker) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] != 0) {
                tracker.placeNumber(i, j, board[i][j]);
            }
        }
    }
}

// Función para imprimir el tablero
void printBoard(const vector<vector<int>>& board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Función para guardar el tablero resuelto en un archivo JSON
void saveBoardToJson(const vector<vector<int>>& board, int n, const string& outputFileName) {
    json j;
    j["size"] = n;
    j["board"] = board;

    ofstream outFile(outputFileName);
    if (outFile) {
        outFile << j.dump(4);
        cout << "Tablero resuelto guardado en: " << outputFileName << endl;
    } else {
        cout << "Error al escribir el archivo: " << outputFileName << endl;
    }
}

int main() {
    string filename;

    // Solicitar el nombre del archivo JSON
    cout << "Ingrese el nombre del archivo JSON: ";
    cin >> filename;

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error al abrir el archivo: " << filename << endl;
        return 1;
    }

    json jsonInput;
    inFile >> jsonInput;

    int n = jsonInput["size"];
    if (n > MAX_SIZE || sqrt(n) != (int)sqrt(n)) {
        cout << "Tamaño inválido. n debe ser un cuadrado perfecto y menor o igual a " << MAX_SIZE << "." << endl;
        return 1;
    }

    vector<vector<int>> board(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = jsonInput["board"][i][j];
        }
    }

    cout << "Tablero cargado desde el archivo:" << endl;
    printBoard(board, n);

    Tracker tracker(n);
    initializeTracker(board, n, tracker);

    double start = clock();
    if (solveSudoku(board, n, tracker)) {
        double end = clock();
        cout << "Tablero resuelto:" << endl;
        printBoard(board, n);
        saveBoardToJson(board, n, "solucion.json");
        cout << "Tiempo total de resolución: " << (end - start) / CLOCKS_PER_SEC << " segundos" << endl;
    } else {
        cout << "No se encontró solución para este tablero." << endl;
    }

    return 0;
}
