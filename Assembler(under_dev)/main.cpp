#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <bitset>
#include <iomanip>

using namespace std;

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"      // For Errors
#define GREEN   "\033[32m"      // For Success
#define YELLOW  "\033[33m"      // For Warnings/Prompt
#define CYAN    "\033[36m"      // For Headers
#define BOLD    "\033[1m"

// global variables
string filename;
string line;
bool assembled = false;
bool iamsure = false;
int program_counter; // for @ label assignation
int memory_counter;
vector<string> bincode; 
vector<string> hexcode;

// Constants 
unordered_map<string, int> ConstantsInProgram;
unordered_map<string, int> tag_id;

unordered_map<string, int> src = {
    {"R0", 0}, {"r0", 0},
    {"R1", 1}, {"r1", 1},
    {"R2", 2}, {"r2", 2},
    {"R3", 3}, {"r3", 3},
    {"RA", 4}, {"rA", 4}, {"Ra", 4},
    {"RB", 5}, {"rB", 5}, {"Rb", 5},
    {"CPU_OUT", 6},
    {"ALU_OUT", 4}
};

unordered_map<string, int> opcodes = {
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
    {"R0", 0}, {"r0", 0},
    {"R1", 1}, {"r1", 1},
    {"R2", 2}, {"r2", 2},
    {"R3", 3}, {"r3", 3},
    {"RA", 4}, {"rA", 4}, {"Ra", 4},
    {"RB", 5}, {"rB", 5}, {"Rb", 5},
    {"OUT", 6}, {"CPU_OUT", 6}
};

unordered_map<string, int> flags = {
    {"JZ", 0},
    {"JGREATER", 1},
    {"JSMALLER", 2},
    {"JC", 3}
};

int parseHex(string hex_str) {
    if (hex_str.substr(0, 2) == "0x" || hex_str.substr(0, 2) == "0X") {
        return stoi(hex_str, nullptr, 16);
    }
    return stoi(hex_str);
}

void initial_command(){
    cout << CYAN << "=============================================================" << RESET << endl;
    cout << CYAN << "|                                                           |" << RESET << endl;
    cout << CYAN << "|             " << BOLD << "SCESNA 16-bit Assembler v1.0" << RESET << CYAN << "                  |" << RESET << endl;
    cout << CYAN << "|            Command-Line Interface (C++17)                 |" << RESET << endl;
    cout << CYAN << "|                                                           |" << RESET << endl;
    cout << CYAN << "=============================================================" << RESET << endl;
    cout << endl;
    cout << YELLOW << "Available commands:" << RESET << endl;
    cout << "  assemble         - Assemble .scesna file" << endl;
    cout << "  view hex         - Display machine code in hexadecimal" << endl;
    cout << "  view bin         - Display machine code in binary" << endl;
    cout << "  view const       - Display symbol table" << endl; 
    cout << "  help             - Show this help message" << endl;
    cout << "  exit             - Exit the Assembler" << endl;
}



void type_00(string opcode, string arg1, string arg2){
    int opc = opcodes[opcode];
    int s = src[arg1];
    int d = dest[arg2];
    
    int instruction = (0 << 14) | (opc << 10) | (s << 7) | (d << 4);
    
    string binary = bitset<16>(instruction).to_string();
    bincode.push_back(binary);
    program_counter++;
}

void type_01(string reg, int addr){
    int reg_id = dest[reg];
    
    int instruction = (1 << 14) | (reg_id << 12) | addr;
    
    string binary = bitset<16>(instruction).to_string();
    bincode.push_back(binary);
    program_counter++;
}

void type_10(string flag, string label){
    int flg = flags[flag];
    int addr = tag_id[label];
    
    int instruction = (2 << 14) | (flg << 12) | addr;
    
    string binary = bitset<16>(instruction).to_string();
    bincode.push_back(binary);
    program_counter++;
}

void type_11(string reg, int addr){
    int reg_id = dest[reg];
    
    int instruction = (3 << 14) | (reg_id << 12) | addr;
    
    string binary = bitset<16>(instruction).to_string();
    bincode.push_back(binary);
    program_counter++;
}

void binToHex(){
    hexcode.clear();
    for(string bin : bincode){
        unsigned int val = stoul(bin, nullptr, 2);
        stringstream ss;
        ss << uppercase << hex << setfill('0') << setw(4) << val;
        hexcode.push_back(ss.str());
    }
}

