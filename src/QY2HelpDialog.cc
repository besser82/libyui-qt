/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                        ***  Qt4 plugin  ***                                         ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		QY2HelpDialog.cc

  Author:	Stephan Kulow <coolo@suse.de>

  Textdomain	"qt"

/-*/

#include <QY2HelpDialog.h>
#include "ui_QHelpDialog.h"
#include <QDebug>
#include <QTextObject>
#include "YQi18n.h"
#include "YQUI.h"
#include "QY2Styler.h"


#include "icons/viewmag.xpm"

#ifdef TEXTDOMAIN
#    undef TEXTDOMAIN
#endif

#define TEXTDOMAIN "qt"


QY2HelpDialog::QY2HelpDialog( const QString& helpText, QWidget *parent )
    : QDialog( parent )
{
    _ui = new Ui_QHelpDialog();
    _ui->setupUi( this );
    _ui->textBrowser->setText( helpText );

    _ui->label->setPixmap ( QPixmap( viewmag ) );
    connect( _ui->lineEdit, SIGNAL( textEdited( QString ) ),
             SLOT( searchStringChanged( QString ) ) );

    _ui->lineEdit->setFocus( Qt::OtherFocusReason );
    _ui->pushButton->setAutoDefault(false);

    YQUI::setTextdomain( TEXTDOMAIN );

    // Window title for help wizard window
    setWindowTitle( _( "Help" ) );

    // Close button for wizard help window
    _ui->pushButton->setText( _( "&Close" ) );

    QY2Styler::styler()->registerWidget( this );
}

void QY2HelpDialog::setHelpText( const QString& helpText )
{
    _ui->textBrowser->setText( helpText );
    _ui->lineEdit->setText( QString() );
    _ui->lineEdit->setFocus( Qt::OtherFocusReason );
}

QY2HelpDialog::~QY2HelpDialog()
{
    QY2Styler::styler()->unregisterWidget( this );
    delete _ui;
}

void QY2HelpDialog::searchStringChanged( QString text )
{
    QTextCharFormat fmt;
    fmt.setBackground( Qt::yellow );
    QTextDocument *d = _ui->textBrowser->document();

    QTextCursor all(d);
    all.select ( QTextCursor::Document);
    all.setCharFormat( QTextCharFormat() );

    _marks.clear();

    QTextCursor c( d );

    while ( true )
    {
        c = d->find( text, c );
        if ( c.isNull() )
            break;
        c.setCharFormat( fmt );
        c.select( QTextCursor::WordUnderCursor );
        _marks.push_back( c );
    }
}

void QY2HelpDialog::retranslate()
{
    setWindowTitle( _( "Help" ) );
    _ui->pushButton->setText( _( "&Close" ) );
}


#include "QY2HelpDialog.moc"
