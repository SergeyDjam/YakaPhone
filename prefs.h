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
#ifndef PREFS_H
#define PREFS_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QSettings>
#include <QApplication>
#include <QLocale>
#include <QRect>
#include <QPoint>
#include <QMessageBox>

#include <iaxclient.h>
#include <math.h>

#include "account.h"
#include "callrecord.h"

class Prefs : public QObject {
    Q_OBJECT
public:
    Prefs(QObject *parent = 0, const char *name = 0);
    ~Prefs();

    void loadSettings();
    void saveSettings();

	/* callregister/callrecord routines */
    QList<CallRecord*> getCallRecords();
    CallRecord* getCallRecord(int a);
    //CallRecord* getNextCallRecord();
    //CallRecord* getPreviousCallRecord();
    void loadCallRecords();
    
    /* account routines */
    QList<Account*> getAccounts();
    Account* getAccount(int i);
    int createNewAccount(QString accName);
    void addAccount(Account* acc);
    void removeAccount(int accNumber);
    void saveAccount(int accNumber);
    void setDefaultAccountId(QString df);
    QString getDefaultAccountId();
    void loadAccounts();
    
    /* device routines */

    void detectDevices();
    void updateDevices();

    QStringList getInputDeviceNames();
    QStringList getOutputDeviceNames();
    QStringList getRingDeviceNames();
    int getInputDevice();
    int getOutputDevice();
    int getRingDevice();
    bool getBuzzerRing();
    void setInputDevice(int input);
    void setOutputDevice(int output);
    void setRingDevice(int ring);
    void setBuzzerRing(bool buzzer);
    void saveDeviceSettings();
    void loadDeviceSettings();
    void loadFilterSettings();
    void saveFilterSettings();
    void loadMiscSettings();
    void saveMiscSettings();
    void loadSessionSettings();
    void saveSessionSettings();
    int getFilterFlag();
    void setFilterFlag(int flag);
    void setSilenceThreshold(int level);
    int getSilenceThreshold();
    bool isDocked();
    bool isHiddenOnStartup();
    bool isConnectedOnStartup();
    void setDocked(bool d);
    void setHiddenOnStartup(bool hidden);
    void setConnectedOnStartup(bool c);
    void setShowAccountNumberCombos(bool c);
    void setShowDialHangupButtons(bool c);
    void setShowCallRegister(bool c);
    void setShowDialPad(bool c);
    bool getShowAccountNumberCombos();
    bool getShowDialHangupButtons();
    bool getShowCallRegister();
    bool getShowDialPad();
    bool getShowToolbar();
    void setShowToolbar(bool flag);
    void setPreferredLanguage(QString lang);
    QString getPreferredLanguage();
    QPoint getWindowPosition();
    void setWindowPosition(QPoint wPosition);
    QRect getWidgetGeometry();
    void setWidgetGeometry(QRect wGeometry);
    Account * findAccountById(QString id);    
    int getMaxCalls();
    void setMaxCalls(int max);
    void setRaiseWindow(bool raise);
    bool getRaiseWindow();
    void setExecCommandOnRing(bool flag);
    void setCommandOnRing(QString command);
    void setIncomingRingMute(bool flag);
    void setOutgoingRingMute(bool flag);
    bool getExecCommandOnRing();
    QString getCommandOnRing();
    bool getIncomingRingMute();
    bool getOutgoingRingMute();
        
public:

private:
    QString defaultAccountId;
    QList<Account*> accounts;
    QList<CallRecord*> callrecords;
    QStringList inputDeviceNames;
    QStringList outputDeviceNames;
    QStringList ringDeviceNames;
    int inputDevice;
    int outputDevice;
    int ringDevice;
    bool buzzerRing;
    int filterFlag;
    int silenceThreshold;
    bool docked;
    bool hiddenOnStartup;
    bool connectedOnStartup;
    bool showDialHangupButtons;
    bool showAccountNumberCombos;
    bool showCallRegister;
    bool showDialPad;
    bool raiseWindow;
    bool showToolbar;
    bool incomingRingMute;
    bool outgoingRingMute;
    bool execCommandOnRing;
    QString commandOnRing;
    int maxCallsNumber;
    QString preferredLanguage;
    QPoint windowPosition;
    QRect widgetGeometry;
    
private:
    QSettings *getSettings();
};

#endif
