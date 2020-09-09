/********************************************************************************
** Form generated from reading UI file 'qualityfactordialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUALITYFACTORDIALOG_H
#define UI_QUALITYFACTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QualityFactorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QualityFactorDialog)
    {
        if (QualityFactorDialog->objectName().isEmpty())
            QualityFactorDialog->setObjectName(QString::fromUtf8("QualityFactorDialog"));
        QualityFactorDialog->resize(375, 174);
        verticalLayout = new QVBoxLayout(QualityFactorDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(QualityFactorDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSlider = new QSlider(QualityFactorDialog);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setValue(100);
        horizontalSlider->setSliderPosition(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksBelow);

        horizontalLayout->addWidget(horizontalSlider);

        spinBox = new QSpinBox(QualityFactorDialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setEnabled(false);
        spinBox->setMaximum(100);
        spinBox->setValue(100);

        horizontalLayout->addWidget(spinBox);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 89, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(QualityFactorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QualityFactorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), QualityFactorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QualityFactorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(QualityFactorDialog);
    } // setupUi

    void retranslateUi(QDialog *QualityFactorDialog)
    {
        QualityFactorDialog->setWindowTitle(QCoreApplication::translate("QualityFactorDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("QualityFactorDialog", "Quality factor:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QualityFactorDialog: public Ui_QualityFactorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUALITYFACTORDIALOG_H
