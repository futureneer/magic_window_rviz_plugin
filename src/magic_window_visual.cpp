/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreOverlayContainer.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreHardwarePixelBuffer.h>

// QT 
#include <QImage>
#include <QString>
#include <QPainter>
#include <QFileInfo>
#include <iostream>

#include <rviz/ogre_helpers/arrow.h>

#include "magic_window_visual.h"

namespace magic_window_rviz_plugin
{

// BEGIN_TUTORIAL
MagicWindowVisual::MagicWindowVisual( Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node ){
  // Get OGRE Environment pointers
  scene_manager_ = scene_manager;
  frame_node_ = parent_node->createChildSceneNode();

  // Get image into QImage
  QString path = "/home/kel/catkin_ws/src/magic_window_rviz_plugin/test_images/forest.jpg";
  QString fileName = QFileInfo(path).fileName();
  QImage qImage(path);
  std::cerr<<"Loading texture: "<<path.toStdString()<<std::endl;

  // Create Texture
  Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

  // Convert to 32-bit RGB
  if (qImage.format() != QImage::Format_RGB32)
    qImage = qImage.convertToFormat(QImage::Format_RGB32);
  
  // Create an Ogre::Image from the QImage
  Ogre::Image image;
  image.loadDynamicImage(
    qImage.bits(), 
    qImage.width(), 
    qImage.height(), 
    Ogre::PF_X8R8G8B8);

  // Create a texture from the image
  Ogre::TexturePtr texture = manager->loadImage("plane_texture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,image);

  // Create Material
  Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("plane_material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  mat->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
  mat->getTechnique(0)->getPass(0)->createTextureUnitState("plane_texture");
  mat->getTechnique(0)->setCullingMode( Ogre::CULL_NONE );

  mPlane = new Ogre::MovablePlane("Plane");
  mPlane->d = 0;
  mPlane->normal = Ogre::Vector3::UNIT_Y;
  Ogre::MeshManager::getSingleton().createPlane("plane_mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, 1, 1, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
  mPlaneEnt = scene_manager_->createEntity("plane_entity", "plane_mesh");
  mPlaneEnt->setMaterialName("plane_material");
  // Add to scene
  frame_node_->attachObject(mPlaneEnt);
}


MagicWindowVisual::~MagicWindowVisual(){
  // Destroy the frame node since we don't need it anymore.
  scene_manager_->destroySceneNode( frame_node_ );
}

void MagicWindowVisual::updateImage(const QString& image_path){
  // Get image into QImage
  QString path = image_path;
  QString fileName = QFileInfo(path).fileName();
  QImage qImage(path);
  std::cerr<<"Loading texture: "<<path.toStdString()<<std::endl;
  if (!qImage.isNull()){

    // Create Texture
    Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

    // Convert to 32-bit RGB
    if (qImage.format() != QImage::Format_RGB32)
      qImage = qImage.convertToFormat(QImage::Format_RGB32);
    
    // Create an Ogre::Image from the QImage
    Ogre::Image image;
    image.loadDynamicImage(
      qImage.bits(), 
      qImage.width(), 
      qImage.height(), 
      Ogre::PF_X8R8G8B8);

    // Create a texture from the image
    manager->remove("plane_texture");
    Ogre::TexturePtr texture = manager->loadImage("plane_texture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,image);

    Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("plane_material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("plane_texture");
    mat->getTechnique(0)->setCullingMode( Ogre::CULL_NONE );
  }else{
    std::cerr<<"Path does not point to a real image: "<<path.toStdString()<<std::endl;
  }
}

void MagicWindowVisual::setMessage( const sensor_msgs::Image::ConstPtr& msg ){
  // const geometry_msgs::Vector3& a = msg->linear_acceleration;

  // Convert the geometry_msgs::Vector3 to an Ogre::Vector3.
  // Ogre::Vector3 acc( a.x, a.y, a.z );

  // Find the magnitude of the acceleration vector.
  // float length = acc.length();

  // Scale the arrow's thickness in each dimension along with its length.
  // Ogre::Vector3 scale( length, length, length );
  // acceleration_arrow_->setScale( scale );

  // Set the orientation of the arrow to match the direction of the
  // acceleration vector.
  // acceleration_arrow_->setDirection( acc );
}

void MagicWindowVisual::setFrameScale( const Ogre::Vector3& scale){
  frame_node_->setScale(scale);
}

void MagicWindowVisual::setFrameScale( const float& scale){
  frame_node_->setScale(scale,scale,scale);
}

void MagicWindowVisual::setFramePosition( const Ogre::Vector3& position ){
  frame_node_->setPosition( position );
}

void MagicWindowVisual::setFrameOrientation( const Ogre::Quaternion& orientation ){
  frame_node_->setOrientation( orientation );
}

} // end namespace magic_window_rviz_plugin

