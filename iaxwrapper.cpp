#include "iaxwrapper.h"

#include <QMessageBox>

IaxWrapper* callbackCaller;

/*
	Global method used as callback function in libiaxclient
	Forwards incoming events to void IaxWrapper::handleIaxCEvent(void* callbackCaller, iaxc_event e) via customEvent(QCustomEvent)
*/

int iaxc_callback(iaxc_event e)
{
  // wrap iaxc_event into a QEvent
  IaxWrapperEvent * wrapperEvent = new IaxWrapperEvent(e);
  
  QApplication::postEvent(callbackCaller, wrapperEvent);
  return 1;
}

/*
	Initialization and destruction of the wrapper
*/

IaxWrapper::IaxWrapper(QObject *parent, const char *name)
    : QObject(parent)
{
	this->setObjectName(name);
	
  this->showStats = false;
  this->registered=false; // we are not ready to call
  callbackCaller = this; // to use it in the class callback function

  //load the settings prior to do anything else
  this->settings = new Prefs(this,0);
  this->settings->loadSettings();
  
  /* allocate memory for the statistics vars*/
  this->rtt = (int*) malloc(sizeof(int));
  this->localNetstat = (struct iaxc_netstat*) malloc(sizeof(struct iaxc_netstat));
  this->remoteNetstat = (struct iaxc_netstat*) malloc(sizeof(struct iaxc_netstat));
  
  this->ringInTone = NULL;
  this->ringTimer = new QTimer(); // repeats ringing every 4 seconds

  this->callDurationTimer = new QTimer(); // shows duration of the time every second

  connect(ringTimer, SIGNAL(timeout()), this, SLOT(ring()));
  connect(callDurationTimer, SIGNAL(timeout()), this, SLOT(callDuration()));
  
  char* version = (char*)malloc(256);
  this->iaxc_ver = iaxc_version(version);
  
  //debug("Using IAXClient ver. %s\n", iaxc_ver);

}

IaxWrapper::~IaxWrapper()
{
  this->shutdown_iax_client();
}

Prefs* IaxWrapper::getSettings()
{
  return this->settings;
}

/*
	High-level Methods interfacing the wrapped libiaxclient functions
*/

void IaxWrapper::registerWithServer()
{

  // start the action
  this->start_iax_client();
  this->registerMultipleAccounts();

  /* 
     Initially the idea was to enable controls for dialing only if we have Reigstration
     Accepted, but letting the people use the controls even if not registered is OK as we may
     have such servers that don't allow/require/need registration but allow calling.
  */
  this->registered = true;
  // and tell the GUI to enable "unregister" button or other
  //emit signalRegistered();
}

int IaxWrapper::registerAccount(Account* account) {
   //const char* func = "IaxWrapper::registerMultipleAccounts()";
   // ("%s registering accounts..\n", func);

    //const char* accName = (account->accAlias).toUtf8();
    char* uname = qstrdup( (account->username).toUtf8() );
    char* passwd = qstrdup( (account->password).toUtf8() );
    char* hst = qstrdup( (account->iaxServer).toUtf8() );
    
    if ( (account->registerAccount) && (account->iaxServer != ""))
    {
      // tell to an Asterisk server where we are
      account->iaxRegistrationId = iaxc_register(uname, passwd, hst);
      // ("%s register %s [%s:xxx@%s], given ID = %d\n",  func, accName, uname, hst, account->iaxRegistrationId);
      emit signalRegistrationRequested(account);
    }
    else
    {
      // ("%s skipped registration %s [%s:xxx@%s]\n", func, accName,uname, hst);
      account->iaxRegistrationId = -1; //skipped
    }
    return account->iaxRegistrationId;
}

int IaxWrapper::unregisterAccount(Account* account) {
	int result = -1;
	if (account->iaxRegistrationId != -1)
	{
		result = iaxc_unregister(account->iaxRegistrationId);
		//emit signalUnregistrationRequested(account);
		account->setState(ACCOUNT_INACTIVE);
	}
	return result;
}

void IaxWrapper::registerMultipleAccounts()
{
  // loop through all accounts and call register

  for (int i=0; i<settings->getAccounts().count(); i++)
  {
    Account * account = settings->getAccount(i);
    this->registerAccount(account);
  }
}

