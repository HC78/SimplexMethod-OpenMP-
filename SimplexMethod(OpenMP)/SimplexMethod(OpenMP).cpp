#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <chrono> // For measuring execution time
#include <random> // For generating random data
#include <omp.h>
#include <fstream> // For file output
#include <cstdlib> 
#include <string> 
//cplex
// read "C:\Users\SEOW HUI CHEE\source\repos\dspcassignment\dspcassignment\problem.mps"
//optimize
//display solution variables -

using namespace std;
using namespace std::chrono;

void writeMPSFile(const string& filename, const vector<vector<double>>& A, const vector<double>& B, const vector<double>& C) {
	ofstream mpsFile(filename);
	if (!mpsFile.is_open()) {
		cerr << "Failed to open MPS file for writing." << endl;
		return;
	}

	int rows = A.size();
	int cols = A[0].size();

	// Write the header
	mpsFile << "NAME          LP_Problem" << endl;
	mpsFile << "OBJSENSE" << endl;
	mpsFile << "    MAX" << endl;
	mpsFile << "ROWS" << endl;
	mpsFile << " N  OBJ" << endl;
	for (int i = 0; i < rows; ++i) {
		mpsFile << " L  C" << i + 1 << endl;
	}

	// Write the columns
	mpsFile << "COLUMNS" << endl;
	for (int j = 0; j < cols; ++j) {
		for (int i = 0; i < rows; ++i) {
			if (A[i][j] != 0) {
				mpsFile << "    X" << j + 1 << "  C" << i + 1 << "  " << A[i][j] << endl;
			}
		}
		mpsFile << "    X" << j + 1 << "  OBJ  " << C[j] << endl;
	}

	// Write the RHS
	mpsFile << "RHS" << endl;
	for (int i = 0; i < rows; ++i) {
		mpsFile << "    RHS1  C" << i + 1 << "  " << B[i] << endl;
	}

	mpsFile << "ENDATA" << endl;
	mpsFile.close();
}

vector<vector<double>> generateLargeMatrix(int rows, int cols) {
	vector<vector<double>> matrix(rows, vector<double>(cols, 0.0));
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0.0, 50.0);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			matrix[i][j] = dis(gen);
		}
	}

	return matrix;
}

vector<double> generateLargeVector(int size) {
	vector<double> vec(size, 0.0);
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0.0, 50.0);

	for (int i = 0; i < size; ++i) {
		vec[i] = dis(gen);
	}

	return vec;
}

int selectPivotRow(int pivotColumn, int rows, vector<vector<double>>& tableau) {
	int pivotRow = -1, i;
	double minRatio = numeric_limits<double>::max();

#pragma omp parallel
	{
		int localPivotRow = -1;
		double localMinRatio = numeric_limits<double>::max();
#pragma omp for
		for (i = 0; i < rows; ++i) {
			if (tableau[i][pivotColumn] > 0) {
				double ratio = tableau[i].back() / tableau[i][pivotColumn];
				if (ratio < localMinRatio) {
					localMinRatio = ratio;
					localPivotRow = i;
				}
			}
		}
#pragma omp critical
		{
			if (localMinRatio < minRatio) {
				minRatio = localMinRatio;
				pivotRow = localPivotRow;
			}
		}
	}
	return pivotRow;
}

void pivot(int pivotRow, int pivotColumn, vector<vector<double>>& tableau, vector<int>& basicVars) {
	double pivotElement = tableau[pivotRow][pivotColumn];
	int i, j;

#pragma omp parallel for
	for (j = 0; j < tableau[0].size(); ++j) {
		tableau[pivotRow][j] /= pivotElement;
	}

#pragma omp parallel for
	for (i = 0; i < tableau.size(); ++i) {
		if (i != pivotRow) {
			double ratio = tableau[i][pivotColumn];
			for (int j = 0; j < tableau[0].size(); ++j) {
				tableau[i][j] -= ratio * tableau[pivotRow][j];
			}
		}
	}

	basicVars[pivotRow] = pivotColumn; // Update basic variable index
}

