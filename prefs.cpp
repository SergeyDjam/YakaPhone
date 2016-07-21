/***************************************************************************
 *   Copyright (C) 2004 by Emil Stoyanov                                   *
 *   emosto@users.sourceforge.net                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "prefs.h"

Prefs::Prefs(QObject *parent, const char *name)
    : QObject(parent)
{
	this->setObjectName(name);
	
  setInputDevice(0);
  setOutputDevice(0);
  setRingDevice(0);
  setFilterFlag(IAXC_FILTER_AAGC|IAXC_FILTER_ECHO|IAXC_FILTER_DENOISE|IAXC_FILTER_CN);
  silenceThreshold = -99; // default
}

Prefs::~Prefs()
{}

void Prefs::loadSettings()
{
  // Load CallRecords although not settings per se!
  loadCallRecords();
  
  loadAccounts();
  
  // load device settings
  loadDeviceSettings();
  
  // load filter settings
  loadFilterSettings();
  
  // load miscellaneous settings
  loadMiscSettings();
  
  // load session settings
  loadSessionSettings();
}

void Prefs::loadAccounts()
{
  QSettings * accSettings = getSettings();
  
  // read stored account names
  accSettings->beginGroup(QString::fromUtf8("accounts"));
  QStringList accs = accSettings->childGroups();
  defaultAccountId = accSettings->value(QString::fromUtf8("defaultAccountId"),QString::fromUtf8("0")).toString();
  delete accSettings;
  
  // prepare the accounts vector
  //accounts.setAutoDelete(true);
  accounts.clear();
  for (QStringList::Iterator it = accs.begin();it!=accs.end(); ++it) {
    // get the name of the next acount
    QString nextAcc = *it;
    // create an instance with that name
    Account *account = new Account(nextAcc);
    // set the values
    account->load();
    // and append the account to the list of accounts
    accounts.append(account);
  }
}

void Prefs::loadCallRecords()
{

  QSettings * callRecordSettings = getSettings();
  
  // read stored account names
  callRecordSettings->beginGroup(QString::fromUtf8("callregister/calls"));
  QStringList callRecordsFromSettings = callRecordSettings->childGroups();
  delete callRecordSettings;
  
  // prepare the accounts vector
  //accounts.setAutoDelete(true);
  this->callrecords.clear();
  int callRecordCounter = 0;
  int maxCallRecords = 20; // This is a fixed value that should actually be configurable!
  //Do reverse recursion such that you remove the oldest records!!
  for (int i = callRecordsFromSettings.size()-1; i >= 0; --i)
  {
  	//QMessageBox::information(NULL, QString::fromUtf8("Yakaphone"), QString::number(i));
    // create an instance with that name
    CallRecord *callRecord = new CallRecord();
    // set the values
    callRecord->load(callRecordsFromSettings.at(i));
    if (callRecord->getCallerIdNumber() == CallRecord::mailBoxCallerIdNumber)
    {
    	// Remove calls to mailbox from list!!!
		callRecord->remove();
	}
	else
	{
	    if(callRecordCounter < maxCallRecords)
	    {
	    	// and append the call record to the list of call records!
	    	this->callrecords.append(callRecord);
	    }
	    else
	    {
	    	callRecord->remove();
	    }
	    callRecordCounter++;		
	}

  }
  /*
  for (QStringList::Iterator it = callRecords.begin();it!=callRecords.end(); ++it) 
  {
    // get the name of the next acount
    QString nextCallRecord = *it;
    // create an instance with that name
    CallRecord *callRecord = new CallRecord();
    // set the values
    callRecord->load(nextCallRecord);
    if(callRecordCounter < maxCallRecords)
    {
    	// and append the call record to the list of call records!
    	callrecords.append(callRecord);
    }
    else
    {
    	callRecord->remove();
    }
    callRecordCounter++;  
  }
  */

}