void IaxWrapper::unregisterFromServer()
{
 for (int i=0; i<settings->getAccounts().count(); i++) {
 	Account* account = settings->getAccount(i);
	if ((account->registerAccount) &&(account->state != ACCOUNT_INACTIVE))
	{
		unregisterAccount(account);
		account->disconnect(SIGNAL(signalRegistrationAccepted(Account*)));
		account->disconnect(SIGNAL(signalRegistrationConnecting(Account*)));
		account->disconnect(SIGNAL(signalRegistrationInactive(Account*)));
		account->disconnect(SIGNAL(signalRegistrationRejected(Account*)));
		account->disconnect(SIGNAL(signalRegistrationTimeout(Account*)));
	}
 }
  this->stop_iax_client();
  this->registered=false;
	emit signalUnregistered();
}

int IaxWrapper::findAccountById(QString accountId)
{
  int accNumber =0;
  // locate the number of the account
  for (int i=0; i<settings->getAccounts().count(); i++)
    if (accountId==settings->getAccount(i)->accId)
    {
      accNumber=i;
      break;
    }
  return accNumber;
}

void IaxWrapper::dial(QString accountId, QString contactName, QString contactNumber)
{

  CallRecord * callRecord = createCallRecord(accountId, contactName, contactNumber, true);
  int accountNumber = findAccountById(callRecord->getAccountId());

  Account* account = settings->getAccount(accountNumber);
  QString uname = account->username;
  QString passwd = account->password;
  QString hst = account->iaxServer;

  if (this->registered) // if user is allowed to dial
  {
    QString full_url = uname+QString::fromUtf8(":")+passwd+QString::fromUtf8("@")+hst+QString::fromUtf8("/")+callRecord->getCallerIdNumber();
    const char *dest = "";
    configureCall(accountNumber);
    if (hst!=QString::fromUtf8(""))
    {
      dest = full_url.toUtf8();
    }
    else
    {
      dest = (callRecord->getCallerIdNumber()).toUtf8();
    }
    // first, emit signal to notify the GUI //
    // then call our friend
    //debug( "IaxWrapper::dial(int, QString) dialing %s:%s@%s/%s ..\n", (const  char*)uname, "xxx", (const char*) hst, (const char*)callRecord->getCallerIdNumber());
    iaxc_call(qstrdup(dest));
    callSession[getSelectedCall()] = *callRecord;
    emit signalCalling(getSelectedCall(), QString::fromUtf8("Calling..."));
  }
}

/*
	Methods interfacing directly with libiaxclient
*/

bool IaxWrapper::init_iax_client()
{
  //int result = iaxc_initialize(AUDIO_INTERNAL,getSettings()->getMaxCalls());
  int result = iaxc_initialize(getSettings()->getMaxCalls());
  //debug( "IaxWrapper::iaxc_initialize() result = %d\n", result);
  if (result == -1)
  {
    // hopefully not many people will have to read this..
    this->fatal_error(QString::fromUtf8("cannot initialize iaxclient!\n"));
    return false;
  }
  iaxc_set_formats(IAXC_FORMAT_GSM,IAXC_FORMAT_GSM|IAXC_FORMAT_SPEEX|IAXC_FORMAT_ALAW|IAXC_FORMAT_ULAW|IAXC_FORMAT_ILBC);
  return true;
}

void IaxWrapper::shutdown_iax_client()
{
  // shutdown
  //debug( "IaxWrapper::shutdown_iax_client() shutting down..\n");
  iaxc_shutdown();
  //debug( "IaxWrapper::shutdown_iax_client() done.\n");
}

void IaxWrapper::start_iax_client()
{
  // we want iaxevents to be passed to iaxc_callback
  iaxc_set_event_callback(iaxc_callback);
  // fire
  //debug( "IaxWrapper::start_iax_client() starting processing thread..\n");
  int result = iaxc_start_processing_thread();
  //debug( "IaxWrapper::iaxc_start_processing_thread() result = %d\n", result);
  if (result == -1)
    this->fatal_error(QString::fromUtf8("cannot start processing thread!\n"));
}

void IaxWrapper::stop_iax_client()
{
  // close any active calls
  iaxc_dump_all_calls();
  // wait a bit
  iaxc_millisleep(1000);
  // stop the thread
  iaxc_stop_processing_thread();
  // reset type of call
}