void define_type(string line){
    stringstream ss(line);
    string first_word;
    ss >> first_word;

    if (first_word == "#define") {
        string name;
        int val;
        ss >> name >> val;
        ConstantsInProgram[name] = val;
        memory_counter++;
    }
    else if (first_word[0] == '@') {
        string id_name = first_word.substr(1);
        tag_id[id_name] = program_counter;
    }
    else if (first_word[0] == '/' && first_word[1] == '/') {
        return;
    }
    else {
        string opcode = first_word;
        string arg1, arg2, arg3;
        ss >> arg1 >> arg2 >> arg3;

        if (arg1.back() == ',') arg1.pop_back();
        if (arg2.back() == ',') arg2.pop_back();

        if (opcode == "HALT") {
            int opc = opcodes[opcode];
            int instruction = (0 << 14) | (opc << 10);
            string binary = bitset<16>(instruction).to_string();
            bincode.push_back(binary);
            program_counter++;
        }
        else if (opcode == "LOAD") {
            int addr = parseHex(arg2);
            type_11(arg1, addr);
        }
        else if (opcode == "STORE") {
            int addr = parseHex(arg2);
            type_01(arg1, addr);
        }
        else if (flags.count(opcode)) {
            string label = arg1;
            if (label[0] == '@') label = label.substr(1);
            if (tag_id.count(label)) {
                type_10(opcode, label);
            }
        }
        else if (opcodes.count(opcode) && src.count(arg1)) {
            type_00(opcode, arg1, arg2);
        }
    }
}
void assembler(string filename) {
    //check if with .scensa
    if (filename.length() < 7 || filename.substr(filename.length() - 7) != ".scesna") {
        cerr << RED << "> Error: Invalid file extension. Please use a .scesna file." << RESET << endl;
        return;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << RED << "> File could not be opened!" << RESET << endl;
        return;
    }

    //reset 
    program_counter = 0;
    memory_counter = 0;
    ConstantsInProgram.clear();
    tag_id.clear();
    bincode.clear();
    hexcode.clear();

    while (getline(file, line)) {
        if (line.empty() || line.find_first_not_of(" \t\n\v\f\r") == string::npos) {
            continue;
        }

        define_type(line);
    }

    file.close();

    assembled = true;
    iamsure = false;
    cout << GREEN << "> Assembly complete!" << RESET << endl;
    cout << CYAN << "> Lines of code: " << RESET << program_counter << endl;
}

int main() {
    initial_command();
    string full_command;

    while (true) {
        cout << YELLOW << "> " << RESET;
        
        if (!getline(cin >> ws, full_command)) break;

        if (full_command == "exit") {
            break;
        } 
        else if (full_command == "help") {
            initial_command();
        } 
        else if (full_command == "view hex") {
            if (assembled) {
                if (!iamsure) {
                    binToHex();
                    iamsure = true;
                }
                for (const string& s : hexcode) {
                    cout << GREEN << s << RESET << endl;
                }
            } else {
                cout << RED << "> You must assemble a file first" << RESET << endl;
            }
        } 
        else if (full_command == "view bin") {
            if (assembled) {
                for (const string& s : bincode) {
                    cout << GREEN << s << RESET << endl;
                }
            } else {
                cout << RED << "> You must assemble a file first" << RESET << endl;
            }
        } 
        else if (full_command == "view const") {
            if (assembled) {
                cout << CYAN << "Labels:" << RESET << endl;
                for (auto const& [key, val] : tag_id) {
                    cout << GREEN << key << RESET << ": " << val << endl;
                }
                cout << CYAN << "Constants:" << RESET << endl;
                for (auto const& [key, val] : ConstantsInProgram) {
                    cout << GREEN << key << RESET << ": " << val << endl;
                }
            } else {
                cout << RED << "> You must assemble a file first" << RESET << endl;
            }
        } 
        else if (full_command == "assemble") {
            cout << YELLOW << "> enter the filename: " << RESET << endl;
            cout << YELLOW << "> " << RESET;
            getline(cin >> ws, filename);
            assembler(filename);
        } 
        else if (full_command == "fun") {
            cout << RED << BOLD << endl;
            cout << "   ** ** " << endl;
            cout << " **** **** " << endl;
            cout << " ********* " << endl;
            cout << "  ******* " << endl;
            cout << "   ***** " << endl;
            cout << "    *** " << endl;
            cout << "     * " << endl;
            cout << RESET << endl;
            cout << CYAN << "> SCESNA sends its regards! <" << RESET << endl;
        }
        else {
            cout << RED << "> Unknown command: " << full_command << RESET << endl;
        }
    }
    return 0;
}