#include "database/database.h"
#include "ui/mainwindow.h"
#include "ui/ui.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    // Create an instance of the DatabaseManager
    DatabaseManager data_base("audio_station_loop.db", "database/schema/schema.sql");
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    UI user_interface(data_base);

    return app.exec();
}
