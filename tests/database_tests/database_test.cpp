#include "database.h"
#include <gtest/gtest.h>

class DatabaseManagerTest : public ::testing::Test {
protected:
    void set_up() override {
        db_manager = new DatabaseManager("test_db.db", "../src/database/schema/schema.sql");
    }

    void tear_down() override {
        delete db_manager;
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
    bool success = db_manager->edit_data("Loop", "loop_name='New Loop 1', loop_length='6:00'", "loop_id=1");
    EXPECT_TRUE(success);

    // Retrieve the edited data from the Loop table and perform assertions
    std::string retrieved_data = db_manager->retrieve_data("Loop");
    EXPECT_FALSE(retrieved_data.empty());


    // Edit data in the Track table
    success = db_manager->edit_data("Track", "track_name='New Track 1', track_type='New Type'", "track_id=1");
    EXPECT_TRUE(success);

    // Retrieve the edited data from the Track table and perform assertions
    retrieved_data = db_manager->retrieve_data("Track");
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
    EXPECT_FALSE(retrieved_data.empty());

    // Retrieve data from the Track table
    retrieved_data = db_manager->retrieve_data("Track");
    EXPECT_FALSE(retrieved_data.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
