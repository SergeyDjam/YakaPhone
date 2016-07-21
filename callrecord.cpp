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
#include "callrecord.h"

int CallRecord::nextRecord;
QString CallRecord::mailBoxCallerIdNumber = QString::fromUtf8("9252");

CallRecord::CallRecord()
{
  callStartTime = QString::number(QDateTime::currentDateTime().toTime_t());
  this->id = callStartTime+QString::number(CallRecord::nextRecord);
  nextRecord++;
  this->answered = false;
  this->outgoing = true;
  this->incoming = false;
  this->active = false;
  this->callerIdName = QString::fromUtf8("user");
  this->callerIdNumber = QString::fromUtf8("0123456789");
  this->callEndTime = QString::fromUtf8("0");
  this->accountId=QString::fromUtf8("0");
}

CallRecord::~CallRecord()
{
}

void CallRecord::load() {
  load(id);
}

void CallRecord::load(QString id)
{
  this->id = id;
  QSettings*  settings = getSettings();
  
  this->callStartTime = settings->value(callStartTimeKey, QString::fromUtf8("0")).toString();
  this->callerIdName = settings->value(callerIdNameKey, QString::fromUtf8("user")).toString();
  this->callerIdNumber = settings->value(callerIdNumberKey, QString::fromUtf8("12345678")).toString();
  this->outgoing = settings->value(outgoingKey, true).toBool();
  this->answered =  settings->value(answeredKey, true).toBool();
  this->callEndTime = settings->value(callEndTimeKey,QString::fromUtf8("0")).toString();
  this->accountId = settings->value(accountIdKey, QString::fromUtf8("0")).toString();
  delete settings;
}

bool CallRecord::save()
{    
  QSettings*  settings = getSettings();
  
  bool writable = settings->isWritable();
  if(writable)
  {
  settings->setValue(callStartTimeKey, callStartTime);
  settings->setValue(callerIdNameKey, callerIdName);
  settings->setValue(callerIdNumberKey, callerIdNumber);
  settings->setValue(outgoingKey, outgoing);
  settings->setValue(answeredKey, answered);
  settings->setValue(callEndTimeKey, callEndTime);
 settings->setValue(accountIdKey, accountId);
  }
  
  delete settings;
  
  return writable;
  
}

void CallRecord::remove() {
  QSettings*  settings = getSettings();
  
  //QMessageBox::information(NULL, QString::fromUtf8("Yakaphone"), this->id);
  QString recordGroupPrefix = QString::fromUtf8("/callregister/calls/")+this->id+QString::fromUtf8("/");
  settings->remove(recordGroupPrefix);
  /*
  settings->remove(callStartTimeKey);
  settings->remove(callerIdNameKey);
  settings->remove(callerIdNumberKey);
  settings->remove(outgoingKey);
  settings->remove(answeredKey);
  settings->remove(callEndTimeKey);
  settings->remove(accountIdKey);
  */
  delete settings;
}

QSettings * CallRecord::getSettings() {
  
  QSettings* settings = new QSettings();
  QString recordGroupPrefix = QString::fromUtf8("/callregister/calls/")+this->id+QString::fromUtf8("/");
  this->callStartTimeKey = recordGroupPrefix+QString::fromUtf8("callStartTime");
  this->callerIdNameKey = recordGroupPrefix+QString::fromUtf8("callerIdName");
  this->callerIdNumberKey = recordGroupPrefix+QString::fromUtf8("callerIdNumber");
  this->outgoingKey = recordGroupPrefix+QString::fromUtf8("outgoing");
  this->answeredKey = recordGroupPrefix+QString::fromUtf8("answered");
  this->callEndTimeKey = recordGroupPrefix+QString::fromUtf8("callEndTime");
  this->accountIdKey = recordGroupPrefix+QString::fromUtf8("accountId");
  
  return settings;
}

QString CallRecord::getCallerIdName()
{
  return this->callerIdName;
}

QString CallRecord::getCallerIdNumber() {
  return this->callerIdNumber;
}

bool CallRecord::isAnswered() {
  return this->answered;
}

bool CallRecord::isOutgoing() {
  return this->outgoing;
}

bool CallRecord::isIncoming() {
  return this->incoming;
}

bool CallRecord::isActive() {
  return this->active;
}

QString CallRecord::getId() {
  return this->id;
}

void CallRecord::setCallerIdName(QString name) {
  this->callerIdName = name;
}

void CallRecord::setCallerIdNumber(QString number) {
  this->callerIdNumber = number;
}

void CallRecord::setAnswered(bool flag) {
  this->answered = flag;
}

void CallRecord::setActive(bool flag) {
  this->active = flag;
}

void CallRecord::setOutgoing(bool flag) {
  this->outgoing = flag;
}

void CallRecord::setIncoming(bool flag) {
  this->incoming = flag;
}

void CallRecord::setCallStartTime(QString sTime) {
  this->callStartTime = sTime;
}

void CallRecord::setCallEndTime(QString eTime) {
  this->callEndTime = eTime;
}

QString CallRecord::getCallStartTime() {
  return this->callStartTime;
}

QString CallRecord::getCallEndTime() {
  return this->callEndTime;
}

QString CallRecord::getAccountId() {
  return this->accountId;
}

void CallRecord::setAccountId(QString id) {
  this->accountId = id;
}

void CallRecord::setCustomProperty(const QString& name, QString value) {
  customProperty[name] = value;
}

QString CallRecord::getCustomProperty(QString name) 
{
 	QString result = customProperty[name];
	 if (result == NULL)
	 { 
	 	return QString::fromUtf8("false");
	 }
	 else
	 {
	  	return result;
	 }
}

QMap<QString, QString> CallRecord::getCustomPropertyList() {
  return this->customProperty;
}

void CallRecord::setCustomPropertyList(QMap<QString,QString> cp) {
  this->customProperty = cp;
}

CallRecord* CallRecord::copy() {
  CallRecord * record = new CallRecord();
  record->id = id;
  record->setAccountId(getAccountId());
  record->setAnswered(isAnswered());
  record->setCallEndTime(getCallEndTime());
  record->setCallStartTime(getCallStartTime());
  record->setCallerIdName(getCallerIdName());
  record->setCallerIdNumber(getCallerIdNumber());
  record->setCustomPropertyList(getCustomPropertyList());
  record->setOutgoing(isOutgoing());
  return record;
}
