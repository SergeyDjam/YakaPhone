#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{ 
	this->settingsDialog = NULL;
	
	this->mainWindowDraggable = false;
	    
	this->callRecordCounter = 0;
	this->justDroppedForEdit = true;
	this->setFocusPolicy(Qt::StrongFocus);
	
    setupUi(this);
    
	//this->centralwidget->setWindowOpacity(0.5); // Make it semi-transparent!
	//QRegion myMask(0, 0, 100, 100);
	//this->setMask(myMask);
	//this->setWindowOpacity(0.5); // Make it semi-transparent!

    QObject::connect(this->pushButton_Settings, SIGNAL(clicked()), this, SLOT(settings()));
    //QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    
	QObject::connect(this->pushButton_DialAnsDrop, SIGNAL(clicked()), this, SLOT(dial_Ans_Drop()));
	QObject::connect(this->pushButton_HoldResume, SIGNAL(clicked()), this, SLOT(hold_Resume()));
	QObject::connect(this->pushButton_Credit, SIGNAL(clicked()), this, SLOT(credit()));
	
	QObject::connect(this->pushButton_Number_0, SIGNAL(clicked()), this, SLOT(digit0Pressed()));
	QObject::connect(this->pushButton_Number_1, SIGNAL(clicked()), this, SLOT(digit1Pressed()));	
	QObject::connect(this->pushButton_Number_2, SIGNAL(clicked()), this, SLOT(digit2Pressed()));	
	QObject::connect(this->pushButton_Number_3, SIGNAL(clicked()), this, SLOT(digit3Pressed()));	
	QObject::connect(this->pushButton_Number_4, SIGNAL(clicked()), this, SLOT(digit4Pressed()));	
	QObject::connect(this->pushButton_Number_5, SIGNAL(clicked()), this, SLOT(digit5Pressed()));	
	QObject::connect(this->pushButton_Number_6, SIGNAL(clicked()), this, SLOT(digit6Pressed()));	
	QObject::connect(this->pushButton_Number_7, SIGNAL(clicked()), this, SLOT(digit7Pressed()));	
	QObject::connect(this->pushButton_Number_8, SIGNAL(clicked()), this, SLOT(digit8Pressed()));	
	QObject::connect(this->pushButton_Number_9, SIGNAL(clicked()), this, SLOT(digit9Pressed()));
	
	QObject::connect(this->pushButton_Number_Star, SIGNAL(clicked()), this, SLOT(digitStarPressed()));	
	QObject::connect(this->pushButton_Number_Pound, SIGNAL(clicked()), this, SLOT(digitPoundPressed()));
	
	QObject::connect(this->pushButton_Minimize, SIGNAL(clicked()), this, SLOT(showMinimized ()));	
	QObject::connect(this->pushButton_Close, SIGNAL(clicked()), this, SLOT(close()));	
	
	QObject::connect(this->pushButton_CallRecord_Forward, SIGNAL(clicked()), this, SLOT(scrollCallRecord_Forward()));	
	QObject::connect(this->pushButton_CallRecord_Backward, SIGNAL(clicked()), this, SLOT(scrollCallRecord_Backward()));			
     
    QObject::connect(QCoreApplication::instance (), SIGNAL(aboutToQuit()), this, SLOT(stopTheMusic()));

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::displayRingRecordAt(int index)
{
    if (!this->wrapper->isCallActive() )
    {
		if (!this->preferences->getCallRecords().isEmpty())
		{
			//this->statusbar->showMessage("Counter: " + QString::number(this->callRecordCounter));
			
			int callRecsCount = this->preferences->getCallRecords().size();
			if(index < 0)
			{
				// At the beginning of Call Record List!
				this->callRecordCounter = 0;
				this->pushButton_CallRecord_Backward->setEnabled(false);
			}
			else if(index >= callRecsCount)
			{
				// At the End of Call Record List!
				this->callRecordCounter = callRecsCount-1;
				this->pushButton_CallRecord_Forward->setEnabled(false);
			}
			else
			{
				// Somewhere inbetween!!
				this->pushButton_CallRecord_Backward->setEnabled(true);
				this->pushButton_CallRecord_Forward->setEnabled(true);
			}
					
			CallRecord* callRecord = this->preferences->getCallRecord(this->callRecordCounter);
			this->label_TelNumber->setText(callRecord->getCallerIdNumber());
			
			bool ok;
			QDateTime aDateTime = QDateTime::currentDateTime();
			aDateTime.setTime_t( callRecord->getCallStartTime().toUInt(&ok, 10) );
			if(ok)
			{
				QString aDateTimeStr = aDateTime.toString(Qt::LocalDate);
				this->label_TimerText->setText("    " + aDateTimeStr);
			}
		}
		else
		{
			// No call records so do nothing!!!
		}
    }
    else
    {
    	//Still in an active call... ignore!
    }		
}

