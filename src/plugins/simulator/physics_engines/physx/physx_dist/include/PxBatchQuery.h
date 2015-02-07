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


#ifndef PX_PHYSICS_NX_SCENEQUERY
#define PX_PHYSICS_NX_SCENEQUERY
/** \addtogroup scenequery 
@{ */

#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxPhysXConfig.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxShape.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxBatchQueryDesc.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxQueryReport.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/PxQueryFiltering.h>

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxBoxGeometry;
class PxSphereGeometry;
struct PxQueryCache;

/**
\brief Batched queries object. This is used to perform several queries at the same time. 

@see PxScene, PxScene.createBatchQuery
*/
class PxBatchQuery
{
	public:

	/**
	\brief Executes batched queries.
	*/
	virtual	void							execute() = 0;

	/**
	\brief Gets the prefilter shader in use for this scene query.

	\return Prefilter shader.

	@see PxBatchQueryDesc.preFilterShade PxBatchQueryPreFilterShader
	*/
	virtual	PxBatchQueryPreFilterShader getPreFilterShader() const = 0;

	/**
	\brief Gets the postfilter shader in use for this scene query.

	\return Postfilter shader.

	@see PxBatchQueryDesc.preFilterShade PxBatchQueryPostFilterShader
	*/
	virtual	PxBatchQueryPostFilterShader getPostFilterShader() const = 0;


	/**
	\brief Gets the shared global filter data in use for this scene query.

	\return Shared filter data for filter shader.

	@see getFilterShaderDataSize() PxBatchQueryDesc.filterShaderData PxBatchQueryPreFilterShader, PxBatchQueryPostFilterShader
	*/
	virtual	const void*						getFilterShaderData() const	= 0;

	/**
	\brief Gets the size of the shared global filter data (#PxSceneDesc.filterShaderData)

	\return Size of shared filter data [bytes].

	@see getFilterShaderData() PxBatchQueryDesc.filterShaderDataSize PxBatchQueryPreFilterShader, PxBatchQueryPostFilterShader
	*/
	virtual	PxU32							getFilterShaderDataSize() const	= 0;


	/**
	\brief Retrieves the client specified with PxBatchQueryDesc::ownerClient at creation time.

	It is not possible to change this value after creating the scene query.

	@see PxBatchQueryDesc::ownerClient
	*/
	virtual PxClientID						getOwnerClient() const = 0;

	/**
 	\brief Sets new user memory pointers.
 
 	It is not possible to change the memory during query execute.
 
 	@see PxBatchQueryDesc
 	*/
 	virtual	void							setUserMemory(const PxBatchQueryMemory&) = 0;

	/**
 	\brief Gets the user memory pointers. 	
 
 	@see PxBatchQueryDesc
 	*/
 	virtual	const PxBatchQueryMemory&		getUserMemory() = 0;

	/**
	\brief Releases PxBatchQuery from PxScene

	@see PxScene, PxScene.createBatchQuery
	*/
	virtual	void							release() = 0;


	/**
	\brief PS3 only. Enables or disables SPU execution for this query.

	@see PxBatchQueryDesc
	*/
	virtual void							setRunOnSpu(bool runOnSpu) = 0;

	/**
	\brief PS3 only. Returns true if this query should run on SPU.

	@see PxBatchQueryDesc
	*/
	virtual bool							getRunOnSpu() = 0;

	/**
	\brief Find all objects which a ray intersects.

	hintFlags is a combination of #PxHitFlag flags.

	\note Make certain that the direction vector of the ray is normalized.

	\note Shooting a ray from within an object leads to different results depending on the shape type. Please check the details in article SceneQuery. User can ignore such objects by using one of the provided filter mechanisms.

	\param[in] origin		Origin of the ray.
	\param[in] unitDir		Normalized direction of the ray.
	\param[in] distance		Length of the ray. Needs to be larger than 0.
	\param[in] maxTouchHits	Maximum number of hits to record in the touch buffer for this query. Default=0 reports a single blocking hit. If maxTouchHits is set to 0 all hits are treated as blocking by default.
	\param[in] hitFlags		Specify which properties per hit should be computed and saved in hit array and blocking hit. Combination of #PxHitFlag flags
	\param[in] filterData	filterData which is passed to the filer shader. See #PxQueryFilterData #PxBatchQueryPreFilterShader, #PxBatchQueryPostFilterShader
	\param[in] userData		user can assign this to a value of his choice, usually to identify this particular query
	\param[in] cache		Cached hit shape (optional). Query is tested against cached shape first. If no hit is found the ray gets queried against the scene.
							Note: Filtering is not executed for a cached shape if supplied; instead, if a hit is found, it is assumed to be a blocking hit.
							Note: Using past touching hits as cache will produce incorrect behavior since the cached hit will always be treated as blocking.
	
	@see PxQueryFilterData PxBatchQueryPreFilterShader PxBatchQueryPostFilterShader PxRaycastHit PxScene::raycast
	*/
	virtual void raycast(
		const PxVec3& origin, const PxVec3& unitDir, PxReal distance = PX_MAX_F32, PxU16 maxTouchHits = 0,
		PxHitFlags hitFlags = PxHitFlag::ePOSITION|PxHitFlag::eNORMAL|PxHitFlag::eDISTANCE,
		const PxQueryFilterData& filterData = PxQueryFilterData(),
		void* userData = NULL, const PxQueryCache* cache = NULL) const = 0;


