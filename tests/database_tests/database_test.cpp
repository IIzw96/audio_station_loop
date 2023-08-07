#include "../src/database/database.h"
#include <gtest/gtest.h>


class DatabaseManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_manager = new DatabaseManager("test_db.db", "../../src/database/schema/schema.sql");
    }

    void TearDown() override {
        delete db_manager;
        remove_temp_database_file();
    }

    static void remove_temp_database_file() {
        const std::string temp_database_file = "test_db.db";
        std::remove(temp_database_file.c_str());
    }

    DatabaseManager* db_manager;
};

TEST_F(DatabaseManagerTest, insert_data) {
    // Insert data into the Loop table
    bool success = db_manager->insert_data("Loop", "NULL, 'Loop 1', '5:30', 120");
    EXPECT_TRUE(success);

    // Retrieve the inserted data from the Loop table and perform assertions
    std::string retrieved_data = db_manager->retrieve_data("Loop");
    EXPECT_FALSE(retrieved_data.empty());

    // Insert data into the Track table
    success = db_manager->insert_data("Track", "NULL, 1, 'Track 1', 'Type 1', 'track_file_path'");
    EXPECT_TRUE(success);

    // Retrieve the inserted data from the Track table and perform assertions
    retrieved_data = db_manager->retrieve_data("Track");
    EXPECT_FALSE(retrieved_data.empty());
}

TEST_F(DatabaseManagerTest, edit_data) {
    // Edit data in the Loop table
    bool success = db_manager -> insert_data("Loop", "NULL, 'Loop 1', '5:30', 120");
    success = db_manager -> edit_data("Loop", "loop_name='New Loop 1', loop_length='6:00'", "loop_id=1");
    EXPECT_TRUE(success);

    // Retrieve the edited data from the Loop table and perform assertions
    std::string retrieved_data = db_manager -> retrieve_data("Loop");
    EXPECT_FALSE(retrieved_data.empty());


    // Edit data in the Track table
    success = db_manager -> insert_data("Track", "NULL, 1, 'Track 1', 'Type 1', 'track_file_path'");
    success = db_manager -> edit_data("Track", "track_name='New Track 1', track_type='New Type'", "track_id=1");
    EXPECT_TRUE(success);

    // Retrieve the edited data from the Track table and perform assertions
    retrieved_data = db_manager -> retrieve_data("Track");
    EXPECT_FALSE(retrieved_data.empty());
}

TEST_F(DatabaseManagerTest, DeleteData) {
    // Delete data from the Loop table
    bool success = db_manager->delete_data("Loop", "loop_id=1");
    EXPECT_TRUE(success);

    // Retrieve the remaining data from the Loop table and perform assertions
    std::string retrieved_data = db_manager->retrieve_data("Loop");
    EXPECT_TRUE(retrieved_data.empty());

    // Delete data from the Track table
    success = db_manager->delete_data("Track", "track_id=1");
    EXPECT_TRUE(success);

    // Retrieve the remaining data from the Track table and perform assertions
    retrieved_data = db_manager->retrieve_data("Track");
    EXPECT_TRUE(retrieved_data.empty());
}

TEST_F(DatabaseManagerTest, RetrieveData) {
    // Retrieve data from the Loop table
    std::string retrieved_data = db_manager->retrieve_data("Loop");
    EXPECT_TRUE(retrieved_data.empty());

    // Retrieve data from the Track table
    retrieved_data = db_manager->retrieve_data("Track");
    EXPECT_TRUE(retrieved_data.empty());
}

TEST_F(DatabaseManagerTest, CreateLoop) {
    // Creating Loop
    std::string loop_1 = "1";
    int status = db_manager->create_loop(loop_1);
    EXPECT_EQ(0, status);
}

TEST_F(DatabaseManagerTest, InsertIntoColumn) {
    // Inserting column into table
    std::string table = "Loop";
    std::string column = "loop_length";
    std::string data = "'50'";
    int status = db_manager->insert_into_column(table, column, data);
    EXPECT_EQ(0, status);
}

TEST_F(DatabaseManagerTest, FindLoopId) {
    bool success = db_manager -> insert_data("Loop", "NULL, 'Loop 1', '5:30', 120");
    std::string loop_name = "Loop 1";
    int status = db_manager -> find_loop_id(loop_name);
    EXPECT_EQ(1, status);
}

TEST_F(DatabaseManagerTest, ReplaceTrackToLoop) {
    int loop_id = 1;
    std::string track_name = "Track11";
    int status = db_manager -> replace_track_to_loop(loop_id, track_name);
    EXPECT_EQ(0, status);
}

TEST_F(DatabaseManagerTest, LoadTracksByLoop) {
    bool success = db_manager -> insert_data("Loop", "NULL, 'Loop 1', '5:30', 120");
    int loop_id = 1;
    std::string track_name = "Track11";
    int status = db_manager -> replace_track_to_loop(loop_id, track_name);
    std::vector<std::string> file_paths = db_manager -> load_tracks_by_loop(loop_id);
    EXPECT_EQ(file_paths[0], "fake_file_path");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
