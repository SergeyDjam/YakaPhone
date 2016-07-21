#include "mainwindow.h"
#include "yakacallstyle.h"

#include <QApplication>

int main(int argc, char *argv[])
{	
 	
    QApplication app(argc, argv);
    
    QCoreApplication::setOrganizationName("Yaka Software");
    QCoreApplication::setOrganizationDomain("yakasoftware.com");
    QCoreApplication::setApplicationName("YakaPhone"); 
    
    //QApplication::setColorSpec(QApplication::ManyColor);  
    //QApplication::setStyle(new YakaCallStyle);
    
    MainWindow *mainWindow  = new MainWindow;
    IaxWrapper* wrp = new IaxWrapper();
	
    mainWindow->show();
	mainWindow->initIAX(wrp);
	    
    return app.exec();

}
