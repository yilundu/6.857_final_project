#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void print_example_banner(string title);
void example_lr();

class Matrix {
   public:
      Matrix add_matrix(Matrix & other) {
         cout << "Adding two matricies together..." << endl;
         std::vector< std::vector<double> > result;
         for (int i = 0; i < n_rows; ++i)
         {
            std::vector<double> row_result;
            for (int j= 0; j < n_cols; ++j){
               double new_value =  values[i][j]+ other.values[i][j];
               row_result.emplace_back(new_value);
            }
            result.emplace_back(row_result);
         }
         return Matrix(result);
      }
        
      Matrix multiply(Matrix & other) {
         cout << "Multiplying 2 matricies together..."  <<endl;
         int rows = n_rows; 
         int cols = other.n_cols;
         std::vector< std::vector<double> > result = initialize_empty(rows, cols);
         for (int i = 0; i < n_rows; ++i)
         {
            for (int j= 0; j < other.n_cols; ++j){
                for (int k=0; k < n_cols; ++k){
                    double product = values[i][k] * other.values[k][j];
                    double new_value = product + result[i][j];
                    
                    result[i][j] = new_value;
                }
            }
         }
         return Matrix(result);
      }

      Matrix multiply_constant(double constant) {
         cout << "Mutliplying a matrix by a constant..." <<endl;
         std::vector< std::vector<double> > result;
         for (int i = 0; i < n_rows; ++i)
         {
            std::vector<double> row_result;
            for (int j= 0; j < n_cols; ++j){
               double new_value = values[i][j]* constant;
               row_result.emplace_back(new_value);
            }
           result.emplace_back(row_result);
         }
         return Matrix(result);
      }

      Matrix get_transpose(){
         cout << "Calculating the transpose of a matrix..." <<endl;
         std::vector< std::vector<double> > result;
         for (int i = 0; i < n_cols; ++i)
         {
            std::vector<double> row_result;
            for (int j= 0; j < n_rows; ++j){
               double new_value = values[j][i];
               row_result.emplace_back(new_value);
            }
           result.emplace_back(row_result);
         }
         return Matrix(result);
      }

      Matrix get_adjugate(){
        cout << "Calculating the adjugate of a matrix..." <<endl;
        if (n_cols == 2 && n_rows == 2){
            std::vector< std::vector<double> > result = initialize_empty(2, 2);
            //for 2x2 matrix [[a,b],[c,d]] adjugate is [[d, -b],[-c, a]]
            result[0][0] = values[1][1];
            result[0][1] = -1* values[0][1];
            result[1][0] = -1* values[1][0];
            result[1][1] = values[0][0];
            return Matrix(result);
        }
        else{
            cout <<"Cannot determine adjugate matrix, outputting copy of input matrix..." <<endl;
            return Matrix(values);
        }

      }

      void print_decrypted(){
         cout << "Printing matrix..." <<endl;
         for (int i = 0; i < n_rows; ++i)
         {
            for (int j= 0; j < n_cols; ++j){
               cout << values[i][j];
               cout << "\t";
            }
            cout << endl;
         }
      }
 
    double get_determinant(){
        if (n_cols == 2 && n_rows == 2){
            cout << "calculating determinat of a 2x2 matrix" <<endl;
            double ac = values[0][0] * values[1][1];
            double bd = values[0][1] * values[1][0];
            double det = ac- bd;
            return det;
        }
        else{
            cout << "cannot properly output determinat... outputting m[0][0]..." <<endl;
            return values[0][0];
        }
    }

      Matrix (std::vector< std::vector<double> > data);
      std::vector< std::vector<double> > values;
      int n_rows;
      int n_cols;
    
        
   private:
        vector< vector<double>> initialize_empty(int rows, int cols){
            double ZERO = 0;
            vector< vector<double> > result;
            for (int i=0; i < rows; ++i){
                std::vector<double> row_result;
                for (int j=0; j < cols; ++j){
                    row_result.emplace_back(ZERO);
                }
                result.emplace_back(row_result);
            }
            return result;
      }
};