void MainWindow::digitPressed(char digit)// called by all digit buttons to send dtmf
{	
    if (!this->wrapper->isCallActive() )
    {
    	// Not in any active call so you can edit numbers!
    	QString activeNumber = this->label_TelNumber->text();
    	if(this->justDroppedForEdit)
    	{
    		activeNumber = QString::fromUtf8("");
    		this->justDroppedForEdit = false;
    	}
        activeNumber.append(digit);
        this->label_TelNumber->setText(activeNumber);
    }
    else 
    {
    	/* else we send dtmf with the current call */
	    this->wrapper->send_dtmf(digit);
    }
}

void MainWindow::keyEvent_deletePressed()
{
    if (!this->wrapper->isCallActive() )
    {
    	// Not in any active call so you can edit numbers!
    	QString activeNumber = this->label_TelNumber->text();
        activeNumber.chop(1);
        this->label_TelNumber->setText(activeNumber);
        
        this->justDroppedForEdit = false;
    }	
}

void MainWindow::scrollCallRecord_Backward()
{
	this->displayRingRecordAt(--this->callRecordCounter);
}

void MainWindow::scrollCallRecord_Forward()
{
   this->displayRingRecordAt(++this->callRecordCounter);
}

void MainWindow::digit9Pressed() { this->digitPressed('9'); }
void MainWindow::digit8Pressed() { this->digitPressed('8'); }
void MainWindow::digit7Pressed() { this->digitPressed('7'); }
void MainWindow::digit6Pressed() { this->digitPressed('6'); }
void MainWindow::digit5Pressed() { this->digitPressed('5'); }
void MainWindow::digit4Pressed() { this->digitPressed('4'); }
void MainWindow::digit3Pressed() { this->digitPressed('3'); }
void MainWindow::digit2Pressed() { this->digitPressed('2'); }
void MainWindow::digit1Pressed() { this->digitPressed('1'); }
void MainWindow::digit0Pressed() { this->digitPressed('0'); }
void MainWindow::digitStarPressed() { this->digitPressed('*'); }
void MainWindow::digitPoundPressed() { this->digitPressed('#'); }

void MainWindow::keyPressEvent (QKeyEvent * event)
{
	int keyPressed = event->key();
    switch(keyPressed)
    {
	    case Qt::Key_0: { this->pushButton_Number_0->click(); break;}
	    case Qt::Key_1: { this->pushButton_Number_1->click(); break;}
	    case Qt::Key_2: { this->pushButton_Number_2->click(); break;}
	    case Qt::Key_3: { this->pushButton_Number_3->click(); break;}
	    case Qt::Key_4: { this->pushButton_Number_4->click(); break;}
	    case Qt::Key_5: { this->pushButton_Number_5->click(); break;}
	    case Qt::Key_6: { this->pushButton_Number_6->click(); break;}
	    case Qt::Key_7: { this->pushButton_Number_7->click(); break;}
	    case Qt::Key_8: { this->pushButton_Number_8->click(); break;}
	    case Qt::Key_9: { this->pushButton_Number_9->click(); break;}
	    case Qt::Key_Asterisk: { this->pushButton_Number_Star->click(); break;}
	    case Qt::Key_NumberSign: { this->pushButton_Number_Pound->click(); break;}
	    case Qt::Key_Return: { }
	    case Qt::Key_Enter: { this->pushButton_DialAnsDrop->click(); break;}
	    case Qt::Key_Backspace: { }
	    case Qt::Key_Delete: { this->keyEvent_deletePressed(); break;}
	    case Qt::Key_Up: { this->pushButton_CallRecord_Backward->click(); break;}
	    case Qt::Key_Left: { this->pushButton_CallRecord_Backward->click(); break;}
	    case Qt::Key_Down: { this->pushButton_CallRecord_Forward->click(); break;}
	    case Qt::Key_Right: { this->pushButton_CallRecord_Forward->click(); break;}  
	    //case Qt::Key_F1: { this->keyEvent_upPressed(); break;}	
	    //case Qt::Key_F2: { this->keyEvent_downPressed(); break;}	 	     
	    default: { event->ignore(); break; }
    }		
}

