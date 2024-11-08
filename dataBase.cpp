
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
/*
void* row_input(Table* table, int row_num) {
    uint32_t page_num = row_num / 100;
    void* page = table->pages[page_num];
    if(page == NULL) {
        page = malloc(4096);
    }
    uint32_t row_num = row_num % 100;
    return row_num * 291 + page;
}*/

Table* create_table() {
    Table* table = new Table();
    table->num_rows = 0;
    for (int i = 0; i < 100; i++) {
        table->pages[i] = NULL;
    }
    return table;
}

int insert(int id, string username, string email, Table* table, int row_num) {
    void* page = table->pages[row_num / 100];
    if (page == NULL) {
        page = malloc(4096);
        table->pages[row_num / 100] = page;
    }
    Row* row = (Row*)((char*)page + (row_num % 100) * sizeof(Row));
    row->id = id;
    strcpy(row->username, username.c_str());
    strcpy(row->email, email.c_str());
    table->num_rows++;
    return 0;
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
        Table* table = create_table();
        insert(stoi(second_word), third_word, fourth_word, table, table->num_rows);
    }
    else if(first_word == "select") {
        cout << "Invalid Command" << endl;
    }
    else {
        cout << "Invalid Command" << endl;
    }
}