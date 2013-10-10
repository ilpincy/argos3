// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef PX_EXTENSIONS_API_H
#define PX_EXTENSIONS_API_H
/** \addtogroup extensions
  @{
*/

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxErrorCallback.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultAllocator.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxConstraintExt.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDistanceJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxFixedJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxPrismaticJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxRevoluteJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSphericalJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxD6Joint.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultSimulationFilterShader.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultErrorCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultStreams.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxRigidBodyExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxShapeExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxParticleExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxTriangleMeshExt.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultCpuDispatcher.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSmoothNormals.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSimpleFactory.h>

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxVisualDebuggerExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxStringTableExt.h>

#ifdef PX_PS3
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/ps3/PxPS3Extension.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/ps3/PxDefaultSpuDispatcher.h>
#endif

/** \brief Initialize the PhysXExtensions library. 

This should be called before calling any functions or methods in extensions which may require allocation. 
\note This function does not need to be called before creating a PxDefaultAllocator object.

\param physics a PxPhysics object

@see PxCloseExtensions PxFoundation PxPhysics
*/

PX_C_EXPORT bool PX_CALL_CONV PxInitExtensions(physx::PxPhysics& physics);

/** \brief Shut down the PhysXExtensions library. 

This function should be called to cleanly shut down the PhysXExtensions library before application exit. 

\note This function is required to be called to release foundation usage.

@see PxInitExtensions
*/

PX_C_EXPORT void PX_CALL_CONV PxCloseExtensions();

/** \brief Dumps the SDK's meta-data to a stream.

A meta-data file contains information about the SDK's internal classes. Such a file is needed to convert
binary-serialized SDK files from one platform to another. The converter needs meta-data files for the
source and target platforms to perform conversions.
*/

PX_C_EXPORT void PX_CALL_CONV PxDumpMetaData(physx::PxOutputStream& stream, const physx::PxPhysics& sdk);

/**
\brief Collects all PxPhysics-level objects for export.

This function adds all objects managed by the PxPhysics object to the input collection, which can
then be serialized. Collected objects include materials, convex meshes, triangle meshes, and heightfields.

This is a helper function to easily enumerate all objects managed by PxPhysics, but using it is not
mandatory. Users can also manually add a subset of those objects to the collection.

\param[in] sdk Physics SDK to retrieve the objects from. See #PxPhysics
\param[out] collection Collection to which objects will be added. See #PxCollection

@see PxCollection
*/
PX_C_EXPORT void PxCollectForExportSDK(const physx::PxPhysics& sdk, physx::PxCollection& collection);

/**
\brief Collects all PxScene-level objects for export.

This function adds all objects managed by the PxScene object to the input collection, which can
then be serialized. Collected objects include actors, constraints, articulations
and aggregates.

This is a helper function to easily enumerate all objects managed by PxScene, but using it is not
mandatory. Users can also manually add a subset of those objects to the collection.

\param[in] scene Scene to retrieve the objects from. See #PxScene
\param[out] collection Collection to which objects will be added. See #PxCollection

@see PxCollection
*/
PX_C_EXPORT void PxCollectForExportScene(const physx::PxScene& scene, physx::PxCollection& collection);

/** @} */
#endif // PX_EXTENSIONS_API_H
