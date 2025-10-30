#include <iostream>                      // Provides std::cout, std::cin, std::endl.
#include <vector>                        // Provides std::vector (dynamic arrays).
#include <fstream>                       // Provides std::ifstream for file input.
#include <iomanip>                       // Provides std::setw for aligned printing.
#include <string>                        // Provides std::string and std::getline.
#include <limits>                        // Provides std::numeric_limits for flushing input.
#include <sstream>                       // Provides std::istringstream for parsing from strings.
#include <climits>                       // Provides INT_MIN and INT_MAX bounds.
using namespace std;                     // Make standard library names shorter (beginner-friendly).

using Matrix = vector<vector<long long>>;    // Alias for a square matrix of long long values.

// Loads N and then two N×N matrices from a text file.                         //
bool loadMatrices(const string& filename, Matrix& A, Matrix& B, int& N) {       // Function to read matrices from file.
    ifstream fin(filename);                                                     // Open the file for reading.
    if (!fin) {                                                                 // Check if the file failed to open.
        cout << "Error opening file.\n";                                        // Inform the user about the issue.
        return false;                                                           // Signal failure.
    }
    if (!(fin >> N) || N <= 0) {                                                // Read N and validate it is positive.
        cout << "Invalid N.\n";                                                 // Inform invalid size.
        return false;                                                           // Signal failure.
    }

    A.assign(N, vector<long long>(N, 0));                                       // Resize A to N×N and fill with zeros.
    B.assign(N, vector<long long>(N, 0));                                       // Resize B to N×N and fill with zeros.

    for (int i = 0; i < N; i++)                                                 // Loop over rows of A.
        for (int j = 0; j < N; j++)                                             // Loop over columns of A.
            if (!(fin >> A[i][j])) {                                            // Try to read next number for A.
                cout << "Not enough numbers for A.\n";                          // If missing, report error.
                return false;                                                   // Signal failure.
            }

    for (int i = 0; i < N; i++)                                                 // Loop over rows of B.
        for (int j = 0; j < N; j++)                                             // Loop over columns of B.
            if (!(fin >> B[i][j])) {                                            // Try to read next number for B.
                cout << "Not enough numbers for B.\n";                          // If missing, report error.
                return false;                                                   // Signal failure.
            }

    return true;                                                                // Successfully loaded both matrices.
}

// Prints a matrix with aligned columns and an optional title.                  //
void printMatrix(const Matrix& M, const string& title) {                         // Function to print matrix M.
    if (!title.empty()) cout << title << "\n";                                  // If a title is given, print it first.
    int N = (int)M.size();                                                      // Get the matrix dimension N.
    for (int i = 0; i < N; i++) {                                               // Iterate over rows.
        for (int j = 0; j < N; j++)                                             // Iterate over columns.
            cout << setw(6) << M[i][j];                                         // Print each value right-aligned width 6.
        cout << "\n";                                                           // End the current row with newline.
    }
}

// Returns the sum A + B into a new matrix C (size N×N).                        //
Matrix add(const Matrix& A, const Matrix& B, int N) {                            // Function to add two matrices.
    Matrix C(N, vector<long long>(N, 0));                                       // Prepare result matrix C filled with 0.
    for (int i = 0; i < N; i++)                                                 // Loop rows.
        for (int j = 0; j < N; j++)                                             // Loop columns.
            C[i][j] = A[i][j] + B[i][j];                                        // Element-wise addition.
    return C;                                                                   // Return the result matrix.
}

// Returns the product C = A * B (size N×N).                                    //
Matrix multiply(const Matrix& A, const Matrix& B, int N) {                       // Function to multiply two matrices.
    Matrix C(N, vector<long long>(N, 0));                                       // Prepare result matrix C filled with 0.
    for (int i = 0; i < N; i++)                                                 // For each row of A.
        for (int k = 0; k < N; k++)                                             // For each column index that pairs A and B.
            for (int j = 0; j < N; j++)                                         // For each column of B.
                C[i][j] += A[i][k] * B[k][j];                                   // Accumulate A[i][k] * B[k][j] into C[i][j].
    return C;                                                                   // Return the product.
}

