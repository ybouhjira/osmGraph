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
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
typedef std::pair<std::string, std::string> StringPair;

StringPair str_coords_from_string(const std::string &str)
{
    QString qstr(str.c_str());
    QStringList parts = qstr.split(",");

    auto it = parts.begin();

    auto lat = it->trimmed().toStdString();
    it++;
    auto lon = it->trimmed().toStdString();

    return std::make_pair(lat, lon);
}


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

    auto vertices = boost::vertices(graph);

    for (auto it = vertices.first; it != vertices.second; it++) {
        auto pos = boost::get(boost::vertex_name, graph, *it);
        double lat, lng;
        std::tie(lat, lng) = coords_from_string(pos);
        std::ostringstream jsOut;
        jsOut << "insertMarker({lat:" << lat <<", " << "lng : " << lng<<"})";
        auto js = QString(jsOut.str().c_str());
        std::cout << js.toStdString() << std::endl;
        _ui
                ->webView
                ->page()
                ->mainFrame()
                ->evaluateJavaScript(js);
    }


    auto edges = boost::edges(graph);

    for (auto it = edges.first; it != edges.second; it++) {
        auto source = boost::source(*it, graph);
        auto target = boost::target(*it, graph);

        auto sourcePos = boost::get(boost::vertex_name, graph, source);
        auto targetPos = boost::get(boost::vertex_name, graph, target);

        auto sourceLatLng = coords_from_string(sourcePos);
        auto targetLatLng = coords_from_string(targetPos);

        std::ostringstream jsOut;
        jsOut << "insertLine("
              << "["
              << "  {lat: " << sourceLatLng.first
              << ", lng: " << sourceLatLng.second << "},"
              << "  {lat: " << targetLatLng.first
              << ", lng: " << targetLatLng.second << "}"
              << "])";
        auto js = QString(jsOut.str().c_str());
        std::cout << js.toStdString() << std::endl;
        _ui
                ->webView
                ->page()
                ->mainFrame()
                ->evaluateJavaScript(js);
    }

    _ui
            ->webView
            ->page()
            ->mainFrame()
            ->evaluateJavaScript("centerMap()");
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

