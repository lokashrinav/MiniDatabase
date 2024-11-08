
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
    string command;
    while(true) {
        cout << ">" << endl;
        getline(cin, command);
        if(command == ".exit") {
            break;
        } else if(command[0] == '.') {
            if(dot_command(command)) {
                continue;
            } else {
                cout << "Unrecognized command '" << command << "'." << endl;
            }
        } else {
            if(normal_command(command)) {
                return 1;
            }
        }
    }
}

struct Row {
    uint32_t id;
    char username[255];
    char email[32];
};

struct Table {
    uint32_t num_rows;
    void* pages[100];
};

int row_input(Table table, int row_num) {
    int page_num = row_num / 100;
    if(table.pages[page_num] == NULL) {
        table.pages[page_num] = malloc(4096);
    }
    int row_num = row_num % 100;
    return page_num * 291 + row_num;
}

Table* create_table() {
    Table* table = new Table();
    table->num_rows = 0;
    return table;
}

int insert(int id, string username, string email) {
    Row row;
    row.id = id;
    row.username = username;
    row.email = email;
}

int dot_command(string input) {
    if(input == ".exit") {
        exit(0);
    } else {
        return 0;
    }
}

int normal_command(string input) {
    istringstream iss(input);
    string first_word, second_word, third_word, fourth_word;
    iss >> first_word;
    if(first_word == "insert") {
        iss >> second_word >> third_word >> fourth_word;
        if(iss.fail() || second_word.empty() || third_word.empty() || fourth_word.empty()) {
            cout << "Error: Missing arguments for 'insert' command." << endl;
            return 0;
        }
    }
    else if(first_word == "select") {
        cout << "Invalid Command" << endl;
    }
    else {
        cout << "Invalid Command" << endl;
    }
}