// Computes the sum of the main diagonal (top-left to bottom-right).            //
long long mainDiagonalSum(const Matrix& M, int N) {                              // Function to sum primary diagonal.
    long long s = 0;                                                            // Accumulator for the sum.
    for (int i = 0; i < N; i++) s += M[i][i];                                   // Add elements where row == col.
    return s;                                                                   // Return the sum.
}

// Computes the sum of the secondary diagonal (top-right to bottom-left).       //
long long secondaryDiagonalSum(const Matrix& M, int N) {                         // Function to sum secondary diagonal.
    long long s = 0;                                                            // Accumulator for the sum.
    for (int i = 0; i < N; i++) s += M[i][N - 1 - i];                           // Add elements where col = N-1-row.
    return s;                                                                   // Return the sum.
}

// Swaps two rows r1 and r2 if both indices are within [0, N).                  //
bool swapRows(Matrix& M, int N, int r1, int r2) {                                // Function to swap two rows.
    if (r1 < 0 || r2 < 0 || r1 >= N || r2 >= N) return false;                   // Validate indices are in range.
    if (r1 == r2) return true;                                                  // No-op if rows are the same.
    swap(M[r1], M[r2]);                                                         // Swap the entire row vectors.
    return true;                                                                // Signal success.
}

// Swaps two columns c1 and c2 if both indices are within [0, N).               //
bool swapCols(Matrix& M, int N, int c1, int c2) {                                // Function to swap two columns.
    if (c1 < 0 || c2 < 0 || c1 >= N || c2 >= N) return false;                   // Validate indices are in range.
    if (c1 == c2) return true;                                                  // No-op if columns are the same.
    for (int i = 0; i < N; i++) swap(M[i][c1], M[i][c2]);                       // Swap column elements in each row.
    return true;                                                                // Signal success.
}

// Updates one cell (r, c) to a new value if indices are valid.                 //
bool updateCell(Matrix& M, int N, int r, int c, long long val) {                 // Function to update a single entry.
    if (r < 0 || c < 0 || r >= N || c >= N) return false;                       // Validate indices are in range.
    M[r][c] = val;                                                              // Assign the new value.
    return true;                                                                // Signal success.
}

// Parses a base-10 integer from a string line into 'out', with bounds checks.  //
static bool parseInt(const string& s, int& out) {                                // Helper to parse int from string.
    istringstream iss(s);                                                        // Create a string stream for parsing.
    long long tmp;                                                               // Use long long to detect overflow.
    if (!(iss >> tmp)) return false;                                             // Fail if no integer was extracted.
    if (tmp < INT_MIN || tmp > INT_MAX) return false;                            // Reject if outside 32-bit int range.
    out = (int)tmp;                                                              // Safe to cast to int.
    return true;                                                                 // Parsing succeeded.
}

