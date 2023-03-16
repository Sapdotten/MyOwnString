#include <iostream>
#include <string>
using namespace std;

struct String {
    char* _string; //8
    union {
        unsigned long long int _size=0;//8
        char _short_string[24];//по 4 байта на 4 символа
    };

    String() : _size(0), _string(nullptr) {};
    String(const char* str) {
        if (strlen(str) < 24) {
            strcpy(_short_string, str);
            _string = _short_string;
        }
        else {
            _size = strlen(str);
            _string = new char[_size+1];
            strcpy(_string, str);
        }
    }
    unsigned long long size() {
        if (_string==_short_string) {
            return strlen(_string);
        }
        return _size;
    }

};




int main()
{
    String str1("Hello");
    String str2;
    cout << str1.size()<<endl<<str2.size();
}
