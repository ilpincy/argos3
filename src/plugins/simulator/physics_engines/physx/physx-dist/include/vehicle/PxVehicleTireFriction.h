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

#ifndef PX_VEHICLE_TIREFRICTION_H
#define PX_VEHICLE_TIREFRICTION_H
/** \addtogroup vehicle
  @{
*/

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxSimpleTypes.h>

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxMaterial;

/**
\brief Driving surface type 
\brief Each PxMaterial is given a corresponding PxVehicleDrivableSurfaceType.
@see PxMaterial, PxVehicleDrivableSurfaceToTireFrictionPairs
*/	
struct PxVehicleDrivableSurfaceType
{
	enum
	{
		eSURFACE_TYPE_UNKNOWN=0xffffffff
	};
	PxU32 mType;
};

/**
\brief Friction for each combination of driving surface type and tire type.
*/
class PxVehicleDrivableSurfaceToTireFrictionPairs
{
public:

	friend class VehicleSurfaceTypeHashTable;

	enum
	{
		eMAX_NUM_SURFACE_TYPES=256
	};

	/**
	\brief Allocate the memory for a PxVehicleDrivableSurfaceToTireFrictionPairs instance
	that can hold data for combinations of tire type and surface type up to maxNumTireTypes and maxNumSurfaceTypes.
	\brief maxNumSurfaceTypes must be less than or equal to eMAX_NUM_SURFACE_TYPES
	@see setup
	*/
	static PxVehicleDrivableSurfaceToTireFrictionPairs* allocate
		(const PxU32 maxNumTireTypes, const PxU32 maxNumSurfaceTypes);

	/**
	\brief Set up a PxVehicleDrivableSurfaceToTireFrictionPairs instance for combinations of numTireTypes tire types and numSurfaceTypes surface types.
	\brief numTireTypes must be less than or equal to maxNumTireTypes specified in allocate().
	\brief numSurfaceTypes must be less than or equal to maxNumSurfaceTypes specified in allocate().
	\brief drivableSurfaceMaterials is an array of numSurfaceTypes materials.
	\brief drivableSurfaceTypes is an array of numSurfaceTypes types.
	\brief If the PxMaterial that touches the tire corresponds to drivableSurfaceMaterials[x] then the drivable surface
	\brief type is drivableSurfaceTypes[x].mType
	\brief The friction of each surface-type/tire-type pair is determined by the corresponding value set by setTypePairFriction.
	\brief A friction value of 1.0 will be assigned to each combination of tire and surface type.  To override this use setTypePairFriction.
	@see release, setTypePairFriction, getTypePairFriction, PxVehicleTireData.mType
	*/
	void setup
		(const PxU32 numTireTypes, const PxU32 numSurfaceTypes, 
		 const PxMaterial** drivableSurfaceMaterials, const PxVehicleDrivableSurfaceType* drivableSurfaceTypes);

	/**
	\brief Deallocate a PxVehicleDrivableSurfaceToTireFrictionPairs instance
	*/
	void release();

	/**
	\brief Return the friction for a specified pair of tire/drivable surface type pair.
	*/
	void setTypePairFriction(const PxU32 surfaceType, const PxU32 tireType, const PxReal value);

	/**
	\brief Return the friction for a specified pair of tire/drivable surface type pair.
	*/
	PxReal getTypePairFriction(const PxU32 surfaceType, const PxU32 tireType) const;

	/**
	\brief Return the maximum number of surface types
	@see allocate
	*/
	PxU32 getMaxNumSurfaceTypes() const {return mMaxNumSurfaceTypes;}

	/**
	\brief Return the maximum number of tire types
	@see allocate
	*/
	PxU32 getMaxNumTireTypes() const {return mMaxNumTireTypes;}

private:

	/**
	\brief Ptr to base address of a 2d PxReal array with dimensions [mNumSurfaceTypes][mNumTireTypes]
	\brief Each element of the array describes the maximum friction provided by a surface type-tire type combination.
	\brief eg the friction corresponding to a combination of surface type x and tire type y is  mPairs[x][y]
	*/
	PxReal* mPairs;					

	/** 
	\brief Ptr to 1d array of material ptrs that is of length mNumSurfaceTypes.
	\brief If the PxMaterial that touches the tire corresponds to mDrivableSurfaceMaterials[x] then the drivable surface
	\brief type is mDrivableSurfaceTypes[x].mType and the friction for that contact is 
	\brief mPairs[mDrivableSurfaceTypes[x].mType][y], assuming a tire type y.
	\brief If the PxMaterial that touches the tire is not found in mDrivableSurfaceMaterials then the friction is 
	\brief  mPairs[0][y], assuming a tire type y.
	*/
	const PxMaterial** mDrivableSurfaceMaterials;

	/**
	\brief Ptr to 1d array of PxVehicleDrivableSurfaceType that is of length mNumSurfaceTypes.
	\brief If the PxMaterial that touches the tire is found in mDrivableSurfaceMaterials[x] then the drivable surface
	\brief type is mDrivableSurfaceTypes[x].mType and the friction for that contact is 
	\brief mPairs[mDrivableSurfaceTypes[x].mType][y], assuming a tire type y.
	\brief If the PxMaterial that touches the tire is not found in mDrivableSurfaceMaterials then the friction is 
	\brief  mPairs[0][y], assuming a tire type y.
	*/
	PxVehicleDrivableSurfaceType* mDrivableSurfaceTypes;

	/**
	\brief Number of different driving surface types.
	\brief mDrivableSurfaceMaterials and mDrivableSurfaceTypes are both 1d arrays of length mMaxNumSurfaceTypes.
	\brief mNumSurfaceTypes must be less than or equal to mMaxNumSurfaceTypes.
	*/	
	PxU32 mNumSurfaceTypes;			

	/**
	\brief Maximum number of different driving surface types.
	\brief mMaxNumSurfaceTypes must be less than or equal to eMAX_NUM_SURFACE_TYPES.
	*/	
	PxU32 mMaxNumSurfaceTypes;			

	/**
	\brief Number of different tire types.
	\brief Tire types stored in PxVehicleTireData.mType
	*/	
	PxU32 mNumTireTypes;			

	/**
	\brief Maximum number of different tire types.
	\brief Tire types stored in PxVehicleTireData.mType
	*/	
	PxU32 mMaxNumTireTypes;			


#ifndef PX_X64
	PxU32 mPad[1];
#else
	PxU32 mPad[2];
#endif

	PxVehicleDrivableSurfaceToTireFrictionPairs(){}
	~PxVehicleDrivableSurfaceToTireFrictionPairs(){}
};
PX_COMPILE_TIME_ASSERT(0==(sizeof(PxVehicleDrivableSurfaceToTireFrictionPairs) & 15));

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif //PX_VEHICLE_TIREFRICTION_H
