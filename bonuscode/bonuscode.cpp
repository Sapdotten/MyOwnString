#include <iostream>
#include <stdexcept>

// String() +
// String(const char*) +
// String(const String&) +
// operator= +
// ~String +
// +=, = +
// ==, != +
// string s.substr(int index, int length) +
// size() +
// []x2 +



struct String {
    char* _string; //8
    union {
        unsigned long long int _size=0;//8
        char _short_string[24];//по 4 байта на 4 символа
    };

public:
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
    bool is_static() const{
        /*возращает true, если строка не использует динамику*/
        return(_string == _short_string);
    }

    String(const String& str) {
        if (!str.is_static()){
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
        if (str.is_static()) {
            if (is_static()) {
                char temp[24];
                strcpy(temp, _short_string);
                strcpy(_short_string, str._short_string);
                strcpy(str._short_string, temp);
            }
            else {
                unsigned long long temp = _size;
                if (_string == nullptr) {
                    *this = String(str._string);
                    str = String();
                }
                else{
                    strcpy(_short_string, str._short_string);
                    str._size = temp;
                    str._string = _string;
                    _string = _short_string;
                }
                
            }
            
        }
        else if(is_static()){
            if (str._string == nullptr) {
                str = String(str._string);
                *this = String();
            }
            else {
                unsigned long long temp = str._size;
                strcpy(str._short_string, _short_string);
                _size = temp;
                _string = str._string;
                str._string = str._short_string;
            }
            
        }
        else {
            std::swap(_string, str._string);
            std::swap(_size, str._size);
        }
    }
    String& operator=(const String str){
        this->swap(str);
        return *this;
    }
    ~String(){
        if(!is_static())
        delete[] _string;
    }


    String operator=(const char* str) {
        String a(str);
        return a;
    }
    String& operator+=(const String& b) {
        if (_string == nullptr || b._string == nullptr)
            throw std::invalid_argument("");

        int delta = 4 - (size() + 1) % 4;
        int size = this->size() + b.size();
        if (size<24 || b.size() <= delta) {
            strcat(_string, b._string);
        }
        else {
            char *ptr = new char[size + (5 - (size + 1) % 4)];
            strcpy(ptr, _string);
            strcat(ptr, b._string);
            delete[] _string;
            _string = ptr;
        }
        if (!is_static())
            _size = size;
        return *this;
    }
    String operator+(const String& b) const{
        if (_string == nullptr || b._string == nullptr)
            throw std::invalid_argument("");

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
            if(!a.is_static())
            delete[] a._string;
            a._string = ptr;
        }
        if (!a.is_static())
            a._size = size;
        return a;
    }

    char operator[](int index) const {
        if(index>_size)
            throw std::out_of_range("");
        if (index < 0)
            throw std::invalid_argument("");
        return _string[index];
    }
    char& operator[](int index){
        if (index > _size)
            throw std::out_of_range("");
        if (index < 0)
            throw std::invalid_argument("");
        return _string[index];
    }

    bool operator==(const String& b) const {
        if(_string==nullptr || b._string == nullptr)
            throw std::invalid_argument("impossible to compare void strings");
        return !strcmp(this->_string, b._string);
    }
    bool operator!=(const String& b) const {
        return !(*this == b);
    }

    String substr(int index, int length) {
        if (_string == nullptr)
            throw std::invalid_argument("param can't be void string");
        if (index + length >= _size)
            throw std::out_of_range("");
        if (index < 0 || length <= 0)
            throw std::invalid_argument("");

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
    String static_str("There is no dynamic mem");
    String dynam_str("This string is using a heap");
    std::cout << dynam_str.is_static() << " " << static_str.is_static();
    String result = static_str + dynam_str;
    std::cout << std::endl << result.c_str() << " " << result.size() << " " << result.is_static();
 
    
    return 0;
}
