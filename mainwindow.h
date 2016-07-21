#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QCoreApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QBitmap>

#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "iaxwrapper.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    
private:
	bool mainWindowDraggable;

	int callRecordCounter; // Use when flipping through call records
	void displayRingRecordAt(int index);
	bool justDroppedForEdit;
	SettingsDialog* settingsDialog;
	IaxWrapper* wrapper;
	Prefs * preferences;
	Account* defaultAccount;
	void createDefaultAccount();
	void writeSettings();
	
	void digitPressed(char digit); // called by all digit buttons to send dtmf
	void keyEvent_deletePressed();
	
	void updateUIControlsEnabled(bool dialAnsDrop, bool holdResume, bool credit , bool settings);
	//void updateUIControlsIcons(const char* dialAnsDrop, const char* holdResume);
	void updateUIControlsIcons(QString dialAnsDrop, QString holdResume);
	
protected:
	virtual void keyPressEvent (QKeyEvent * event);
	virtual void mouseMoveEvent ( QMouseEvent * event ); 
	virtual void mousePressEvent ( QMouseEvent * event ); 
	virtual void mouseReleaseEvent ( QMouseEvent * event ); 
	
public:	
	void initIAX(IaxWrapper* wrp);
    
private slots:
	void	digit0Pressed();
	void	digit1Pressed();
	void	digit2Pressed();
	void	digit3Pressed();
	void	digit4Pressed();
	void	digit5Pressed();
	void	digit6Pressed();
	void	digit7Pressed();
	void	digit8Pressed();
	void	digit9Pressed();
	void	digitPoundPressed(); 
	void	digitStarPressed();
	
	void dial_Ans_Drop();
	void hold_Resume();
	void credit();
	
	void scrollCallRecord_Backward();
	void scrollCallRecord_Forward();		
	
	void settings();
	void settingsAccepted(); 
	void settingsRejected();
	
	// Get down to telephony business
	void startTheMusic(bool initOK, bool changedSettings);
	// Run down, unregister and do general cleanup!
	void stopTheMusic();
	
protected slots:
  void fatalError(QString errorMsg);  	
	
public slots:
  /* Client init, registration and destruction */
  virtual void          handleIaxWrapperEventRegistered();
  virtual void          handleIaxWrapperEventUnregistered();
  
  /* Events generated from Wrapper */
  virtual void          handleIaxWrapperEventCalling(int callNumber, QString status);
  virtual void          handleIaxWrapperEventAnsweredCall(int callNo);
  virtual void          handleIaxWrapperEventHoldCall(int callNo);
  virtual void          handleIaxWrapperEventResumedCall(int callNo);
  
  /* Audio and mic levels */
  virtual void          handleIaxWrapperEventLevels(int inValue, int outValue, int threshold);
  
  /* Call states */
  virtual void          handleIaxWrapperEventFree();
  virtual void          handleIaxWrapperEventBusy(int callNo);
  virtual void          handleIaxWrapperEventTransfer(int callNo);
  virtual void          handleIaxWrapperEventOutgoingRinging(int callNo);
  virtual void          handleIaxWrapperEventIncomingRinging(int callNo);
  virtual void          handleIaxWrapperEventComplete(int callNo);
  virtual void          handleIaxWrapperEventInactive(int callNo);
  virtual void          handleIaxWrapperEventRegistrationInactive(Account* account);
  virtual void          handleIaxWrapperEventRegistrationConnecting(Account* account);
  virtual void          handleIaxWrapperEventRegistrationRequested(Account* account);
  virtual void          handleIaxWrapperEventRegistrationAccepted(Account* account);
  virtual void          handleIaxWrapperEventRegistrationRejected(Account* account);
  virtual void          handleIaxWrapperEventRegistrationTimeout(Account* account);
  virtual void          updateCallDuration(int callNo, int hours, int minutes, int seconds);
	
};

#endif // MAINWINDOW_H
