#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QSettings>
#include <QVector>
#include <QStringList>
#include <QDateTime>

#define ACCOUNT_INACTIVE 		0
#define ACCOUNT_CONNECTING		1
#define ACCOUNT_ACCEPTED		2
#define ACCOUNT_REJECTED		3
#define ACCOUNT_TIMEOUT			4


class Account : public QObject
{
Q_OBJECT
public:
    Account(QString name);

    ~Account();
    bool save();
    bool load();
    void remove();
    void setState(int state);
    
public:
    QString accId;
    QString accAlias;
    QString accIcon;
    QString accountName;
    QString iaxServer;
    QString username;
    QString password;
    QString callerId;
    QString callerIdNumber;
    QString codec;
    bool registerAccount;
    int state;
    int iaxRegistrationId;  // this id is set by iaxc_register
    
    static int nextAcc;
    
private:
    QString accAliasKey;
    QString accName;
    QString accServer;
    QString accUsername;
    QString accPassword;
    QString accCallerId;
    QString accCallerIdNumber;
    QString accCodec;
    QString accRegisterAccount;

private:

    void setKeys(QSettings * settings);

signals:
    void signalRegistrationInactive(Account* account);
    void signalRegistrationConnecting(Account* account);
    void signalRegistrationAccepted(Account* account);
    void signalRegistrationRejected(Account* account);
    void signalRegistrationTimeout(Account* account);

};

#endif
