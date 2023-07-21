#include "database/database.h"
#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    // Create an instance of the DatabaseManager
    DatabaseManager data_base("audio_station_loop.db", "database/schema/schema.sql");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // return 0;
}