void IaxWrapper::send_dtmf(char digit)
{
  // allow sending dtmf only if we are in a state of a call (ACTIVE)
  // TODO:  this is a cheap version - has to be changed
  if (this->registered)
  {
    //printf ("IaxWrapper::send_dtmf(char digit) sending DTMF%c\n", digit);
    // I like playing with these voice menus
    iaxc_send_dtmf(digit);
  }
}

void IaxWrapper::answer_call(int callNumber)
{
  QString func = QString::fromUtf8("IaxWrapper::answer_call()");
  // answer the call
  iaxc_answer_call(callNumber);
  // (func+" answered call  %d\n", getSelectedCall());
  // notify GUI
  emit signalAnsweredCall(callNumber);
}

void IaxWrapper::transfer_call(int callNo, QString number)
{
  QString func = QString::fromUtf8("IaxWrapper::transfer_call(")+number+QString::fromUtf8(")");
  // answer the call
  const char* exten = number.toUtf8();
  iaxc_blind_transfer_call(callNo, qstrdup(exten) );
  // (func+" transferred call  %d\n", callNo);
}

void IaxWrapper::select_call(int callNumber)
{
  QString func = QString::fromUtf8("IaxWrapper::select_call(")+QString::number(callNumber)+QString::fromUtf8(")");
  //debug(func,"\n");
  iaxc_select_call(callNumber);
  // (func+" selected call  %d\n", getSelectedCall());
}

void IaxWrapper::hold_call(int callNumber)
{
  QString func = QString::fromUtf8("IaxWrapper::hold_call()");
  // answer the call
  iaxc_quelch(callNumber, 0);
  // (func+" hold call  %d\n", callNumber);
  // notify GUI
  CallRecord& record = callSession[callNumber];
  record.setCustomProperty(QString::fromUtf8("isHold"),QString::fromUtf8("true"));
  emit signalHoldCall(callNumber);
}

void IaxWrapper::resume_call(int callNumber)
{
  QString func = QString::fromUtf8("IaxWrapper::resume_call()");
  // answer the call
  iaxc_unquelch(callNumber);
  //KiaxCallRecord * record = callSession.at(getSelectedCall());
  CallRecord& record = callSession[callNumber];
  record.setCustomProperty(QString::fromUtf8("isHold"), QString::fromUtf8("false"));
  //debug (func+" resume call  %d\n", callNumber);
  // notify GUI
  emit signalResumedCall(callNumber);
}

void IaxWrapper::reject_call(int callNo)
{
  QString func = QString::fromUtf8("IaxWrapper::reject_call()");
  //debug(func+"  rejecting call %d..\n", callNo);
  iaxc_reject_call_number(callNo);
  //debug(func+"  rejected call %d\n", callNo);
}

void IaxWrapper::interrupt_call()
{
  //printf("IaxWrapper::interrupt_call() hanging up and exiting..\n");
  // hang up the call
  iaxc_dump_call();
  // wait a bit
  iaxc_millisleep(1000);
}

int IaxWrapper::getSelectedCall()
{
  return iaxc_selected_call();
}

Account* IaxWrapper::getAccountByRegistrationId(int regId) {
	int number = settings->getAccounts().count();
	for (int i=0;i<number;i++)
	{
		Account* account = settings->getAccount(i);
		if (account->iaxRegistrationId==regId)
			return account;
	}
	return NULL;
}

// Overwridden function to capture user( IAX ) events
bool IaxWrapper::event(QEvent * e)
{
    if (e->type() == QEvent::User)
    {
        IaxWrapperEvent *wrapperEvent = static_cast<IaxWrapperEvent*>(e);
        this->handleIaxCEvent(wrapperEvent->iaxClientEvent);
        return true;
    }
    else
    {
        return false;
    }
}

