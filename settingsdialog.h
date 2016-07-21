#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <iaxclient.h>

#include "ui_settingsdialog.h"

class SettingsDialog : public QDialog, public Ui::SettingsDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0);

public:    
	void setCodecByName(QString codecName);
	QString getSelectedCodecName();
    
private slots:
	void accept();    
};

#endif // SETTINGSDIALOG_H
