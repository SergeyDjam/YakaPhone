#include "account.h"

int Account::nextAcc;

Account::Account(QString name)
    : QObject(0)
{
	//this->setObjectName(name);
	
  	uint time = QDateTime::currentDateTime().toTime_t();
  if(name == NULL)
  {
  	 accId = QString::number(time) + QString::number(Account::nextAcc);
  }
  else
  {
  	 accId = name;
  }
  nextAcc++;
  accAlias = "Default";
  iaxServer = "62.75.184.220";
  username = "";
  password = "";
  callerId = "";
  callerIdNumber = "12345678";
  codec = "ulaw";
  registerAccount= true;
  accIcon = "contact.png";
}


Account::~Account()
{
}


void Account::setKeys(QSettings * settings) {

  // address the account - a new
  accName = "/accounts/"+accId+"/";
  
  // set storage keys
  accAliasKey = accName+"alias";
  accServer = accName+"server";
  accUsername = accName+"username";
  accPassword = accName+"password";
  accCallerId = accName+"callerId";
  accCallerIdNumber = accName+"callerIdNumber";
  accCodec = accName+"codec";
  accRegisterAccount = accName+"registerAccount";
}

bool Account::save()
{
  QSettings* settings = new QSettings();
  setKeys(settings);
  bool writable  = settings->isWritable();
  
  	if(writable)
  	{
	  settings->setValue(accAliasKey, accAlias);
	  settings->setValue(accServer, iaxServer);
	  settings->setValue(accUsername,username);
	  settings->setValue(accPassword,password);
	  settings->setValue(accCallerId,callerId);
	  settings->setValue(accCallerIdNumber,callerIdNumber);
	  settings->setValue(accCodec,codec);
	  settings->setValue(accRegisterAccount,registerAccount);
	}
  
  delete settings;
  
  return writable;
  
}

bool Account::load()
{
  QSettings* settings = new QSettings();
  setKeys(settings);
  
  // default values;
  QString defServer="62.75.184.220";
  QString defUsername="";
  QString defPassword="";
  QString defCallerId="";
  QString defCallerIdNumber="";
  QString defCodec="ulaw";
  
  // read stored data
  accAlias = settings->value(accAliasKey, "Default").toString();
  iaxServer = settings->value(accServer, defServer).toString();
  username = settings->value(accUsername, defUsername).toString();
  password = settings->value(accPassword, defPassword).toString();
  callerId = settings->value(accCallerId, defCallerId).toString();
  callerIdNumber = settings->value(accCallerIdNumber, defCallerIdNumber).toString();
  codec = settings->value(accCodec, defCodec).toString();
  registerAccount = settings->value(accRegisterAccount, true).toBool();
  
  delete settings;
  
  return true;
  
}


void Account::remove()
{
  QSettings* settings = new QSettings();
  setKeys(settings);
  
  settings->remove(accAliasKey);
  settings->remove(accServer);
  settings->remove(accUsername);
  settings->remove(accPassword);
  settings->remove(accCallerId);
  settings->remove(accCallerIdNumber);
  settings->remove(accCodec);
  settings->remove(accRegisterAccount);
  
  delete settings;
}

void Account::setState(int state) {
   this->state = state;
   switch (state) {
	case ACCOUNT_INACTIVE : {
		emit signalRegistrationInactive(this);
		break;
	}
	case ACCOUNT_CONNECTING : {
		emit signalRegistrationConnecting(this);
		break;
	}
	case ACCOUNT_ACCEPTED : {
		emit signalRegistrationAccepted(this);
		break;
	}
	case ACCOUNT_REJECTED: {
		emit signalRegistrationRejected(this);
		break;
	}
	case ACCOUNT_TIMEOUT: {
		emit signalRegistrationTimeout(this);
		break;
	}
   }
}
