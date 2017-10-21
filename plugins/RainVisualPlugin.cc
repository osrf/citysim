/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <OGRE/OgreBillboardParticleRenderer.h>
#include <gazebo/rendering/Camera.hh>
#include <gazebo/rendering/RenderEvents.hh>
#include <gazebo/rendering/Scene.hh>
#include <gazebo/rendering/ogre_gazebo.h>
#include <gazebo/rendering/RenderingIface.hh>

#include "RainVisualPlugin.hh"

namespace gazebo
{
  /// \internal
  /// \class RainVisualPlugin RainVisualPlugin.hh
  /// \brief Private data for the RainVisualPlugin class.
  class RainVisualPluginPrivate
  {
    public: Ogre::ParticleSystem *particleSystem = nullptr;
    public: Ogre::ParticleEmitter *particleEmitter = nullptr;

    /// \brief Event connections
    public: std::vector<event::ConnectionPtr> connections;
  };
}

using namespace gazebo;

GZ_REGISTER_VISUAL_PLUGIN(RainVisualPlugin)

/////////////////////////////////////////////////
RainVisualPlugin::RainVisualPlugin()
    : dataPtr(new RainVisualPluginPrivate)
{
}

/////////////////////////////////////////////////
RainVisualPlugin::~RainVisualPlugin()
{
}

/////////////////////////////////////////////////
void RainVisualPlugin::Load(rendering::VisualPtr _visual,
    sdf::ElementPtr /*_sdf*/)
{
  if (!_visual)
    gzerr << "Invalid visual pointer." << std::endl;

  auto scene = _visual->GetScene();

  
  this->dataPtr->particleSystem =
      scene->OgreSceneManager()->createParticleSystem(
      "rain", "Water/Rain");
  this->dataPtr->particleEmitter =
      this->dataPtr->particleSystem->getEmitter(0);
  this->dataPtr->particleEmitter->setEmissionRate(1000);
  Ogre::SceneNode *rainNode =
    scene->OgreSceneManager()->getRootSceneNode()->createChildSceneNode();
  rainNode->translate(0, 0, 75);
  rainNode->attachObject(this->dataPtr->particleSystem);
  // Fast-forward the rain so it looks more natural
  this->dataPtr->particleSystem->fastForward(10);
  // It can't be set in .particle file, and we need it ;)
   static_cast<Ogre::BillboardParticleRenderer*>(
     this->dataPtr->particleSystem->getRenderer())->setBillboardOrigin(
     Ogre::BBO_BOTTOM_CENTER);

  this->dataPtr->connections.push_back(
      rendering::Events::ConnectToggleLayer(
      std::bind(&RainVisualPlugin::ToggleLayer, this, std::placeholders::_1)));

  return;
}

/////////////////////////////////////////////////
void RainVisualPlugin::ToggleLayer(const int32_t _layer)
{
  if (_layer == 0)
  {
    if (this->dataPtr->particleEmitter->getEmissionRate() > 0)
      this->dataPtr->particleEmitter->setEmissionRate(0);
    else
      this->dataPtr->particleEmitter->setEmissionRate(1000);
  }
}