/*
     Procedures handling iaxclient events (forwarded from iaxc_callback(iaxc_event e)
*/
void IaxWrapper::handleIaxCEvent(iaxc_event e)
{
  switch(e.type)
  {
	  case IAXC_EVENT_LEVELS:
	    // monitor input and output signal levels (audio)
	    event_level(e.ev.levels.input, e.ev.levels.output);
	    break;
	  case IAXC_EVENT_REGISTRATION:
	    // monitor registration status
	    event_registration(e.ev.reg.id, e.ev.reg.reply, e.ev.reg.msgcount);
	    break;
	  case IAXC_EVENT_TEXT:
	    event_text(e.ev.text.type, e.ev.text.message);
	    break;
	  case IAXC_EVENT_STATE:
	    event_state(e.ev.call.callNo, e.ev.call.state, e.ev.call.remote,
	                e.ev.call.remote_name, e.ev.call.local,
	                e.ev.call.local_context, e.ev.reg.id);
	    break;
	  default:
	    event_unknown(e.type);
	    break;
  }
}

void IaxWrapper::event_level(float in, float out)
{
  emit signalLevels((int)-in, (int) -out, (int) 0);
}

void IaxWrapper::event_registration(int id, int reply, int msgcount)
{
  //debug("Registration id = %d, reply = %d, msgcount = %d\n", id, reply, msgcount);
  Account * account = getAccountByRegistrationId(id);
  if (account!=NULL)
  {
	  switch (reply)
	  {
		case IAXC_REGISTRATION_REPLY_ACK :
		{
			//account->setRegistrationAccepted(true);
			//emit signalRegistrationAccepted(account);
			account->setState(ACCOUNT_ACCEPTED);
			break;
		}
		case IAXC_REGISTRATION_REPLY_REJ :
		{
			//account->setRegistrationAccepted(false);
			//emit signalRegistrationRejected(account);
			account->setState(ACCOUNT_REJECTED);
			break;
		}
		case IAXC_REGISTRATION_REPLY_TIMEOUT :
		{
			//emit signalRegistrationTimeout(account);
			account->setState(ACCOUNT_TIMEOUT);
			break;
		}
	  }
  }
}

void IaxWrapper::event_state(int callNo, int state, char *remote, char *remote_name,
                             char *local, char *local_context, int reg_id)
{
  
  QString func = "IaxWrapper::event_state()";
  //debug (func+" processing IAX event..\n");

  /* Call state masks */
  bool free   = state & IAXC_CALL_STATE_FREE;
  bool active   = state & IAXC_CALL_STATE_ACTIVE;
  bool outgoing = state & IAXC_CALL_STATE_OUTGOING;
  bool ringing  = state & IAXC_CALL_STATE_RINGING;
  bool complete = state & IAXC_CALL_STATE_COMPLETE;
  bool selected = state & IAXC_CALL_STATE_SELECTED;
  bool busy = state & IAXC_CALL_STATE_BUSY;
  bool transfer = state & IAXC_CALL_STATE_TRANSFER;

  //debug (func+" call state %d : active=%d, outgoing=%d, ringing=%d, complete=%d, selected=%d registrationId=%d\n", callNo, active, outgoing, ringing, complete, selected, reg_id);

  // remove the pass (when we are in outgoing call, it is visible)
  QString remoteStr = QString(remote_name);
  // store here remote_name with suppressed pass
  QString modifiedStr = QString::fromUtf8("");
  int passStart = remoteStr.indexOf(QString::fromUtf8(":"), 0, Qt::CaseInsensitive) + 1;
  int passEnd = remoteStr.indexOf(QString::fromUtf8("@"), 0, Qt::CaseInsensitive);
  int passLength = passEnd-passStart;
  if (passStart>0)
  {
    modifiedStr = remoteStr.replace(passStart,passLength,QString::fromUtf8("xxx"));
  }

  //debug (func+" call info %d : remote=%s, remote_name=%s, local=%s, local_context=%s\n", callNo, remote, (const char*)modifiedStr, local, local_context);
  QString remoteContext(remote);
  QString localName(local);
  QString localContext(local_context);
  QString remoteName(remote_name);

  QString remoteExten = remote;

  if (active)
  { // there is a call progress
    //ougoing calls
    if ((outgoing) && ringing)
    {
      //debug(func+" ACTIVE_OUTGOING_RINGING %d : %s, reigstrationId = %d\n",callNo, (const char *) remoteExten, reg_id);
      // we are making the call so we have already created a record
      CallRecord& record = callSession[callNo];
      record.setOutgoing(true);
      record.setIncoming(false);
      record.setActive(true);
      emit signalOutgoingRinging(callNo);
    }
    // incoming calls
    if ((!outgoing)&& ringing)
    {
      //debug(func+" ACTIVE_INCOMING_RINGING %d: %s, registrationId = %d\n",callNo, (const char *) remoteExten, reg_id);
      CallRecord * record = createCallRecord(settings->getDefaultAccountId(), remoteName, remoteExten, false);
      callSession[callNo] = *record;
      int accountNumber = findAccountById(record->getAccountId());
      configureCall(accountNumber);
		
      record->setOutgoing(false);
      record->setIncoming(true);		
	  record->setActive(true);
      emit signalIncomingRinging(callNo);
    }
    // incoming and outgoing, but active
    if (complete && &selected)
    {
      //debug(func+" ACTIVE_CALL_ESTABLISHED %d: %s\n",callNo, (const char *) remoteExten);
      // we have already answered the call so we have a created record
      CallRecord& record = callSession[callNo];

      if (!record.isAnswered())
      {
        record.setCallStartTime(QString::number(QDateTime::currentDateTime().toTime_t()));
      }
      if (!callDurationTimer->isActive())
      {
        callDurationTimer->start(1000); // notify every second
      }
      record.setAnswered(true);
      record.setActive(true);
      emit signalComplete(callNo);
    }         
  }
  else
  { 
  	// there is no call progress, hangup
    //debug(func+" INACTIVE %d\n", callNo);
    CallRecord& record = callSession[callNo];
    if (callSession.count()==1) //stop the timer only if this is the only call
      callDurationTimer->stop();
    if (record.isAnswered()) record.setCallEndTime(QString::number(QDateTime::currentDateTime().toTime_t()));
    	record.save();
    	
	record.setActive(false);    	
    emit signalInactive(callNo);
      
  }
	// Added by RND for extended states
	if (free)
	{
	  emit signalFree();
	}
	if (busy)
	{
	  emit signalBusy(callNo);
	} 
	if (transfer)
	{
	  emit signalTransfer(callNo);
	}
}

