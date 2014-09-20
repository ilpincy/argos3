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

#ifndef PX_VEHICLE_DRIVE_TANK_H
#define PX_VEHICLE_DRIVE_TANK_H
/** \addtogroup vehicle
  @{
*/

#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleDrive.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleWheels.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleComponents.h>


#ifndef PX_DOXYGEN
namespace physx
{
#endif

struct PxFilterData;
class PxGeometry;
class PxPhysics;
class PxBatchQuery;
class PxVehicleDrivableSurfaceToTireFrictionPairs;
class PxShape;
class PxMaterial;
class PxRigidDynamic;

/**
\brief The ordering of the wheels of a PxVehicleDriveTank.

@see PxVehicleWheelsSimData, PxVehicleWheelsDynData
*/
struct PxVehicleDriveTankWheelOrder
{
	enum Enum
	{
		eFRONT_LEFT=0,
		eFRONT_RIGHT,
		e1ST_FROM_FRONT_LEFT,
		e1ST_FROM_FRONT_RIGHT,
		e2ND_FROM_FRONT_LEFT,
		e2ND_FROM_FRONT_RIGHT,
		e3RD_FROM_FRONT_LEFT,
		e3RD_FROM_FRONT_RIGHT,
		e4TH_FROM_FRONT_LEFT,
		e4TH_FROM_FRONT_RIGHT,
		e5TH_FROM_FRONT_LEFT,
		e5TH_FROM_FRONT_RIGHT,
		e6TH_FROM_FRONT_LEFT,
		e6TH_FROM_FRONT_RIGHT,
		e7TH_FROM_FRONT_LEFT,
		e7TH_FROM_FRONT_RIGHT,
		e8TH_FROM_FRONT_LEFT,
		e8TH_FROM_FRONT_RIGHT,
		e9TH_FROM_FRONT_LEFT,
		e9TH_FROM_FRONT_RIGHT
	};
};


/**
\brief The control inputs for a PxVehicleDriveTank.

@see PxVehicleDriveDynData::setAnalogInput, PxVehicleDriveDynData::getAnalogInput
*/

struct PxVehicleDriveTankControl
{
	enum Enum
	{
		eANALOG_INPUT_ACCEL=0,
		eANALOG_INPUT_BRAKE_LEFT,	
		eANALOG_INPUT_BRAKE_RIGHT,	
		eANALOG_INPUT_THRUST_LEFT,	
		eANALOG_INPUT_THRUST_RIGHT,	
		eMAX_NB_DRIVETANK_ANALOG_INPUTS
	};
};

/**
\brief Two driving models are supported.

@see PxVehicleDriveTank::setDriveModel
*/
struct PxVehicleDriveTankControlModel
{
	enum Enum
	{
		eSTANDARD=0,
		eSPECIAL
	};
};


/**
\brief Data structure with instanced dynamics data and configuration data of a tank.
*/
class PxVehicleDriveTank : public PxVehicleDrive
{
public:

	friend class PxVehicleUpdate;


	/**
	\brief Allocate a PxVehicleTankDrive instance for a tank with nbWheels

	\param[in] nbWheels is the number of wheels on the vehicle.

	\note It is assumed that all wheels are driven wheels.

	\return The instantiated vehicle.

	@see free, setup
	*/
	static PxVehicleDriveTank* allocate(const PxU32 nbWheels);

	/**
	\brief Deallocate a PxVehicleDriveTank instance.
	@see allocate
	*/
	void free();

	/**
	\brief Set up a tank using simulation data for the wheels and drive model.
	\param[in] physics is a PxPhysics instance that is needed to create special vehicle constraints that are maintained by the vehicle.
	\param[in] vehActor is a PxRigidDynamic instance that is used to represent the tank in the PhysX SDK.
	\param[in] wheelsData describes the configuration of all suspension/tires/wheels of the tank. The tank instance takes a copy of this data.
	\param[in] driveData describes the properties of the tank's drive model (gears/engine/clutch/autobox).  The tank instance takes a copy of this data.
	\param[in] nbDrivenWheels is the number of wheels on the tank.
	\note It is assumed that the first shapes of the actor are the wheel shapes, followed by the chassis shapes.  To break this assumption use PxVehicleWheelsSimData::setWheelShapeMapping.
	@see allocate, free, setToRestState, PxVehicleWheelsSimData::setWheelShapeMapping
	*/
	void setup
		(PxPhysics* physics, PxRigidDynamic* vehActor, 
		 const PxVehicleWheelsSimData& wheelsData, const PxVehicleDriveSimData& driveData,
		 const PxU32 nbDrivenWheels);

