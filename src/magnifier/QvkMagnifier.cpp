/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkMagnifier.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QRegion>

QvkMagnifier::QvkMagnifier()
{
    faktor = 2; // Vergrößerungsfaktor
    label = new QLabel( this );

    slot_magnifier200x200();

    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar
    border = 3;
    distanceCopyMagnifier = 30;

    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
    label->setAlignment( Qt::AlignCenter );
    label->setScaledContents( true );

    QPalette pal = QPalette();
    pal.setColor( QPalette::Window, Qt::gray );
    setAutoFillBackground( true );
    setPalette( pal );

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 40 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mytimer() ) );
}


QvkMagnifier::~QvkMagnifier()
{
}


void QvkMagnifier::slot_magnifierShow( bool value )
{
    if ( value == true )
    {
        show();
        timer->start();
    }

    if ( value == false )
    {
        close();
        timer->stop();
    }
}


void QvkMagnifier::slot_magnifier200x200()
{
    distanceX = 50;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier400x200()
{
    distanceX = 100;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier600x200()
{
    distanceX = 150;
    distanceY = 50; // distanceY ist der Abstand Cursor zur Lupe
    resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
    label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}



void QvkMagnifier::setMagnifier()
{
    bool debug = false;

    // Magnifier area middle
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionMiddle( this->width()/2,
                          0,
                          screen->size().width() - 2 * this->width()/2,
                          screen->size().height() - this->height() - distanceY - distanceCopyMagnifier );

    if ( regionMiddle.contains( screenCursorPos ) )
    {
        int valueY = globalCursorPos.y() + distanceY;
        if ( ( screenCursorPos.y() + distanceY ) <= ( 2 * distanceY ) ) {
            valueY = screen->geometry().top() + 2 * distanceY;
        }

        // Move works with global mouse coordinates like screen->geometry().left() and globalCursorPos
        move( screen->geometry().left() + screenCursorPos.x() - this->width()/2, valueY + distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionMiddle:" << regionMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier right
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionRight( screen->size().width() - this->width()/2,
                         0,
                         this->width()/2,
                         screen->size().height() - this->height() - distanceY - distanceCopyMagnifier );

    if ( regionRight.contains( screenCursorPos ) )
    {
        int valueY = globalCursorPos.y() + distanceY;
        if ( ( screenCursorPos.y() + distanceY ) <= ( 2 * distanceY ) ) {
            valueY = screen->geometry().top() + 2 * distanceY;
        }

        move( screen->geometry().right() - width(), valueY + distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionRightMiddle:" << regionRight
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier bottom right
    QRegion regionBottomRight( screen->size().width() - this->width()/2,
                               screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                               this->width(),
                               this->height() + distanceY + distanceCopyMagnifier );

    if ( regionBottomRight.contains( screenCursorPos ) )
    {
        move( screen->geometry().right() - this->width(), globalCursorPos.y() - this->height() - 2*distanceY - distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionBottomRight:" << regionBottomRight
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier bottom middle
    QRegion regionBottomMiddle( this->width()/2,
                                screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                                screen->size().width(),
                                this->height() + distanceY + distanceCopyMagnifier );

    if ( regionBottomMiddle.contains( screenCursorPos ) )
    {
        move( screen->geometry().left() + screenCursorPos.x() - this->width()/2, globalCursorPos.y() - this->height() - 2*distanceY - distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionBottomMiddle:" << regionBottomMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier left bottom
    QRegion regionBottomLeft( 0,
                              screen->size().height() - this->height() - distanceY - distanceCopyMagnifier,
                              this->width()/2,
                              this->height() + distanceY + distanceCopyMagnifier );

    if ( regionBottomLeft.contains( screenCursorPos ) )
    {
        move( screen->geometry().left(), globalCursorPos.y() - this->height() - 2*distanceY - distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionBottomLeft:" << regionBottomLeft
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Magnifier left
    // Region beinhalte absolute Bildschirmwerte
    QRegion regionLeft( 0,
                        0,
                        this->width()/2,
                        screen->size().height() - this->height() - distanceY - distanceCopyMagnifier );

    if ( regionLeft.contains( screenCursorPos ) )
    {
        int valueY = globalCursorPos.y() + distanceY;
        if ( ( screenCursorPos.y() + distanceY ) <= ( 2 * distanceY ) ) {
            valueY = screen->geometry().top() + 2 * distanceY;
        }

        move( screen->geometry().left(), valueY + distanceCopyMagnifier );

        if ( debug == true ) { qDebug() << "Magnifier regionLeft:" << regionLeft
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }
}


void QvkMagnifier::slot_mytimer()
{
    bool debug = true;

    globalCursorPos = QCursor::pos();
    if( debug == true ) { qDebug() << "slot_mytimer globalCursorPos:" << globalCursorPos; }

    screen = QGuiApplication::screenAt( globalCursorPos );
    if ( screen ) {
        if( debug == true ) { qDebug() << "slot_mytimer screen:" << screen; }
    } else {
        // screenAt found no screen. Without a return the Application is crashed
        return;
    }

    screenCursorPos = globalCursorPos - screen->geometry().topLeft(); // screenCursorPos beginnt bei 0 auf dem jeweiliegen Bildschirm
    if( debug == true ) { qDebug() << "slot_mytimer screenCursorPos:" << screenCursorPos; }

    screenList = QGuiApplication::screens();
    if( debug == true ) { qDebug() << "slot_mytimer screenList:" << screenList; }

    screenIndex = screenList.indexOf( screen );
    if( debug == true ) { qDebug() << "slot_mytimer screenIndex:" << screenIndex; }

    QPixmap pixmap;

    setMagnifier();

    // Grab middle
    QRegion regionAreaMiddle( this->width()/2,
                              0,
                              screen->size().width() - 2 * this->width()/2,
                              screen->size().height() - 2 * this->height()/2 ) ;

    if ( regionAreaMiddle.contains( screenCursorPos ) )
    {
        int valueX = screenCursorPos.x() - distanceX;
        if ( screenCursorPos.x() <= distanceX ) {
            valueX = 0;
        }

        int valueY = screenCursorPos.y() - distanceY;
        if ( screenCursorPos.y() <= distanceY ) {
            valueY = 0;
        }

        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     valueX,
                                     valueY,
                                     2 * distanceX ,
                                     2 * distanceY );

        label->setPixmap( pixmap );


        if ( debug == true ) { qDebug() << "Grab regionAreaMiddle:" << regionAreaMiddle
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }
        return;
    }


    // Grab right
    QRegion regionRight( screen->size().width() - this->width()/2,
                         0,
                         this->width()/2,
                         screen->size().height() - 2 * this->height()/2 );

    if ( regionRight.contains( screenCursorPos ) )
    {
        int valueX = screenCursorPos.x() - distanceX;
        if ( screenCursorPos.x() + distanceX >= screen->size().width() ) {
            valueX = screen->size().width() - 2 * distanceX;
        }

        int valueY = screenCursorPos.y() - distanceY;
        if ( screenCursorPos.y() <= distanceY ) {
            valueY = 0;
        }

        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     valueX,
                                     valueY,
                                     this->width()/2,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionRight:" << regionRight
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }


    // Grab left
    QRegion regionLeft( 0,
                        0,
                        0 + this->width()/2,
                        screen->size().height() - 2 * this->height()/2 );

    if ( regionLeft.contains( screenCursorPos ) )
    {
        int valueX = screenCursorPos.x() - distanceX;
        if ( screenCursorPos.x() <= distanceX ) {
            valueX = 0;
        }

        int valueY = screenCursorPos.y() - distanceY;
        if ( screenCursorPos.y() <= distanceY ) {
            valueY = 0;
        }

        WId id = 0;
        pixmap = screen->grabWindow( id,
                                     valueX,
                                     valueY,
                                     2 * distanceX,
                                     2 * distanceY );

        label->setPixmap( pixmap );

        if ( debug == true ) { qDebug() << "Grab regionLeft:" << regionLeft
                                        << "globalCursorPos:" << globalCursorPos
                                        << "screenCursorPos:" << screenCursorPos << screenIndex; }

        return;
    }
}