void MainWindow::mouseMoveEvent ( QMouseEvent * event )
{
	//QMessageBox::information(this, "Yakaphone", "mouseMoveEvent");
	if(this->mainWindowDraggable)
	{
		this->move(event->globalX() - 100, event->globalY() - 50);
	}
	else
	{

	}	
}
void MainWindow::mousePressEvent ( QMouseEvent * event )
{
	//QMessageBox::information(this, "Yakaphone", "mousePressEvent");
	//this->statusbar->showMessage(QString::number(event->x()) + " : " + QString::number(event->y()) );
	int globalY = event->y();
	if(globalY < 25)
	{
		this->mainWindowDraggable = true;
	}
	else
	{
		this->mainWindowDraggable = false;
	}
}
void MainWindow::mouseReleaseEvent ( QMouseEvent * event )
{
	//QMessageBox::information(this, "Yakaphone", "mouseReleaseEvent");
	this->mainWindowDraggable = false;
}

void MainWindow::dial_Ans_Drop()
{
    if (!this->wrapper->callSession.count()>0 || !(this->wrapper->callSession[this->wrapper->getSelectedCall()].isActive()) ) /* if we dont have any call then print the chars in the combobox */
    {
    	QString activeNumber = this->label_TelNumber->text();
		this->wrapper->dial(this->defaultAccount->accId, QString::fromUtf8(""), activeNumber);
    }
    else
    {
	    if (this->wrapper->callSession[this->wrapper->getSelectedCall()].isOutgoing())
	    { //if we are in a call then action is hangup
	        this->wrapper->interrupt_call();
	    }
	    else
	    { // if we are receiving call then we are rejecting
	        this->wrapper->reject_call(this->wrapper->getSelectedCall());
	    }	
    }
}

void MainWindow::hold_Resume()
{
    if (this->wrapper->callSession.count() > 0 )
    {
    	QString isHold = this->wrapper->callSession[this->wrapper->getSelectedCall()].getCustomProperty(QString::fromUtf8("isHold"));
    	if(isHold == QString::fromUtf8("true"))
    	{
    		this->wrapper->resume_call(this->wrapper->getSelectedCall());
    	}
    	else
    	{
    		this->wrapper->hold_call(this->wrapper->getSelectedCall());
    	}
    }
}

void MainWindow::credit()
{
	this->wrapper->dial(this->defaultAccount->accId, QString::fromUtf8(""), CallRecord::mailBoxCallerIdNumber);
}