// Entry point of the program.                                                  //
int main(int argc, char** argv) {                                               // main function with argc/argv.
    ios::sync_with_stdio(false);                                                // Speed up I/O by unsyncing with C I/O.
    cin.tie(nullptr);                                                           // Disable tie to avoid flushing on input.

    string filename;                                                            // Holds the path to the input file.
    if (argc >= 2) {                                                            // If a filename was provided on command line,
        filename = argv[1];                                                     // use it directly.
    } else {                                                                    // Otherwise, ask the user interactively.
        cout << "Enter input filename: ";                                       // Prompt the user for a filename.
        if (!getline(cin, filename) || filename.empty()) {                      // Read a full line; require non-empty.
            cout << "No filename.\n";                                           // Inform missing filename.
            return 1;                                                           // Exit with error.
        }
    }

    Matrix A, B;                                                                // Matrices A and B to operate on.
    int N = 0;                                                                  // Dimension of the square matrices.
    if (!loadMatrices(filename, A, B, N)) return 1;                             // Load matrices; exit if it fails.

    cin.clear();                                                                // Clear any stream error flags.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');                        // Discard leftover characters on the line.

    cout << "Loaded from '" << filename << "'.\n";                              // Confirm successful load.
    printMatrix(A, "Matrix A:");                                                // Print matrix A.
    printMatrix(B, "Matrix B:");                                                // Print matrix B.

    while (true) {                                                              // Interactive loop for operations.
        cout << "\n1) Add (A+B)\n"                                              // Show menu option 1: addition.
             << "2) Multiply (A*B)\n"                                           // Show menu option 2: multiplication.
             << "3) Diagonal sums (pick A/B)\n"                                 // Option 3: diagonal sums.
             << "4) Swap rows (pick A/B)\n"                                     // Option 4: swap rows.
             << "5) Swap cols (pick A/B)\n"                                     // Option 5: swap columns.
             << "6) Update cell (pick A/B)\n"                                   // Option 6: update one cell.
             << "7) Reload file\n"                                              // Option 7: reload matrices from file.
             << "0) Exit\n"                                                     // Option 0: quit program.
             << "Select: ";                                                     // Prompt for a menu selection.

        string line;                                                            // Buffer to read the whole user line.
        if (!getline(cin, line)) break;                                         // If input ends (EOF), exit the loop.

        size_t p = 0;                                                           // Index to first non-space character.
        while (p < line.size() && isspace((unsigned char)line[p])) ++p;         // Skip leading whitespace safely.

        int op;                                                                 // Parsed menu choice.
        if (!parseInt(line.substr(p), op)) {                                    // Try to parse the choice as integer.
            cout << "Invalid option.\n";                                        // Inform invalid input.
            continue;                                                           // Ask again.
        }

        if (op == 0) {                                                          // If user chose to exit,
            cout << "Bye.\n";                                                   // Say goodbye.
            break;                                                              // Break out of the loop.
        }

        if (op == 1) {                                                          // Menu option 1: A + B.
            Matrix C = add(A, B, N);                                            // Compute the sum matrix.
            printMatrix(C, "A + B:");                                           // Print the result.
        } else if (op == 2) {                                                   // Menu option 2: A * B.
            Matrix C = multiply(A, B, N);                                       // Compute the product matrix.
            printMatrix(C, "A * B:");                                           // Print the result.
        } else if (op == 3) {                                                   // Menu option 3: Diagonal sums.
            cout << "Choose matrix (1=A, 2=B): ";                               // Ask which matrix to use.
            string wline; if (!getline(cin, wline)) break;                      // Read the choice line.
            int w; if (!parseInt(wline, w)) {                                   // Parse it as an integer.
                cout << "Invalid.\n";                                           // Invalid selection message.
                continue;                                                       // Return to menu.
            }
            if (w == 1) {                                                       // If A selected,
                cout << "Main: " << mainDiagonalSum(A, N) << "\n";              // Print main diagonal sum of A.
                cout << "Secondary: " << secondaryDiagonalSum(A, N) << "\n";    // Print secondary diagonal sum of A.
            } else if (w == 2) {                                                // If B selected,
                cout << "Main: " << mainDiagonalSum(B, N) << "\n";              // Print main diagonal sum of B.
                cout << "Secondary: " << secondaryDiagonalSum(B, N) << "\n";    // Print secondary diagonal sum of B.
            } else {                                                            // Otherwise, invalid matrix choice.
                cout << "Invalid.\n";                                           // Inform invalid selection.
            }
        } else if (op == 4) {                                                   // Menu option 4: Swap rows.
            cout << "Choose matrix (1=A, 2=B): ";                               // Ask which matrix to modify.
            string wline; if (!getline(cin, wline)) break;                      // Read the line.
            int w; if (!parseInt(wline, w)) {                                   // Parse integer.
                cout << "Invalid.\n";                                           // Invalid selection.
                continue;                                                       // Return to menu.
            }
            cout << "Enter r1 r2 (0-based): ";                                  // Ask for two row indices.
            string rr; if (!getline(cin, rr)) break;                            // Read the pair line.
            istringstream rs(rr);                                               // Create a parser for the pair.
            int r1, r2;                                                         // Row indices to swap.
            if (!(rs >> r1 >> r2)) {                                            // Try to read both integers.
                cout << "Invalid input.\n";                                     // If failed, report error.
                continue;                                                       // Return to menu.
            }
            bool ok = (w == 1) ? swapRows(A, N, r1, r2)                         // Perform swap on A if chosen.
                               : (w == 2) ? swapRows(B, N, r1, r2)              // Or on B if chosen.
                                          : false;                              // Invalid matrix selection.
            if (!ok) cout << "Out of bounds.\n";                                // Report invalid indices.
            else printMatrix(w == 1 ? A : B, "After row swap:");                // Print updated matrix.
        } else if (op == 5) {                                                   // Menu option 5: Swap columns.
            cout << "Choose matrix (1=A, 2=B): ";                               // Ask which matrix to modify.
            string wline; if (!getline(cin, wline)) break;                      // Read the line.
            int w; if (!parseInt(wline, w)) {                                   // Parse integer.
                cout << "Invalid.\n";                                           // Invalid selection.
                continue;                                                       // Return to menu.
            }
            cout << "Enter c1 c2 (0-based): ";                                  // Ask for two column indices.
            string cc; if (!getline(cin, cc)) break;                            // Read the pair line.
            istringstream cs(cc);                                               // Create a parser for the pair.
            int c1, c2;                                                         // Column indices to swap.
            if (!(cs >> c1 >> c2)) {                                            // Try to read both integers.
                cout << "Invalid input.\n";                                     // If failed, report error.
                continue;                                                       // Return to menu.
            }
            bool ok = (w == 1) ? swapCols(A, N, c1, c2)                         // Perform swap on A if chosen.
                               : (w == 2) ? swapCols(B, N, c1, c2)              // Or on B if chosen.
                                          : false;                              // Invalid matrix selection.
            if (!ok) cout << "Out of bounds.\n";                                // Report invalid indices.
            else printMatrix(w == 1 ? A : B, "After col swap:");                // Print updated matrix.
        } else if (op == 6) {                                                   // Menu option 6: Update a cell.
            cout << "Choose matrix (1=A, 2=B): ";                               // Ask which matrix to modify.
            string wline; if (!getline(cin, wline)) break;                      // Read the line.
            int w; if (!parseInt(wline, w)) {                                   // Parse integer.
                cout << "Invalid.\n";                                           // Invalid selection.
                continue;                                                       // Return to menu.
            }
            cout << "Enter r c value (0-based): ";                              // Ask for row, column, and new value.
            string rc; if (!getline(cin, rc)) break;                            // Read the triple line.
            istringstream rcs(rc);                                              // Create a parser for the triple.
            int r, c; long long v;                                              // Indices and the new value.
            if (!(rcs >> r >> c >> v)) {                                        // Try to read all three items.
                cout << "Invalid input.\n";                                     // Report parsing error.
                continue;                                                       // Return to menu.
            }
            bool ok = (w == 1) ? updateCell(A, N, r, c, v)                      // Update A if chosen.
                               : (w == 2) ? updateCell(B, N, r, c, v)           // Or update B if chosen.
                                          : false;                              // Invalid matrix selection.
            if (!ok) cout << "Out of bounds.\n";                                // Report invalid indices.
            else printMatrix(w == 1 ? A : B, "After update:");                  // Print updated matrix.
        } else if (op == 7) {                                                   // Menu option 7: Reload from file.
            if (loadMatrices(filename, A, B, N)) {                              // Try to reload matrices from file.
                cout << "Reloaded.\n";                                          // Confirm reload.
                printMatrix(A, "Matrix A:");                                    // Print A again.
                printMatrix(B, "Matrix B:");                                    // Print B again.
            } else {                                                            // If reload failed,
                cout << "Reload failed.\n";                                     // report the failure.
            }
        } else {                                                                // Any other numeric option is unknown.
            cout << "Unknown option.\n";                                        // Inform the user.
        }
    }
    return 0;                                                                   // Return success status.
}
