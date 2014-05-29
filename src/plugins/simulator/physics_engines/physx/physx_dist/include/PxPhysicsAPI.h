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


#ifndef PX_PHYSICS_NXPHYSICS_API
#define PX_PHYSICS_NXPHYSICS_API
/** \addtogroup physics
@{
*/

/**
This is the main include header for the Physics SDK, for users who
want to use a single #include file.

Alternatively, one can instead directly #include a subset of the below files.
*/

// Foundation SDK 
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/Px.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxAllocatorCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxAssert.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxBitAndData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxBounds3.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxBroadcastingAllocator.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxErrorCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxErrors.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxFoundation.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxIntrinsics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxMat33.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxMat44.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxMath.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxPlane.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxPreprocessor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxQuat.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxSimpleTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxStrideIterator.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxString.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxTransform.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxUnionCast.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxVec2.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxVec3.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxVec4.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/PxVersionNumber.h>

//Not physics specific utilities and common code
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxCoreUtilityTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxIO.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxMathUtils.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxPhysXCommonConfig.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxRenderBuffer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxBase.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxTolerancesScale.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxTypeInfo.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxStringTable.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxSerializer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxMetaData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxMetaDataFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/PxSerialFramework.h>

//Profiling 
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileBase.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileCompileTimeEventFilter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileContextProvider.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventBufferClient.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventBufferClientManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventFilter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventHandler.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventId.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventMutex.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventNames.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEvents.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventSender.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileEventSystem.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileMemoryEventTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileScopedEvent.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileZone.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/PxProfileZoneManager.h>

//Connecting to Visual Debugger Directly
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdBits.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdConnection.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdConnectionFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdConnectionManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdDataStream.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdDataStreamHelpers.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdErrorCodes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdImmediateRenderer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdNetworkStreams.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdObjectModelBaseTypeDefs.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdObjectModelBaseTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdObjectModelMetaData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/PvdSimpleErrorStream.h>

//Connecting the SDK to Visual Debugger
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pvd/PxVisualDebugger.h>

//Task Manager
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxCpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxCudaContextManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxCudaMemoryManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxGpuCopyDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxGpuCopyDescQueue.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxGpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxGpuTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxSpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxSpuTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/PxTaskManager.h>


//Geometry Library
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxBoxGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxCapsuleGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxConvexMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxConvexMeshGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxGeometryHelpers.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxGeometryQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxHeightField.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxHeightFieldDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxHeightFieldFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxHeightFieldGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxHeightFieldSample.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxMeshQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxMeshScale.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxPlaneGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxSimpleTriangleMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxSphereGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxTriangle.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxTriangleMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/PxTriangleMeshGeometry.h>


// PhysX Core SDK
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxActor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxAggregate.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxArticulation.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxArticulationJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxArticulationLink.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxBatchQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxBatchQueryDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxClient.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxConstraint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxConstraintDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxContact.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxContactModifyCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxDeletionListener.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxFiltering.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxForceMode.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxLockedData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxMaterial.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxPhysics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxPhysXConfig.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxQueryFiltering.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxQueryReport.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxRigidActor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxRigidBody.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxRigidDynamic.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxRigidStatic.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxScene.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxSceneDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxSceneLock.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxShape.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxSimulationEventCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxSimulationStatistics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxSpatialIndex.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxVisualizationParameter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxVolumeCache.h>

//Character Controller
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxBoxController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxCapsuleController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxCharacter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxControllerBehavior.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxControllerManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxControllerObstacles.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/PxExtended.h>

//Cloth Simulation
#if PX_USE_CLOTH_API
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/PxCloth.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/PxClothCollisionData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/PxClothFabric.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/PxClothParticleData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/PxClothTypes.h>
#endif

//Cooking (data preprocessing)
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/Pxc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/PxConvexMeshDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/PxCooking.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/PxGaussMapLimit.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/PxTriangleMeshDesc.h>

//Extensions to the SDK
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxDefaultStreams.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxDistanceJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxExtensionsAPI.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxFixedJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxJointLimit.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxParticleExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxPrismaticJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxRevoluteJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxRigidBodyExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxShapeExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxSimpleFactory.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxSmoothNormals.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxSphericalJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxStringTableExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxTriangleMeshExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxVisualDebuggerExt.h>

//Serialization
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxSerialization.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxBinaryConverter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxRepXSerializer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/PxJointRepXSerializer.h>

//Particle Simulation
#if PX_USE_PARTICLE_SYSTEM_API
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleBase.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleBaseFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleCreationData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleFluid.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleFluidReadData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleReadData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/PxParticleSystem.h>
#endif

//Vehicle Simulation
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleComponents.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleDrive.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleDrive4W.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleDriveTank.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleSDK.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleShaders.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleTireFriction.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleUpdate.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleUtilControl.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleUtilSetup.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleUtilTelemetry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleWheels.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleNoDrive.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/PxVehicleDriveNW.h>

/** @} */
#endif