void printResults(int rows, int cols, vector<vector<double>>& tableau, vector<int>& basicVars) {
	cout << "Optimal Solution:" << endl;
	vector<double> variableValues(cols, 0.0);
	for (int i = 0; i < rows; ++i) {
		if (basicVars[i] < cols) {
			variableValues[basicVars[i]] = tableau[i].back();
		}
	}

	for (int i = 0; i < cols; ++i) {
		//used for verify small size data variable result
		if (variableValues[i] != 0) { //temporary put to no print if=0
			cout << "x" << i + 1 << " = " << fixed << setprecision(16) << variableValues[i] << endl;
		}
	}
	cout << "Maximum Z = " << fixed << setprecision(16) << tableau[rows].back() << endl;
}

int selectPivotColumn(int rows, int cols, vector<vector<double>>& tableau) {
	int pivotColumn = -1, j;
	double minValue = 0.0;

#pragma omp parallel
	{
		int localPivotColumn = -1;
		double localMinValue = 0.0;
#pragma omp for
		for (j = 0; j < cols; ++j) {
			if (tableau[rows][j] < localMinValue) {
				localMinValue = tableau[rows][j];
				localPivotColumn = j;
			}
		}

#pragma omp critical
		{
			if (localMinValue < minValue) {
				minValue = localMinValue;
				pivotColumn = localPivotColumn;
			}
		}
	}

	return pivotColumn;
}

long Simplex2(const vector<vector<double>>& A, const vector<double>& B, const vector<double>& C, int threadNum) {
	int rows = A.size();
	int cols = A[0].size();
	vector<vector<double>> tableau;
	vector<int> basicVars;
	tableau.resize(rows + 1, vector<double>(cols + rows + 1, 0.0));
	basicVars.resize(rows);

	omp_set_num_threads(threadNum);

	// Set up the tableau
#pragma omp parallel for
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			tableau[i][j] = A[i][j];
		}
		tableau[i][cols + i] = 1; // Set the identity matrix for slack variables
		tableau[i].back() = B[i];
		basicVars[i] = cols + i; // Initialize basic variables (slack variables)
	}

#pragma omp parallel for
	for (int j = 0; j < cols; ++j) {
		tableau[rows][j] = -C[j];
	}
	tableau[rows].back() = 0; // Initialize the objective function's value

	auto start = high_resolution_clock::now();

	while (true) {
		int pivotColumn = selectPivotColumn(rows, cols, tableau);
		if (pivotColumn == -1) break; // Optimal solution found

		int pivotRow = selectPivotRow(pivotColumn, rows, tableau);
		if (pivotRow == -1) {
			cout << "The problem is unbounded." << endl;
			return 0.0;
		}

		pivot(pivotRow, pivotColumn, tableau, basicVars);
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	printResults(rows, cols, tableau, basicVars);
	cout << "Execution Time openMp: " << duration.count() << " microseconds" << endl;
	return duration.count();
}


//SERIAL BELOW
int selectPivotRowS(int pivotColumn, int rows, vector<vector<double>>& tableau) {
	int pivotRow = -1;
	double minRatio = numeric_limits<double>::max();


	int localPivotRow = -1;
	double localMinRatio = numeric_limits<double>::max();

	for (int i = 0; i < rows; ++i) {
		if (tableau[i][pivotColumn] > 0) {
			double ratio = tableau[i].back() / tableau[i][pivotColumn];
			if (ratio < localMinRatio) {
				localMinRatio = ratio;
				localPivotRow = i;
			}
		}
	}
	if (localMinRatio < minRatio) {
		minRatio = localMinRatio;
		pivotRow = localPivotRow;
	}


	return pivotRow;
}

void pivotS(int pivotRow, int pivotColumn, vector<vector<double>>& tableau, vector<int>& basicVars) {
	double pivotElement = tableau[pivotRow][pivotColumn];

	for (int j = 0; j < tableau[0].size(); ++j) {
		tableau[pivotRow][j] /= pivotElement;
	}

	for (int i = 0; i < tableau.size(); ++i) {
		if (i != pivotRow) {
			double ratio = tableau[i][pivotColumn];
			for (int j = 0; j < tableau[0].size(); ++j) {
				tableau[i][j] -= ratio * tableau[pivotRow][j];
			}
		}
	}

	basicVars[pivotRow] = pivotColumn; // Update basic variable index
}

