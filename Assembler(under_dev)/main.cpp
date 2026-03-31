#include <iostream>

using namespace std;

void initial_command(){
    cout << "=============================================================" << endl;
    cout << "|                                                           |" << endl;
    cout << "|      SIDDHCESNA-II 16-bit Assembler v1.0                  |" << endl;
    cout << "|          Command-Line Interface (C++17)                   |" << endl;
    cout << "|                                                           |" << endl;
    cout << "=============================================================" << endl;
    cout << endl;
    cout << "Available commands:" << endl;
    cout << "  assemble <file>  - Assemble .sc file" << endl;
    cout << "  view hex         - Display machine code in hexadecimal" << endl;
    cout << "  view bin         - Display machine code in binary" << endl;
    cout << "  view const       - Display symbol table" << endl;
    cout << "  help             - Show this help message" << endl;
    cout << "  exit             - Exit the Assembler" << endl;
    cout << endl;
}

int main(){
    
    initial_command();
    // main loop
    while(true){
        cout << "> ";
        
    }

    return 0;
}