	/**
	\brief Allocate and set up a tank using simulation data for the wheels and drive model.
	\param[in] physics is a PxPhysics instance that is needed to create special vehicle constraints that are maintained by the tank.
	\param[in] vehActor is a PxRigidDynamic instance that is used to represent the tank in the PhysX SDK.
	\param[in] wheelsData describes the configuration of all suspension/tires/wheels of the tank. The tank instance takes a copy of this data.
	\param[in] driveData describes the properties of the tank's drive model (gears/engine/clutch/differential/autobox).  The tank instance takes a copy of this data.
	\param[in] nbDrivenWheels is the number of wheels on the tank.
	\note It is assumed that the first shapes of the actor are the wheel shapes, followed by the chassis shapes.  To break this assumption use PxVehicleWheelsSimData::setWheelShapeMapping.
	\return The instantiated vehicle.
	@see allocate, free, setToRestState, PxVehicleWheelsSimData::setWheelShapeMapping
	*/
	static PxVehicleDriveTank* create
		(PxPhysics* physics, PxRigidDynamic* vehActor, 
		 const PxVehicleWheelsSimData& wheelsData, const PxVehicleDriveSimData& driveData,
		 const PxU32 nbDrivenWheels);

	/**
	\brief Set the control model used by the tank.
	\note eDRIVE_MODEL_STANDARD: turning achieved by braking on one side, accelerating on the other side.
	\note eDRIVE_MODEL_SPECIAL: turning achieved by accelerating forwards on one side, accelerating backwards on the other side.
	\note The default value is eDRIVE_MODEL_STANDARD
	*/
	void setDriveModel(const PxVehicleDriveTankControlModel::Enum driveModel)
	{
		mDriveModel=driveModel;
	}

	/**
	\brief Return the control model used by the tank.
	*/
	PxVehicleDriveTankControlModel::Enum getDriveModel() const {return mDriveModel;}

	/**
	\brief Set a vehicle to its rest state.  Aside from the rigid body transform, this will set the vehicle and rigid body 
	to the state they were in immediately after setup or create.
	\note Calling setToRestState invalidates the cached raycast hit planes under each wheel meaning that suspension line
	raycasts need to be performed at least once with PxVehicleSuspensionRaycasts before calling PxVehicleUpdates. 
	@see setup, create, PxVehicleSuspensionRaycasts, PxVehicleUpdates
	*/
	void setToRestState();

	/**
	\brief Simulation data that models vehicle components
	@see setup, create
	*/
	PxVehicleDriveSimData mDriveSimData;

private:
	/**
	\brief Test if the instanced dynamics and configuration data has legal values.
	*/
	bool isValid() const;

	/**
	\brief Drive model
	@see setDriveModel, getDriveModel, PxVehicleDriveTankControlModel
	*/
	PxVehicleDriveTankControlModel::Enum mDriveModel;

	PxU32 mPad[3];

//serialization
public:
											PxVehicleDriveTank(PxBaseFlags baseFlags) : PxVehicleDrive(baseFlags)			{}
	static		PxVehicleDriveTank*			createObject(PxU8*& address, PxDeserializationContext& context);
	static		void						getBinaryMetaData(PxOutputStream& stream);
	virtual		const char*					getConcreteTypeName()		const	{	return "PxVehicleDriveTank";	}
	virtual		bool						isKindOf(const char* name)	const	{	return !strcmp("PxVehicleDriveTank", name) || PxBase::isKindOf(name); }
protected:
											PxVehicleDriveTank();
											~PxVehicleDriveTank(){}
//~serialization
};
PX_COMPILE_TIME_ASSERT(0==(sizeof(PxVehicleDriveTank) & 15));

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif //PX_VEHICLE_DRIVE_TANK_H
