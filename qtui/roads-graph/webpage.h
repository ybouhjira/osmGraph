#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QObject>
#include <QWebPage>

class WebPage : public QWebPage
{
public:
    using QWebPage::QWebPage;

protected:
    void javaScriptConsoleMessage(const QString &message,
                                  int lineNumber,
                                  const QString &sourceID) override;
};

#endif // WEBPAGE_H