void printResultsS(int rows, int cols, vector<vector<double>>& tableau, vector<int>& basicVars) {
	cout << "Optimal Solution:" << endl;
	vector<double> variableValues(cols, 0.0);
	for (int i = 0; i < rows; ++i) {
		if (basicVars[i] < cols) {
			variableValues[basicVars[i]] = tableau[i].back();
		}
	}

	for (int i = 0; i < cols; ++i) {
		//used for verify small size data variable result
		if (variableValues[i] != 0) { //temporary put to no print if=0
			cout << "x" << i + 1 << " = " << fixed << setprecision(16) << variableValues[i] << endl;
		}
	}
	cout << "Maximum Z = " << fixed << setprecision(16) << tableau[rows].back() << endl;
}

int selectPivotColumnS(int rows, int cols, vector<vector<double>>& tableau) {
	int pivotColumn = -1;
	double minValue = 0.0;

	int localPivotColumn = -1;
	double localMinValue = 0.0;

	for (int j = 0; j < cols; ++j) {
		if (tableau[rows][j] < localMinValue) {
			localMinValue = tableau[rows][j];
			localPivotColumn = j;
		}
	}

	if (localMinValue < minValue) {
		minValue = localMinValue;
		pivotColumn = localPivotColumn;
	}

	return pivotColumn;
}

long Simplex(const vector<vector<double>>& A, const vector<double>& B, const vector<double>& C) {
	int rows = A.size();
	int cols = A[0].size();
	vector<vector<double>> tableau;
	vector<int> basicVars;
	tableau.resize(rows + 1, vector<double>(cols + rows + 1, 0.0));
	basicVars.resize(rows);

	// Set up the tableau
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			tableau[i][j] = A[i][j];
		}
		tableau[i][cols + i] = 1; // Set the identity matrix for slack variables
		tableau[i].back() = B[i];
		basicVars[i] = cols + i; // Initialize basic variables (slack variables)
	}

	for (int j = 0; j < cols; ++j) {
		tableau[rows][j] = -C[j];
	}
	tableau[rows].back() = 0; // Initialize the objective function's value

	auto start = high_resolution_clock::now(); // Start measuring time

	while (true) {
		int pivotColumn = selectPivotColumnS(rows, cols, tableau);
		if (pivotColumn == -1) break; // Optimal solution found

		int pivotRow = selectPivotRowS(pivotColumn, rows, tableau);
		if (pivotRow == -1) {
			cout << "The problem is unbounded." << endl;
			return 0.0;
		}

		pivotS(pivotRow, pivotColumn, tableau, basicVars);
	}

	auto stop = high_resolution_clock::now(); // Stop measuring time
	auto duration = duration_cast<microseconds>(stop - start);

	printResultsS(rows, cols, tableau, basicVars);
	cout << "Execution Time for Serial: " << duration.count() << " microseconds" << endl;
	return duration.count();
}


