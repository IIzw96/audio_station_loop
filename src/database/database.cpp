#include "database.h"

DatabaseManager::DatabaseManager(const std::string& file_name, const std::string& schema_file_path) :
    file_name_(file_name), schema_file_path_(schema_file_path), db_(nullptr), has_changes_(false) {
    open_database();
    create_tables_from_schema();
}

DatabaseManager::~DatabaseManager() {
    close_database();
    if (has_changes_) {
        save_database();
    }
}

bool DatabaseManager::insert_data(const std::string& table_name, const std::string& data) {
    std::string sql = "INSERT INTO " + table_name + " VALUES (" + data + ")";
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error inserting data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    has_changes_ = true;
    return true;
}

bool DatabaseManager::edit_data(const std::string& table_name, const std::string& set_clause, const std::string& where_clause) {
    std::string sql = "UPDATE " + table_name + " SET " + set_clause + " WHERE " + where_clause;
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error editing data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    has_changes_ = true;
    return true;
}

bool DatabaseManager::delete_data(const std::string& table_name, const std::string& where_clause) {
    std::string sql = "DELETE FROM " + table_name + " WHERE " + where_clause;
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error deleting data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    has_changes_ = true;
    return true;
}

std::string DatabaseManager::retrieve_data(const std::string& table_name) {
    std::string sql = "SELECT * FROM " + table_name;
    std::string result;
    int return_status = sqlite3_exec(db_, sql.c_str(), callback, &result, nullptr);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error retrieving data: " << sqlite3_errmsg(db_) << std::endl;
    }
    return result;
}

void DatabaseManager::open_database() {
    int return_status = sqlite3_open(file_name_.c_str(), &db_);
    if (return_status != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        db_ = nullptr;
    }
}

void DatabaseManager::create_tables_from_schema() {
    std::string schema = read_schema_from_file(schema_file_path_);
    char* err_msg;
    int return_status = sqlite3_exec(db_, schema.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error executing schema: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

void DatabaseManager::close_database() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void DatabaseManager::save_database() {
    sqlite3* backupDb;
    int return_status = sqlite3_open(file_name_.c_str(), &backupDb);
    if (return_status != SQLITE_OK) {
        std::cerr << "Cannot open backup database: " << sqlite3_errmsg(backupDb) << std::endl;
        return;
    }
    return_status = sqlite3_exec(backupDb, ".backup 'your_saved_database_file.db'", nullptr, nullptr, nullptr);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error saving database: " << sqlite3_errmsg(backupDb) << std::endl;
    }
    sqlite3_close(backupDb);
}

std::string DatabaseManager::read_schema_from_file(const std::string& file_name) {
    std::ifstream file(file_name);
    std::string schema((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return schema;
}

int DatabaseManager::callback(void* data, int argc, char** argv, char** /* Not used */) {
    std::string* result = static_cast<std::string*>(data);
    for (int i = 0; i < argc; ++i) {
        *result += argv[i] ? argv[i] : "NULL";
        *result += " ";
    }
    *result += "\n";
    return 0;
}
