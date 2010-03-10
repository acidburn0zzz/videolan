/*****************************************************************************
 * MediaListView.h:
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
 * Authors: Thomas Boquet <thomas.boquet@gmail.com>
 *          Hugo Beauzée-Luyssen <beauze.h@vlmc.org>
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

#ifndef MEDIALISTVIEW_H
#define MEDIALISTVIEW_H

#include "StackViewController.h"
#include "ListViewController.h"
#include "MediaCellView.h"
#include "Library.h"
#include "Media.h"

class   MediaContainer;

class MediaListView : public ListViewController
{
    Q_OBJECT

public:
    MediaListView( StackViewController* nav, MediaContainer* mc );
    virtual ~MediaListView();

private:
    StackViewController*        m_nav;
    QUuid                       m_currentUuid;
    QHash<QUuid, QWidget*>      m_cells;
    QUuid                       m_lastUuidClipListAsked;
    MediaContainer*             m_mediaContainer;

public slots:
    void        cellSelection( const QUuid& uuid );
    void        showSubClips( const QUuid& uuid );
    void        clear();

private slots:
    void        clipRemoved( const Clip* );
    void        newClipLoaded( Clip* clip );

signals:
    void        clipSelected( Clip* );
    void        clipDeleted( const QUuid& );
};
#endif // MEDIALISTVIEW_H
