#ifndef OSMDIALOG_H
#define OSMDIALOG_H

#include <QDialog>
#include <utility>
#include <boost/optional/optional.hpp>

namespace Ui {
    class OsmDialog;
}

typedef std::tuple<QString, QString, QString, QString> MapRect;
class OsmDialog : public QDialog
{
    Q_OBJECT

private:
    explicit OsmDialog(QWidget *parent = 0);

public:
    ~OsmDialog();

    static boost::optional<MapRect> getRect();

private:
    Ui::OsmDialog *_ui;
};

#endif // OSMDIALOG_H