void IaxWrapper::event_text(int type, char *message)
{
  if (message)
  {
    //debug( "IaxWrapper::event_text() Message: Type=%d Message=%s\n", type, message);
    switch(type)
    {
	    case IAXC_TEXT_TYPE_STATUS:
	      emit signalTextMessage(message);
	      break;
    }
  }
}

void IaxWrapper::event_unknown(int type)
{
  //debug( "IaxWrapper::event_unknown() Uknown message: Type=%d\n", type);
}

void IaxWrapper::fatal_error(QString err)
{
  emit fatalError(err);
}

/*
	Implements a customEvent handler for posted messages
	from iaxc_callback. This solves the problem of asynchronous
	access to the GUI thread.
*/

void IaxWrapper::iaxWrapperEvent(IaxWrapperEvent * event)
{
  handleIaxCEvent(event->iaxClientEvent);
}


CallRecord* IaxWrapper::createCallRecord(QString accountId, QString name, QString number, bool outgoing)
{
  CallRecord * record = new CallRecord();
  record->setAccountId(accountId);
  record->setCallerIdName(name);
  record->setCallerIdNumber(number);
  record->setOutgoing(outgoing);
  record->setAnswered(false);
  return record;
}

/*
  Private methods
*/
uint IaxWrapper::getCodecMaskFromName(QString codecName)
{

  if (codecName == QString::fromUtf8("ilbc"))
    return IAXC_FORMAT_ILBC;
  if (codecName==QString::fromUtf8("gsm"))
    return IAXC_FORMAT_GSM;
  if (codecName==QString::fromUtf8("alaw"))
    return IAXC_FORMAT_ALAW;    
  if (codecName==QString::fromUtf8("ulaw"))
    return IAXC_FORMAT_ULAW;
  if (codecName==QString::fromUtf8("speex"))
    return IAXC_FORMAT_SPEEX;
  // if none of them  for some reason...
  return IAXC_FORMAT_ULAW;
}

