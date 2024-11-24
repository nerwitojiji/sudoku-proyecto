#include <iostream>
#include <vector>
#include <cmath>

#define MAX_SIZE 25 // Tama�o m�ximo permitido del tablero

using namespace std;

// Funci�n para verificar si un n�mero es v�lido en una posici�n espec�fica
bool isValid(const vector<vector<int>>& board, int row, int col, int num, int n) {
    int sqrtN = sqrt(n); // Tama�o de las submatrices (bloques)

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

// Funci�n recursiva para resolver el Sudoku
bool solveSudoku(vector<vector<int>>& board, int n) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            // Si la celda est� vac�a
            if (board[row][col] == 0) {
                // Probar n�meros del 1 al n
                for (int num = 1; num <= n; num++) {
                    if (isValid(board, row, col, num, n)) {
                        board[row][col] = num; // Asignar n�mero

                        // Llamada recursiva para resolver el resto del tablero
                        if (solveSudoku(board, n))
                            return true;

                        // Si no se encuentra soluci�n, deshacer el cambio
                        board[row][col] = 0;
                    }
                }
                return false; // No se encontr� un n�mero v�lido
            }
        }
    }
    return true; // Tablero resuelto
}

// Funci�n para imprimir el tablero
// Funci�n para imprimir el tablero en el formato de la imagen
void printBoard(const vector<vector<int>>& board, int n) {
    cout << "{" << endl; // Abrir la matriz
    for (int i = 0; i < n; i++) {
        cout << "    {";
        for (int j = 0; j < n; j++) {
            cout << board[i][j];
            if (j != n - 1) cout << ", "; // Agregar coma entre elementos
        }
        cout << "}";
        if (i != n - 1) cout << ","; // Agregar coma entre filas excepto la �ltima
        cout << endl;
    }
    cout << "}" << endl; // Cerrar la matriz
}


int main() {
    int n;
    cout << "Ingrese el tama�o del tablero (n x n, donde n = a^2 y n <= " << MAX_SIZE << "): ";
    cin >> n;

    if (n > MAX_SIZE || sqrt(n) != (int)sqrt(n)) {
        cout << "Tama�o inv�lido. n debe ser un cuadrado perfecto y menor o igual a " << MAX_SIZE << "." << endl;
        return 1;
    }

    vector<vector<int>> board(n, vector<int>(n));

    cout << "Ingrese el tablero de Sudoku (use 0 para celdas vac�as):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    if (solveSudoku(board, n)) {
        cout << "Tablero resuelto:" << endl;
        printBoard(board, n);
    } else {
        cout << "No se encontr� soluci�n para este tablero." << endl;
    }

    return 0;
}
