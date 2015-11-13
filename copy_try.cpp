#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
//char *strdup (const char *s) {
    //char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    //if (d == NULL) return NULL;          // No memory
    //strcpy (d,s);                        // Copy the characters
    //return d;                            // Return the new string
//}

class MyClass {  
    private:      
        char* str;  
    public:      
        MyClass();
        MyClass(char* aStr);      
        MyClass& operator=(const MyClass& obj);      
        void Print();      
        ~MyClass();
};

MyClass::MyClass() {
}

MyClass::MyClass(char* aStr) {  
    cout << "In constructor ..." << endl;  
    str = strdup(aStr);
}

// Copy assignment
MyClass& MyClass::operator=(const MyClass& other) {
    cout << "In copy assignment ..." << endl;
    str = strdup(other.str);
    return *this;
}

void MyClass::Print() {  
    cout << str << endl;
}

MyClass::~MyClass() {  
    cout << "In destructor ..." << endl;  
    delete str;
}

int main()
{  
    // Create obj1
    MyClass* obj1 = new MyClass("Hello World");
    obj1->Print();

    // Assignment. obj1 to obj2
    MyClass obj2;
    obj2 = *obj1;

    // Cleanup obj1
    delete obj1;

    obj2.Print();
    return 0;
}