void MainWindow::settings()
{
	if (this->settingsDialog == NULL) 
	{
		this->settingsDialog = new SettingsDialog(this);
		this->settingsDialog->setModal(true);
		
		//Set the values to the current account values!!!
		//QMessageBox::information(this, "Yakaphone", "setAccountSettings()");
		this->settingsDialog->lineEdit_Username->setText(this->defaultAccount->username);
		this->settingsDialog->lineEdit_Password->setText(this->defaultAccount->password);
		this->settingsDialog->lineEdit_ServerName->setText(this->defaultAccount->iaxServer);
		
		//QMessageBox::information(this, "Yakaphone", "setCodecSettings()");
		this->settingsDialog->setCodecByName(this->defaultAccount->codec);							
		
		//QMessageBox::information(this, "Yakaphone", "setDeviceSettings()");
		this->preferences->detectDevices();
		QStringList outputDeviceNames = this->preferences->getOutputDeviceNames();
		this->settingsDialog->comboBox_AudioOutput->addItems(outputDeviceNames);
		this->settingsDialog->comboBox_AudioOutput->setCurrentIndex(this->preferences->getOutputDevice());
		QStringList inputDeviceNames = this->preferences->getInputDeviceNames();
		this->settingsDialog->comboBox_AudioInput->addItems(inputDeviceNames);
		this->settingsDialog->comboBox_AudioInput->setCurrentIndex(this->preferences->getInputDevice());
		QStringList ringDeviceNames = this->preferences->getRingDeviceNames();
		this->settingsDialog->comboBox_AudioRing->addItems(ringDeviceNames);
		this->settingsDialog->comboBox_AudioRing->setCurrentIndex(this->preferences->getRingDevice());
		this->settingsDialog->checkBox_RingBuzzer->setChecked(this->preferences->getBuzzerRing());
		
		//QMessageBox::information(this, "Yakaphone", "setFilterSettings()");
		int filterFlag = this->preferences->getFilterFlag();
		this->settingsDialog->checkBox_NoiseRecuction->setChecked(filterFlag & IAXC_FILTER_DENOISE);
		this->settingsDialog->checkBox_AnalogAutomaticGainControl->setChecked(filterFlag & IAXC_FILTER_AAGC);
		this->settingsDialog->checkBox_AutomaticGainControl->setChecked(filterFlag & IAXC_FILTER_AGC);
		this->settingsDialog->checkBox_EchoCancellation->setChecked(filterFlag & IAXC_FILTER_ECHO);
		this->settingsDialog->checkBox_ComfortNoise->setChecked(filterFlag & IAXC_FILTER_CN);
		if(this->preferences->getSilenceThreshold() < 0)
		{
			this->settingsDialog->checkBox_SilenceSuppression->setChecked(false);
		}
		else
		{
			this->settingsDialog->checkBox_SilenceSuppression->setChecked(true);
		}
		
		//QMessageBox::information(this, "Yakaphone", "connect...");
    	QObject::connect(this->settingsDialog, SIGNAL(accepted()), this, SLOT(settingsAccepted()));
    	QObject::connect(this->settingsDialog, SIGNAL(rejected()), this, SLOT(settingsRejected()));
	}
	this->settingsDialog->exec();
}

void MainWindow::settingsAccepted()
{
	// Store the set values of the account and save them!
	this->defaultAccount->username = this->settingsDialog->lineEdit_Username->text();
	this->defaultAccount->password = this->settingsDialog->lineEdit_Password->text();
	this->defaultAccount->iaxServer = this->settingsDialog->lineEdit_ServerName->text();
	this->defaultAccount->codec = this->settingsDialog->getSelectedCodecName();

	
	this->preferences->setOutputDevice(this->settingsDialog->comboBox_AudioOutput->currentIndex());
	this->preferences->setInputDevice(this->settingsDialog->comboBox_AudioInput->currentIndex());
	this->preferences->setRingDevice(this->settingsDialog->comboBox_AudioRing->currentIndex());
	this->preferences->setBuzzerRing(this->settingsDialog->checkBox_RingBuzzer->isChecked());
	
	int aFilterFlag = 0;
	if (this->settingsDialog->checkBox_NoiseRecuction->isChecked()) aFilterFlag |= IAXC_FILTER_DENOISE;
	if (this->settingsDialog->checkBox_AnalogAutomaticGainControl->isChecked()) aFilterFlag |= IAXC_FILTER_AAGC;
	if (this->settingsDialog->checkBox_AutomaticGainControl->isChecked()) aFilterFlag |= IAXC_FILTER_AGC;
	if (this->settingsDialog->checkBox_EchoCancellation->isChecked()) aFilterFlag |= IAXC_FILTER_ECHO;
	if (this->settingsDialog->checkBox_ComfortNoise->isChecked()) aFilterFlag |= IAXC_FILTER_CN;
	this->preferences->setFilterFlag(aFilterFlag);
	if (this->settingsDialog->checkBox_SilenceSuppression->isChecked())
	{
		this->preferences->setSilenceThreshold(1);//auto
	}
	else
	{
		this->preferences->setSilenceThreshold(-99);
	}
  	
	this->defaultAccount->save();
	this->preferences->saveSettings();
	
	this->preferences->updateDevices();
	
this->startTheMusic(true, true); // Start the music each time u change settings	

}

