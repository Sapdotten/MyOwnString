#include <iostream>
using namespace std;

class String {
public:
    char a[9] = "fuck you";
    union {
        float d;
        double f;
        const short int x = 8;
    };
    
};




int main()
{
    String a;
    std::cout << a.a<<sizeof(String);
}
