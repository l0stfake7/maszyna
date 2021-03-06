﻿#pragma once

#include <vector>
#include "dumb3d.h"
#include "float3d.h"
#include "dynobj.h"

// collection of virtual light sources present in the scene
// used by the renderer to determine most suitable placement for actual light sources during render
struct light_array {

public:
// types
    struct light_record {

        light_record( TDynamicObject const *Owner, int const Lightindex) :
                                      owner(Owner),    index(Lightindex)
        {};

        TDynamicObject const *owner; // the object in world which 'carries' the light
        int index{ -1 }; // 0: front lights, 1: rear lights
        Math3D::vector3 position; // position of the light in 3d scene
        Math3D::vector3 direction; // direction of the light in 3d scene
        float3 color{ 255.0f / 255.0f, 241.0f / 255.0f, 224.0f / 255.0f }; // color of the light, default is halogen light
        float intensity{ 0.0f }; // (combined) intensity of the light(s)
        int count{ 0 }; // number (or pattern) of active light(s)
    };

// methods
    // adds records for lights of specified owner to the collection
    void
        insert( TDynamicObject const *Owner );
    // removes records for lights of specified owner from the collection
    void
        remove( TDynamicObject const *Owner );
    // updates records in the collection
    void
        update();

// types
    typedef std::vector<light_record> lightrecord_array;

// members
    lightrecord_array data;
};
