#include "yakacallstyle.h"

void YakaCallStyle::polish(QPalette &palette)
{
	QColor yakawhite(255, 255, 255);
	QColor yakared(153, 0, 0);
    QColor yakayellow(227, 177, 0);
    
    palette.setBrush(QPalette::Window, yakawhite);
    palette.setBrush(QPalette::BrightText, Qt::white);
    //palette.setBrush(QPalette::Base, yakayellow);
    palette.setBrush(QPalette::Highlight, yakared);

}

void YakaCallStyle::polish(QWidget *widget)
{
    if (false)
    {

    }
    else if(widget->inherits("QMainWindow"))
    {
    	//int x, y, width, height;
        //option->rect.getRect(&x, &y, &width, &height);      	
		//QRegion mask(0, 0, widget->size().width() - 4, widget->size().height() - 10);
		//widget->setMask(mask);
		
	    QPixmap xpm;
	    xpm.load( QString::fromUtf8(":/application/WindowMask.png"), 0, Qt::ColorOnly | Qt::DiffuseDither | Qt::DiffuseAlphaDither | Qt::PreferDither );
	    widget->setMask(xpm.createHeuristicMask(false));

		QPalette palette;
    	palette.setBrush(widget->backgroundRole(), QBrush(xpm));
    	widget->setPalette(palette);
    	    		  
		//widget->setWindowOpacity(0.9); 	 	
    }  
    else if(widget->objectName() == QString::fromUtf8("centralwidget"))
    {
        QStyle::polish(widget);
    }      
    else
    {
        QStyle::polish(widget);
    }	
}

void YakaCallStyle::unpolish(QWidget *widget)
{
    if (false)
    {

    }
    else if(false)
    {
    	
    }
    else
    {
        QStyle::unpolish(widget);
    }	        
}

int YakaCallStyle::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric)
    {
	    case PM_ProgressBarChunkWidth:
	    {
            return QStyle::pixelMetric(metric, option, widget) / 2;
	    }     
	    default:
	    {
            return QStyle::pixelMetric(metric, option, widget);
	    }
    }
    return QStyle::pixelMetric(metric, option, widget);
}

int YakaCallStyle::styleHint(StyleHint hint, const QStyleOption *option,
                                  const QWidget *widget,
                                  QStyleHintReturn *returnData) const
{
    switch (hint)
    {
	    case SH_WindowFrame_Mask:
	    {
            return QStyle::styleHint(hint, option, widget, returnData);
	    }
	    default:
	    {
            return QStyle::styleHint(hint, option, widget, returnData);
	    }
    }	
    return QStyle::styleHint(hint, option, widget, returnData);
}

