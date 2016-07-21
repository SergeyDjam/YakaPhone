#include "settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    //QMetaObject::connectSlotsByName(this);
}

void SettingsDialog::accept()
{
	QDialog::accept();
}

QString SettingsDialog::getSelectedCodecName()
{
	QString returnCodec = QString::fromUtf8("ulaw");
	if( this->radioButton_CodecULaw->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecULaw->text();
	}
	else if( this->radioButton_CodecALaw->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecALaw->text();
	}
	else if( this->radioButton_CodecGSM->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecGSM->text();
	}
	else if( this->radioButton_CodecILBC->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecILBC->text();
	}
	else if( this->radioButton_CodecSpeex->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecSpeex->text();
	}
	else if( this->radioButton_CodecG723_1->isChecked() )
	{ 
		returnCodec = this->radioButton_CodecG723_1->text();
	}
	return returnCodec;
}

void SettingsDialog::setCodecByName(QString codecName)
{
	if( codecName == this->radioButton_CodecULaw->text() )
	{ 
		this->radioButton_CodecULaw->setChecked(true);
	}
	else if( codecName == this->radioButton_CodecALaw->text() )
	{ 
		this->radioButton_CodecALaw->setChecked(true);
	}
	else if( codecName == this->radioButton_CodecGSM->text() )
	{ 
		this->radioButton_CodecGSM->setChecked(true);
	}
	else if( codecName == this->radioButton_CodecILBC->text() )
	{ 
		this->radioButton_CodecILBC->setChecked(true);
	}
	else if( codecName == this->radioButton_CodecSpeex->text() )
	{ 
		this->radioButton_CodecSpeex->setChecked(true);
	}
	else if( codecName == this->radioButton_CodecG723_1->text() )
	{ 
		this->radioButton_CodecG723_1->setChecked(true);
	}
}

