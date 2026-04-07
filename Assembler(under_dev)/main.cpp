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
    {"JC", 3},
    {"JZERO", 0}
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
    cout << CYAN << "|             " << BOLD << "SCESNA 16-bit Assembler v1.4" << RESET << CYAN << "                  |" << RESET << endl;
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



void type_00(string opcode, string src_str, string dest_str){
    int opc = opcodes[opcode];
    int s = src[src_str];
    int d = dest[dest_str];
    
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

void type_10(string flag, int addr){
    int flg = flags[flag];
    
    int instruction = (2 << 14) | (flg << 12) | addr;
    
    string binary = bitset<16>(instruction).to_string();
    bincode.push_back(binary);
    program_counter++;
}

void type_11(string reg, int addr){
    int reg_id = dest[reg];
    if (reg_id > 3) {
        cerr << RED << "> Error: Only R0-R3 can be used with LOAD/STORE." << RESET << endl;
        return;
    }

    int instruction = (3 << 14) | ((reg_id & 0x3) << 12) | (addr & 0xFFF);
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
        string name, val_str;
        ss >> name >> val_str;
        if (!name.empty() && name.back() == ',') name.pop_back();
        ConstantsInProgram[name] = parseHex(val_str);
    }
    else if (first_word[0] == '@') {
        return; // Labels handled in pre-scan
    }
    else if (first_word.substr(0,2) == "//") {
        return;
    }
    else {
        string opcode = first_word;
        if (!opcode.empty() && opcode.back() == ',') opcode.pop_back();
        string arg1, arg2;
        ss >> arg1 >> arg2;

        if (!arg1.empty() && arg1.back() == ',') arg1.pop_back();
        if (!arg2.empty() && arg2.back() == ',') arg2.pop_back();

        if (opcode == "HALT") {
            int opc = opcodes[opcode];
            int instruction = (0 << 14) | (opc << 10);
            bincode.push_back(bitset<16>(instruction).to_string());
            program_counter++;
        }
        else if (opcode == "LOAD") {
            int addr = ConstantsInProgram.count(arg2) ? ConstantsInProgram[arg2] : parseHex(arg2);
            type_11(arg1, addr);
        }
        else if (opcode == "STORE") {
            int addr = ConstantsInProgram.count(arg2) ? ConstantsInProgram[arg2] : parseHex(arg2);
            type_01(arg1, addr);
        }
        else if (flags.count(opcode)) {
            string label = (arg1 == "," || arg1.empty()) ? arg2 : arg1;
            if (!label.empty() && label[0] == '@') label = label.substr(1);
            int addr = tag_id.count(label) ? tag_id[label] : parseHex(label);
            type_10(opcode, addr);
        }
        else if (opcode == "MOV") {
            string src_s = arg1, dest_s = arg2;
            if (dest_s.empty() && src_s.find(',') != string::npos) {
                size_t pos = src_s.find(',');
                dest_s = src_s.substr(pos + 1);
                src_s = src_s.substr(0, pos);
            }
            type_00("PASSA", src_s, dest_s); 
        }
        else if (opcode == "OUT") {
            type_00("PASSA", arg1, "CPU_OUT");
        }
        else if (opcodes.count(opcode)) {
            type_00(opcode, "ALU_OUT", arg1);
        }
    }
}

void assembler(string fname) {
    ifstream file(fname);
    if (!file.is_open()) { cerr << RED << "File Error!" << RESET << endl; return; }

    program_counter = 0;
    tag_id.clear();
    ConstantsInProgram.clear();
    bincode.clear();

    // Pass 1: Pre-scan for labels
    string temp;
    while (getline(file, temp)) {
        if (temp.empty() || temp.substr(0,2) == "//") continue;
        stringstream ss(temp);
        string word; ss >> word;
        if (word[0] == '@') tag_id[word.substr(1)] = program_counter;
        else if (word != "#define") program_counter++;
    }
    
    // Pass 2: Actual assembly
    file.clear(); file.seekg(0);
    int final_pc = 0;
    program_counter = 0; 
    while (getline(file, temp)) {
        if (temp.empty() || temp.substr(0,2) == "//") continue;
        define_type(temp);
    }
    file.close();
    assembled = true; iamsure = false;
    cout << GREEN << "> Assembly complete! Lines: " << bincode.size() << RESET << endl;
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