	/**
	\brief Test overlap between a geometry and objects in the scene. Returns all objects which overlap the geometry object.
	
	\note Filtering: Overlap tests do not distinguish between touching and blocking hit types (see #PxQueryHitType). Both get written to the hit buffer.

	\param[in] geometry		Geometry of object to check for overlap (supported types are: box, sphere, capsule, convex).
	\param[in] pose			Pose of the object.
	\param[in] maxTouchHits	Maximum number of hits to record in the touch buffer for this query. Default=0 reports a single blocking hit. If maxTouchHits is set to 0 all hits are treated as blocking by default.
	\param[in] filterData	Filtering data and simple logic. See #PxQueryFilterData #PxBatchQueryPreFilterShader, #PxBatchQueryPostFilterShader
	\param[in] userData		user can assign this to a value of his choice, usually to identify this particular query
	\param[in] cache		Cached hit shape (optional). Query is tested against cached shape first. If no hit is found the ray gets queried against the scene.
							Note: Filtering is not executed for a cached shape if supplied; instead, if a hit is found, it is assumed to be a blocking hit.
							Note: Using past touching hits as cache will produce incorrect behavior since the cached hit will always be treated as blocking.

	@see PxQueryFilterData PxBatchQueryPreFilterShader PxBatchQueryPostFilterShader 
	*/
	virtual void overlap(
		const PxGeometry& geometry, const PxTransform& pose, PxU16 maxTouchHits = 0,
		const PxQueryFilterData& filterData = PxQueryFilterData(), void* userData=NULL, const PxQueryCache* cache = NULL) const = 0;

	/**
	\brief Sweep returning multiple hits.
	
	Find all rigid actors that get hit along the sweep. Each hit contains data as specified by the outputFlags field.

	\note Touching hits are not ordered.

	\note If a shape from the scene is already overlapping with the query shape in its starting position, the hit is returned unless eASSUME_NO_INITIAL_OVERLAP was specified.

	\param[in] geometry		Geometry of object to sweep (supported types are: box, sphere, capsule, convex).
	\param[in] pose			Pose of the sweep object.
	\param[in] unitDir		Normalized direction of the sweep.
	\param[in] distance		Sweep distance. Needs to be larger than 0. Will be clamped to PX_MAX_SWEEP_DISTANCE.
	\param[in] maxTouchHits	Maximum number of hits to record in the touch buffer for this query. Default=0 reports a single blocking hit. If maxTouchHits is set to 0 all hits are treated as blocking by default.
	\param[in] hitFlags		Specify which properties per hit should be computed and written to touch hit array and blocking hit. Combination of #PxHitFlag flags
	\param[in] filterData	Filtering data and simple logic.
	\param[in] userData user can assign this to a value of his choice, usually to identify this particular query
	\param[in] cache		Cached hit shape (optional). Query is tested against cached shape first. If no hit is found the ray gets queried against the scene.
							Note: Filtering is not executed for a cached shape if supplied; instead, if a hit is found, it is assumed to be a blocking hit.
							Note: Using past touching hits as cache will produce incorrect behavior since the cached hit will always be treated as blocking.
	\param[in] inflation	This parameter creates a skin around the swept geometry which increases its extents for sweeping. The sweep will register a hit as soon as the skin touches a shape, and will return the corresponding distance and normal.
							Note: ePRECISE_SWEEP doesn't support inflation. Therefore the sweep will be performed with zero inflation.

	@see PxHitFlags PxQueryFilterData PxBatchQueryPreFilterShader PxBatchQueryPostFilterShader PxSweepHit
	*/
	virtual void sweep(
		const PxGeometry& geometry, const PxTransform& pose, const PxVec3& unitDir, const PxReal distance,
		PxU16 maxTouchHits = 0, PxHitFlags hitFlags = PxHitFlag::ePOSITION|PxHitFlag::eNORMAL|PxHitFlag::eDISTANCE,
		const PxQueryFilterData& filterData = PxQueryFilterData(), void* userData=NULL, const PxQueryCache* cache = NULL,
		const PxReal inflation = 0.f) const = 0;

protected:
	virtual	~PxBatchQuery() {}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
