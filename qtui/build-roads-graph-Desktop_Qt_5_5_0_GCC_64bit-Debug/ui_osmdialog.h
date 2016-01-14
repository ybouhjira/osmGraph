/********************************************************************************
** Form generated from reading UI file 'osmdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSMDIALOG_H
#define UI_OSMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OsmDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *topLabel;
    QLineEdit *topLineEdit;
    QLabel *leftLabel;
    QLineEdit *leftLineEdit;
    QLabel *bottomLabel;
    QLineEdit *bottomLineEdit;
    QLabel *rightLabel;
    QLineEdit *rightLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OsmDialog)
    {
        if (OsmDialog->objectName().isEmpty())
            OsmDialog->setObjectName(QStringLiteral("OsmDialog"));
        OsmDialog->resize(400, 300);
        verticalLayoutWidget = new QWidget(OsmDialog);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 9, 381, 281));
        mainLayout = new QVBoxLayout(verticalLayoutWidget);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        mainLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        topLabel = new QLabel(verticalLayoutWidget);
        topLabel->setObjectName(QStringLiteral("topLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, topLabel);

        topLineEdit = new QLineEdit(verticalLayoutWidget);
        topLineEdit->setObjectName(QStringLiteral("topLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, topLineEdit);

        leftLabel = new QLabel(verticalLayoutWidget);
        leftLabel->setObjectName(QStringLiteral("leftLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, leftLabel);

        leftLineEdit = new QLineEdit(verticalLayoutWidget);
        leftLineEdit->setObjectName(QStringLiteral("leftLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, leftLineEdit);

        bottomLabel = new QLabel(verticalLayoutWidget);
        bottomLabel->setObjectName(QStringLiteral("bottomLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, bottomLabel);

        bottomLineEdit = new QLineEdit(verticalLayoutWidget);
        bottomLineEdit->setObjectName(QStringLiteral("bottomLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, bottomLineEdit);

        rightLabel = new QLabel(verticalLayoutWidget);
        rightLabel->setObjectName(QStringLiteral("rightLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, rightLabel);

        rightLineEdit = new QLineEdit(verticalLayoutWidget);
        rightLineEdit->setObjectName(QStringLiteral("rightLineEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, rightLineEdit);


        mainLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);


        retranslateUi(OsmDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), OsmDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), OsmDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(OsmDialog);
    } // setupUi

    void retranslateUi(QDialog *OsmDialog)
    {
        OsmDialog->setWindowTitle(QApplication::translate("OsmDialog", "Dialog", 0));
        topLabel->setText(QApplication::translate("OsmDialog", "Top", 0));
        leftLabel->setText(QApplication::translate("OsmDialog", "Left", 0));
        bottomLabel->setText(QApplication::translate("OsmDialog", "Bottom", 0));
        rightLabel->setText(QApplication::translate("OsmDialog", "Right", 0));
    } // retranslateUi

};

namespace Ui {
    class OsmDialog: public Ui_OsmDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSMDIALOG_H
