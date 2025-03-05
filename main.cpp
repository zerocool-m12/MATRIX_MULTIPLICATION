#include<iostream>
#include<tuple>
#include<random>
#include<chrono>
#include<algorithm>
#include<iomanip>
template <typename **ITER, typename **FUNC>
void **generate(**ITER begin, **ITER end, **FUNC foo){
    while(begin != end){
        *begin = foo();
        begin++;
    }
}
template<int X, int Y, int M, int N>
std::tuple<int**,std::size_t,std::size_t> matrix_multiplication (int(&arr)[X][Y], int(&arr2)[M][N]){
    if(Y != M){
        std::cerr<<"ERROR: Row size of the first matrix does not match the column size of the second matrix!"<<std::endl;
        return std::tuple<int**,std::size_t, std::size_t>(nullptr,0,0);
    }
    if(X == 0 or Y == 0 or M == 0 or N == 0){
        std::cerr<<"ERROR: Unable to process zero size!"<<std::endl;
        return std::tuple<int**,std::size_t, std::size_t>(nullptr,0,0);
    }
    int**arr_result = new int*[X];
    for(std::size_t i = 0; i < X; i++){
        arr_result[i] = new int[N]{};
    }
    for(std::size_t i = 0; i < X; i++){
        for(std::size_t j = 0; j < N; j++){
            for(std::size_t k = 0; k < Y; k++)
              arr_result[i][j] += arr[i][k] * arr2[k][j];
        }
    }
    return std::tuple<int**,std::size_t, std::size_t>(arr_result,X,N);
}
int main(){
    int arr[5][3];
    int arr2[3][2];
    std::mt19937 rd (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>dist(1,100);
    auto randomize = [&]{return dist(rd);};     // function for generating random numbers
    for(int i = 0; i < sizeof arr / sizeof arr[0]; i++)
      __generate (arr[i], arr[i] + (sizeof arr[i] / sizeof (int)),randomize);
    std::cout<<"The elements of the first array:-"<<std::endl;
    for(const auto & i : arr){
      for(const auto & j : i)
        std::cout<<j<<'\t';
      std::cout.put('\n');
    }
    auto separator = []{std::cout<<std::string(30,'*')<<std::endl;};
    separator();
    for(int i = 0; i < sizeof arr2 / sizeof arr2[0]; i++)
      __generate (arr2[i], arr2[i] + (sizeof arr2[i] / sizeof (int)),randomize);
    std::cout<<"The elements of the second array:-"<<std::endl;
    for(const auto & i : arr2){
      for(const auto & j : i)
        std::cout<<j<<'\t';
      std::cout.put('\n');
    }
    separator();
    std::cout<<"The product of multiplying both arrays:-"<<std::endl;
    std::tuple<int**,std::size_t, std::size_t>arr_result = matrix_multiplication(arr,arr2);
    try{
        if(! std::get<0>(arr_result)) throw "ERROR: Something went wrong!\n";
    }catch( const char * e){
        std::cerr<<e<<std::endl;
        return 1;
    }
    for(std::size_t i = 0; i < std::get<1>(arr_result); i++){
        for(std::size_t j = 0; j < std::get<2>(arr_result); j++)
          std::cout<<std::get<0>(arr_result)[i][j]<<'\t';
        std::cout.put('\n');
    }
    for(std::size_t i = 0; i < std::get<1>(arr_result); i++){
        delete [] std::get<0>(arr_result)[i];
    }
    delete [] std::get<0>(arr_result);
    return 0;
}
