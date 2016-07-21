#ifndef IAXWRAPPER_H_
#define IAXWRAPPER_H_


#include <QObject>
#include <QApplication>
#include <QEvent>
//include <QCustomEvent>
#include <QEvent>
#include <QMutex>
#include <QMap>
#include <QTimer>

//include <iaxclient.h>
#include "iaxclient.h"

#include "iaxwrapperevent.h"
#include "prefs.h"
#include "account.h"
#include "callrecord.h"

// A global callback function used by libiaxclient
int iaxc_callback(iaxc_event e);

class IaxWrapper : public QObject
{
Q_OBJECT

public:
    IaxWrapper(QObject *parent = 0, const char *name = 0);
    ~IaxWrapper();

public slots: 
  /* High-Level GUI related methods */
  virtual void registerWithServer();
  virtual void unregisterFromServer();
  //virtual void dial(QString contactNumber);
  virtual void dial(QString accountId, QString contactName, QString contactNumber);  
  virtual void answer_call(int callNo);
  virtual void hold_call(int callNo);
  virtual void resume_call(int callNo);
  virtual void interrupt_call();
  virtual void reject_call(int callNo);
  virtual void select_call(int callNumber);
  virtual void transfer_call(int callNo, QString number);
  virtual void send_dtmf(char digit);
  virtual void ring(); // called by the timer
  virtual void startRing(); // duration varies based on reason
  virtual void stopRing();

  /* Initialization routines */
  virtual bool init_iax_client();  // called from main win constructor
  virtual void start_iax_client();  // starts iaxclient thread
  virtual void stop_iax_client();  // stops iaxclient thread
  virtual int registerAccount(Account* account); // register account
  virtual int unregisterAccount(Account* account); // unregister account
  virtual void registerMultipleAccounts();
  virtual void configureCall(int accountNumber);
  virtual void shutdown_iax_client(); // called from ~IaxWrapper
  virtual iaxc_sound* initTone(int F1, int F2, int Dur, int Len, int Repeat);
  //virtual void debugStatistics(int callNo, int callInterval);  
  
protected slots:
	void callDuration();
  
signals: 
  /* Signals to notify the GUI */ 
  /* Wrapper events */
  //void signalCalling(QString callerId, QString callerStatus); //used only for ringing out
  void signalFree();
  void signalCalling(int callNumber, QString callerStatus); //used only for ringing out
  void signalAnsweredCall(int callNo);
  void signalHoldCall(int callNo);
  void signalResumedCall(int callNo);
  void signalBusy(int callNo);
  void signalTransfer(int callNo);  
  void signalRegistered();
  void signalUnregistered();
  
  /*Callback events */
  void signalLevels(int inValue, int outValue, int threshold ); // audio and mic levels
  void signalOutgoingRinging(int callNo); // ringing status of the tab created by singalCalling()
  void signalIncomingRinging(int callNo); // create tab and set status rining
  void signalComplete(int callNo); // call established, ready to speak
  void signalInactive(int callNo); // inactive call - hangup, forwarded to signalHangup(QString)
  void signalRegistrationRequested(Account* account);
  //void signalRegistrationAccepted(Account* account);
  //void signalRegistrationRejected(Account* account);
  //void signalRegistrationTimeout(Account* account);  
  void signalTextMessage(int id, int status); // inactive call - hangup, forwarded to   signalHangup(QString)
  void signalTextMessage(QString message);
  void signalDebug(const char * error);
  
  void callTime(int callNo, int hours, int minutes, int seconds); // inform about the time of the call - every second
  void fatalError(QString message);
  
private:
  Prefs * settings;
  struct iaxc_sound* ringInTone;
  struct iaxc_sound* ringOutTone;
  QTimer* ringTimer;
  QTimer* callDurationTimer;
  int * rtt;
  iaxc_netstat * localNetstat;
  iaxc_netstat * remoteNetstat;
  bool showStats;
  char* iaxc_ver;
  
  /* Lower level libiaxclient routines */
  void fatal_error(QString err);
  uint getCodecMaskFromName(QString codecName);
  int findAccountById(QString accountId);
  
protected:
	bool event(QEvent * e);  
  
public:  
  Account* getAccountByRegistrationId(int regId);  
  bool registered;// to be sure we have a person who can receive calls
  QMap<int, CallRecord> callSession; //map callNo to callRecord
  QMutex callSessionMutex;  

  bool isCallActive();
    
  int getSelectedCall();
  QString getIaxClientVersion(); 
  
  virtual void iaxWrapperEvent(IaxWrapperEvent * event); 
  
  /* Event Handling Routines */
  void handleIaxCEvent(iaxc_event e); // event callback
  void event_level(float in, float out); // monitors video and audio signal levels
  void event_registration(int id, int reply, int msgcount);// monitors registration status
  void event_state(int callNo, int state, char *remote, char *remote_name, char *local, char *local_context, int reg_id);
  void event_text(int type, char *message);
  void event_unknown(int type); 
  
  Prefs * getSettings();
  
  CallRecord* createCallRecord(QString accountId, QString name, QString number, bool outgoing);    
  
  void debug(const char* log, ...);
  void setShowStats(bool flag);
  bool getShowStats();  
};

#endif /*IAXWRAPPER_H_*/