void IaxWrapper::configureCall(int accountNumber)
{
  QString func = QString::fromUtf8("IaxWrapper::configureCall(acc number ") + QString::number(accountNumber) + QString::fromUtf8(")");
  //debug( func+" configuring call..\n");
  Account * account = settings->getAccount(accountNumber);
  //debug( func+" account number=%d, account alias=%s\n",accountNumber,(const char*)account->accAlias);
  // get Caller ID info
  char* c_callerId = qstrdup( (account->callerId).toUtf8() );
  //debug( func+" callerId=%s\n",c_callerId);
  char * c_callerIdNumber = qstrdup( (account->callerIdNumber).toUtf8() );
  //debug( func+" callerIdNumber=%s\n",c_callerIdNumber);
  iaxc_set_callerid(c_callerId, c_callerIdNumber);
  // get preferred codec - default ILBC
  iaxc_set_formats( getCodecMaskFromName(account->codec), IAXC_FORMAT_GSM|IAXC_FORMAT_SPEEX|IAXC_FORMAT_ULAW|IAXC_FORMAT_ILBC);
  //debug( func+" codec=%s\n",(const char*)account->codec);
  // turn on filters
  int flag = settings->getFilterFlag();
  //debug( func+" filter flag=%d\n",flag);
  iaxc_set_filters(flag);
  // set silence threshold (1-auto, 0 - mute)
  iaxc_set_silence_threshold(settings->getSilenceThreshold());
  //debug( func+" silence threshold=%d\n",settings->getSilenceThreshold());
  // set up audio devices
  iaxc_audio_devices_set(settings->getInputDevice(),settings->getOutputDevice(), settings->getRingDevice());
  //debug( func+" devices in=%d, out=%d, ring=%d\n",settings->getInputDevice(), settings->getOutputDevice(), settings->getRingDevice());
  iaxc_set_audio_output(0); //umute output (if muted for some reason)
}

/*
   Portions of tone generation code from iaxComm's Ringer Class 
   (c) Michael Van Donselaar
*/
iaxc_sound* IaxWrapper::initTone(int F1, int F2, int Dur, int Len, int Repeat)
{
  iaxc_sound* tone;
  tone = (iaxc_sound *)malloc(sizeof(iaxc_sound));
  tone->channel = 1;
  tone->len  = Len;
  tone->data = (short *)calloc(tone->len , sizeof(short));


  for( int i=0;i < Dur; i++ )
  {
    tone->data[i] = (short)(0x7fff*0.4*sin((double)i*F1*M_PI/8000))
                   + (short)(0x7fff*0.4*sin((double)i*F2*M_PI/8000));
  }

  tone->repeat = Repeat;
  return tone;

}

void IaxWrapper::ring()
{
 	ringInTone = initTone(1900, 2400, 400,500,20);
	iaxc_play_sound(ringInTone, 1);
}

void IaxWrapper::startRing()
{
  ring(); // start ringing, the timer will wait and then ring again
  ringTimer->start();
}

void IaxWrapper::stopRing()
{
  /* In case there was no ring signal.. we have to check*/
  if (ringInTone!= NULL) {
  	iaxc_stop_sound(ringInTone->id);
	delete ringInTone;
  }
  ringTimer->stop();
  ringInTone=NULL;
}

void IaxWrapper::callDuration()
{
  QMutexLocker locker(&callSessionMutex); /* to synchronize access to callSession */
  int callsNumber = callSession.count();
  for (int i=0;i<callsNumber;i++)
  {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int callNumber = callSession.keys()[i];
    int callStartTime=strtol( ( callSession[callNumber].getCallStartTime() ).toUtf8(),0,10);
    uint currentTime = QDateTime::currentDateTime().toTime_t();
    uint interval = currentTime - callStartTime;
    div_t hours_calc = div(interval, 3600);
    hours = hours_calc.quot;
    div_t minutes_calc = div(hours_calc.rem, 60);
    minutes = minutes_calc.quot;
    seconds = minutes_calc.rem;
    //debugStatistics(callNumber, interval);
    if (callSession[callNumber].isAnswered()) /* display time only if the call was answered */
      emit callTime(callNumber, hours, minutes, seconds);
  }
}


bool IaxWrapper::getShowStats()
{
  return showStats;
}

void IaxWrapper::setShowStats(bool flag)
{
  showStats = flag;
}

bool IaxWrapper::isCallActive()
{
    if ( (this->callSession.count()>0) && ( this->callSession[this->getSelectedCall()].isActive()) )
    {
    	return true;
    }
    else 
    {
    	return false;
    }
}

QString IaxWrapper::getIaxClientVersion()
{
 return QString(iaxc_ver);
}
