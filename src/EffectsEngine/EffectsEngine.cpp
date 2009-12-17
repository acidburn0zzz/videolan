/*****************************************************************************
 * EffectsEngine.cpp: Main class of the effects engine
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Vincent Carrubba <cyberbouba@gmail.com>
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

#include <QtDebug>
#include "EffectsEngine.h"

// CTOR & DTOR

EffectsEngine::EffectsEngine( void ) : m_patch( NULL ), m_bypassPatch( NULL )
{
   m_inputLock = new QReadWriteLock;

   m_videoInputs = new OutSlot<LightVideoFrame>[64];
   m_videoOutputs = new InSlot<LightVideoFrame>[64];
   start();

   if ( EffectNode::createRootNode( "RootNode" ) == false )
       qDebug() << "RootNode creation failed!!!!!!!!!!";
   else
   {
       qDebug() << "RootNode successfully created.";
       if ( EffectNode::createRootNode( "BypassRootNode" ) == false )
           qDebug() << "BypassRootNode creation failed!!!!!!!!!!";
       else
       {
           qDebug() << "BypassRootNode successfully created.";
           m_patch = EffectNode::getRootNode( "RootNode" );
           m_bypassPatch = EffectNode::getRootNode( "BypassRootNode" );
           quint32	i;

           for ( i = 0 ; i < 64; ++i)
           {
               qDebug() << "PONIES";
               m_patch->createStaticVideoInput();
               m_bypassPatch->createStaticVideoInput();
           }
           m_patch->createStaticVideoOutput();
           m_bypassPatch->createStaticVideoOutput();

           QList<QString> fuck = m_patch->getChildsTypesNamesList();

           QList<QString>::iterator fuckit = fuck.begin();
           QList<QString>::iterator fuckend = fuck.end();

           for ( ; fuckit != fuckend; ++fuckit )
           {
               qDebug() << *fuckit;
           }

           m_patch->createChild( "libVLMC_MixerEffectPlugin" );
           m_bypassPatch->createChild( "libVLMC_MixerEffectPlugin" );

           EffectNode* tmp;

           if ( ( tmp = m_patch->getChild( 1 ) ) == NULL )
           {
               qDebug() << "POJEF";
           }
           else
           {
               qDebug() << "NAME = " << tmp->getInstanceName();
               qDebug() << "ID = " << tmp->getInstanceId();
           }
       }
   }
}

EffectsEngine::~EffectsEngine()
{
    stop();
    delete [] m_videoInputs;
    delete [] m_videoOutputs;

    if ( m_patch )
        EffectNode::deleteRootNode( "RootNode" );
    if ( m_bypassPatch )
        EffectNode::deleteRootNode( "BypassRootNode" );
    delete m_inputLock;
}

//

EffectNode* EffectsEngine::operator->( void )
{
    QReadLocker    rl( &m_rwl );
    return ( m_patch );
}

EffectNode const * EffectsEngine::operator->( void ) const
{
    QReadLocker    rl( &m_rwl );
    return ( m_patch );
}

EffectNode* EffectsEngine::operator*( void )
{
    QReadLocker    rl( &m_rwl );
    return ( m_patch );
}

EffectNode const * EffectsEngine::operator*( void ) const
{
    QReadLocker    rl( &m_rwl );
    return ( m_patch );
}

// MAIN METHOD

void	EffectsEngine::render( void )
{
  QWriteLocker    lock( m_inputLock );
  ( m_effects[0] )->render();
  ( m_effects[1] )->render();
  return ;
}

// BYPASSING

void		EffectsEngine::enable( void )
{
    return ;
}

void		EffectsEngine::disable( void )
{
    return ;
}

// INPUTS & OUTPUTS METHODS


void	EffectsEngine::setInputFrame( LightVideoFrame& frame, quint32 tracknumber )
{
    QWriteLocker    lock( m_inputLock );

    m_videoInputs[tracknumber] = frame;
    return ;
}


LightVideoFrame const &	EffectsEngine::getOutputFrame( quint32 tracknumber ) const
{
  return ( m_videoOutputs[tracknumber] );
}

//
// PRIVATES METHODS
//

// START & STOP

void	EffectsEngine::start( void )
{
  loadEffects();
  patchEffects();
  return ;
}

void	EffectsEngine::stop( void )
{
  unloadEffects();
  return ;
}

// EFFECTS LOADING & UNLOADING

void	EffectsEngine::loadEffects( void )
{
    QList<QString> list = m_enf.getEffectNodeTypesNamesList();
    quint32     i;
    quint32     size = list.size();


    for ( i = 0; i < size; ++i )
        qDebug() << "typename : " << list.at( i );
    m_enf.createEffectNodeInstance( "libVLMC_GreenFilterEffectPlugin" );
    m_enf.createEffectNodeInstance( "libVLMC_MixerEffectPlugin" );

    list = m_enf.getEffectNodeInstancesNamesList();
    size = list.size();
    for ( i = 0; i < size; ++i )
        qDebug() << "instancename : " << list.at( i );

    m_effects[0] = m_enf.getEffectNodeInstance( "libVLMC_MixerEffectPlugin_2" );
    m_effects[1] = m_enf.getEffectNodeInstance( "libVLMC_GreenFilterEffectPlugin_1" );
    return ;
}

void	EffectsEngine::unloadEffects( void )
{
    m_enf.deleteEffectNodeInstance( "libVLMC_GreenFilterEffectPlugin_1" );
    m_enf.deleteEffectNodeInstance( "libVLMC_MixerEffectPlugin_2" );
    return ;
}

// EFFECTS PATCHING

void	EffectsEngine::patchEffects( void )
{
    quint32	i;

    QReadLocker lock( m_inputLock );
    for ( i = 0; i < 64; ++i )
    {
        m_effects[0]->connect( m_videoInputs[i], i );
    }
    m_effects[0]->connectOutput( 0 , m_effects[1], 0 );
    m_effects[1]->connect( 0, m_videoOutputs[0] );
    return ;
}
