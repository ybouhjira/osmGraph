#include "osmdialog.h"
#include "ui_osmdialog.h"

OsmDialog::OsmDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::OsmDialog)
{

    _ui->setupUi(this);
    _ui->mainLayout->setSpacing(10);
    setLayout(_ui->mainLayout);

}

OsmDialog::~OsmDialog()
{
    delete _ui;
}

boost::optional<MapRect> OsmDialog::getRect()
{
    OsmDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        return boost::make_optional(
                    std::make_tuple(dialog._ui->topLineEdit->text(),
                                    dialog._ui->rightLineEdit->text(),
                                    dialog._ui->bottomLineEdit->text(),
                                    dialog._ui->leftLineEdit->text()));
    }

    return boost::optional<MapRect>();
}
