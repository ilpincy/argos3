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

#ifndef PX_VEHICLE_NO_DRIVE_H
#define PX_VEHICLE_NO_DRIVE_H
/** \addtogroup vehicle
  @{
*/

#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleWheels.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleComponents.h>


#ifndef PX_DOXYGEN
namespace physx
{
#endif

struct PxFilterData;
class PxGeometry;
class PxPhysics;
class PxBatchQuery;
struct PxRaycastQueryResult;
class PxVehicleDrivableSurfaceToTireFrictionPairs;
class PxShape;
class PxMaterial;
class PxRigidDynamic;

/**
\brief Data structure with instanced dynamics data and configuration data of a vehicle with no drive model.
*/
class PxVehicleNoDrive : public PxVehicleWheels
{
public:

	friend class PxVehicleUpdate;

	/**
	\brief Allocate a PxVehicleNoDrive instance for a vehicle without drive model with numWheels
	@see free, setup
	*/
	static PxVehicleNoDrive* allocate(const PxU32 numWheels);

	/**
	\brief Deallocate a PxVehicleNoDrive instance.
	@see allocate
	*/
	void free();

	/**
	\brief Set up a PxVehicleNoDrive instance with 
	\brief (i)	a PxPhysics instance - needed to setup special vehicle constraints maintained by the vehicle.
	\brief (ii)	a PxRigidDynamic instance - the rigid body representation of the vehicle in the physx sdk.
	\brief (iii)a PxVehicleWheelsSimData instance describing the wheel/suspension/tires - the vehicle instance takes a copy of this input data.
	@see allocate, free, setToRestState
	*/
	void setup
		(PxPhysics* physics, PxRigidDynamic* vehActor, const PxVehicleWheelsSimData& wheelsData);

	/**
	\brief Create a PxVehicleNoDrive instance with 
	\brief (i)	a PxPhysics instance - needed to setup special vehicle constraints maintained by the vehicle.
	\brief (ii)	a PxRigidDynamic instance - the rigid body representation of the vehicle in the physx sdk.
	\brief (iii)a PxVehicleWheelsSimData instance describing the wheel/suspension/tires - the vehicle instance takes a copy of this input data.
	@see free, setToRestState, setup
	*/
	static PxVehicleNoDrive* create
		(PxPhysics* physics, PxRigidDynamic* vehActor, const PxVehicleWheelsSimData& wheelsData);

	/**
	\brief Set a vehicle to its rest state.
	\brief Call after setup
	@see setup
	*/
	void setToRestState();

	/**
	\brief Set the brake torque to be applied to a specific wheel
	*/
	void setBrakeTorque(const PxU32 id, const PxReal brakeTorque);

	/**
	\brief Set the drive torque to be applied to a specific wheel
	*/
	void setDriveTorque(const PxU32 id, const PxReal driveTorque);

	/**
	\brief Set the steer angle to be applied to a specific wheel
	*/
	void setSteerAngle(const PxU32 id, const PxReal steerAngle);

	/**
	\brief Get the brake torque that has been applied to a specific wheel
	*/
	PxReal getBrakeTorque(const PxU32 id) const;

	/**
	\brief Get the drive torque that has been applied to a specific wheel
	*/
	PxReal getDriveTorque(const PxU32 id) const;

	/**
	\brief Get the steer angle that has been applied to a specific wheel
	*/
	PxReal getSteerAngle(const PxU32 id) const;


private:

	PxVehicleNoDrive()
	{
	}

	~PxVehicleNoDrive()
	{
	}

	PxReal* mSteerAngles;
	PxReal* mDriveTorques;
	PxReal* mBrakeTorques;
	PxReal* mPad;

	/**
	\brief Test if the instanced dynamics and configuration data has legal values.
	*/
	bool isValid() const;
};
PX_COMPILE_TIME_ASSERT(0==(sizeof(PxVehicleNoDrive) & 15));

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif //PX_VEHICLE_NO_DRIVE_H