void Prefs::saveSettings()
{
  // save all accounts
  for (int i=0;i<accounts.count();i++) saveAccount(i);
  // save the defaultAccountId
  QSettings * accSettings = getSettings();
  accSettings->setValue(QString::fromUtf8("/accounts/defaultAccountId"), defaultAccountId);
  delete accSettings;
  // save device settings
  saveDeviceSettings();
  // save filter settings
  saveFilterSettings();
  // save miscellaneous settings
  saveMiscSettings();
}

Account* Prefs::getAccount(int i)
{
  return accounts.at(i);
}

QList<Account*> Prefs:: getAccounts()
{
  return accounts;
}

CallRecord* Prefs::getCallRecord(int i)
{
  return callrecords.at(i);
}

QList<CallRecord*> Prefs:: getCallRecords()
{
  return callrecords;
}

int Prefs::createNewAccount(QString accName)
{
  Account* newAcc = new Account(accName);
  addAccount(newAcc);
  return accounts.count()-1;
}

void Prefs::addAccount(Account* acc)
{
  accounts.append(acc);
}

void Prefs::removeAccount(int accNumber)
{
  Account* acc = accounts.at(accNumber);
  acc->remove();
  accounts.removeAt(accNumber);
}

void Prefs::saveAccount(int accNumber)
{
  Account* acc = accounts.at(accNumber);
  acc->save();

}

QString Prefs::getDefaultAccountId()
{
  return defaultAccountId;
}

void Prefs::setDefaultAccountId(QString df)
{
  defaultAccountId = df;
}

void Prefs::detectDevices()
{

  struct iaxc_audio_device *devices;
  int devNumber,input,output,ring;
  long devCapabilities;
  QString devName;
  int result = iaxc_audio_devices_get(&devices, &devNumber, &input, &output, &ring);
  //fprintf(stderr, "iaxc_aduio_devices_get = %d\n", result);

  inputDeviceNames.clear();
  outputDeviceNames.clear();
  ringDeviceNames.clear();

  for(int i=0; i<devNumber; i++)
  {
    devCapabilities = devices->capabilities;
    devName = QString::fromUtf8(devices->name);
    if(devCapabilities & IAXC_AD_INPUT)  inputDeviceNames.append(devName);
    if(devCapabilities & IAXC_AD_OUTPUT) outputDeviceNames.append(devName);
    if(devCapabilities & IAXC_AD_RING) ringDeviceNames.append(devName);
    devices++;
  }

}

void Prefs::updateDevices()
{
  iaxc_audio_devices_set(inputDevice,outputDevice,ringDevice);
}

void Prefs::loadDeviceSettings()
{
  QSettings * settings = getSettings();
  inputDevice = settings->value(QString::fromUtf8("/devices/inputDevice"),0).toInt();
  outputDevice = settings->value(QString::fromUtf8("/devices/outputDevice"),0).toInt();
  ringDevice = settings->value(QString::fromUtf8("/devices/ringDevice"),0).toInt();
  buzzerRing = settings->value(QString::fromUtf8("/devices/buzzerRing"),false).toBool();
  delete settings;
}

void Prefs::saveDeviceSettings()
{
  QSettings * settings = getSettings();
  settings->setValue(QString::fromUtf8("/devices/inputDevice"), inputDevice);
  settings->setValue(QString::fromUtf8("/devices/outputDevice"), outputDevice);
  settings->setValue(QString::fromUtf8("/devices/ringDevice"), ringDevice);
  settings->setValue(QString::fromUtf8("/devices/buzzerRing"), buzzerRing);
  delete settings;
}