void MainWindow::kruskal()
{
    typedef boost::adjacency_list
            <boost::vecS, boost::vecS, boost::undirectedS,
            boost::property<boost::vertex_name_t, std::string>,
            boost::property <boost::edge_weight_t, double>> Graph;
    typedef Graph::edge_descriptor Edge;

    Graph graph;
    std::istringstream xmlStrStream(m_xml.toStdString());
    boost::dynamic_properties dynamicProps;
    dynamicProps.property("weight", boost::get(boost::edge_weight, graph));
    dynamicProps.property("name", boost::get(boost::vertex_name, graph));

    try {
        boost::read_graphml(xmlStrStream, graph, dynamicProps);
        std::vector<Edge> spanning_tree;

        boost::kruskal_minimum_spanning_tree(graph, std::back_inserter(
                                                 spanning_tree));


        auto vertices = boost::vertices(graph);

        for (auto it = vertices.first; it != vertices.second; it++) {
            auto pos = boost::get(boost::vertex_name, graph, *it);
            double lat, lng;
            std::tie(lat, lng) = coords_from_string(pos);
            std::ostringstream jsOut;
            jsOut << "insertMarker({"
                  << "  lat:" << lat << ", "
                  << "  lng : " << lng
                  << "})";
            auto js = QString(jsOut.str().c_str());
            std::cout << js.toStdString() << std::endl;
            _ui
                ->webView
                ->page()
                ->mainFrame()
                ->evaluateJavaScript(js);
        }


        auto edges = boost::edges(graph);
        std::cout << "count kruskal : "
                  << std::distance(edges.first, edges.second)
                  << std::endl;

        for (auto it = spanning_tree.begin(); it != spanning_tree.end(); it++) {
            auto source = boost::source(*it, graph);
            auto target = boost::target(*it, graph);

            auto sourcePos = boost::get(boost::vertex_name, graph, source);
            auto targetPos = boost::get(boost::vertex_name, graph, target);

            auto sourceLatLng = coords_from_string(sourcePos);
            auto targetLatLng = coords_from_string(targetPos);

            std::ostringstream jsOut;
            jsOut << "insertLine("
                  << "["
                  << "  {lat: " << sourceLatLng.first
                  << ", lng: " << sourceLatLng.second << "},"
                  << "  {lat: " << targetLatLng.first
                  << ", lng: " << targetLatLng.second << "}"
                  << "], 'blue', 5, 1)";
            auto js = QString(jsOut.str().c_str());
            std::cout << js.toStdString() << std::endl;
            _ui
                    ->webView
                    ->page()
                    ->mainFrame()
                    ->evaluateJavaScript(js);
        }

        _ui
                ->webView
                ->page()
                ->mainFrame()
                ->evaluateJavaScript("centerMap()");

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void MainWindow::dijkstra()
{
    QMessageBox::warning(this, "dijkstra", "dijkstra");
    typedef int Weight;
    //    typedef boost::property<boost::edge_weight_t, Weight> WeightProperty;
    //    typedef boost::property<boost::vertex_name_t, std::string> NameProperty;
    //    typedef boost::property<boost::vertex_index_t, int> IndexProperty;
    //    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
    //            NameProperty, WeightProperty > Graph;

    typedef boost::adjacency_list
            <boost::vecS, boost::vecS, boost::undirectedS,
            boost::property<boost::vertex_name_t, std::string>,
            boost::property <boost::edge_weight_t, double>> Graph;

    typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;
    typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap;
//    typedef boost::property_map < Graph, boost::vertex_name_t >::type NameMap;
    typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
    typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;
    Graph g;

    std::istringstream xmlStrStream(m_xml.toStdString());
    boost::dynamic_properties dynamicProps;
    dynamicProps.property("weight", boost::get(boost::edge_weight, g));
    dynamicProps.property("name", boost::get(boost::vertex_name, g));
    boost::read_graphml(xmlStrStream, g, dynamicProps);

    std::vector<Vertex> predecessors(num_vertices(g)); // To store parents
    std::vector<Weight> distances(num_vertices(g)); // To store distances
    IndexMap indexMap = boost::get(boost::vertex_index, g);
    PredecessorMap predecessorMap(&predecessors[0], indexMap);
    DistanceMap distanceMap(&distances[0], indexMap);
    // Compute shortest paths from v0 to all vertices, and store the output in predecessors and distances
    // boost::dijkstra_shortest_paths(g, v0, boost::predecessor_map(predecessorMap).distance_map(distanceMap));
    // This is exactly the same as the above line - it is the idea of "named parameters" - you can pass the
    // prdecessor map and the distance map in any order.
    boost::dijkstra_shortest_paths(g,
                                   *(boost::vertices(g).first),
                                   boost::distance_map(distanceMap).predecessor_map(predecessorMap));

    std::cout << "distances and parents:" << std::endl;
//    NameMap nameMap = boost::get(boost::vertex_name, g);

//    boost::graph_traits < Graph >::vertex_iterator vi, vend;
//    for (std::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi) {
//        std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
//        std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
//                                                                         endl;
//    }
//    std::cout << std::endl;

//    std::ofstream dot_file("figs/dijkstra-eg.dot");

//    dot_file << "digraph D {\n"
//             << "  rankdir=LR\n"
//             << "  size=\"4,3\"\n"
//             << "  ratio=\"fill\"\n"
//             << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    boost::graph_traits < Graph >::edge_iterator ei, ei_end;
    for (std::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
        boost::graph_traits < Graph >::edge_descriptor e = *ei;
        boost::graph_traits < Graph >::vertex_descriptor
                u = boost::source(e, g),
                v = boost::target(e, g);

        auto sourcePos = boost::get(boost::vertex_name, g, u);
        auto targetPos = boost::get(boost::vertex_name, g, u);

        auto sourceLatLng = coords_from_string(sourcePos);
        auto targetLatLng = coords_from_string(targetPos);

        std::ostringstream jsOut;
        jsOut << "insertLine("
              << "["
              << "  {lat: " << sourceLatLng.first
              << ", lng: " << sourceLatLng.second << "},"
              << "  {lat: " << targetLatLng.first
              << ", lng: " << targetLatLng.second << "}"
              << "], 'yellow', 5)";

        auto js = QString(jsOut.str().c_str());

        std::cout << "dijkstra " << js.toStdString() << std::endl;

        _ui
            ->webView
            ->page()
            ->mainFrame()
            ->evaluateJavaScript(js);
    }
}
