#include <iostream>
#include <stdexcept>

// String() +
// String(const char*) +
// String(const String&) +
// operator= +
// ~String +
// +=, = +
// ==, != +
// string s.substr(int index, int length) 
// size() +
// []x2 +



struct String {
    char* _string; //8
    union {
        unsigned long long int _size=0;//8
        char _short_string[24];//по 4 байта на 4 символа
    };


    String() :_string(nullptr), _size(0) {};

    String(const char* str) {
        if (strlen(str) < 24) {
            strcpy(_short_string, str);
            _string = _short_string;
        }
        else {
            _size = strlen(str);
            _string = new char[_size+(5-(_size+1)%4)];
            /*Поскольку компилятор будет выравнивать слова чтобы оптимизировать пространство, то кол-во выделенной
            памяти будет кратно 4,поэтому лучше использовать оставшуюся память при создании строки,
            длина которой не кратна 4*/
            strcpy(_string, str);
        }
    }



    unsigned long long size() const{
        if (_string==_short_string) {
            return strlen(_string);
        }
        return _size;
    }

    char* c_str() const{
        return _string;
    }



    String(const String& str) {
        if (str._string != str._short_string){
            _size = str._size;
            _string = new char[_size + (5 - (_size + 1) % 4)];
            strcpy(_string, str._string);
        }
        else {
            strcpy(_short_string, str._short_string);
            _string = _short_string;
        }
    }

    void swap(String str) {
        if (str._string == str._short_string) {
            if (_string == _short_string) {
                char temp[24];
                strcpy(temp, _short_string);
                strcpy(_short_string, str._short_string);
                strcpy(str._short_string, temp);
            }
            else {
                unsigned long long temp = _size;
                strcpy(_short_string, str._short_string);
                str._size = temp;
                str._string = _string;
                _string = _short_string;
            }
            
        }
        else if(_string ==_short_string){
            unsigned long long temp = str._size;
            strcpy(str._short_string, _short_string);
            _size = temp;
            _string = str._string;
            str._string = str._short_string;
        }
        else {
            std::swap(_string, str._string);
            std::swap(_size, str._size);
        }
    }

    String& operator=(const String str)
    {
        this->swap(str);
        return *this;
    }

    ~String(){
        if(_string!=_short_string)
        delete[] _string;
    }



    String operator=(const char* str) {
        String a(str);
        return a;
    }

    String& operator+=(const String& str) {
        int delta = 4 - (size() + 1) % 4;
        int size = this->size() + str.size();
        if (size<24 || str.size() <= delta) {
            strcat(_string, str._string);
        }
        else {
            char *ptr = new char[size + (5 - (size + 1) % 4)];
            strcpy(ptr, _string);
            strcat(ptr, str._string);
            delete[] _string;
            _string = ptr;
        }
        if (_string != _short_string)
            _size = size;
        return *this;
    }

    String operator+(const String& b) const{
        String a = *this;
        int delta = 4 - (a.size() + 1) % 4;
        int size = a.size() + b.size();
        if (size < 24 || b.size() <= delta) {
            strcat(a._string, b._string);
        }
        else {
            char* ptr = new char[size + (5 - (size + 1) % 4)];
            strcpy(ptr, a._string);
            strcat(ptr, b._string);
            delete[] a._string;
            a._string = ptr;
        }
        if (a._string != a._short_string)
            a._size = size;
        return a;
    }

    char operator[](int index) const {
        if(index>_size)
            throw std::out_of_range("");
        return _string[index];
    }
    char& operator[](int index){
        if (index > _size)
            throw std::out_of_range("");
        return _string[index];
    }

    bool operator==(const String& b) const {
        return !strcmp(this->_string, b._string);
    }
    bool operator!=(const String& b) const {
        return !(*this == b);
    }

    String substr(int index, int length) {
        if (index + length >= _size)
            throw std::out_of_range("");
        char* str = new char[length + 1];
        strncpy(str, _string + index, length);
        str[length] = 0;
        String a(str);
        delete[] str;
        return a;
    }
};




int main()
{
    String str1("There is a test string");
    String str2("!");
    String str3 = "aaaaaaaaaaaaaaaaaaaaaaaaa";
    int size = str1.size();
    str3=str1+ str2;
    String str4;
    String sub = str1.substr(3, 10);
    std::cout << str1.size()<<" + "<<str2.size()<<" = "<<str3.size()<<std::endl;
    std::cout << str1.c_str() << " + " << str2._string<<" = "<<str3._string;
    std::cout <<std::endl<< (str1==str4);

}