void YakaCallStyle::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
	int x, y, width, height;
	int xRnd = 6;
	int yRnd = 6;
    option->rect.getRect(&x, &y, &width, &height);
    
    QColor yakared(153, 0, 0); //Border Color
    QColor yakayellow(227, 177, 0);
	            	
    switch (element)
    {
	    case PE_FrameGroupBox:
	        {      	
	 			if(widget->objectName() == QString::fromUtf8("groupBox_Display"))
			    {	
			        QLinearGradient gradient(x, y, x, y + height);
			        gradient.setColorAt(0.0, yakayellow);
			        gradient.setColorAt(0.7, Qt::white);
			        gradient.setColorAt(1.0, Qt::white);       
			        //gradient.setColorAt(1.0, yakared);	               
		                
		            painter->setPen(QPen(yakared, 1, Qt::SolidLine, Qt::RoundCap,
			                            Qt::RoundJoin));
		            painter->setBrush(gradient);
		            painter->drawRect(x, y, width-2, height-2);
		            
			        QPainterPath textPath;
			        QFont timesFont(QString::fromUtf8("African [Verdana]"), width/10);
			        timesFont.setStyleStrategy(QFont::PreferAntialias);
			        textPath.addText(x + (width/8), y + (height/2), timesFont, QString::fromUtf8("YakaCall"));
			        painter->setPen(yakayellow.light(180));
			        painter->drawPath(textPath);		                			    
			    }			    			        
			    else
			    {
                    QStyle::drawPrimitive(element, option, painter, widget);
			    }	        	          			
				break;
	        }
	    case PE_Frame:
	        {
	 			if(widget->objectName() == QString::fromUtf8("frame_Dialpad"))
			    {	
			    	//QMessageBox::information(QApplication::activeWindow(), "Yakaphone", "frame_Dialpad");    
			        QLinearGradient gradient(x, y, x, y + height);
			        gradient.setColorAt(0.0, Qt::white);
			        gradient.setColorAt(0.7, Qt::white);
			        gradient.setColorAt(1.0, yakared);              
		                
		            painter->setPen(QPen(yakared, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		            painter->setBrush(gradient);
		            painter->drawRect(x, y, width-7, height-9); 
			    }
	 			else if(widget->objectName() == QString::fromUtf8("centralwidget"))
			    {	
			    	//QMessageBox::information(NULL, QString::fromUtf8("Yakaphone"), QString::fromUtf8("centralwidget"));			    
				    QPixmap xpm;
				    xpm.load( QString::fromUtf8(":/application/WindowsMask.png"), 0, Qt::ThresholdDither | Qt::ThresholdAlphaDither | Qt::AvoidDither );
				    painter->drawPixmap(x, y, xpm);
			    }			    		      
			    else
			    {
			    	//QMessageBox::information(NULL, "Yakaphone", widget->objectName());
                    QStyle::drawPrimitive(element, option, painter, widget);
			    }	        	          
	        }        
	        break;        
	    default:
	    {
            QStyle::drawPrimitive(element, option, painter, widget);
	    }
    } 
}

void YakaCallStyle::drawControl(ControlElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
	int x, y, width, height;
    option->rect.getRect(&x, &y, &width, &height);
    QColor yakared(153, 0, 0); //Border Color
    
    switch (element)
    {
	    case CE_ProgressBarGroove:
	        {   
                //QStyle::drawControl(element, option, painter, widget);
				painter->setPen(Qt::black);
				painter->drawRect(x, y, width - 1, height - 1);
	        }      
	        break;
	    case CE_ProgressBarContents:
	        {   
                QStyle::drawControl(element, option, painter, widget);
	        	/*     
		        QLinearGradient gradient(x, y, x + width, y);
		        gradient.setColorAt(0.0, Qt::white);
		        gradient.setColorAt(1.0, yakared);
		        
		        painter->setBrush(gradient);				
				painter->drawRect(x, y, width - 3, height - 3);
				*/
	        }      
	        break;	         
	    default:
            QStyle::drawControl(element, option, painter, widget);
    }
	
    //QStyle::drawControl(element, option, painter, widget);
}

void YakaCallStyle::drawComplexControl( ComplexControl control, const QStyleOptionComplex * option, QPainter * painter, const QWidget * widget) const
{
	/*
    switch (control)
    {
	    case CC_TitleBar:
	        {   
	        	// This applies fo child windows!      	
				//return;
	        }              
	    default:
	    	{
	        
	    	}
    }
    */
    
    QStyle::drawComplexControl(control, option, painter, widget);
}


QPixmap YakaCallStyle::standardPixmap(StandardPixmap standardPixmap, const QStyleOption * option, const QWidget * widget) const
{
	/*
    switch (standardPixmap) 
    {
	    case SP_TitleBarCloseButton:
	        {         
				
	        }
	        break;
	    case SP_TitleBarMinButton:
	        {          
				// Leave this empty so nothing will be drawn... default looks very ugly ;-)
	        }        
	        break;
	    default:
            return QStyle::standardPixmap(standardPixmap, option, widget);
    }
    */
   return QStyle::standardPixmap(standardPixmap, option, widget);
}
