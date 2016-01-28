#include <boost/graph/graphml.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osmdialog.h"
#include "webpage.h"
#include "googlemaps.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QWebFrame>
#include <QWebPage>
#include <sstream>
#include <QObject>
#include <cstdio>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    setCentralWidget(_ui->webView);
    m_gmaps.reset(new GoogleMaps(_ui->webView));

    QFile htmlFile(":/files/index.html");

    if (htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        _ui
                ->webView
                ->page()
                ->mainFrame()
                ->setHtml(QString::fromUtf8(htmlFile.readAll()));
    else
        QMessageBox::warning(this, "Error", "Coudln't open index.html");

    // Connect Signals & slots
    connect(_ui->webView, &QWebView::loadFinished,
            this, &MainWindow::enableToolBar);
    connect(_ui->actionZone_OSM, &QAction::triggered,
            this, &MainWindow::showOsmDialog);
    connect(_ui->actionGraphML, &QAction::triggered,
            this, &MainWindow::openGraphMl);
    connect(_ui->actionKruskal, &QAction::triggered,
            this, &MainWindow::kruskal);
    connect(_ui->actionDijkstra, &QAction::triggered,
            this, &MainWindow::dijkstra);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

std::pair<double, double> coords_from_string(const std::string &str)
{
    QString qstr(str.c_str());
    QStringList parts = qstr.split(",");

    auto it = parts.begin();

    double lat = it->trimmed().toDouble();
    it++;
    double lon = it->trimmed().toDouble();

    return std::make_pair(lat, lon);
}


StringPair str_coords_from_string(const std::string &str)
{
    QString qstr(str.c_str());
    QStringList parts = qstr.split(",");

    auto it = parts.begin();

    auto lat = it->trimmed();
    it++;
    auto lon = it->trimmed();

    return std::make_pair(lat, lon);
}


void MainWindow::readGraphML()
{

}

void MainWindow::drawGraph()
{
    auto vertices = boost::vertices(m_graph);

    for (auto it = vertices.first; it != vertices.second; it++) {
        auto name = boost::get(boost::vertex_name, m_graph, *it);
        auto pos = str_coords_from_string(name);
        m_gmaps->drawVertex(pos.first, pos.second);
    }

    auto edges = boost::edges(m_graph);
    for (auto it = edges.first; it != edges.second; it++) {
        auto source = str_coords_from_string(
                          boost::get(boost::vertex_name,
                                     m_graph,
                                     boost::source(*it, m_graph)));

        auto target = str_coords_from_string(
                          boost::get(boost::vertex_name,
                                     m_graph,
                                     boost::target(*it, m_graph)));

        m_gmaps->drawEdge(source.first,
                         source.second,
                         target.first,
                         target.second,
                         "brown",
                         2,
                         0.5);
    }

    m_gmaps->center();
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
        auto xml = QString::fromUtf8(file.readAll());

        std::istringstream xmlStrStream(xml.toStdString());
        boost::dynamic_properties dynamicProps;
        dynamicProps.property("weight", boost::get(boost::edge_weight, m_graph));
        dynamicProps.property("name", boost::get(boost::vertex_name, m_graph));

        boost::read_graphml(xmlStrStream, m_graph, dynamicProps);
        drawGraph();

    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier");
    }
}

void MainWindow::enableToolBar()
{
    _ui->actionGraphML->setEnabled(true);
    _ui->actionDijkstra->setEnabled(true);
    _ui->actionKruskal->setEnabled(true);
    _ui->actionZone_OSM->setEnabled(true);
}

void MainWindow::kruskal()
{
    try {
        // do kruskal
        std::vector<Edge> spanning_tree;
        boost::kruskal_minimum_spanning_tree(m_graph,
                                             std::back_inserter(spanning_tree));

        // draw edges
        for (auto it = spanning_tree.begin(); it != spanning_tree.end(); it++) {
            auto source = str_coords_from_string(
                              boost::get(boost::vertex_name,
                                         m_graph,
                                         boost::source(*it, m_graph)));
            auto target = str_coords_from_string(
                              boost::get(boost::vertex_name,
                                         m_graph,
                                         boost::target(*it, m_graph)));

            m_gmaps->drawEdge(source.first,
                             source.second,
                             target.first,
                             target.second,
                             "darkblue",
                             5);
        }

        m_gmaps->center();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void MainWindow::dijkstra()
{
    // TODO :
    //    typedef int Weight;

    //    // TODO : move from here
    //    typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;
    //    typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap;
    //    typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& >
    //            PredecessorMap;
    //    typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& >
    //            DistanceMap;



    //    std::vector<Vertex> predecessors(num_vertices(m_graph)); // To store parents
    //    std::vector<Weight> distances(num_vertices(m_graph)); // To store distances
    //    IndexMap indexMap = boost::get(boost::vertex_index, m_graph);
    //    PredecessorMap predecessorMap(&predecessors[0], indexMap);
    //    DistanceMap distanceMap(&distances[0], indexMap);
    //    boost::dijkstra_shortest_paths(m_graph,
    //                                   *(boost::vertices(m_graph).first),
    //                                   boost::distance_map(distanceMap)
    //                                   .predecessor_map(predecessorMap));

    //    std::cout << "distances and parents:" << std::endl;

    //    boost::graph_traits < Graph >::edge_iterator ei, ei_end;
    //    for (std::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei) {
    //        boost::graph_traits < Graph >::edge_descriptor e = *ei;
    //        boost::graph_traits < Graph >::vertex_descriptor
    //                u = boost::source(e, m_graph),
    //                v = boost::target(e, m_graph);

    //        auto sourceName = boost::get(boost::vertex_name, m_graph, u);
    //        auto targetName = boost::get(boost::vertex_name, m_graph, v);

    //        auto source = coords_from_string(sourceName);
    //        auto target = coords_from_string(targetName);

    //        GoogleMaps(_ui->webView).drawEdge(source.first,
    //                                          source.second,
    //                                          target.first,
    //                                          target.second,
    //                                          "brown",
    //                                          5);
    //    }
}
