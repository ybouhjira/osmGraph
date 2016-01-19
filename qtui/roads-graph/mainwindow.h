#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "webpage.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * Reads from m_xml and shows it on the map
     * @brief reads the GraphML text
     */
    void readGraphML();

    void kruskal();
    void dijkstra();

public slots:
    void showOsmDialog();
    void openGraphMl();
    void enableToolBar();

private:
    Ui::MainWindow *_ui;
    QString m_xml;
};

#endif // MAINWINDOW_H
