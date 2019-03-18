/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>

///Collision Shapes
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btCylinderShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btConeShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btCompoundShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btEmptyShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h>

///Narrowphase Collision Detector
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h>

//#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>

///Dispatching and generation of collision pairs (broadphase)
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>

///Math library & Utils
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btQuaternion.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btTransform.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btDefaultMotionState.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btQuickprof.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btIDebugDraw.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/LinearMath/btSerializer.h>

#endif  //BULLET_COLLISION_COMMON_H
