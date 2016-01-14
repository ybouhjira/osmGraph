#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showOsmDialog();
    void openGraphMl();

private:
    Ui::MainWindow *_ui;
    QString m_xml;
};

#endif // MAINWINDOW_H
