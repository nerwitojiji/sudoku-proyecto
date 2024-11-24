#include <iostream>
#include <vector>
#include <cmath>

#define MAX_SIZE 25 // Tamaño máximo permitido del tablero

using namespace std;

// Función para verificar si un número es válido en una posición específica
bool isValid(const vector<vector<int>>& board, int row, int col, int num, int n) {
    int sqrtN = sqrt(n); // Tamaño de las submatrices (bloques)

    // Verificar la fila y columna
    for (int i = 0; i < n; i++) {
        if (board[row][i] == num || board[i][col] == num)
            return false;
    }

    // Verificar el bloque 3x3 (o sqrt(n)x sqrt(n))
    int startRow = row / sqrtN * sqrtN;
    int startCol = col / sqrtN * sqrtN;
    for (int i = 0; i < sqrtN; i++) {
        for (int j = 0; j < sqrtN; j++) {
            if (board[startRow + i][startCol + j] == num)
                return false;
        }
    }

    return true;
}

// Función recursiva para resolver el Sudoku
bool solveSudoku(vector<vector<int>>& board, int n) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            // Si la celda está vacía
            if (board[row][col] == 0) {
                // Probar números del 1 al n
                for (int num = 1; num <= n; num++) {
                    if (isValid(board, row, col, num, n)) {
                        board[row][col] = num; // Asignar número

                        // Llamada recursiva para resolver el resto del tablero
                        if (solveSudoku(board, n))
                            return true;

                        // Si no se encuentra solución, deshacer el cambio
                        board[row][col] = 0;
                    }
                }
                return false; // No se encontró un número válido
            }
        }
    }
    return true; // Tablero resuelto
}

// Función para imprimir el tablero
// Función para imprimir el tablero en el formato de la imagen
void printBoard(const vector<vector<int>>& board, int n) {
    cout << "{" << endl; // Abrir la matriz
    for (int i = 0; i < n; i++) {
        cout << "    {";
        for (int j = 0; j < n; j++) {
            cout << board[i][j];
            if (j != n - 1) cout << ", "; // Agregar coma entre elementos
        }
        cout << "}";
        if (i != n - 1) cout << ","; // Agregar coma entre filas excepto la última
        cout << endl;
    }
    cout << "}" << endl; // Cerrar la matriz
}


int main() {
    int n;
    cout << "Ingrese el tamaño del tablero (n x n, donde n = a^2 y n <= " << MAX_SIZE << "): ";
    cin >> n;

    if (n > MAX_SIZE || sqrt(n) != (int)sqrt(n)) {
        cout << "Tamaño inválido. n debe ser un cuadrado perfecto y menor o igual a " << MAX_SIZE << "." << endl;
        return 1;
    }

    vector<vector<int>> board(n, vector<int>(n));

    cout << "Ingrese el tablero de Sudoku (use 0 para celdas vacías):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    if (solveSudoku(board, n)) {
        cout << "Tablero resuelto:" << endl;
        printBoard(board, n);
    } else {
        cout << "No se encontró solución para este tablero." << endl;
    }

    return 0;
}
