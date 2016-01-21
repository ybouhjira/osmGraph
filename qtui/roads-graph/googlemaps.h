#ifndef GOOGLEMAPS_H
#define GOOGLEMAPS_H

#include <QWebView>

class GoogleMaps
{
public:
    GoogleMaps(QWebView *webView);

    void drawVertex(QString lat, QString lng);

    void drawEdge(QString sourceLat,
                  QString sourceLng,
                  QString targetLat,
                  QString tagetLng,
                  QString color = "#f00",
                  size_t weight = 2,
                  size_t opacity = 1);

    void center();

private:
    void eval(QString js);
private:
    static bool const LOG_ENABLED = true;
    QWebView *m_webView;
};

#endif // GOOGLEMAPS_H
