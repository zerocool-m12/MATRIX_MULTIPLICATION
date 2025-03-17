#include<iostream>
#include<chrono>
#include<random>
class matrix{
    int**arr;
    std::size_t rows;
    std::size_t cols;
    void deleter(){
        for(std::size_t i = 0; i < rows; i++)
            delete [] arr[i];
        delete [] arr;
    }
    void deleter(matrix & other){
        for(std::size_t i = 0; i < other.rows; i++)
          delete [] other.arr [i];
        delete [] other.arr;
    }
    public:
    matrix() : arr(nullptr), rows(1), cols(1){}
    matrix (const matrix & other) : rows(other.rows), cols(other.cols){
      try{
        arr = new int*[rows];
        for(std::size_t i = 0; i < rows; i++)
          arr[i] = new int[cols];
      }catch (const std::bad_alloc & e){
        deleter();
        throw;
      }
      for(std::size_t i = 0; i < rows; i++)
        std::copy (other.arr[i], other.arr[i] + cols, arr[i] );
    }
    matrix & operator = (const matrix & other){
        if(this != &other){        // this technique called the copy swap idiom, it is conventional and well-known in copy assignment operations
            matrix temp (other);
            std::swap(rows,temp.rows);
            std::swap(cols,temp.cols);
            std::swap(arr,temp.arr);
        }
        return *this;
    }
    matrix (matrix && other) noexcept : rows (other.rows), cols (other.cols), arr(other.arr){
        other.rows = 0;
        other.cols = 0;
        other.arr = nullptr;
    }
    matrix & operator = (matrix && other) noexcept {
        if(this != &other){
            delete [] arr;
            arr = other.arr;
            rows = other.rows;
            cols = other.cols;
            other.rows = 0;
            other.cols = 0;
            other.arr = nullptr;
        }
        return *this;
    }
    matrix(const std::size_t & r, const std::size_t & c) : rows(r), cols(c) {
        try{
            if(rows < 1 or cols < 1) throw std::runtime_error("Invalid size of matrix!\n");
        }catch (const std::runtime_error & e){
            std::cerr<<e.what()<<std::endl;
            this->arr = nullptr;
            exit(EXIT_FAILURE);
        }
        this->arr = new int*[rows];
        for(std::size_t i = 0; i < rows; i++){
            this->arr[i] = new int[cols]{};
        }
    }
    matrix operator * (matrix & other) const{
        try{
            if(this->cols not_eq other.rows) {
                throw std::logic_error("The size of columns of the first matrix does not match the size of the rows of the second matrix!\n");
            }
        }catch(const std::logic_error & e){
            std::cerr<<e.what()<<std::endl;
            exit(EXIT_FAILURE);
        }
        matrix res_mat(this->rows,other.cols);
        for(std::size_t i = 0; i < this->rows; i++)
          for(std::size_t j = 0; j < other.cols; j++)
            for(std::size_t k = 0; k < this->cols; k++)
              res_mat.arr[i][j] += this->arr[i][k] * other.arr[k][j];
        return res_mat;  
              
    }
    const std::size_t row_size() const {return rows;}
    const std::size_t col_size() const {return cols;}
    int*operator [](const std::size_t & x){
        try{
            if(x >= rows or x <0)
             throw std::out_of_range("Out of bound!\n");
        }catch(const std::out_of_range & e){
            std::cerr<<e.what()<<std::endl;
            deleter();
        }
        return this->arr[x];
    }
    const int*operator [](const std::size_t & x) const{
        if(x >= rows or x <0)
          throw std::out_of_range("Out of bound!\n");
        return this->arr[x];
    }
    void print(){
        if(this->rows == 0 or this->cols == 0){
            std::cerr<<"Invalid matrix!"<<std::endl;
            return;
        }
        for(std::size_t i = 0; i < rows; i++){
            for(std::size_t j = 0; j < cols; j++)
              std::cout<<this->arr[i][j]<<'\t';
            std::cout.put('\n');
        }
    }
    ~matrix(){
        deleter();
        rows = 0;
        cols = 0;
        std::clog<<"This matrix has been deleted!"<<std::endl;
    }
};
int main(){
    std::size_t arr_row,arr_col,arr2_row, arr2_col;
    std::cout<<"Please enter the size of ROWS in the first matrix:-"<<std::endl;
    while(true){
        std::cin>>arr_row;
        if(std::cin.fail() or arr_row < 1){
            std::cerr<<"Invalid size! Please enter the size of ROWS of the first matrix"<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }else break;
    }
    std::cout<<"Please enter the size of COLUMNS of the first matrix:-"<<std::endl;
    while(true){
        std::cin>>arr_col;
        if(std::cin.fail() or arr_col < 1){
            std::cerr<<"Invalid size! Please enter the size of COLUMNS of the first matrix"<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }else break;
    }
    std::cout<<"Please enter the size of ROWS of the second matrix:-"<<std::endl;
    while(true){
        std::cin>>arr2_row;
        if(std::cin.fail() or arr2_row < 1 or (arr2_row not_eq arr_col)){
            std::cerr<<"Invalid size or the size of ROWS in the second matrix does not match the size of COLUMNS in the first matrix!"<<std::endl<<"Please enter the size of the ROWS of the second matrix:-"<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }else break;
    }
    std::cout<<"Please enter the size of COLUMNS of the second matrix:-"<<std::endl;
    while(true){
        std::cin>>arr2_col;
        if(std::cin.fail() or arr2_col < 1 ){
            std::cerr<<"Invalid size! Please enter the size of the COLUMNS of the second matrix:-"<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }else break;
    }
    auto separator = []{std::cout<<std::string(50,'*')<<std::endl;};    
    std::mt19937 rd(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>dist(1,199);
    matrix mat1 (arr_row,arr_col), mat2 (arr2_row,arr2_col);
    for(std::size_t i = 0; i < mat1.row_size(); i++)
      for(std::size_t j = 0; j < mat1.col_size(); j++)
        mat1[i][j] = dist(rd);
    std::cout<<"The element of the first matrix:-"<<std::endl;
    mat1.print();
    separator();
    std::cout<<"The elements of the second matrix:-"<<std::endl;
    for(std::size_t i = 0; i < mat2.row_size(); i++)
      for(std::size_t j = 0; j < mat2.col_size(); j++)
        mat2[i][j] = dist(rd);
    mat2.print();
    separator();
    std::cout<<"The product of both matrices multiplication:-"<<std::endl;
    matrix product_mat = mat1 * mat2;
    product_mat.print();
    separator();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cout<<"Press any key to close this window..."<<std::endl;
    std::cin.get();
    return 0;
}
