#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osmdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QWebPage>
#include <QWebFrame>
#include <cstdio>
#include <sstream>
#include <QFileDialog>
#include <QFile>
#include <sstream>
#include <qdebug.h>

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
    connect(_ui->actionGraphML, &QAction::triggered,
            this, &MainWindow::openGraphMl);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

//struct VertexInfo
//{
//    double lat;
//    double lon;
//};

void MainWindow::readGraphML()
{
    typedef boost::adjacency_list<
            boost::vecS,
            boost::vecS,
            boost::undirectedS,
            boost::property<boost::vertex_name_t, std::string>,
            boost::property <boost::edge_weight_t, std::string>> Graph;

    std::istringstream xmlStrStream(m_xml.toStdString());
    Graph graph;
    boost::dynamic_properties dynamicProps;
    dynamicProps.property("weight", boost::get(boost::edge_weight, graph));
    dynamicProps.property("name", boost::get(boost::vertex_name, graph));

    boost::read_graphml(xmlStrStream, graph, dynamicProps);

    std::cout << "Hello " << std::endl;
    auto vertices = boost::vertices(graph);

    std::cout << std::distance(vertices.first, vertices.second)
              << " vertices read."
              << std::endl;
    for (auto it = vertices.first; it != vertices.second; it++) {
        auto pos = boost::get(boost::vertex_name, graph, *it);

        std::ostringstream jsOut;
        jsOut << "insertMarker(" << pos << ")" << std::endl;
        auto js = QString(jsOut.str().c_str());
        std::cout << js.toStdString() << std::endl;
        _ui
            ->webView
            ->page()
            ->mainFrame()
            ->evaluateJavaScript(js);
    }
}


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
        m_xml = strStream.str().c_str();
    }
}


void MainWindow::openGraphMl()
{
    auto path = QFileDialog::getOpenFileName(this, "GraphML", "", "*.graphml");
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_xml = file.readAll().toStdString().c_str();
        readGraphML();

    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier");
    }
}