Matrix::Matrix (std::vector< std::vector<double> > data) {
  values = data;
  n_rows = data.size();
  n_cols = data[0].size();

}




int main()
{
    // Example: Basics

    example_lr();

    // Wait for ENTER before closing screen.
    cout << "Press ENTER to exit" << endl;
    char ignore;
    cin.get(ignore);

    return 0;
}

Matrix read_data_file(string & file_name, bool is_vector){
    cout << "About to get file data " << file_name << "..." <<endl;
    string line;
    ifstream myfile (file_name);
    vector< vector<double>> matrix_data; 
    if (myfile.is_open())
    {
        while ( getline (myfile,line) ){
            int first_tab = line.find("\t");
            int end = line.find("\n");
            if (end == -1){
                end = line.length();
            }
            vector<double> row_data;
            //int last_tab = line.find("\t", first_tab+2);

            if (is_vector){
                double i_0= atof(line.c_str()); 
                row_data.emplace_back(i_0);
                matrix_data.emplace_back(row_data);
            }
            else{
                string i_0_str = line.substr(0,first_tab);
                double i_0= atof(i_0_str.c_str());
                string i_1_str = line.substr(first_tab,end);
                double i_1 = atof(i_1_str.c_str());
                row_data.emplace_back(i_0);
                row_data.emplace_back(i_1);
                matrix_data.emplace_back(row_data);
            }
            //c = atoi(b.c_str());
            //cout << line << '\n';
        }
        myfile.close();
    }
    else cout << "Unable to open file" << file_name << endl;
    Matrix mat = Matrix(matrix_data);
    mat.print_decrypted();
    return mat; 
}

void example_lr()
{
    print_example_banner("Linear Regression");

    string file_name_data  = "../sealcryto/data.txt";
    string file_name_labels = "../sealcrypto/labels.txt";
    Matrix X = read_data_file(file_name_data, false);
    Matrix y = read_data_file(file_name_labels, true);
    cout << "Encrypted data matrix and label vector have been created." << endl;
    cout << "Preparing to do linear regression..." << endl <<endl <<endl;
    cout << "Computing (X^T * X)^(-1) * X^(T)y..." << endl;
    //Model= (X^T * X)^(-1) * X^(T)y


    //Part 1 X^(T)y
    cout << "Computing X^(T)y" <<endl;
    Matrix X_T = X.get_transpose();
    Matrix X_T_y = X_T.multiply(y);
    cout << "Done..." << endl <<endl <<endl;

    cout << "Computing (X^T * X)^(-1)" <<endl;
    cout << "(X^T * X)^(-1) = (1/(det(X^T*X)) * Adj(X^T *X)" << endl;
    //Fomula X^T * X)^(-1) = (1/(det(X^T*X)) * Adj(X^T *X)

    cout << "Computing X^T*X" <<endl;
    Matrix X_T_X = X_T.multiply(X);

    cout << "Computing Adj(X^T *X)" <<endl;
    Matrix adj_xtx = X_T_X.get_adjugate();

    cout << "Computing det(X^T*x)"<<endl;
    double det = X_T_X.get_determinant();

    cout << "Done..." <<endl <<endl <<endl;

    cout << "Last step computing Adj(X^T *X) * X^(T)y" << endl;

    Matrix final = adj_xtx.multiply(X_T_y);

    cout << "Done with linear Regression!" << endl <<endl <<endl;

    cout << "Matrix: " << endl;
    final.print_decrypted();
    cout << endl <<endl;

    cout << "1/determinant multiple: " << endl;
    cout << det << endl;
}

void print_example_banner(string title)
{
    if (!title.empty())
    {
        size_t title_length = title.length();
        size_t banner_length = title_length + 2 + 2 * 10;
        string banner_top(banner_length, '*');
        string banner_middle = string(10, '*') + " " + title + " " + string(10, '*');

        cout << endl
            << banner_top << endl
            << banner_middle << endl
            << banner_top << endl
            << endl;
    }
}