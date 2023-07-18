#include "database/database.h"
#include "ui/ui.h"

int main() {
    // Create an instance of the DatabaseManager
    DatabaseManager data_base("audio_station_loop.db", "database/schema/schema.sql");
    UI user_interface(data_base);

    return 0;
}