//ori
int main() {
	int numConstraints, numVariables;

	while (true) {
		cout << "Enter the number of constraints: ";
		if (cin >> numConstraints) {
			break;
		}
		else {
			cout << "Invalid input. Please enter an integer." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	while (true) {
		cout << "Enter the number of variables: ";
		if (cin >> numVariables) {
			break;
		}
		else {
			cout << "Invalid input. Please enter an integer." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
		}
	}

	// Generate random data for a large problem
	vector<vector<double>> A = generateLargeMatrix(numConstraints, numVariables);
	vector<double> B = generateLargeVector(numConstraints);
	vector<double> C = generateLargeVector(numVariables);

	// Write to MPS file for external solver
	writeMPSFile("problem.mps", A, B, C);

	ofstream outFile("performance_gains.txt");

	cout << "Serial (" << numConstraints << "*" << numVariables << ")->" << endl;
	long ori = Simplex(A, B, C);

	//cout << "2 threads(" << numConstraints << "*" << numVariables << ")->" << endl;
	//long time1 = Simplex2(A, B, C, 2);
	//double gain1 = static_cast<double>(ori) / time1;
	//outFile << "2 threads: " << gain1 << endl;

	//cout << "4 threads(" << numConstraints << "*" << numVariables << ")->" << endl;
	//long time2 = Simplex2(A, B, C, 4);
	//double gain2 = static_cast<double>(ori) / time2;
	//outFile << "4 threads: " << gain2 << endl;

	//cout << "8 threads(" << numConstraints << "*" << numVariables << ")->" << endl;
	//long time3 = Simplex2(A, B, C, 8);
	//double gain3 = static_cast<double>(ori) / time3;
	//outFile << "8 threads: " << gain3 << endl;

	cout << "16 threads(" << numConstraints << "*" << numVariables << ")->" << endl;
	long time4 = Simplex2(A, B, C, 16);
	double gain4 = static_cast<double>(ori) / time4;
	outFile << "16 threads: " << gain4 << endl;

	/*cout << "32 threads(" << numConstraints << "*" << numVariables << ")->" << endl;
	long time5 = Simplex2(A, B, C, 32);
	double gain5 = static_cast<double>(ori) / time5;
	outFile << "32 threads: " << gain5 << endl;*/

	outFile.close();

	//cout << "Performance Gain (2 threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << static_cast<double>(ori) / time1 << endl;
	//cout << "Performance Gain (4 threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << static_cast<double>(ori) / time2 << endl;
	//cout << "Performance Gain (8 threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << static_cast<double>(ori) / time3 << endl;
	cout << "Performance Gain (16 threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << static_cast<double>(ori) / time4 << endl;
	//cout << "Performance Gain (32 threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << static_cast<double>(ori) / time5 << endl;

	// Write to MPS file for external solver
	//writeMPSFile("problem.mps", A, B, C);


	return 0;
}


//test small
//int main() {
//	/*
//	// To test use small size datasets and then check the results with known output results, which you can check with any online sources (https://cbom.atozmath.com/CBOM/Simplex.aspx?q=sm&q1=3%603%60MAX%60Z%60x1%2cx2%2cx3%606%2c5%2c4%602%2c1%2c1%3b1%2c3%2c2%3b2%2c1%2c2%60%3c%3d%2c%3c%3d%2c%3c%3d%60180%2c300%2c240%60%60D%60false%60true%60false%60true%60false%60false%60true%601&dp=4&do=1#PrevPart)
//	vector<vector<double>> A = {
//		 {2, 1, 1},
//		 {1, 3, 2},
//		 {2, 1, 2}
//	};
//	vector<double> B = { 180, 300, 240 };
//	vector<double> C = { 6, 5, 4 };
//
//	//for serial
//	long ori = Simplex(A, B, C);
//
//	//for openmp
//	long time2 = Simplex2(A, B, C, 16);
//    cout << "Performance Gain (serial VS parallel) for 16 threads: " << fixed << setprecision(16) << static_cast<double>(ori) / time2 << endl;
//	
//	*/
//   // Generate random data for a large problem
//   vector<vector<double>> A = generateLargeMatrix(numConstraints, numVariables);
//   vector<double> B = generateLargeVector(numConstraints);
//   vector<double> C = generateLargeVector(numVariables);
//
//   ofstream outFile("performance_gains.txt");
//
//   cout << "Serial (10000*10000)->" << endl;
//   long ori = Simplex(A, B, C);
//
//   cout << "2 threads(10000*10000)->" << endl;
//   long time1 = Simplex2(A, B, C, 2);
//   double gain1 = static_cast<double>(ori) / time1;
//   outFile << "2 threads: " << gain1 << endl;
//
//   cout << "4 threads(10000*10000)->" << endl;
//   long time2 = Simplex2(A, B, C, 4);
//   double gain2 = static_cast<double>(ori) / time2;
//   outFile << "4 threads: " << gain2 << endl;
//
//   cout << "8 threads(10000*10000)->" << endl;
//   long time3 = Simplex2(A, B, C, 8);
//   double gain3 = static_cast<double>(ori) / time3;
//   outFile << "8 threads: " << gain3 << endl;
//
//   cout << "16 threads(10000*10000)->" << endl;
//   long time4 = Simplex2(A, B, C, 16);
//   double gain4 = static_cast<double>(ori) / time4;
//   outFile << "16 threads: " << gain4 << endl;
//
//   cout << "32 threads(10000*10000)->" << endl;
//   long time5 = Simplex2(A, B, C, 32);
//   double gain5 = static_cast<double>(ori) / time5;
//   outFile << "32 threads: " << gain5 << endl;
//
//   outFile.close();
//
//   cout << "Performance Gain (2 threads vs serial for 10000x10000): " << fixed << setprecision(16) << static_cast<double>(ori) / time1 << endl;
//   cout << "Performance Gain (4 threads vs serial for 10000x10000): " << fixed << setprecision(16) << static_cast<double>(ori) / time2 << endl;
//   cout << "Performance Gain (8 threads vs serial for 10000x10000): " << fixed << setprecision(16) << static_cast<double>(ori) / time3 << endl;
//   cout << "Performance Gain (16 threads vs serial for 10000x10000): " << fixed << setprecision(16) << static_cast<double>(ori) / time4 << endl;
//   cout << "Performance Gain (32 threads vs serial for 10000x10000): " << fixed << setprecision(16) << static_cast<double>(ori) / time5 << endl;
//   
//	return 0;
//}

//last graph
//int main() {
//	vector<int> problemSizes = { 2000, 4000, 6000, 8000, 10000, 20000, 40000 };
//	vector<int> threadCounts = { 2, 4, 8, 16, 32 };
//	ofstream outFile("performance_gains.txt");
//
//	for (int size : problemSizes) {
//		int numConstraints = size;
//		int numVariables = size;
//
//		// Generate random data for a large problem
//		vector<vector<double>> A = generateLargeMatrix(numConstraints, numVariables);
//		vector<double> B = generateLargeVector(numConstraints);
//		vector<double> C = generateLargeVector(numVariables);
//
//		cout << "Serial (" << numConstraints << "*" << numVariables << ")->" << endl;
//		long ori = Simplex(A, B, C);
//
//		for (int threads : threadCounts) {
//			cout << threads << " threads(" << numConstraints << "*" << numVariables << ")->" << endl;
//			long time = Simplex2(A, B, C, threads);
//			double gain = static_cast<double>(ori) / time;
//			outFile << size << " " << threads << " " << gain << endl;
//
//			cout << "Performance Gain (" << threads << " threads vs serial for " << numConstraints << "x" << numVariables << "): " << fixed << setprecision(16) << gain << endl;
//		}
//	}
//
//	outFile.close();
//
//	return 0;
//}









//#include <iostream>
//#include <vector>
//#include <iomanip>
//#include <algorithm>
//#include <chrono> // for timing
//#include <omp.h> // for OpenMP
//
//using namespace std;
//
//int openMP(bool useParallel) {
//    cout << "\n                 ****SiMplex Algorithm ****\n\n";
//    if (useParallel) {
//        omp_set_num_threads(16); // Use 16 threads
//    }
//    else {
//        omp_set_num_threads(1); // Use only 1 thread (master thread)
//    }
//
//    // Inputs
//    int num_vars = 100000000; // Simulate 100 million constraint variables
//    vector<double> b = { 8, 10,28 };  // b contains the amount of resources
//    vector<double> c(num_vars, 1); // c contains coefficients of the objective function Z (all 1 for simplicity)
//
//    // Initialize
//    vector<double> cb = { c[num_vars - 1], c[num_vars - 2] }; // coefficients of basic variables in Z
//    vector<double> xb = { b[0], b[1] }; // xb is the value of the basic variables
//    vector<int> B = { num_vars - 1, num_vars - 2 }; // Basic variables (identity matrix)
//
//    // Start timing the process
//    auto start = chrono::high_resolution_clock::now();
//
//    // Initial simplex table (just using minimal example for testing purposes)
//    vector<vector<double>> table = {
//        {double(B[0]), cb[0], xb[0]}, // Add just a few constraint variables for testing
//        {double(B[1]), cb[1], xb[1]}
//    };
//
//    // if min problem, set this to 1
//    bool MIN = false;
//
//    bool reached = false; // when optimality reached
//    bool unbounded = false;
//    int itr = 1;
//
//    // Simplex iterations (simplified without showing the whole matrix)
//    while (!reached) {
//        vector<double> rel_prof(num_vars, 0);
//
//        // Parallelize the calculation of relative profits
//#pragma omp parallel for
//        for (int i = 0; i < num_vars; i++) {
//            rel_prof[i] = c[i] - (table[0][1] * table[0][1] + table[1][1] * table[1][1]); // Simplified
//        }
//
//        // Check optimality
//        double max_rel_prof = *max_element(rel_prof.begin(), rel_prof.end());
//        if (max_rel_prof <= 0) {
//            reached = true;
//            break;
//        }
//
//        // Simulate minimal processing to prevent unbounded case
//        itr++;
//        if (itr > 100) { // arbitrary stopping condition for large problem
//            break;
//        }
//    }
//
//    // End timing
//    auto end = chrono::high_resolution_clock::now();
//    chrono::duration<double> w_time = end - start;
//
//    // Print optimal value and wall time
//    double z = 0;
//    for (int i = 0; i < B.size(); i++) {
//        z += c[B[i]] * table[i][2];
//    }
//
//    if (MIN) {
//        cout << "Optimal value of Z: " << -z << "\n";
//    }
//    else {
//        cout << "Optimal value of Z: " << z << "\n";
//    }
//
//    cout << "Time taken for computation: " << w_time.count() << " seconds\n";
//
//    return w_time.count();
//}
//
//int serial() {
//    cout << "\n                 ****SiMplex Algorithm ****\n\n";
//
//    // Inputs
//    int num_vars = 100000000; // Simulate 100 million constraint variables
//    vector<double> b = { 8, 10 };  // b contains the amount of resources
//    vector<double> c(num_vars, 1); // c contains coefficients of the objective function Z (all 1 for simplicity)
//
//    // Initialize
//    vector<double> cb = { c[num_vars - 1], c[num_vars - 2] }; // coefficients of basic variables in Z
//    vector<double> xb = { b[0], b[1] }; // xb is the value of the basic variables
//    vector<int> B = { num_vars - 1, num_vars - 2 }; // Basic variables (identity matrix)
//
//    // Start timing the process
//    auto start = chrono::high_resolution_clock::now();
//
//    // Initial simplex table (just using minimal example for testing purposes)
//    vector<vector<double>> table = {
//        {double(B[0]), cb[0], xb[0]}, // Add just a few constraint variables for testing
//        {double(B[1]), cb[1], xb[1]}
//    };
//
//    // if min problem, set this to 1
//    bool MIN = false;
//
//    bool reached = false; // when optimality reached
//    bool unbounded = false;
//    int itr = 1;
//
//    // Simplex iterations (simplified without showing the whole matrix)
//    while (!reached) {
//        vector<double> rel_prof(num_vars, 0);
//
//        // Simulate relative profit calculation without explicitly storing a huge matrix
//        for (int i = 0; i < num_vars; i++) {
//            rel_prof[i] = c[i] - (table[0][1] * table[0][1] + table[1][1] * table[1][1]); // Simplified
//        }
//
//        // Check optimality
//        if (*max_element(rel_prof.begin(), rel_prof.end()) <= 0) {
//            reached = true;
//            break;
//        }
//
//        // Simulate minimal processing to prevent unbounded case
//        itr++;
//        if (itr > 100) { // arbitrary stopping condition for large problem
//            break;
//        }
//    }
//
//    // End timing
//    auto end = chrono::high_resolution_clock::now();
//    chrono::duration<double> w_time = end - start;
//
//    // Print optimal value and wall time
//    double z = 0;
//    for (int i = 0; i < B.size(); i++) {
//        z += c[B[i]] * table[i][2];
//    } 
//
//    if (MIN) {
//        cout << "Optimal value of Z: " << -z << "\n";
//    }
//    else {
//        cout << "Optimal value of Z: " << z << "\n";
//    }
//
//    cout << "Time taken for original computation: " << w_time.count() << " seconds\n";
//
//    return w_time.count();
//}
//
//int main() {
//    double parallelTime = openMP(true); // Parallel version
//    double serialTime = serial(); // Serial version
//
//    cout << "Performance gain: " << serialTime / parallelTime << endl;
//
//    return 0;
//}
