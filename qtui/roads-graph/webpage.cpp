#include "webpage.h"
#include <iostream>

void WebPage::javaScriptConsoleMessage(const QString &message,
                                       int lineNumber,
                                       const QString &sourceID)
{
    std::cerr << message.toStdString()
              << " line(" << lineNumber << ") "
              << " sourceID : "
              << sourceID.toStdString()
              << std::endl;
}
