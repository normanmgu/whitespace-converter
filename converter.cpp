#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include "string.h"

/* WHITESPACE CONVERSION
 * This progran convert whitespace indentation from space
 * to taps and vice versa. This program also has the ability
 * to choose where if you want 2 space indention or 4 space
 * indentation or N space indentation
*/

void TEST_vars(std::string file_name, std::string w1, unsigned n1,std::string w2, unsigned n2) {
    std::cout << "file_name: " << file_name << std::endl;
    std::cout << "w1: " << w1 << std::endl;
    std::cout << "n1: " << n1 << std::endl;
    std::cout << "w2: " << w2 << std::endl;
    std::cout << "n2: " << n2 << std::endl;
}

void TEST_buffer(std::vector<std::string> buffer) {
    for(unsigned i = 0; i < buffer.size(); i++) {
        std::cout << i << " " << buffer[i] << std::endl;
    }
}

void parseArgv(std::string &file_name, std::string &w1, unsigned &n1, std::string &w2, 
    unsigned &n2, char* argv[]) {
    file_name = argv[1];
    w1 = argv[2];
    n1 = atoi(argv[3]);
    w2 = argv[4];
    n2 = atoi(argv[5]); 
}

void fileCanOpen(std::string file_name) {
    std::fstream File;
    File.open(file_name, std::ios::in);
    if(!File.is_open()) {
        std::cout << "Could not open file: " << file_name << std::endl;
        exit(0);
    }
    File.close();
}

bool isValidChoice(std::string w) {
    std::string s = "space";
    std::string t = "tab";
    if(w != s && w != t) return false;
    return true;
}

bool spaceOrTab(std::string w) {
    std::string s = "space";
    std::string t = "tab";
    if(w == s) return true;
    else return false;
}

void createBuffer(std::string file_name, std::vector<std::string> &buffer) {
    std::fstream File;
    std::string line;

    File.open(file_name, std::ios::in);
    while(File.peek() != EOF) {
        getline(File, line);
        buffer.push_back(line);
    }
}

std::string replace(std::string l, char  old, char new_, unsigned n1, unsigned n2) { 
    std::string line = l;
    unsigned count = 0;
    unsigned ccount = 0;
    for(unsigned i = 0; i < line.size(); i++) {
        if(line[i] == old) count++; 
        else {
            break;
        }
        if(count == n1) {
            for(unsigned j = 0; j < n1; j++) {
                line.erase(0, 1);
                i -= 1;
            }
            count = 0;
            ccount++;
        }
    }
    for(unsigned i = 0; i < ccount; i++) {
        for(unsigned j = 0; j < n2; j++) {
            line.insert(line.begin(), new_);
            //std::cout << "should been inserting here" << std::endl;
        }
    }
    return line;
}

void updateBuffer(std::vector<std::string> &buffer, bool type_1, bool type_2,
        unsigned n1, unsigned n2) {
    char current, old, new_;

    if (type_1) old = ' ';
    else old = '\t';
    if(type_2) new_ = ' ';
    else new_ = '\t';
    
    unsigned count;
    for(unsigned i = 0; i < buffer.size();i++) {
        count = 0;
        for(unsigned j = 0; j < buffer.size(); j++) {
            //std::cout << "(i,j) " << "(" << i << "," << j << ")" << std::endl;
            if(count ==  n1) { 
                buffer.at(i) = replace(buffer.at(i), old, new_, n1, n2);
                count = 0;
            }
            if(buffer[i][j] == old) count++;
            else break;
        }
    }
}

void saveFile(std::string file_name, std::vector<std::string> buffer) {
    std::fstream File;
    File.open(file_name, std::ios::out);
    for(unsigned i = 0; i < buffer.size(); i++) {
        File << buffer[i] << std::endl;
    }
    File.close();
}

int main(int argc, char* argv[]) {

    if (argc != 6) {
        std::cout<<"Invalid arguments, enter in the following format:"<<std::endl;
        std::cout<<"file_name whitespace_type1 num whitespace_type2 num"<<std::endl;
        exit(0);
    }

    std::string file_name;
    std::string w1, w2;             // from whitespace(w1) to whitespace(2)
    unsigned n1, n2;                // num of whitespace(w1) and num of whitespace(n2) 
                                    
    parseArgv(file_name, w1 ,n1 ,w2 ,n2 , argv);
    //TEST_vars(file_name, w1, n1, w2, n2);
    fileCanOpen(file_name);

    if(!isValidChoice(w1) || !isValidChoice(w2)) {
        std::cout << "Error: Please enter valid whitespace arguments" << std::endl;
        exit(0);
    }
    
    std::vector<std::string> buffer;
    createBuffer(file_name, buffer);
    TEST_buffer(buffer);

    bool type_1, type_2;
    type_1 = spaceOrTab(w1);
    type_2 = spaceOrTab(w2);

    updateBuffer(buffer, type_1, type_2, n1, n2); 

    if( remove(file_name.c_str()) != 0 )
        perror( "Error deleting file" );
    else { 
        puts( "File successfully deleted" );
        std::cout << "Creating updated file..." << std::endl;
    }
    saveFile(file_name, buffer);
    TEST_buffer(buffer);
    return 0;
}
