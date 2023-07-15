#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <fstream>
#include <iostream>
#include <sqlite3.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& file_name, const std::string& schema_file_path);
    ~DatabaseManager();

    bool insert_data(const std::string& table_name, const std::string& data);
    bool edit_data(const std::string& table_name, const std::string& set_clause, const std::string& where_clause);
    bool delete_data(const std::string& table_name, const std::string& where_clause);
    std::string retrieve_data(const std::string& table_name);

private:
    std::string file_name_;
    std::string schema_file_path_;
    sqlite3* db_;
    bool has_changes_;

    void open_database();
    void create_tables_from_schema();
    void close_database();
    void save_database();
    std::string read_schema_from_file(const std::string& file_name);
    static int callback(void* data, int argc, char** argv, char** /* Not used */);
};

#endif // DATABASE_H
