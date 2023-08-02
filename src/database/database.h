#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sqlite3.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& file_name, const std::string& schema_file_path);
    ~DatabaseManager();

    bool insert_data(const std::string& table_name, const std::string& data);
    bool edit_data(const std::string& table_name, const std::string& set_clause, const std::string& where_clause);
    bool delete_data(const std::string& table_name, const std::string& where_clause);
    std::string retrieve_data(const std::string& table_name);
    void create_loop(const std::string &loop_name);
    int insert_into_column(const std::string& table_name, const std::string &column, const std::string& data);
    int find_loop_id(const std::string& loop_name);
    int replace_track_to_loop(const int& loop_id, const std::string& track_name );
    std::vector<std::string> load_tracks_by_loop(const int& loop_id);
    void save_database();
private:
    std::string file_name_;
    std::string schema_file_path_;
    sqlite3* db_;
    bool has_changes_;

    void open_database();
    void create_tables_from_schema();
    void close_database();
    std::string read_schema_from_file(const std::string& file_name);
    static int callback(void* data, int argc, char** argv, char** /* Not used */);
    static int callback_get_id(void* not_used, int argc, char** argv, char** colName);
    static int callback_load_tracks(void* file_arr, int argc, char** argv, char** colName);
};

#endif // DATABASE_H
