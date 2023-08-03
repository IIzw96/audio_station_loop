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

int DatabaseManager::create_loop(const std::string &loop_name) {
    std::string sql = "INSERT OR IGNORE INTO Loop (loop_name) VALUES (" + loop_name + ");";
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error retrieving data: " << sqlite3_errmsg(db_) << std::endl;
        sqlite3_free(err_msg);
        return return_status;
    }
    return return_status;
}

int DatabaseManager::insert_into_column(const std::string& table_name, const std::string &column, const std::string& data) {
    std::string sql = "INSERT INTO " + table_name + " (" + column + ")" + "VALUES (" + data + ")";
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error inserting data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return return_status;
    }
    has_changes_ = true;
    return return_status;
}

int DatabaseManager::find_loop_id(const std::string& loop_name){
    std::string sql = "SELECT loop_id FROM Loop WHERE loop_name = " + loop_name;
    char* err_msg;
    int single_int_value;
    int return_status = sqlite3_exec(db_, sql.c_str(), callback_get_id, &single_int_value, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error inserting data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return -1;
    }
    std::cout << single_int_value << std::endl;
    return single_int_value;
}

int DatabaseManager::replace_track_to_loop(const int& loop_id, const std::string& track_name ) {
    std::string sql = "REPLACE INTO Track (track_name, track_file_path, loop_id) VALUES ('" + track_name +"', " + "'fake_file_path', " + std::to_string(loop_id) + ");"; // can be edited to add all track columns during save
    std::cout << sql <<std::endl;
    char* err_msg;
    int return_status = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error inserting data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

std::vector<std::string> DatabaseManager::load_tracks_by_loop(const int& loop_id) {
    std::string sql = "SELECT track_file_path FROM Track WHERE loop_id = " + std::to_string(loop_id);
    char* err_msg;
    std::vector<std::string> file_paths;
    int return_status = sqlite3_exec(db_, sql.c_str(), callback_load_tracks, &file_paths, &err_msg);
    if(return_status != SQLITE_OK) {
        std::cerr << "Error reading data: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return file_paths;
    }
    if (file_paths.empty()) {
        file_paths.push_back("");
        return file_paths;
    }
    return file_paths;
}
void DatabaseManager::open_database() {
    int return_status = sqlite3_open(file_name_.c_str(), &db_);
    std::cout << return_status <<std::endl;
    if (return_status != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        db_ = nullptr;
    }
}

void DatabaseManager::create_tables_from_schema() {
    std::string schema = read_schema_from_file(schema_file_path_);
    char* err_msg;
    std::cout << "Creating tables..." << std::endl;
    std::cout << schema.c_str() << std::endl;
    std::string sch = "CREATE TABLE IF NOT EXISTS Loop ("
                "loop_id INTEGER PRIMARY KEY,"
                "loop_name TEXT UNIQUE,"
                "loop_length TEXT,"
                "loop_bpm INTEGER);"

            "CREATE TABLE IF NOT EXISTS Track ("
                "track_id INTEGER PRIMARY KEY,"
                "loop_id INTEGER,"
                "track_name TEXT UNIQUE,"
                "track_type TEXT,"
                "track_file_path TEXT,"
                "FOREIGN KEY (loop_id) REFERENCES Loop(loop_id))";
    int return_status = sqlite3_exec(db_, sch.c_str(), nullptr, nullptr, &err_msg);
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
    sqlite3* backup_db;
    int return_status = sqlite3_open(file_name_.c_str(), &backup_db);
    if (return_status != SQLITE_OK) {
        std::cerr << "Cannot open backup database: " << sqlite3_errmsg(backup_db) << std::endl;
        return;
    }
    return_status = sqlite3_exec(backup_db, ".backup 'audio_station_loop.db'", nullptr, nullptr, nullptr);
    if (return_status != SQLITE_OK) {
        std::cerr << "Error saving database: " << sqlite3_errmsg(backup_db) << std::endl;
    }
    sqlite3_close(backup_db);
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

int DatabaseManager::callback_get_id(void* single_int, int argc, char** argv, char** colName) {
    int s = *(int*)single_int;
    const char *single = argv[0];
    int sti = std::stoi(single);
    s = sti;
    *(int*)single_int = s;
    return 0;
}

int DatabaseManager::callback_load_tracks(void* file_arr, int argc, char** argv, char** colName) {
    std::vector<std::string>* results = (std::vector<std::string>*)file_arr;

    if (argv[0] == nullptr) {
        return 0;
    }
    for (int i = 0; i < argc; i++) {
        results->push_back(argv[i]);
    }
   return 0;
}
