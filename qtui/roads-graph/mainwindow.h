#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <boost/graph/adjacency_list.hpp>
#include <QMainWindow>
#include <memory>
#include "googlemaps.h"
#include "webpage.h"

namespace Ui {
    class MainWindow;
}

typedef boost::adjacency_list
        <boost::vecS, boost::vecS, boost::directedS,
        boost::property<boost::vertex_name_t, std::string>,
        boost::property <boost::edge_weight_t, double>> Graph;

typedef std::pair<QString, QString> StringPair;
typedef Graph::edge_descriptor Edge;


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
    void drawGraph();

    void kruskal();
    void dijkstra();

public slots:
    void showOsmDialog();
    void openGraphMl();
    void enableToolBar();

private:
    Ui::MainWindow *_ui;
    QString m_xml;
    Graph m_graph;
    std::unique_ptr<GoogleMaps> m_gmaps;
};

#endif // MAINWINDOW_H
