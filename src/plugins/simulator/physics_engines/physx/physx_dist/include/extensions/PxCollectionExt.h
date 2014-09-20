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


#ifndef PX_COLLECTION_EXT_H
#define PX_COLLECTION_EXT_H
/** \addtogroup extensions
@{
*/

#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxCollection.h>

#ifndef PX_DOXYGEN
namespace physx
{
#endif

	class PxCollectionExt
	{
	public:
		/**
		\brief Removes and releases all object from a collection.
		
		The Collection itself is not released.

		It is assumed that the application holds a reference to each of the objects in the collection, with the exception of objects that are 
		subordinate (PxBase::isSubordinate). This is for example not the case if the collection contains a shape that was created with PxRigidActor::createShape, 
		as the application gives up the reference to the shape with this call. Objects that violate this assumption need to be removed from the collection 
		prior to calling releaseObjects.

		\param[in] collection to remove and release all object from.
		*/
		static void	releaseObjects(PxCollection& collection);

		/**
		\brief Removes objects of a given type from a collection, potentially adding them to another collection.

		\param[in,out] collection Collection from which objects are removed
		\param[in] concreteType PxConcreteType of sdk objects that should be removed	
		\param[in,out] to Optional collection to which the removed objects are added

		@see PxCollection, PxConcreteType
		*/	
		static void remove(PxCollection& collection, PxType concreteType, PxCollection* to = NULL);


		/**
		\brief Collects all PxPhysics-level objects for serialization.

		This function adds all objects managed by the PxPhysics object to the input collection, which can
		then be serialized. Instances of the following types are included: PxConvexMesh, PxTriangleMesh, 
		PxHeightField, PxShape, PxMaterial and PxClothFabric.

		This is a helper function to easily enumerate all objects managed by PxPhysics. 
		Users can also manually add a subset of those objects to the collection.

		\param[in] physics Physics SDK to retrieve the objects from. See #PxPhysics
		\return Collection to which objects will be added. See #PxCollection

		@see PxCollection, PxPhysics
		*/
		static  PxCollection*	createCollection(PxPhysics& physics);
	
		/**
		\brief Collects all PxScene-level objects for serialization.

		This function adds all objects managed by the PxScene object to the input collection, which can
		then be serialized. Instances of the following types are included: PxActor, PxArticulation, 
		PxAggregate and PxJoint (other PxConstraint types are not included).
	
		This is a helper function to easily enumerate all objects managed by PxScene. 
		Users can also manually add a subset of those objects to the collection.

		\param[in] scene Scene to retrieve the objects from. See #PxScene
		\return Collection to which objects will be added. See #PxCollection

		@see PxCollection, PxScene
		*/
		static	PxCollection*	createCollection(PxScene& scene);
	};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
