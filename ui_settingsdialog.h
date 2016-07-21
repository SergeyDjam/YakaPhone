#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class Ui_SettingsDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;
    QTabWidget *tabAbout;
    QWidget *tab_Register;
    QLineEdit *lineEdit_Username;
    QLineEdit *lineEdit_Password;
    QLineEdit *lineEdit_ServerName;
    QLabel *label_Username;
    QLabel *label_Password;
    QLabel *label_ServerName;
    QLabel *label_GetAccount;
    QWidget *tab_Audio;
    QComboBox *comboBox_AudioOutput;
    QComboBox *comboBox_AudioInput;
    QCheckBox *checkBox_RingBuzzer;
    QComboBox *comboBox_AudioRing;
    QLabel *label_AudioOutput;
    QLabel *label_AudioInput;
    QLabel *label_AudioRing;
    QWidget *tab_Codec;
    QRadioButton *radioButton_CodecALaw;
    QRadioButton *radioButton_CodecULaw;
    QRadioButton *radioButton_CodecILBC;
    QRadioButton *radioButton_CodecSpeex;
    QRadioButton *radioButton_CodecG723_1;
    QRadioButton *radioButton_CodecGSM;
    QWidget *tab_Filter;
    QCheckBox *checkBox_AutomaticGainControl;
    QCheckBox *checkBox_NoiseRecuction;
    QCheckBox *checkBox_AnalogAutomaticGainControl;
    QCheckBox *checkBox_EchoCancellation;
    QCheckBox *checkBox_ComfortNoise;
    QCheckBox *checkBox_SilenceSuppression;
    QWidget *tab_About;
    QTextEdit *textEdit;

    void setupUi(QDialog *SettingsDialog)
    {
    SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
    SettingsDialog->resize(QSize(408, 171).expandedTo(SettingsDialog->minimumSizeHint()));
    layoutWidget = new QWidget(SettingsDialog);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(320, 20, 77, 106));
    vboxLayout = new QVBoxLayout(layoutWidget);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    vboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    vboxLayout->addWidget(cancelButton);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem);

    tabAbout = new QTabWidget(SettingsDialog);
    tabAbout->setObjectName(QString::fromUtf8("tabAbout"));
    tabAbout->setGeometry(QRect(10, 10, 301, 151));
    tab_Register = new QWidget();
    tab_Register->setObjectName(QString::fromUtf8("tab_Register"));
    lineEdit_Username = new QLineEdit(tab_Register);
    lineEdit_Username->setObjectName(QString::fromUtf8("lineEdit_Username"));
    lineEdit_Username->setGeometry(QRect(100, 10, 151, 20));
    lineEdit_Password = new QLineEdit(tab_Register);
    lineEdit_Password->setObjectName(QString::fromUtf8("lineEdit_Password"));
    lineEdit_Password->setGeometry(QRect(100, 40, 151, 20));
    lineEdit_Password->setEchoMode(QLineEdit::Password);
    lineEdit_ServerName = new QLineEdit(tab_Register);
    lineEdit_ServerName->setObjectName(QString::fromUtf8("lineEdit_ServerName"));
    lineEdit_ServerName->setGeometry(QRect(100, 70, 151, 20));
    label_Username = new QLabel(tab_Register);
    label_Username->setObjectName(QString::fromUtf8("label_Username"));
    label_Username->setGeometry(QRect(10, 14, 61, 21));
    label_Password = new QLabel(tab_Register);
    label_Password->setObjectName(QString::fromUtf8("label_Password"));
    label_Password->setGeometry(QRect(10, 44, 61, 21));
    label_ServerName = new QLabel(tab_Register);
    label_ServerName->setObjectName(QString::fromUtf8("label_ServerName"));
    label_ServerName->setGeometry(QRect(10, 74, 71, 21));
    label_GetAccount = new QLabel(tab_Register);
    label_GetAccount->setObjectName(QString::fromUtf8("label_GetAccount"));
    label_GetAccount->setGeometry(QRect(6, 100, 271, 20));
    tabAbout->addTab(tab_Register, QApplication::translate("SettingsDialog", "Register", 0));
    tab_Audio = new QWidget();
    tab_Audio->setObjectName(QString::fromUtf8("tab_Audio"));
    comboBox_AudioOutput = new QComboBox(tab_Audio);
    comboBox_AudioOutput->setObjectName(QString::fromUtf8("comboBox_AudioOutput"));
    comboBox_AudioOutput->setGeometry(QRect(60, 10, 211, 22));
    comboBox_AudioInput = new QComboBox(tab_Audio);
    comboBox_AudioInput->setObjectName(QString::fromUtf8("comboBox_AudioInput"));
    comboBox_AudioInput->setGeometry(QRect(60, 40, 211, 22));
    checkBox_RingBuzzer = new QCheckBox(tab_Audio);
    checkBox_RingBuzzer->setObjectName(QString::fromUtf8("checkBox_RingBuzzer"));
    checkBox_RingBuzzer->setGeometry(QRect(170, 100, 111, 18));
    comboBox_AudioRing = new QComboBox(tab_Audio);
    comboBox_AudioRing->setObjectName(QString::fromUtf8("comboBox_AudioRing"));
    comboBox_AudioRing->setGeometry(QRect(60, 70, 211, 22));
    label_AudioOutput = new QLabel(tab_Audio);
    label_AudioOutput->setObjectName(QString::fromUtf8("label_AudioOutput"));
    label_AudioOutput->setGeometry(QRect(20, 18, 41, 16));
    label_AudioInput = new QLabel(tab_Audio);
    label_AudioInput->setObjectName(QString::fromUtf8("label_AudioInput"));
    label_AudioInput->setGeometry(QRect(20, 48, 31, 16));
    label_AudioRing = new QLabel(tab_Audio);
    label_AudioRing->setObjectName(QString::fromUtf8("label_AudioRing"));
    label_AudioRing->setGeometry(QRect(20, 78, 31, 16));
    tabAbout->addTab(tab_Audio, QApplication::translate("SettingsDialog", "Audio", 0));
    tab_Codec = new QWidget();
    tab_Codec->setObjectName(QString::fromUtf8("tab_Codec"));
    radioButton_CodecALaw = new QRadioButton(tab_Codec);
    radioButton_CodecALaw->setObjectName(QString::fromUtf8("radioButton_CodecALaw"));
    radioButton_CodecALaw->setGeometry(QRect(110, 30, 61, 18));
    radioButton_CodecALaw->setChecked(false);
    radioButton_CodecULaw = new QRadioButton(tab_Codec);
    radioButton_CodecULaw->setObjectName(QString::fromUtf8("radioButton_CodecULaw"));
    radioButton_CodecULaw->setGeometry(QRect(30, 30, 61, 20));
    radioButton_CodecULaw->setChecked(true);
    radioButton_CodecILBC = new QRadioButton(tab_Codec);
    radioButton_CodecILBC->setObjectName(QString::fromUtf8("radioButton_CodecILBC"));
    radioButton_CodecILBC->setGeometry(QRect(30, 80, 61, 18));
    radioButton_CodecSpeex = new QRadioButton(tab_Codec);
    radioButton_CodecSpeex->setObjectName(QString::fromUtf8("radioButton_CodecSpeex"));
    radioButton_CodecSpeex->setGeometry(QRect(110, 80, 61, 18));
    radioButton_CodecG723_1 = new QRadioButton(tab_Codec);
    radioButton_CodecG723_1->setObjectName(QString::fromUtf8("radioButton_CodecG723_1"));
    radioButton_CodecG723_1->setEnabled(false);
    radioButton_CodecG723_1->setGeometry(QRect(190, 80, 61, 18));
    radioButton_CodecG723_1->setCheckable(true);
    radioButton_CodecGSM = new QRadioButton(tab_Codec);
    radioButton_CodecGSM->setObjectName(QString::fromUtf8("radioButton_CodecGSM"));
    radioButton_CodecGSM->setGeometry(QRect(190, 30, 61, 18));
    tabAbout->addTab(tab_Codec, QApplication::translate("SettingsDialog", "Codec", 0));
    tab_Filter = new QWidget();
    tab_Filter->setObjectName(QString::fromUtf8("tab_Filter"));
    checkBox_AutomaticGainControl = new QCheckBox(tab_Filter);
    checkBox_AutomaticGainControl->setObjectName(QString::fromUtf8("checkBox_AutomaticGainControl"));
    checkBox_AutomaticGainControl->setGeometry(QRect(10, 50, 131, 18));
    checkBox_AutomaticGainControl->setChecked(true);
    checkBox_NoiseRecuction = new QCheckBox(tab_Filter);
    checkBox_NoiseRecuction->setObjectName(QString::fromUtf8("checkBox_NoiseRecuction"));
    checkBox_NoiseRecuction->setGeometry(QRect(10, 20, 111, 18));
    checkBox_NoiseRecuction->setChecked(true);
    checkBox_AnalogAutomaticGainControl = new QCheckBox(tab_Filter);
    checkBox_AnalogAutomaticGainControl->setObjectName(QString::fromUtf8("checkBox_AnalogAutomaticGainControl"));
    checkBox_AnalogAutomaticGainControl->setGeometry(QRect(10, 80, 121, 18));
    checkBox_EchoCancellation = new QCheckBox(tab_Filter);
    checkBox_EchoCancellation->setObjectName(QString::fromUtf8("checkBox_EchoCancellation"));
    checkBox_EchoCancellation->setGeometry(QRect(150, 50, 111, 18));
    checkBox_ComfortNoise = new QCheckBox(tab_Filter);
    checkBox_ComfortNoise->setObjectName(QString::fromUtf8("checkBox_ComfortNoise"));
    checkBox_ComfortNoise->setGeometry(QRect(150, 20, 111, 18));
    checkBox_SilenceSuppression = new QCheckBox(tab_Filter);
    checkBox_SilenceSuppression->setObjectName(QString::fromUtf8("checkBox_SilenceSuppression"));
    checkBox_SilenceSuppression->setGeometry(QRect(150, 80, 121, 20));
    tabAbout->addTab(tab_Filter, QApplication::translate("SettingsDialog", "Filter", 0));
    tab_About = new QWidget();
    tab_About->setObjectName(QString::fromUtf8("tab_About"));
    textEdit = new QTextEdit(tab_About);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setGeometry(QRect(-1, -1, 301, 131));
    tabAbout->addTab(tab_About, QApplication::translate("SettingsDialog", "About", 0));
    label_Username->setBuddy(lineEdit_Username);
    label_Password->setBuddy(lineEdit_Password);
    label_ServerName->setBuddy(lineEdit_ServerName);
    label_AudioOutput->setBuddy(comboBox_AudioOutput);
    label_AudioInput->setBuddy(comboBox_AudioInput);
    label_AudioRing->setBuddy(comboBox_AudioInput);
    retranslateUi(SettingsDialog);

    QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
    SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Settings Dialog", 0));
    okButton->setText(QApplication::translate("SettingsDialog", "OK", 0));
    cancelButton->setText(QApplication::translate("SettingsDialog", "Cancel", 0));
    lineEdit_ServerName->setText(QApplication::translate("SettingsDialog", "62.75.184.220", 0));
    label_Username->setText(QApplication::translate("SettingsDialog", "UserName:", 0));
    label_Password->setText(QApplication::translate("SettingsDialog", "Password:", 0));
    label_ServerName->setText(QApplication::translate("SettingsDialog", "Server Name:", 0));
    label_GetAccount->setText(QApplication::translate("SettingsDialog", "No account yet? Get one at <a href=\"http://www.yakacall.com\">http://www.yakacall.com</a> !", 0));
    tabAbout->setTabText(tabAbout->indexOf(tab_Register), QApplication::translate("SettingsDialog", "Register", 0));
    checkBox_RingBuzzer->setText(QApplication::translate("SettingsDialog", "Ring with Buzzer", 0));
    label_AudioOutput->setText(QApplication::translate("SettingsDialog", "Output:", 0));
    label_AudioInput->setText(QApplication::translate("SettingsDialog", "Input:", 0));
    label_AudioRing->setText(QApplication::translate("SettingsDialog", "Ring:", 0));
    tabAbout->setTabText(tabAbout->indexOf(tab_Audio), QApplication::translate("SettingsDialog", "Audio", 0));
    radioButton_CodecALaw->setText(QApplication::translate("SettingsDialog", "alaw", 0));
    radioButton_CodecULaw->setText(QApplication::translate("SettingsDialog", "ulaw", 0));
    radioButton_CodecILBC->setText(QApplication::translate("SettingsDialog", "ilbc", 0));
    radioButton_CodecSpeex->setText(QApplication::translate("SettingsDialog", "speex", 0));
    radioButton_CodecG723_1->setText(QApplication::translate("SettingsDialog", "G723.1", 0));
    radioButton_CodecGSM->setText(QApplication::translate("SettingsDialog", "gsm", 0));
    tabAbout->setTabText(tabAbout->indexOf(tab_Codec), QApplication::translate("SettingsDialog", "Codec", 0));
    checkBox_AutomaticGainControl->setText(QApplication::translate("SettingsDialog", "Automatic Gain Control", 0));
    checkBox_NoiseRecuction->setText(QApplication::translate("SettingsDialog", "Noise Reduction", 0));
    checkBox_AnalogAutomaticGainControl->setText(QApplication::translate("SettingsDialog", "Analog Gain Control", 0));
    checkBox_EchoCancellation->setText(QApplication::translate("SettingsDialog", "Echo Cancellation", 0));
    checkBox_ComfortNoise->setText(QApplication::translate("SettingsDialog", "Comfort Noise", 0));
    checkBox_SilenceSuppression->setText(QApplication::translate("SettingsDialog", "Silence Suppression", 0));
    tabAbout->setTabText(tabAbout->indexOf(tab_Filter), QApplication::translate("SettingsDialog", "Filter", 0));
    tabAbout->setTabText(tabAbout->indexOf(tab_About), QApplication::translate("SettingsDialog", "About", 0));
    Q_UNUSED(SettingsDialog);
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

#endif // UI_SETTINGSDIALOG_H
