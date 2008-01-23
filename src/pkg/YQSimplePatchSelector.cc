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

  File:	      YQSimplePatchSelector.cc

  Author:     Stefan Hundhammer <sh@suse.de>

  Textdomain "packages-qt"

/-*/

#include <QApplication>
#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSplitter>

#define y2log_component "qt-pkg"
#include <ycp/y2log.h>

#include "QY2LayoutUtils.h"

#include "YQSimplePatchSelector.h"
#include "YQPkgConflictDialog.h"
#include "YQPkgDiskUsageList.h"
#include "YQPkgPatchFilterView.h"
#include "YQPkgPatchList.h"
#include "YQWizard.h"
#include "YQDialog.h"

#include "utf8.h"
#include "YQUI.h"
#include "YEvent.h"
#include "YQi18n.h"


using std::max;
using std::string;

#define SHOW_DISK_USAGE		0

#define SPACING			6
#define MARGIN			6



YQSimplePatchSelector::YQSimplePatchSelector( YWidget *	parent, long modeFlags )
    : YQPackageSelectorBase( parent, modeFlags )
{
    _patchFilterView	= 0;
    _patchList		= 0;
    _diskUsageList	= 0;
    _wizard		= findWizard();

    basicLayout();
    makeConnections();

#if 0
    _patchList->fillList();
    _patchList->selectSomething();
#endif

    if ( _diskUsageList )
	_diskUsageList->updateDiskUsage();
}



YQWizard *
YQSimplePatchSelector::findWizard() const
{
    YQWizard * wizard = 0;

    YQDialog * dialog = dynamic_cast<YQDialog *> ( YDialog::currentDialog() );

    if ( dialog )
	wizard = dialog->findWizard();

    return wizard;
}



void
YQSimplePatchSelector::basicLayout()
{
    QSplitter * splitter = new QSplitter( Qt::Vertical, this );
    Q_CHECK_PTR( splitter );

    setLayout( new QVBoxLayout( ) );
    layout()->addWidget(splitter);

    //
    // PatchFilterView
    //

    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin( MARGIN );
    QWidget * upper_vbox = new QWidget( splitter );
    Q_CHECK_PTR( upper_vbox );
    upper_vbox->setLayout(layout);
    splitter->addWidget(upper_vbox);

    splitter->setStretchFactor( 0, 1 );

    _patchFilterView = new YQPkgPatchFilterView( upper_vbox );
    layout->addWidget( _patchFilterView );
    Q_CHECK_PTR( _patchFilterView );

    _patchList = _patchFilterView->patchList();
    Q_CHECK_PTR( _patchList );

    //addVSpacing( upper_vbox, MARGIN );

    //
    // Disk Usage
    //


#if SHOW_DISK_USAGE
    layout = new QVBoxLayout;
    QWidget * lower_vbox = new QWidget( splitter );
    lower_vbox->setLayout(layout);

    Q_CHECK_PTR( lower_vbox );
    //addVSpacing( lower_vbox, MARGIN );

    _diskUsageList = new YQPkgDiskUsageList( lower_vbox );
    Q_CHECK_PTR( _diskUsageList );
    layout->addWidget(_diskUsageList);

    splitter->setResizeMode( lower_vbox, QSplitter::FollowSizeHint );
#endif


    //
    // Buttons
    //

    if ( _wizard )	// No button box - add "Details..." button here
    {
	//
	// "Details" button
	//

	//addVSpacing( this, SPACING );
        layout = new QHBoxLayout;
        QWidget * hbox = new QWidget( this );
	Q_CHECK_PTR( hbox );
        hbox->setLayout(layout);
	QPushButton * details_button = new QPushButton( _( "&Details..." ), hbox );
        layout->addWidget(details_button);
	Q_CHECK_PTR( details_button );

	connect( details_button, SIGNAL( clicked() ),
		 this,		 SLOT  ( detailedPackageSelection() ) );

	//addHStretch( hbox );
    }
    else // ! _wizard
    {
	layoutButtons( this );
    }
}


void
YQSimplePatchSelector::layoutButtons( QWidget * parent )
{
    QWidget * button_box = new QWidget( parent );
    QHBoxLayout *layout = new QHBoxLayout( button_box );
    Q_CHECK_PTR( button_box );
    layout->setMargin ( MARGIN  );
    layout->setSpacing( SPACING );


    QPushButton * details_button = new QPushButton( _( "&Details..." ), button_box );
    Q_CHECK_PTR( details_button );
    layout->addWidget(details_button);
    details_button->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) ); // hor/vert

    connect( details_button,	SIGNAL( clicked() ),
	     this,		SLOT  ( detailedPackageSelection() ) );


    layout->addStretch();

    QPushButton * cancel_button = new QPushButton( _( "&Cancel" ), button_box );
    Q_CHECK_PTR( cancel_button );
    layout->addWidget(cancel_button);
    cancel_button->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) ); // hor/vert

    connect( cancel_button, SIGNAL( clicked() ),
	     this,	    SLOT  ( reject()   ) );


    QPushButton * accept_button = new QPushButton( _( "&Accept" ), button_box );
    Q_CHECK_PTR( accept_button );
    layout->addWidget(accept_button);
    accept_button->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) ); // hor/vert

    connect( accept_button, SIGNAL( clicked() ),
	     this,	    SLOT  ( accept()   ) );

    this->layout()->addWidget( button_box );
//    button_box->setFixedHeight( button_box->sizeHint().height() );
}



void
YQSimplePatchSelector::makeConnections()
{
    if ( _patchList && _diskUsageList )
    {
	connect( _patchList,	 SIGNAL( updatePackages()  ),
		 _diskUsageList, SLOT  ( updateDiskUsage() ) );
    }

    y2milestone( "Connection set up" );

    if ( _wizard )
    {
	connect( _wizard, 	SIGNAL( nextClicked()	),
		 this,		SLOT  ( accept()        ) );

	connect( _wizard, 	SIGNAL( backClicked()	),
		 this,		SLOT  ( reject()	) );

	connect( _wizard, 	SIGNAL( abortClicked()	),
		 this,		SLOT  ( reject()	) );
    }
}


void
YQSimplePatchSelector::detailedPackageSelection()
{
    y2milestone( "\"Details..\" button clicked" );
    YQUI::ui()->sendEvent( new YMenuEvent( YCPSymbol( "details" ) ) );
}


void
YQSimplePatchSelector::debugTrace()
{
    y2warning( "debugTrace" );
}



#include "YQSimplePatchSelector.moc"