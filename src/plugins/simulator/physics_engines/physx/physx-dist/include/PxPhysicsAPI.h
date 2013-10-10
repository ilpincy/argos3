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
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/Px.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxAllocatorCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxAssert.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxBitAndData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxBounds3.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxBroadcastingAllocator.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxErrorCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxErrors.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxFoundation.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxIntrinsics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxMat33.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxMat44.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxMath.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxPlane.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxPreprocessor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxQuat.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxSimpleTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxStrideIterator.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxString.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxTransform.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxUnionCast.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxVec2.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxVec3.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxVec4.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/foundation/PxVersionNumber.h>

//Not physics specific utilities and common code
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxCoreUtilities.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxCoreUtilityTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxFieldDescriptor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxFields.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxIO.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxMathUtils.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxPhysXCommon.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxRenderBuffer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxSerialFramework.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxTolerancesScale.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/common/PxTypeInfo.h>

//Profiling 
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileBase.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileCompileTimeEventFilter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileContextProvider.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventBufferClient.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventBufferClientManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventFilter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventHandler.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventId.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventMutex.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventNames.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEvents.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventSender.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileEventSystem.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileMemoryEventTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileScopedEvent.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileZone.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxprofilesdk/PxProfileZoneManager.h>

//Connecting to Visual Debugger Directly
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdBits.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdConnection.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdConnectionFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdConnectionManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdDataStream.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdDataStreamHelpers.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdErrorCodes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdImmediateRenderer.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdNetworkStreams.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdObjectModelBaseTypeDefs.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdObjectModelBaseTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/physxvisualdebuggersdk/PvdObjectModelMetaData.h>

//Connecting the SDK to Visual Debugger
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pvd/PxVisualDebugger.h>

//Task Manager
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxCpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxCudaContextManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxCudaMemoryManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxGpuCopyDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxGpuCopyDescQueue.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxGpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxGpuTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxSpuDispatcher.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxSpuTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxTask.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/pxtask/PxTaskManager.h>


//Geometry Library
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxBoxGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxCapsuleGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxConvexMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxConvexMeshGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxGeometryHelpers.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxGeometryQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxHeightField.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxHeightFieldDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxHeightFieldFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxHeightFieldGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxHeightFieldSample.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxMeshQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxMeshScale.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxPhysXGeomUtils.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxPlaneGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxSimpleTriangleMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxSphereGeometry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxTriangle.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxTriangleMesh.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/geometry/PxTriangleMeshGeometry.h>


// PhysX Core SDK
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxActor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxAggregate.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxArticulation.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxArticulationJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxArticulationLink.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxBatchQuery.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxBatchQueryDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxClient.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxConstraint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxConstraintDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxContact.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxContactModifyCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxFiltering.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxForceMode.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxLockedData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxMaterial.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxMaterialFlags.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxObserver.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxPhysics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxPhysX.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxRigidActor.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxRigidBody.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxRigidDynamic.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxRigidStatic.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxScene.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSceneDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSceneLock.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSceneQueryFiltering.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSceneQueryReport.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxShape.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSimulationEventCallback.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSimulationStatistics.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxStringTable.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxSweepCache.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxVisualizationParameter.h>

//Character Controller
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxBoxController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxCapsuleController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxCharacter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxController.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxControllerBehavior.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxControllerManager.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxControllerObstacles.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/characterkinematic/PxExtended.h>

//Cloth Simulation
#if PX_USE_CLOTH_API
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxCloth.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxClothCollisionData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxClothFabric.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxClothFabricTypes.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxClothReadData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cloth/PxClothTypes.h>
#endif

//Cooking (data preprocessing)
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxBinaryConverter.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/Pxc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxConvexMeshDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxCooking.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxGaussMapLimit.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxTriangleMeshDesc.h>
#if PX_USE_CLOTH_API
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/cooking/PxClothMeshDesc.h>
#endif

//Extensions to the SDK
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDefaultStreams.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxDistanceJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxExtensionsAPI.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxFixedJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxJointLimit.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxJointRepXExtensions.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxParticleExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxPrismaticJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxRevoluteJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxRigidBodyExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxShapeExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSimpleFactory.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSmoothNormals.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxSphericalJoint.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxStringTableExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxTriangleMeshExt.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/extensions/PxVisualDebuggerExt.h>

//Particle Simulation
#if PX_USE_PARTICLE_SYSTEM_API
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleBase.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleBaseFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleCreationData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleFlag.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleFluid.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleFluidReadData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleReadData.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/particles/PxParticleSystem.h>
#endif

//RepX XML data format
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/RepX/RepX.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/RepX/RepXCoreExtensions.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/RepX/RepXUtility.h>

//RepX XML data format version conversion
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/RepXUpgrader/RepXUpgrader.h>

//Vehicle Simulation
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleComponents.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleDrive.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleDrive4W.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleDriveTank.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleSDK.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleShaders.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleTireFriction.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleUpdate.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleUtilControl.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleUtilSetup.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleUtilTelemetry.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/vehicle/PxVehicleWheels.h>

/** @} */
#endif
