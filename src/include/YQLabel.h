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

  File:	      YQLabel.h

  Author:     Mathias Kettner <kettner@suse.de>
  Maintainer: Stefan Hundhammer <sh@suse.de>

/-*/

// -*- c++ -*-

#ifndef YQLabel_h
#define YQLabel_h

#include <qlabel.h>
#include <ycp/YCPString.h>

#include "YLabel.h"


class YUIQt;

class YQLabel : public QLabel, public YLabel
{
    Q_OBJECT

public:

    /**
     * Create a new YQLabel.
     * @param yuiqt the YUIQt, for getting the font info
     * @param parent the parent widget
     * @param text the initial text of the label
     * @param heading true if this should be a big Heading()
     * @param output_field true if this should look like a read-only input field
     */
    YQLabel(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt, YCPString text);

    /**
     * Inherited from YWidget: Sets the enabled state of the
     * widget. All new widgets are enabled per definition. Only
     * enabled widgets can take user input.
     */
    void setEnabling(bool enabled);

    /**
     * Minimum size the widget should have to make it look and feel
     * nice.
     * @dim Dimension, either YD_HORIZ or YD_VERT
     */
    long nicesize(YUIDimension dim);

    /**
     * Sets the new size of the widget.
     */
    void setSize(long newwidth, long newheight);

    /**
     * Change the label text.
     */
    void setLabel(const YCPString& label);
};

#endif // YQLabel_h