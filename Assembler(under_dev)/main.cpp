#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>

using namespace std;

// global variables
string filename;
string line;
bool assembled = false;
int program_counter; // for @ label assignation
int memory_counter;
vector<string> bincode;

// Constants 
unordered_map<string, int> ConstantsInProgram;
unordered_map<string, int> tag_id;

unordered_map<string, int> src = {
    // General Purpose Registers
    {"R0", 0},
    {"R1", 1},
    {"R2", 2}, 
    {"R3", 3},
    {"RA", 4},
    {"RB", 5},
    {"CPU_OUT", 6}, 
    {"ALU_OUT", 4} 
};

unordered_map<string, int> opcodes = {
    // ALU opcodes
    {"ADD",   0},
    {"SUB",   1},
    {"MUL",   2},
    {"DIV",   3},
    {"NOT",   4},
    {"AND",   5},
    {"OR",    6},
    {"XOR",   7},
    {"PASSA", 8},
    {"PASSB", 9},
    {"HALT",  15}
};

unordered_map<string, int> dest = {
    // destination id
    {"r0", 0},
    {"r1", 1},
    {"r2", 2},
    {"r3", 3},
    {"rA", 4},
    {"rB", 5},
    {"OUT", 6}
};

unordered_map<string, int> flags = {
    {"JZ", 0},
    {"JGREATER", 1},
    {"JSMALLER", 2},
    {"JC", 3}
};

void initial_command(){
    cout << "=============================================================" << endl;
    cout << "|                                                           |" << endl;
    cout << "|             SCESNA 16-bit Assembler v1.0                  |" << endl;
    cout << "|            Command-Line Interface (C++17)                 |" << endl;
    cout << "|                                                           |" << endl;
    cout << "=============================================================" << endl;
    cout << endl;
    cout << "Available commands:" << endl;
    cout << "  assemble         - Assemble .sc file" << endl;
    cout << "  view hex         - Display machine code in hexadecimal" << endl;
    cout << "  view bin         - Display machine code in binary" << endl;
    cout << "  view const       - Display symbol table" << endl; 
    cout << "  help             - Show this help message" << endl;
    cout << "  exit             - Exit the Assembler" << endl;
}

void type_01(string line){
    string str_num;

}

void define_type(string line){
    
    stringstream ss(line);
    string first_word;
    ss >> first_word;

    // constants, labels and comments 
    if (first_word == "#define") {
        string name;
        ss >> name;
        ConstantsInProgram[name] = memory_counter;
        memory_counter++;
    }

    else if (first_word[0] == '@') {
        string id_name = first_word.substr(1);
        tag_id[id_name] = program_counter;
    }

    else if (first_word[0] == '/' && first_word[1] == '/' ){
        return;
    }
    // Instruction
    else {
        if(line.find("LOAD") != string::npos){
            // start here
        }

        program_counter++;
    }


}


void assembler(string filename){

    ifstream file(filename);
    if(!file.is_open()){
        cerr << "> File could not be opened!" << endl;
    }

    program_counter = 0;
    memory_counter = 0;
    ConstantsInProgram.clear();

    while(getline(file, line)){
        if(line.empty()){
            continue;
        }

        define_type(line);

    }
}




int main(){
    initial_command();
    // main loop
    while(true){
        cout << "> ";
        string command;
        cin >> command;

        if(command == "exit"){
            break;
        }

        else if(command == "help"){
            initial_command();
        }

        else if(command == "view hex"){
            if(assembled == true){
            }
            else{
                cout << "> You must assemble a file first" << endl;
            }
        }
        
        else if(command == "view bin"){
            if(assembled == true){

            }
            else{
                cout << "> You must assemble a file first" << endl;
            }
        }

        else if(command == "view const"){
            if(assembled == true){

            }
            else{
                cout << "> You must assemble a file first" << endl;
            }
        }

        else if (command == "assemble"){
            cout << "> enter the filename: " << endl;
            cout << "> " ;
            cin >> filename;


            assembler(filename);

            


        }
    
    }
    

    return 0;
}