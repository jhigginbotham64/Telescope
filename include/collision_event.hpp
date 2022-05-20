// 
// Copyright 2022 Joshua Higginbotham
// Created on 08.05.22 by clem (mail@clemens-cords.com)
//

#pragma once

extern "C"
{
    /// \brief event used to check whether two objects are colliding
    struct TS_CollisionEvent
    {
      /// \brief id of first object
      int id1;

      /// \brief id of second object
      int id2;

      /// \brief true if objects collide, false otherwise
      bool colliding;
    };
}