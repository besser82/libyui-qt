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

  File:	      YQReplacePoint.h

  Author:     Mathias Kettner <kettner@suse.de>
  Maintainer: Stefan Hundhammer <sh@suse.de>

/-*/

// -*- c++ -*-

#ifndef YQReplacePoint_h
#define YQReplacePoint_h

#include <qwidget.h>

#include "YReplacePoint.h"

class YUIQt;
class QWidget;

class YQReplacePoint : public QWidget, public YReplacePoint
{
    Q_OBJECT

public:

    /**
     * Creates a new YQReplacePoint.
     */
    YQReplacePoint(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt);

    /**
     * Inherited from YContainerWidget. Called when a child
     * has been added. We call show() for the new widget.
     */
    void childAdded(YWidget *child);

    /**
     * Sets the size
     */
    void setSize(long newwidth, long newheight);
};


#endif // YQReplacePoint_h