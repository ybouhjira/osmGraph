#include "googlemaps.h"
#include <QWebFrame>
#include <cassert>
#include <iostream>

GoogleMaps::GoogleMaps(QWebView *webView)
    : m_webView(webView)
{

}

void GoogleMaps::drawVertex(QString lat, QString lng)
{
    assert(m_webView);

    eval(QString("insertMarker({pos : {lat : %1, lng : %2} })").arg(lat, lng));
}

void GoogleMaps::drawEdge(QString sourceLat,
                          QString sourceLng,
                          QString targetLat,
                          QString targetLng,
                          QString color,
                          size_t weight,
                          size_t opacity)
{
    assert(m_webView);
    assert(opacity <= 1);
    assert(weight != 0);

    QString posObject = "{lat: %1, lng: %2}";
    auto instruction = "insertLine({ \n"
                       "  line : [ %1 , %2 ], \n"
                       "  color : '%3', \n"
                       "  opacity : %4, \n"
                       "  weight : %5 \n"
                       "})";

    eval(QString(instruction).arg(
             posObject.arg(sourceLat, sourceLng),
             posObject.arg(targetLat, targetLng),
             color,
             QString::number(opacity),
             QString::number(weight)));

}

void GoogleMaps::center()
{
    assert(m_webView);
    eval("centerMap()");
}

void GoogleMaps::eval(QString js)
{
    assert(m_webView);

    m_webView
            ->page()
            ->mainFrame()
            ->evaluateJavaScript(js);

    if (LOG_ENABLED)
        std::cout << js.toStdString() << std::endl;
}
