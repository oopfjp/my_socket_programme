#include <iostream>

struct Student{
    int age;
    const char* name;
};

int main(){

    const char s[] = "Hello World\n";
    const void* p = s;
    std::cout << *(char*)p<< std::endl;
    *(char*)p = 'h';
    std::cout << *(char*)p<< std::endl;
    return 0;
}