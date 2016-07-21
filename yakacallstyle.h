#ifndef YAKACALLSTLYE_H
#define YAKACALLSTLYE_H

#include <QMainWindow>
//include <QPlastiqueStyle>
#include <QStyle>
//include <QWindowsXPStyle>
#include <QStyleOption>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QPainter>
#include <QPainterPath>
#include <QBitmap>
#include <QMessageBox>

class YakaCallStyle : public QStyle
{
    Q_OBJECT

public:
    YakaCallStyle() {}
    ~YakaCallStyle() {}
    
public:
    void polish(QPalette &palette);
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawControl(ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    void drawComplexControl ( ComplexControl control, const QStyleOptionComplex * option, QPainter * painter, const QWidget * widget = 0 ) const;
    QPixmap standardPixmap( StandardPixmap standardPixmap, const QStyleOption * option, const QWidget * widget) const;
};

//class YakaCallStyle : public QPlastiqueStyle
//{
//    Q_OBJECT

//public:
//	YakaCallStyle() {}
//	~YakaCallStyle() {}

//public:
//    void polish(QPalette &palette);
//    void polish(QWidget *widget);
//    void unpolish(QWidget *widget);
//    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
//    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;
//    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
//    void drawControl(ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
//	void drawComplexControl ( ComplexControl control, const QStyleOptionComplex * option, QPainter * painter, const QWidget * widget = 0 ) const;
//	QPixmap standardPixmap( StandardPixmap standardPixmap, const QStyleOption * option, const QWidget * widget) const;
//};

#endif // YAKACALLSTLYE_H