void Prefs::saveMiscSettings()
{
  QSettings * settings = getSettings();
  settings->setValue(QString::fromUtf8("/misc/docked"), docked);
  settings->setValue(QString::fromUtf8("/misc/hiddenOnStartup"), hiddenOnStartup);
  settings->setValue(QString::fromUtf8("/misc/connectedOnStartup"), connectedOnStartup);
  settings->setValue(QString::fromUtf8("/misc/raiseWindow"), raiseWindow);
  settings->setValue(QString::fromUtf8("/misc/maxCalls"), maxCallsNumber);
  settings->setValue(QString::fromUtf8("/misc/ExecuteCommandOnRing"), execCommandOnRing);
  settings->setValue(QString::fromUtf8("/misc/CommandOnRing"), commandOnRing);
  delete settings;
}

void Prefs::loadMiscSettings()
{
  QSettings * settings = getSettings();
  setDocked(settings->value(QString::fromUtf8("/misc/docked"),false).toBool());
  setHiddenOnStartup(settings->value(QString::fromUtf8("/misc/hiddenOnStartup"),false).toBool());
  setConnectedOnStartup(settings->value(QString::fromUtf8("/misc/connectedOnStartup"),true).toBool());
  setRaiseWindow(settings->value(QString::fromUtf8("/misc/raiseWindow"),true).toBool());
  setMaxCalls(settings->value(QString::fromUtf8("/misc/maxCalls"),1).toInt());
  setExecCommandOnRing(settings->value(QString::fromUtf8("/misc/ExecuteCommandOnRing"), false).toBool());
  setCommandOnRing(settings->value(QString::fromUtf8("/misc/CommandOnRing"), QString::fromUtf8("")).toString());
  delete settings;
}

void Prefs::loadFilterSettings()
{
  QSettings * settings = getSettings();
  filterFlag = 0;
  if (settings->value(QString::fromUtf8("/filters/NoiseReduction"),true).toBool()) filterFlag |= IAXC_FILTER_DENOISE;
  if (settings->value(QString::fromUtf8("/filters/AGC"),true).toBool()) filterFlag |= IAXC_FILTER_AGC;
  if (settings->value(QString::fromUtf8("/filters/EchoCancelation"),false).toBool()) filterFlag |= IAXC_FILTER_ECHO;
  if (settings->value(QString::fromUtf8("/filters/CNFrames"),true).toBool()) filterFlag |= IAXC_FILTER_CN;
  if (settings->value(QString::fromUtf8("/filters/AAGC"),true).toBool()) filterFlag |= IAXC_FILTER_AAGC;
  incomingRingMute = settings->value(QString::fromUtf8("/filters/MuteIncomingRing"),false).toBool();
  outgoingRingMute = settings->value(QString::fromUtf8("/filters/MuteOutgoingRing"),false).toBool();
  silenceThreshold = settings->value(QString::fromUtf8("/filters/SilenceThreshold"), -99).toInt();
  delete settings;
}

void Prefs::saveFilterSettings()
{
  QSettings * settings = getSettings();
  settings->setValue(QString::fromUtf8("/filters/NoiseReduction"), (filterFlag & IAXC_FILTER_DENOISE)==IAXC_FILTER_DENOISE);
  settings->setValue(QString::fromUtf8("/filters/AGC"),(filterFlag & IAXC_FILTER_AGC)==IAXC_FILTER_AGC);
  settings->setValue(QString::fromUtf8("/filters/EchoCancelation"),(filterFlag & IAXC_FILTER_ECHO)==IAXC_FILTER_ECHO);
  settings->setValue(QString::fromUtf8("/filters/CNFrames"), (filterFlag & IAXC_FILTER_CN)==IAXC_FILTER_CN);
  settings->setValue(QString::fromUtf8("/filters/AAGC"), (filterFlag & IAXC_FILTER_AAGC)==IAXC_FILTER_AAGC);
  settings->setValue(QString::fromUtf8("/filters/SilenceThreshold"), silenceThreshold);
  settings->setValue(QString::fromUtf8("/filters/MuteIncomingRing"), incomingRingMute);
  settings->setValue(QString::fromUtf8("/filters/MuteOutgoingRing"), outgoingRingMute);
  delete settings;
}

