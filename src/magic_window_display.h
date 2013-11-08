/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * Copyright (c) 2013, The Johns Hopkins University
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

#ifndef MAGICWINDOW_DISPLAY_H
#define MAGICWINDOW_DISPLAY_H

#include <boost/circular_buffer.hpp>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <rviz/message_filter_display.h>
#include "rviz/display.h"

namespace Ogre{
class SceneNode;
}

namespace rviz{
class ColorProperty;
class FloatProperty;
class IntProperty;
class VectorProperty;
class StringProperty;
class TfFrameProperty;
}

namespace magic_window_rviz_plugin
{

class MagicWindowVisual;

class MagicWindowDisplay: public rviz::MessageFilterDisplay<sensor_msgs::Image>
{
Q_OBJECT
public:
  MagicWindowDisplay();
  virtual ~MagicWindowDisplay();
protected:
  virtual void onInitialize();
  virtual void reset();
private Q_SLOTS:
  void updateColorAndAlpha();
  void updateHistoryLength();
  void updateScale();
  void updateImage();
  void updateImageTopic();
  void updatePosition(const ros::TimerEvent& event);
private:
  void processMessage( const sensor_msgs::Image::ConstPtr& msg );
  // Variables
  // boost::circular_buffer<boost::shared_ptr<MagicWindowVisual> > visuals_;
  boost::shared_ptr<MagicWindowVisual> static_visual_;
  // rviz::VectorProperty* scale_property_;
  rviz::FloatProperty* scale_property_;
  rviz::TfFrameProperty* tf_frame_property_;
  rviz::StringProperty* image_file_property_;
  ros::Timer update_timer_;
};

} // end namespace magic_window_rviz_plugin

#endif // MAGICWINDOW_DISPLAY_H
