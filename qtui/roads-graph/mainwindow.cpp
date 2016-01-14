#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osmdialog.h"
#include <QFile>
#include <QMessageBox>
#include <QWebPage>
#include <QWebFrame>
#include <cstdio>
#include <sstream>

#include <boost/graph/graphml.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    setCentralWidget(_ui->webView);

    QFile html(":/files/index.html");

    if (html.open(QIODevice::ReadOnly | QIODevice::Text))
        _ui->webView->setHtml(html.readAll().toStdString().c_str());
    else
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier");

    connect(_ui->actionZone_OSM, &QAction::triggered,
            this, &MainWindow::showOsmDialog);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

#include <QTextEdit>

void MainWindow::showOsmDialog()
{
    auto rect = OsmDialog::getRect();

    if (rect) {
        std::ostringstream strStream;
        FILE *fp;
        char buffer[100];

        fp = popen("python2 ../../osm2graphml/__init__.py", "r");
        while (fgets(buffer, sizeof(buffer), fp))
            strStream << buffer;

        pclose(fp);
        std::string xml = strStream.str();

        boost::adjacency_list<> graph;
        boost::read_graphml(std::istringstream(xml) ,graph);
    }
}
