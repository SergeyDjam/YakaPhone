#ifndef IAXWRAPPEREVENT_H
#define IAXWRAPPEREVENT_H

#include <QEvent>
#include <iaxclient.h>

//#define IaxWrapperEventType (QEvent::User + 999)
//enum {IaxWrapperEventType = QEvent::User + 999};

class IaxWrapperEvent : public QEvent
{

public:
    IaxWrapperEvent(iaxc_event e);

    ~IaxWrapperEvent();

public:
    iaxc_event iaxClientEvent;

  	//virtual Type type();     
    
};

#endif
