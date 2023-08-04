#include "database/database.h"
#include "ui/mainwindow.h"
#include "ui/ui.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    // Create an instance of the DatabaseManager
    DatabaseManager data_base("audio_station_loop.db", "database/schema/schema.sql");
    QApplication app(argc, argv);
    UI user_interface(data_base);

    MainWindow window (user_interface);
    user_interface.load_loop(0);
    window.show();
    return app.exec();
}
