#include <iostream> //c++ libraries
#include "iostream" //c libraries
#include "stdio.h"
#include <vector>

//using namespace std;
//BAD IDEA FOR LARGE FILES, TRY TO AVOID

struct A{
    //default public members
    int a;
    float b;
    char* c;

    std::string s;

    A(): a(1), b(2), c(nullptr) {
    }

    A(int i): a(i), b(2), c(nullptr) {}
};

class B{
    //default private members
public:
    int a;
private:
    float b;
    char* c;
};

void f(int **array) {

}

int main() {
    //used w/o command line args

    // printf("Hello world\n");
    // std::cout << "hello world\n";
    // std::cout << "hello world" << std::endl;
    //again, be consistent with what you use to print

    // //pointer vs reference
    // int a = 0;
    // int & b = a;
    // printf("a = %d\n", a);
    // b = 2;
    // printf("a = %d\n", a);
    // printf("b = %d\n", b);
    // //int& c; //references must be initialized
    // int* d;
    //references cannot be changed to reference a different variable
    //if a fixed size of integer is needed is the integers in stdint.h

    // struct A a{3};
    // class B b{};
    // printf("%d\n", a.a);
    // printf("%d\n", b.a);
    // std::cout<<"string = "<<a.s<<"\n";

    // int array[2][3];
    // f(array);

    std::vector<std::vector<int>> array;
}

// int main(int argc, char* argv[]) {
//     //used for command line args

//     // printf("argc = %d\n", argc);
//     // for(int i = 0; i < argc; ++i) {
//     //     printf("argv[%d] = %s\n", i, argv[i]);
//     // }

//     // double d = 2.f;
//     // printf("double is %d\n", d);
// }

//c library functions are a little bit faster than c++ libraries
//if really struggling, try switiching
//try be consistent though

//static keyword means a function/variable is file specific
//extern keyword says "the function/variable is not in this file, go find it"