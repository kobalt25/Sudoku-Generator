#include <iostream>

#include <string>

#include <array>

#include <random>

#include <chrono>


using namespace std;

const int EMPTY_VALUE = 0;
const int GRID_SIZE = 9;
array < int, GRID_SIZE > values{
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9
};
int numberOfSolution = 1;

bool isValid(int puzzle[GRID_SIZE][GRID_SIZE], int row, int col, int value) {
    for (int c = 0; c < GRID_SIZE; c++) {
        if (puzzle[row][c] == value)
            return false;
    }
    for (int r = 0; r < GRID_SIZE; r++) {
        if (puzzle[r][col] == value)
            return false;
    }
    int startRow = floor(row / 3) * 3, startCol = floor(col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (puzzle[r][c] == value)
                return false;
        }
    }
    return true;
}

bool hasEmptyCell(int puzzle[GRID_SIZE][GRID_SIZE]) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (puzzle[r][c] == EMPTY_VALUE)
                return true;
        }
    }
    return false;
}

void printGrid(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            /*if (j % 3 == 0 && j != 0) {
                cout << "| ";
            }*/
            cout << grid[i][j] << " ";
        }
        cout << "\n";
        /*if (i % 3 == 2 && i != 8) {
            cout << string(21, '-') + "\n";
        }*/
    }
}

void generateGrid(int(&grid)[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY_VALUE;
        }
    }
}

bool fillGrid(int(&grid)[GRID_SIZE][GRID_SIZE]) {
    int row, col;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        row = floor(i / GRID_SIZE);
        col = i % GRID_SIZE;
        if (grid[row][col] == EMPTY_VALUE) {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));
            for (int j = 0; j < GRID_SIZE; j++) {
                if (isValid(grid, row, col, values[j])) {
                    grid[row][col] = values[j];
                    if (!hasEmptyCell(grid) || fillGrid(grid)) {
                        return true;
                    }
                }
            }
            break;
        }
    }
    grid[row][col] = EMPTY_VALUE;
    return false;
}

bool solveSudoku(int puzzle[GRID_SIZE][GRID_SIZE], bool visualize = false) {
    int row, col;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        row = floor(i / GRID_SIZE);
        col = i % GRID_SIZE;
        if (puzzle[row][col] == EMPTY_VALUE) {
            for (int value = 1; value <= GRID_SIZE; value++) {
                if (isValid(puzzle, row, col, value)) {
                    puzzle[row][col] = value;
                    if (visualize) {
                        printGrid(puzzle);
                    }
                    if (!hasEmptyCell(puzzle)) {
                        numberOfSolution++;
                        if (visualize) {
                            printGrid(puzzle);
                            return true;
                        }
                        break;
                    }
                    else if (solveSudoku(puzzle, visualize)) {
                        return true;
                    }
                }
            }
            break;
        }
    }
    puzzle[row][col] = EMPTY_VALUE;
    if (visualize) {
        printGrid(puzzle);
    }
    return false;
}

void generatePuzzle(int(&grid)[GRID_SIZE][GRID_SIZE], int difficulty = 1) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY_VALUE;
        }
    }
    fillGrid(grid);
    srand((unsigned)time(0));
    int attempt = difficulty;
    while (attempt > 0) {
        int row = floor(rand() % GRID_SIZE);
        int col = floor(rand() % GRID_SIZE);
        while (grid[row][col] == EMPTY_VALUE) {
            row = floor(rand() % GRID_SIZE);
            col = floor(rand() % GRID_SIZE);
        }
        int backupValue = grid[row][col];
        grid[row][col] = EMPTY_VALUE;
        numberOfSolution = 0;
        solveSudoku(grid);
        if (numberOfSolution != 1) {
            grid[row][col] = backupValue;
            attempt--;
        }
    }
}

int main() {
    int grid[GRID_SIZE][GRID_SIZE];
    generateGrid(grid);
    generatePuzzle(grid);
    printGrid(grid);
}