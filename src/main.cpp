#include "database/database.h"

int main() {
    // Create an instance of the DatabaseManager
    DatabaseManager data_base("audio_station_loop.db", "database/schema/schema.sql");
    return 0;
}
