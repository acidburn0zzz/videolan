/*****************************************************************************
 * Library.cpp: Multimedia library
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/** \file
  * This file the library contains class implementation.
  * It's the the backend part of the Library widget of vlmc.
  * It can load and unload Medias (Medias.h/Media.cpp)
  * It can load and unload Clips (Clip.h/Clip.cpp)
  */

#include "Clip.h"
#include "Library.h"
#include "Media.h"
#include "MetaDataManager.h"

#include <QtDebug>
#include <QDomElement>
#include <QHash>
#include <QUuid>
#include <QXmlStreamWriter>

void
Library::loadProject( const QDomElement& doc )
{
    const QDomElement   medias = doc.firstChildElement( "medias" );

    if ( medias.isNull() == true )
        return ;

    QDomElement media = medias.firstChild().toElement();
    while ( media.isNull() == false )
    {
        if ( media.hasAttribute( "mrl" ) == true )
        {
            QString mrl = media.attribute( "mrl" );
            Media* m = addMedia( mrl );
            m_medias[mrl] = m;
        }
        media = media.nextSibling().toElement();
    }
    const QDomElement   clips = doc.firstChildElement( "clips" );
    if ( clips.isNull() == true )
        return ;
    load( clips, this );
    emit projectLoaded();
}

void
Library::saveProject( QXmlStreamWriter& project )
{
    QHash<QUuid, Clip*>::const_iterator     it = m_clips.begin();
    QHash<QUuid, Clip*>::const_iterator     end = m_clips.end();

    project.writeStartElement( "medias" );
    while ( it != end )
    {
        it.value()->getMedia()->save( project );
        ++it;
    }
    project.writeEndElement();
    project.writeStartElement( "clips" );
    save( project );
    project.writeEndElement();
}