void MainWindow::settingsRejected()
{
	
}

void MainWindow::createDefaultAccount()
{
	QString accountName = QString::fromUtf8("Default_Account");
	int accPos = this->preferences->createNewAccount(accountName);
	this->defaultAccount = this->preferences->getAccount(accPos);
	
	//Important to make this default and save it !!!!
	this->preferences->setDefaultAccountId(this->defaultAccount->accId);
	this->preferences->saveAccount(accPos);
	
	this->preferences->saveSettings();
	
	//this->statusbar->showMessage(this->defaultAccount->accId);
}

void MainWindow::initIAX(IaxWrapper* wrp)
{
	this->wrapper = wrp;
	
    connect(this->wrapper , SIGNAL( signalFree() ), this, SLOT( handleIaxWrapperEventFree() ) );
    connect(this->wrapper , SIGNAL( signalBusy(int) ), this, SLOT( handleIaxWrapperEventBusy(int) ) );
    connect(this->wrapper , SIGNAL( signalTransfer(int) ), this, SLOT( handleIaxWrapperEventTransfer(int) ) );	
    connect(this->wrapper , SIGNAL( signalCalling(int, QString) ), this, SLOT( handleIaxWrapperEventCalling(int, QString) ) );
    connect(this->wrapper , SIGNAL( signalAnsweredCall(int) ), this, SLOT( handleIaxWrapperEventAnsweredCall(int) ) );
    connect(this->wrapper , SIGNAL( signalHoldCall(int) ), this, SLOT( handleIaxWrapperEventHoldCall(int) ) );
    connect(this->wrapper , SIGNAL( signalResumedCall(int) ), this, SLOT( handleIaxWrapperEventResumedCall(int) ) );
    connect(this->wrapper , SIGNAL( signalOutgoingRinging(int) ), this, SLOT( handleIaxWrapperEventOutgoingRinging(int) ) );
    connect(this->wrapper , SIGNAL( signalIncomingRinging(int) ), this, SLOT( handleIaxWrapperEventIncomingRinging(int) ) );
    connect(this->wrapper , SIGNAL( signalComplete(int) ), this, SLOT( handleIaxWrapperEventComplete(int) ) );
    connect(this->wrapper , SIGNAL( signalInactive(int) ), this, SLOT( handleIaxWrapperEventInactive(int) ) );
    connect(this->wrapper , SIGNAL( signalRegistered() ), this, SLOT( handleIaxWrapperEventRegistered() ) );
    connect(this->wrapper , SIGNAL( signalUnregistered() ), this, SLOT( handleIaxWrapperEventUnregistered() ) );
    connect(this->wrapper , SIGNAL( signalLevels(int, int, int)), this, SLOT( handleIaxWrapperEventLevels(int,int, int) ) );
    connect(this->wrapper , SIGNAL( signalRegistrationRequested(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationRequested(Account*) ) );
    connect(this->wrapper , SIGNAL( fatalError(QString) ), this, SLOT( fatalError(QString) ));
    //connect(this->wrapper , SIGNAL( signalDebug(const char*) ), this, SLOT( slotLogWindow(const char*) ));
    connect(this->wrapper , SIGNAL(callTime(int, int, int, int ) ), this, SLOT(updateCallDuration(int, int, int, int )) );
   
   	this->preferences = this->wrapper->getSettings();
   	this->preferences->loadSettings(); // Very Important to get everything on and going !!!
   	
   	QString defaultAcctID = this->preferences->getDefaultAccountId();  	
   	this->defaultAccount = this->preferences->findAccountById(defaultAcctID);
   	if(this->defaultAccount == NULL)
   	{
   		this->createDefaultAccount();
   	}
 
    connect(this->defaultAccount , SIGNAL( signalRegistrationInactive(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationInactive(Account*) ) );
    connect(this->defaultAccount , SIGNAL( signalRegistrationConnecting(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationConnecting(Account*) ) );    	
    connect(this->defaultAccount , SIGNAL( signalRegistrationAccepted(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationAccepted(Account*) ) );
    connect(this->defaultAccount , SIGNAL( signalRegistrationRejected(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationRejected(Account*) ) );
    connect(this->defaultAccount , SIGNAL( signalRegistrationTimeout(Account*) ), this, SLOT( handleIaxWrapperEventRegistrationTimeout(Account*) ) );
       	
   	bool initOK = this->wrapper->init_iax_client();
   	
   	this->startTheMusic(initOK, false);
}

void MainWindow::startTheMusic(bool initOK, bool changedSettings)
{   	  	
   	// Just in case no user is set ask for the settings...
   	// This applies especially for the first start!
   	if(this->defaultAccount->username == QString::fromUtf8(""))
   	{
   		this->settings();
   	}
   	
   	if(initOK)
   	{  		
   		this->wrapper->registerWithServer();	
    	//QMessageBox::information(this, "Account:", this->defaultAccount->username ); 		
   	}
   	else
   	{
   		//this->statusbar->showMessage(QString::fromUtf8("init NOT OK"));
   	}	
}

void MainWindow::stopTheMusic()
{
	if(this->wrapper != NULL)
	{
		this->wrapper->unregisterFromServer();
	}
	else
	{
		
	}
}
void MainWindow::fatalError(QString errorMsg)
{
	QString fatalMsg = QString::fromUtf8("Error message:") + errorMsg + QString::fromUtf8("\nApplication will exit!\n");
	QMessageBox::information(this, QString::fromUtf8("Yakaphone"), fatalMsg);
    qFatal(fatalMsg.toUtf8());
    close();
    QApplication::exit();
}
  
void MainWindow::handleIaxWrapperEventRegistered()
{
	this->label_StatusText->setText(QString::fromUtf8("Registered"));
	this->updateUIControlsEnabled(true, false, true, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));			
}

void MainWindow::handleIaxWrapperEventUnregistered()
{
	this->label_StatusText->setText(QString::fromUtf8("Unregistered"));
	this->updateUIControlsEnabled(false, false, false, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventCalling(int callNo, QString status)
{
	this->label_StatusText->setText(QString::fromUtf8("Calling [") + QString::number(callNo) + QString::fromUtf8("]..."));
	//this->label_StatusText->setText(tr("Calling [") + QString::number(callNo) + "]: " + status);
	//this->label_StatusText->setText(tr("Calling..."));
	this->updateUIControlsEnabled(true, false, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));	
}

void MainWindow::handleIaxWrapperEventAnsweredCall(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("AnsweredCall [") + QString::number(callNo) + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(true, true, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Drop_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));	
}

void MainWindow::handleIaxWrapperEventHoldCall(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("HoldCall [") + QString::number(callNo) + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(true, true, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), 	QString::fromUtf8(":/controlPad/Resume_a.png"));		
}

void MainWindow::handleIaxWrapperEventResumedCall(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("ResumedCall [") + QString::number(callNo) + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(true, true, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventLevels(int inValue, int outValue, int threshold)
{
	int norm_inValue_100 = inValue > 100 ? 0 : inValue;
	int norm_outValue_100 = outValue > 100 ? 0 : outValue;
	
	this->progressBar_Mocrophone->setValue(norm_inValue_100);
	this->progressBar_Loudspeaker->setValue(norm_outValue_100);
}
void MainWindow::handleIaxWrapperEventFree()
{
	this->label_StatusText->setText(QString::fromUtf8("Free"));
	this->updateUIControlsEnabled(true, true, false, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventBusy(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("Busy..."));
	this->updateUIControlsEnabled(true, false, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"),  QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventTransfer(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("Transfer"));
//	this->updateUIControlsEnabled(true, false, false, false, false);
//	this->updateUIControlsIcons(":/controlPad/HangUp_a.png", 	":/controlPad/Hold_a.png");		
}
void MainWindow::handleIaxWrapperEventOutgoingRinging(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("Ringing [OUT]"));
	this->updateUIControlsEnabled(true, false, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventIncomingRinging(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("Ringing [IN]"));
	this->label_TelNumber->setText(QString::number(callNo));
	
	this->updateUIControlsEnabled(true, false, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Answer_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));		
}

void MainWindow::handleIaxWrapperEventComplete(int callNo)
{
	this->label_StatusText->setText(QString::fromUtf8("Connected..."));
	this->updateUIControlsEnabled(true, true, false, false);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/HangUp_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));	
}

void MainWindow::handleIaxWrapperEventInactive(int callNo)
{
	this->justDroppedForEdit = true;
	
	this->callRecordCounter = -1;
	this->preferences->loadCallRecords();
	this->pushButton_CallRecord_Backward->setEnabled(true);
	this->pushButton_CallRecord_Forward->setEnabled(true);	
	
	this->label_StatusText->setText(QString::fromUtf8("Dropped [") + QString::number(callNo) + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(true, false, true, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));
}

void MainWindow::handleIaxWrapperEventRegistrationInactive(Account* account)
{
	this->label_StatusText->setText(QString::fromUtf8("Reg. Inactive [") + account->username + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(false, false, false, true);
}

void MainWindow::handleIaxWrapperEventRegistrationConnecting(Account* account)
{
	this->label_StatusText->setText(QString::fromUtf8("Connecting..."));
	this->updateUIControlsEnabled(false, false, false, false);
}
void MainWindow::handleIaxWrapperEventRegistrationRequested(Account* account)
{
	this->label_StatusText->setText(QString::fromUtf8("Logging in..."));
	this->updateUIControlsEnabled(false, false, false, false);
}

void MainWindow::handleIaxWrapperEventRegistrationAccepted(Account* account)
{
    if ((this->wrapper->callSession.count()>0) && 
    	(  (this->wrapper->callSession[this->wrapper->getSelectedCall()].isOutgoing()) || (this->wrapper->callSession[this->wrapper->getSelectedCall()].isIncoming()) )&&
    	(this->wrapper->callSession[this->wrapper->getSelectedCall()].isActive()) 
    	)
    {
    	// You are currently acti
    }
    else
    {
		this->label_StatusText->setText(QString::fromUtf8("Logged In [ ") + account->username + QString::fromUtf8(" ]"));	
		this->updateUIControlsEnabled(true, false, true, true);
		this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));	
    }	

}
void MainWindow::handleIaxWrapperEventRegistrationRejected(Account* account)
{
	this->label_StatusText->setText(QString::fromUtf8("Reg. Rejected [") + account->username + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(false, false, false, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), QString::fromUtf8(":/controlPad/Hold_a.png"));
}
void MainWindow::handleIaxWrapperEventRegistrationTimeout(Account* account)
{
	this->label_StatusText->setText(QString::fromUtf8("Reg. Timeout [") + account->username + QString::fromUtf8("]"));
	this->updateUIControlsEnabled(false, false, false, true);
	this->updateUIControlsIcons(QString::fromUtf8(":/controlPad/Dial_a.png"), 	QString::fromUtf8(":/controlPad/Hold_a.png"));
}
 
void MainWindow::updateCallDuration(int callNo, int hours, int minutes, int seconds)
{
    QString hoursStr = QString::number(hours);
    if (hours<10)
        hoursStr = QString::fromUtf8("0") + hoursStr;
    QString minutesStr = QString::number(minutes);
    if (minutes<10)
        minutesStr = QString::fromUtf8("0") + minutesStr;
    QString secondsStr = QString::number(seconds);
    if (seconds<10)
        secondsStr = QString::fromUtf8("0") + secondsStr;

    this->label_TimerText->setText(hoursStr + QString::fromUtf8(":") + minutesStr + QString::fromUtf8(":") + secondsStr);
}

void MainWindow::updateUIControlsEnabled(bool dialAnsDrop, bool holdResume, bool credit, bool settings)
{
	this->pushButton_DialAnsDrop->setEnabled(dialAnsDrop);
	this->pushButton_HoldResume->setEnabled(holdResume);
	this->pushButton_Credit->setEnabled(credit);
	this->pushButton_Settings->setEnabled(settings);
}

void MainWindow::updateUIControlsIcons(QString dialAnsDrop, QString holdResume)
{
	QIcon dialAnswerDropIcon(dialAnsDrop);
	QIcon holdResumeIcon(holdResume);
	this->pushButton_DialAnsDrop->setIcon(dialAnswerDropIcon);
	this->pushButton_HoldResume->setIcon(holdResumeIcon);

}
