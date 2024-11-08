
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include <cstring>

using namespace std;

struct Row {
    uint32_t id;
    char username[256];
    char email[33];
};

struct Table {
    uint32_t num_rows;
    void* pages[100];
};

void* row_input(Table* table, int row_num) {
    uint32_t page_num = row_num / 100;
    void* page = table->pages[page_num];
    if(page == NULL) {
        page = malloc(4096);
        table->pages[page_num] = page;
    }
    uint32_t row_offset = row_num % 100;
    cout << sizeof(Row) << endl;
    return row_offset * sizeof(Row) + (char*)page;
}
/*
void* row_input(Table* table, int row_num) {
    uint32_t page_num = row_num / 100;
    void* page = table->pages[page_num];
    if (page == NULL) {
        page = malloc(4096);
        if (page == NULL) {
            cerr << "Error: Memory allocation failed." << endl;
            exit(1);
        }
        table->pages[page_num] = page;
    }
    uint32_t row_offset = row_num % 100;
    return (char*)page + row_offset * sizeof(Row);
}*/

Table* create_table() {
    Table* table = new Table();
    table->num_rows = 0;
    for (int i = 0; i < 100; i++) {
        table->pages[i] = NULL;
    }
    return table;
}

int insert( Table* table, int id, string username, string email, int row_num) {
    void* page = table->pages[row_num / 100];
    if (page == NULL) {
        page = malloc(4096);
        table->pages[row_num / 100] = page;
    }
    Row* row = (Row*)((char*)page + (row_num % 100) * sizeof(Row));
    row->id = id;
    strncpy(row->username, username.c_str(), sizeof(row->username) - 1);
    row->username[sizeof(row->username) - 1] = '\0'; 
    strncpy(row->email, email.c_str(), sizeof(row->email) - 1);
    row->email[sizeof(row->email) - 1] = '\0'; 
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

void select_all(Table* table) {
    for (int i = 0; i < table->num_rows; i++) {
        void* row_location = row_input(table, i);
        Row row;
        memcpy(&row, row_location, sizeof(Row));
        cout << row.id << " " << row.username << " " << row.email << endl;
    }
}

int normal_command(string input, Table* table) {
    istringstream iss(input);
    string first_word, second_word, third_word, fourth_word;
    iss >> first_word;
    if(first_word == "insert") {
        iss >> second_word >> third_word >> fourth_word;
        if(iss.fail() || second_word.empty() || third_word.empty() || fourth_word.empty()) {
            cout << "Error: Missing arguments for 'insert' command." << endl;
            return 0;
        }
        if(table->num_rows >= 100 * (4096/sizeof(Row))) {
            cout << "Error: Table is full." << endl;
            return 0;
        }
        if(stoi(second_word) < 0 || stoi(second_word) > 4294967295) {
            cout << "Error: ID must be positive and lower" << endl;
            return 0;
        }
        if(third_word.length() >= 256) {
            cout << "Error: Username must be less than 256 characters" << endl;
            return 0;
        }
        if(fourth_word.length() >= 33) {
            cout << "Error: Email must be less than 33 characters" << endl;
            return 0;
        }
        insert(table, stoi(second_word), third_word, fourth_word, table->num_rows);
    }
    else if(first_word == "select") {
        select_all(table);
    }
    else {
        cout << "Invalid Command" << endl;
    }
    return 0;
}


int main() {
    Table* table = create_table();
    string command;
    while(true) {
        cout << ">" << " ";
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
            normal_command(command, table);
        }
    }
}