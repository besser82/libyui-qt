/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:	      YQSlider.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

// -*- c++ -*-

#ifndef YQSlider_h
#define YQSlider_h

#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qhbox.h>
#include <ycp/YCPString.h>

#include "YSlider.h"


class QString;
class QLabel;
class QSlider;
class YUIQt;

class YQSlider : public QWidget, public YSlider
{
    Q_OBJECT

public:

    /**
     * Constructor.
     */
    YQSlider( YUIQt *		yuiqt,
	      QWidget *		parent,
	      YWidgetOpt &	opt,
	      const YCPString &	label,
	      int 		minValue,
	      int 		maxValue,
	      int 		initialValue );

    /**
     * Sets the widget's enabled state.
     * Inherited from YWidget.
     */
    void setEnabling(bool enabled);

    /**
     * Preferred size.
     * Inherited from YWidget.
     */
    long nicesize(YUIDimension dim);

    /**
     * Set the new size of the widget.
     * Inherited from YWidget.
     */
    void setSize(long newwidth, long newheight);

    /**
     * Change the Slider label.
     * Calls YSlider::setLabel at the end.
     * Inherited from YSlider.
     */
    void setLabel( const YCPString& label );

    /**
     * Accept the keyboard focus.
     */
    virtual bool setKeyboardFocus();

    /**
     * Change the value.
     * Calls YSlider::setProgress at the end.
     * Inherited from YSlider.
     */
    void setValue( int newValue );

public slots:
    /**
     * Slot for setting the value.
     * This is needed separately from setValue() to avoid infinite loops via
     * ChangeWidget().
     */
    void setValueSlot( int newValue );


protected:

    QLabel *	qt_label;
    QSlider *	qt_slider;
    QSpinBox *	qt_spinbox;
    QVBox *	vbox;
    QHBox *	hbox;
    YUIQt *	yuiqt;
};


#endif // YQSlider_h