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

  File:	      YQImage.cc

  Author:     Mathias Kettner <kettner@suse.de>
  Maintainer: Stefan Hundhammer <sh@suse.de>

/-*/


#include <qpixmap.h>
#include <qmovie.h>
#define y2log_component "qt-ui"
#include <ycp/y2log.h>

#include "utf8.h"
#include "YQImage.h"

// Images
#include "yast2.xpm"



YQImage::YQImage(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt,
		 YUIInterpreter::ImageType img)
    : QLabel(parent)
    , YImage(opt)
{
    init( yuiqt, parent, opt );
    animated = false;
    
    const char **xpm = 0;
    switch (img)
    {
	case YUIInterpreter::IT_SUSEHEADER:
	    xpm = (const char **) yast2_xpm;
	    break;

	case YUIInterpreter::IT_YAST2:
	    xpm = (const char **) yast2_xpm;
	    break;

	default:
	    y2internal("Internal error: Unknown image type %d", img);
	    return; // Should not occur
    }

    yqSetPixmap( QPixmap( xpm ) );
}


YQImage::YQImage(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt,
		 const YCPByteblock& byteblock)
    : QLabel(parent)
    , YImage(opt)
{
    init( yuiqt, parent, opt );

    if ( animated )
    {
	QMovie movie( byteblock->size() );
	movie.pushData( byteblock->value(), byteblock->size() );
	setMovie( movie );
    }
    else
    {
	QPixmap pixmap;
	pixmap.loadFromData( byteblock->value(), byteblock->size() );
	yqSetPixmap( pixmap );
    }
}


YQImage::YQImage(YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt, const YCPString & ycp_file_name )
    : QLabel( parent )
    , YImage( opt)
{
    init( yuiqt, parent, opt );
    QString file_name = fromUTF8( ycp_file_name->value() );
    y2debug( "Loading image from %s", (const char *) file_name );

    if ( animated )
    {
	QMovie movie( file_name );
	
	if ( movie .isNull() )
	{
	    y2error( "Couldn't load animation from %s", (const char *) file_name );
	}
	else
	{
	    setMovie( movie );
	}
    }
    else
    {
	QPixmap pixmap( file_name );

	if ( pixmap.isNull() )
	{
	    y2error( "Couldn't load pixmap from %s", (const char *) file_name );
	}
	else
	{
	    yqSetPixmap( pixmap );
	}
    }
}


void
YQImage::init( YUIQt *yuiqt, QWidget *parent, YWidgetOpt &opt )
{
    setWidgetRep( (QWidget *)this );
    setAlignment( Qt::AlignLeft | Qt::AlignTop );

    zeroWidth	= opt.zeroWidth.value();
    zeroHeight	= opt.zeroHeight.value();
    animated	= opt.animated.value();
    tiled	= opt.tiled.value();
    setScaledContents( opt.scaleToFit.value() );
    windowID	 = winId();
    pixmapHeight = 0;
    pixmapWidth  = 0;

    if ( zeroWidth  )
	setStretchable( YD_HORIZ, true );

    if ( zeroHeight )
	setStretchable( YD_VERT,  true );
    
    if ( opt.easterEgg.value() )
	yuiqt->suseheaderID = winId();
}


void
YQImage::yqSetPixmap( const QPixmap & pixmap )
{
    if ( hasScaledContents() )
    {
	pixmapWidth  = 0;
	pixmapHeight = 0;
	
	QLabel::setPixmap( pixmap );
    }
    else
    {
	pixmapWidth  = pixmap.size().width();
	pixmapHeight = pixmap.size().height();

	setPaletteBackgroundPixmap( pixmap );
    }
}


long YQImage::nicesize( YUIDimension dim )
{
    if ( animated )
    {
	// a QMovie doesn't have a size() method, thus use sizeHint() instead.
	
	if (dim == YD_HORIZ)	return zeroWidth  ? 0L : sizeHint().width();
	else			return zeroHeight ? 0L : sizeHint().height();
    }
    else
    {
	// for non-animated images, the background pixmap is used, thus
	// sizeHint() will always return (0,0) - thus, use the internally
	// stored sizes instead.
	
	if (dim == YD_HORIZ)	return zeroWidth  ? 0L : pixmapWidth;
	else			return zeroHeight ? 0L : pixmapHeight;
    }
}


void YQImage::setSize( long newwidth, long newheight )
{
    resize( newwidth, newheight );
}

#include "YQImage.moc.cc"