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
#ifndef CALLRECORD_H
#define CALLRECORD_H

#include <QString>
#include <QSettings>
#include <QDateTime>
#include <QMap>
//#include <QMessageBox>

class CallRecord
{
public:
    CallRecord();
    ~CallRecord();
    void load(QString time);
    void load();
    bool save();
    void remove();
    QString getCallerIdName();
    QString getCallerIdNumber();
    bool isAnswered();
    bool isOutgoing();
    bool isIncoming();
    bool isActive();
    void setCallerIdName(QString name);
    void setCallerIdNumber(QString number);
    void setAnswered(bool flag);
    void setOutgoing(bool flag);
    void setIncoming(bool flag);
    void setActive(bool flag);
    void setCallStartTime(QString sTime);
    void setCallEndTime(QString eTime);
    void setAccountId(QString id);
    QString getId();
    QString getCallStartTime();
    QString getCallEndTime();
    QString getAccountId();
    void setCustomProperty(const QString& name, QString value);
    QString getCustomProperty(QString name);
    QMap<QString, QString> getCustomPropertyList();
    void setCustomPropertyList(QMap<QString,QString>);
    CallRecord * copy();
    
protected:
    QString id;
    QString callerIdName;
    QString callerIdNumber;
    QString callStartTime;
    QString callEndTime;
    QString accountId;
    QString recordId;
    bool answered;
    bool outgoing;
    bool incoming;
    bool active;

private:

   QString callerIdNameKey;
   QString callerIdNumberKey;
   QString outgoingKey;
   QString answeredKey;
   QString callStartTimeKey;
   QString callEndTimeKey;
   QString accountIdKey;
   
   QSettings * getSettings();
   
   QMap<QString, QString> customProperty;

public:
   static int nextRecord;
   static QString mailBoxCallerIdNumber;
};

#endif
