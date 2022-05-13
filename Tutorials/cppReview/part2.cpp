#include <iostream>
#include "stdio.h"
#include <string>
#include <vector>
#include <unordered_map>

typedef std::vector<int> vi;

#define SIZE 10

struct A{
    //default public members
    int a;
    float b;
    char* c;

    std::string s;

    A (): a(1), b(2), c(nullptr) {
    }

    A(int i) : a(i), b(2), c(nullptr) {}

    void changeMemberA(int a){
        this->a=a;
    }

    void changeMemberB(float b) const{
        //this->b=b;
    }
};

class B{
    //default private members
public:
    int a;
    float b;
private:
    char* c;
};

//void f(int array[][3]){
//}
//void f(int ** array){
//}

void g(const A& a){
    a.changeMemberB(1.f); //declared as const
    //a.changeMemberA(1);     //not const, cannot call
}

template<class T>
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

int main(){
    class A a{};
    a.changeMemberA(1);
    g(a);
    /*
    struct A a{3};

    printf("%d\n", a.a);
    printf("%d\n", b.a);

    std::cout << "string = '"<<a.s<<"'\n";
    */

    /*
    //int array[2][3];
    //f(array);
    //
    //std::vector<std::vector<int>> array;
    std::vector<int> array{1,2,3};

    for (size_t i=0;i<array.size();++i){
        printf("%d\n", array[i]);
    }
    for (auto& a : array){
        a=10;
    }
    for (const auto& a : array){
        printf("%d\n", a);
    }
    */

    //std::unordered_map<std::string, std::vector<int>> map;
    std::unordered_map<std::string, vi> map;
    map["hi"] = {1,2,3};
    printf("%d", SIZE);

    int aa=1,bb=2;
    swap<int>(aa,bb);
    printf("a = %d, b = %d", aa, bb);

    
}
