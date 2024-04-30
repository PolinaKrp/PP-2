#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>


void writeMatrix(int** matrix, int rows, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            file << matrix[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        file << std::endl;
    }
    file.close();
}


void writeMatrixTime(int** matrix, int rows, int cols, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file << matrix[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        file << std::endl;
    }
    file.close();
}


void writeTime(int** res_time, int rows, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            file << res_time[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        if (i < rows - 1) {
            file << std::endl;
        }
    }
    file.close();
}


int** readMatrix(const char* file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return nullptr;
    }

    int rows = 0;
    std::string line;
    while (std::getline(file, line)) {
        rows++;
    }

    int** matrix = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[rows];
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    int row_index = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col_index = 0;

        while (std::getline(ss, cell, ',')) {
            matrix[row_index][col_index] = std::stoi(cell);
            col_index++;
        }

        row_index++;
    }

    return matrix;
}


void clearMemory(int** matrix, int rows) {
    if (matrix == nullptr) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        if (matrix[i] != nullptr) {
            delete[] matrix[i];
            matrix[i] = nullptr;
        }
    }

    delete[] matrix;
    matrix = nullptr;
}


int** MultiplyMatrix(int** matrix, int rows) {
    int i, j, k;
    int** matrix_new = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix_new[i] = new int[rows];
    }

#pragma omp parallel for shared(matrix, matrix_new) private(i, j, k)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < rows; j++) {
            int sum = 0;
            for (k = 0; k < rows; k++) {
                sum += (matrix[i][k] * matrix[k][j]);
            }
            matrix_new[i][j] = sum;
        }
    }

    return matrix_new;
}

int fixTime(int** matrix, int rows, const char* file_name_res) {
    int start = clock();
    int** resultMatrix = MultiplyMatrix(matrix, rows);
    if (resultMatrix == nullptr) {
        return -1;
    }
    int end = clock();
    writeMatrix(resultMatrix, rows, file_name_res);
    int result = end - start;
    return result;
}

int main()
{
    int COUNT_OP = 10;
    int** matrix = readMatrix("C:\\Users\\79276\\Desktop\\PP-lab2\\matrix_500.csv");
    int rows = 500;
    int** matrix_time = new int* [3];
    for (int i = 0; i < 3; i++) {
        matrix_time[i] = new int[COUNT_OP];
    }
    std::cout << std::endl << "Matrix multiplication with size" << " " << rows << "*" << rows << std::endl;
    for (int i = 0; i < COUNT_OP; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\79276\\Desktop\\PP-lab2\\res_500.csv");
        matrix_time[0][i] = time_res;
    }
    std::cout << "The result is written to a file" << std::endl;
    clearMemory(matrix, rows);

    matrix = readMatrix("C:\\Users\\79276\\Desktop\\PP-lab2\\matrix_1000.csv");
    rows = 1000;
    std::cout << std::endl << "Matrix multiplication with size" << " " << rows << "*" << rows << std::endl;
    for (int i = 0; i < COUNT_OP; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\79276\\Desktop\\PP-lab2\\res_1000.csv");
        matrix_time[1][i] = time_res;
    }
    std::cout << "The result is written to a file" << std::endl;
    clearMemory(matrix, rows);

    matrix = readMatrix("C:\\Users\\79276\\Desktop\\PP-lab2\\matrix_1500.csv");
    rows = 1500;
    std::cout << std::endl << "Matrix multiplication with size" << " " << rows << "*" << rows << std::endl;
    for (int i = 0; i < COUNT_OP; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\79276\\Desktop\\PP-lab2\\res_1500.csv");
        matrix_time[2][i] = time_res;
    }
    std::cout << "The result is written to a file" << std::endl;
    std::cout << "The time result is written to a file" << std::endl;
    writeMatrixTime(matrix_time, 3, COUNT_OP, "C:\\Users\\79276\\Desktop\\PP-lab2\\matrix_time.csv");
    clearMemory(matrix, rows);
    clearMemory(matrix_time, 3);
    return 0;
}