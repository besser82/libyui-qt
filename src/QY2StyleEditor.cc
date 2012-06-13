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

  File:		QY2StyleEditor.cc

  Author:	Thomas Goettlicher <tgoettlicher@suse.de>

  Textdomain	"qt"

/-*/

#include "QY2StyleEditor.h"
#include "QY2Styler.h"
#include "ui_QStyleEditor.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextObject>
#include "YQi18n.h"


QY2StyleEditor::QY2StyleEditor( QWidget *parent )
    : QDialog( parent )
{
    ui.setupUi(this);

    ui.textEdit->setPlainText( "/* enter style sheet here  */" );
    ui.closeButton->setAutoDefault(false);
    setWindowTitle( _( "Stylesheet Editor" ) );

    connect( ui.applyButton, SIGNAL( clicked() ),
             this, SLOT( slotApplyStyle() ));

    connect( ui.closeButton, SIGNAL( clicked() ),
             this, SLOT( close() ));

    connect( ui.loadButton, SIGNAL( clicked() ),
             this, SLOT( slotLoadFile() ));

    connect( ui.textEdit, SIGNAL( textChanged() ),
             this, SLOT( slotTextChanged() ));

    connect( ui.autoApply, SIGNAL( stateChanged(int) ),
             this, SLOT( slotTextChanged() ));
}


QY2StyleEditor::~QY2StyleEditor()
{
}

void QY2StyleEditor::slotTextChanged()
{
    if ( ui.autoApply->isChecked() )
        slotApplyStyle();
}

void QY2StyleEditor::slotApplyStyle()
{
    QY2Styler::styler()->setStyleSheet( ui.textEdit->toPlainText() );
}


void QY2StyleEditor::slotLoadFile()
{

    QString fileName = QFileDialog::getOpenFileName( this,      // parent
                                                    QString( "Load stylesheet ..." ), // caption
                                                    QY2Styler::styler()->themeDir(), // dir
                                                    QString( "*.qss"), 0,            // filter
                                                    QFileDialog::DontUseNativeDialog );

    if ( fileName.isEmpty() )
        return;         // user clicked cancel


    QFile file( fileName);

    if ( file.open( QFile::ReadOnly ) )
        ui.textEdit->setPlainText( file.readAll() );
    else
    {
        QMessageBox::warning( this,                     // parent
            QString("Error") ,                          // caption
            QString( "Couldn't load file\n%1" ).arg( fileName ),
            QMessageBox::Ok | QMessageBox::Default,     // button0
            Qt::NoButton,                               // button1
            Qt::NoButton );                             // button2
    }

}



#include "QY2StyleEditor.moc"