void Prefs::loadSessionSettings() {
  QSettings * settings = getSettings();
  showDialHangupButtons = settings->value(QString::fromUtf8("/session/showDialHangupButtons"), true).toBool();
  showAccountNumberCombos = settings->value(QString::fromUtf8("/session/showAccountNumberCombos"), true).toBool();
  showDialPad = settings->value(QString::fromUtf8("/session/showDialPad"), true).toBool();
  showToolbar = settings->value(QString::fromUtf8("/session/showToolbar"), true).toBool();
  showCallRegister = settings->value(QString::fromUtf8("/session/showCallRegister"), true).toBool();
  preferredLanguage = settings->value(QString::fromUtf8("/session/preferredLanguage"), QLocale::languageToString( QLocale::system().language() ) ).toString();
  
  int winPosX = settings->value(QString::fromUtf8("/session/winPosX"), 20).toInt();
  int winPosY = settings->value(QString::fromUtf8("/session/winPosY"), 20).toInt();
  int widgetTop = settings->value(QString::fromUtf8("/session/widgetGeometryTop"), 20).toInt();
  int widgetBottom = settings->value(QString::fromUtf8("/session/widgetGeometryBottom"), 520).toInt();
  int widgetLeft = settings->value(QString::fromUtf8("/session/widgetGeometryLeft"), 20).toInt();
  int widgetRight = settings->value(QString::fromUtf8("/session/widgetGeometryRight"), 280).toInt();
  
  windowPosition.setX(winPosX);
  windowPosition.setY(winPosY);
  widgetGeometry.setTop(widgetTop);
  widgetGeometry.setBottom(widgetBottom);
  widgetGeometry.setLeft(widgetLeft);
  widgetGeometry.setRight(widgetRight);
  
  delete settings;
}

void Prefs::saveSessionSettings() {
  
  int winPosX = windowPosition.x();
  int winPosY = windowPosition.y();
  int wLeft = widgetGeometry.left();
  int wRight = widgetGeometry.right();
  int wTop = widgetGeometry.top();
  int wBottom = widgetGeometry.bottom();
  
  QSettings * settings = getSettings();
  settings->setValue(QString::fromUtf8("/session/showDialHangupButtons"), showDialHangupButtons);
  settings->setValue(QString::fromUtf8("/session/showAccountNumberCombos"), showAccountNumberCombos);
  settings->setValue(QString::fromUtf8("/session/showDialPad"), showDialPad);
  settings->setValue(QString::fromUtf8("/session/showToolbar"), showToolbar);
  settings->setValue(QString::fromUtf8("/session/showCallRegister"), showCallRegister);
  settings->setValue(QString::fromUtf8("/session/preferredLanguage"), preferredLanguage);
  settings->setValue(QString::fromUtf8("/session/winPosX"), winPosX);
  settings->setValue(QString::fromUtf8("/session/winPosY"), winPosY);
  settings->setValue(QString::fromUtf8("/session/widgetGeometryLeft"), wLeft);
  settings->setValue(QString::fromUtf8("/session/widgetGeometryRight"), wRight);
  settings->setValue(QString::fromUtf8("/session/widgetGeometryTop"), wTop);
  settings->setValue(QString::fromUtf8("/session/widgetGeometryBottom"), wBottom);
  
  delete settings;
}
 
Account *Prefs::findAccountById(QString id)
{
    for (int i = 0; i < accounts.size(); ++i)
    {
    	Account *account = accounts.at(i);
        if (account->accId == id)
        {
            return account;
        }
    }
    return NULL; 
}

QStringList Prefs::getInputDeviceNames()
{
  return inputDeviceNames;
}

QStringList Prefs::getOutputDeviceNames()
{
  return outputDeviceNames;
}

QStringList Prefs::getRingDeviceNames()
{
  return ringDeviceNames;
}

int Prefs::getInputDevice()
{
  return inputDevice;
}

int Prefs::getOutputDevice()
{
  return outputDevice;
}

int Prefs::getRingDevice()
{
  return ringDevice;
}

bool Prefs::getBuzzerRing()
{
  return buzzerRing;
}

void Prefs::setInputDevice(int input)
{
  inputDevice = input;
}

void Prefs::setOutputDevice(int output)
{
  outputDevice = output;
}

void Prefs::setRingDevice(int ring)
{
  ringDevice = ring;
}

void Prefs::setBuzzerRing(bool buzzer)
{
  buzzerRing = buzzer;
}

int Prefs::getFilterFlag()
{
  return filterFlag;
}

void Prefs::setFilterFlag(int flag)
{
  filterFlag = flag;
}

QSettings * Prefs::getSettings()
{
  QSettings * settings = new QSettings();
  return settings;
}

void Prefs::setSilenceThreshold(int level)
{
  silenceThreshold = level;
}

int Prefs::getSilenceThreshold()
{
  return silenceThreshold;
}

bool Prefs::isDocked() {
  return docked;
}

bool Prefs::isHiddenOnStartup(){
  return hiddenOnStartup;
}

bool Prefs::isConnectedOnStartup() {
  return connectedOnStartup;
}

void Prefs::setDocked(bool d){
  docked = d;
}

void Prefs::setHiddenOnStartup(bool h){
  hiddenOnStartup = h;
}

void Prefs::setConnectedOnStartup(bool c) {
  connectedOnStartup = c;
}

void Prefs::setShowAccountNumberCombos(bool c) {
  showAccountNumberCombos = c;
}

void Prefs::setShowDialHangupButtons(bool c) {
  showDialHangupButtons = c;
}

void Prefs::setShowCallRegister(bool c) {
  showCallRegister = c;
}

void Prefs::setShowDialPad(bool c) {
  showDialPad = c;
}

bool Prefs::getShowAccountNumberCombos() {
  return showAccountNumberCombos;
}

bool Prefs::getShowDialHangupButtons() {
  return showDialHangupButtons;
}

bool Prefs::getShowCallRegister() {
  return showCallRegister;
}

bool Prefs::getShowDialPad() {
  return showDialPad;
}

void Prefs::setPreferredLanguage(QString lang) {
  preferredLanguage=lang;
}

QString Prefs::getPreferredLanguage() {
  return preferredLanguage;
}

QPoint Prefs::getWindowPosition() {
  return windowPosition;
}

void Prefs::setWindowPosition(QPoint wPosition) {
  windowPosition = wPosition;
}

QRect Prefs::getWidgetGeometry() {
  return widgetGeometry;
}

void Prefs::setWidgetGeometry(QRect wGeometry) {
  widgetGeometry = wGeometry;
}

int Prefs::getMaxCalls() {
  return maxCallsNumber;
}
void Prefs::setMaxCalls(int max) {
  maxCallsNumber = max;
}
void Prefs::setRaiseWindow(bool raise) {
  raiseWindow = raise;
}

bool Prefs::getRaiseWindow() {
  return raiseWindow;
}

void Prefs::setShowToolbar(bool flag) {
  showToolbar = flag;
}

void Prefs::setExecCommandOnRing(bool flag) {
  execCommandOnRing = flag;
}

void Prefs::setIncomingRingMute(bool flag) {
  incomingRingMute = flag;
}

void Prefs::setOutgoingRingMute(bool flag) {
  outgoingRingMute = flag;
}

void Prefs::setCommandOnRing(QString command) {
  commandOnRing = command;
}

bool Prefs::getShowToolbar() {
  return showToolbar;
}

bool Prefs::getExecCommandOnRing() {
  return execCommandOnRing;
}

bool Prefs::getIncomingRingMute() {
  return incomingRingMute;
}

bool Prefs::getOutgoingRingMute() {
  return outgoingRingMute;
}

QString Prefs::getCommandOnRing() {
  return commandOnRing;
}

