/**
 * @file <argos3/plugins/simulator/physx_engines/physx/physx_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_engine.h"
#include "physx_model.h"

#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

#if defined(PX_LINUX)
#   include <malloc.h> // for memalign()
#else
#   include <cstdlib> // for malloc()
#endif

#include <typeinfo>

namespace argos {

   /****************************************/
   /****************************************/

   const Real DEFAULT_STATIC_FRICTION         = 0.7f;
   const Real DEFAULT_DYNAMIC_FRICTION        = 0.5f;
   const Real DEFAULT_RESTITUTION_COEFFICIENT = 0.1f;

   static const size_t CYLINDER_NUM_SLICES = 40;

   /****************************************/
   /****************************************/

   CPhysXEngine::CQueryIgnoreShapes::CQueryIgnoreShapes() {}

   void CPhysXEngine::CQueryIgnoreShapes::Ignore(physx::PxShape** ppc_shapes,
                                                 size_t un_num_shapes) {
      for(size_t i = 0; i < un_num_shapes; ++i) {
         Ignore(ppc_shapes[i]);
      }
   }

   physx::PxQueryHitType::Enum CPhysXEngine::CQueryIgnoreShapes::preFilter(const physx::PxFilterData&,
                                                                           const physx::PxShape* pc_shape,
                                                                           const physx::PxRigidActor*,
                                                                           physx::PxHitFlags&) {
      return
         (std::find(m_vecIgnoredShapes.begin(), m_vecIgnoredShapes.end(), pc_shape) == m_vecIgnoredShapes.end()) ?
         physx::PxQueryHitType::eTOUCH :
         physx::PxQueryHitType::eNONE;
   }

   physx::PxQueryHitType::Enum CPhysXEngine::CQueryIgnoreShapes::postFilter(const physx::PxFilterData&,
                                                                            const physx::PxQueryHit&) {
      return physx::PxQueryHitType::eBLOCK;
   }

   /****************************************/
   /****************************************/

   void* CPhysXEngine::CPhysXEngineAllocatorCallback::allocate(size_t size, const char*, const char*, int) {
#if defined(PX_LINUX)
      /* On 32-bit Linux you must use memalign */
      void* pt_ptr = ::memalign(16, size);
#else
      /* On 64-bit Linux you must use malloc */
      void* pt_ptr = ::malloc(size);
#endif
      PX_ASSERT((reinterpret_cast<size_t>(pt_ptr) & 15)==0);
      return pt_ptr;
   }

   void	CPhysXEngine::CPhysXEngineAllocatorCallback::deallocate(void* pt_ptr) {
      ::free(pt_ptr);
   }

   /****************************************/
   /****************************************/

   CPhysXEngine::CPhysXEngineErrorCallback::CPhysXEngineErrorCallback(CPhysXEngine& c_engine) :
      m_cEngine(c_engine) {
   }

   void CPhysXEngine::CPhysXEngineErrorCallback::reportError(physx::PxErrorCode::Enum,
                                                             const char* pch_message,
                                                             const char* pch_file,
                                                             int n_line) {
      THROW_ARGOSEXCEPTION("Error in PhysX engine \"" <<
                           m_cEngine.GetId() <<
                           "\": " <<
                           pch_message <<
                           " [" <<
                           pch_file <<
                           ":" <<
                           n_line <<
                           "]");
   }

   /****************************************/
   /****************************************/

   static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes c_attributes0,
                                            physx::PxFilterData c_filter_data0, 
                                            physx::PxFilterObjectAttributes c_attributes1,
                                            physx::PxFilterData c_filter_data1,
                                            physx::PxPairFlags& c_pair_flags,
                                            const void* pt_constant_block,
                                            physx::PxU32 un_constant_block_size) {
      /*
       * The shader filter function may not reference any memory other than arguments of the function
       * and its own local stack variables -- because the function may be compiled and executed on a
       * remote processor.
       */
      /* Let triggers through */
      if(physx::PxFilterObjectIsTrigger(c_attributes0) ||
         physx::PxFilterObjectIsTrigger(c_attributes1)) {
         c_pair_flags = physx::PxPairFlag::eTRIGGER_DEFAULT;
         return physx::PxFilterFlag::eDEFAULT;
      }
      /* Generate contacts for all that were not filtered above */
      c_pair_flags = physx::PxPairFlag::eRESOLVE_CONTACTS;
      c_pair_flags |= physx::PxPairFlag::eCCD_LINEAR;
      return physx::PxFilterFlag::eDEFAULT;
   }
   
   /****************************************/
   /****************************************/

   CPhysXEngine::CPhysXEngine() :
      m_unSubdivBPRegions(4),
      m_cErrorCallback(*this),
      m_pcFoundation(NULL),
      m_pcPhysics(NULL),
      m_pcCooking(NULL),
      m_pcCPUDispatcher(NULL),
      m_pcSceneDesc(NULL),
      m_pcScene(NULL),
      m_pcDefaultMaterial(NULL),
      m_pcGroundBody(NULL),
      m_pcGroundShape(NULL) {
   }

   /****************************************/
   /****************************************/

   CPhysXEngine::~CPhysXEngine() {
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CPhysicsEngine::Init(t_tree);
         /*
          * Parse XML
          */
         UInt32 unThreads = 0;
         GetNodeAttributeOrDefault(t_tree, "cpu_threads", unThreads, unThreads);
         if(unThreads > 0) {
            LOG << "[INFO] PhysX engine \""
                << GetId()
                << "\" will use "
                << unThreads
                << " CPU threads internally"
                << std::endl;
         }
         else {
            LOG << "[INFO] PhysX engine \""
                << GetId()
                << "\" won't use extra threads internally"
                << std::endl;
         }
         GetNodeAttributeOrDefault(t_tree, "subdiv_bp_regions", m_unSubdivBPRegions, m_unSubdivBPRegions);
         if((m_unSubdivBPRegions < 1) ||
            (m_unSubdivBPRegions > 16)) {
            THROW_ARGOSEXCEPTION("The accepted range for subdivision of broad-phase regions is [1-16]; subdiv_bp_regions was set to " << m_unSubdivBPRegions);
         }
         /*
          * Init PhysX
          */
         /* Create foundation */
         m_pcFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_cAllocatorCallback, m_cErrorCallback);
         if(m_pcFoundation == NULL) {
            THROW_ARGOSEXCEPTION("Error calling PxCreateFoundation()");
         }
         /* Create physics, using default tolerance */
         m_pcPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pcFoundation, physx::PxTolerancesScale());
         if(m_pcPhysics == NULL) {
            THROW_ARGOSEXCEPTION("Error calling PxCreatePhysics()");
         }
         /* Create cooking subsystem for convex meshes */
         m_pcCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pcFoundation,
                                       physx::PxCookingParams(m_pcPhysics->getTolerancesScale()));
         if (m_pcCooking == NULL) {
            THROW_ARGOSEXCEPTION("Error calling PxCreateCooking()");
         }
         /* Create extension subsystem */
         if (!PxInitExtensions(*m_pcPhysics)) {
            THROW_ARGOSEXCEPTION("Error calling PxInitExtensions()");
         }
         /* Create CPU dispatcher */
         m_pcCPUDispatcher = physx::PxDefaultCpuDispatcherCreate(unThreads);
         if (m_pcCPUDispatcher == NULL) {
            THROW_ARGOSEXCEPTION("Error calling PxDefaultCpuDispatcherCreate(" << unThreads << ")");
         }
         /* Create scene descriptor */
         m_pcSceneDesc = new physx::PxSceneDesc(m_pcPhysics->getTolerancesScale());
         m_pcSceneDesc->gravity = physx::PxVec3(0.0f, 0.0f, -9.81f);
         m_pcSceneDesc->cpuDispatcher = m_pcCPUDispatcher;
         m_pcSceneDesc->flags |= physx::PxSceneFlag::eENABLE_CCD;
         m_pcSceneDesc->filterShader = FilterShader;
         m_pcSceneDesc->broadPhaseType = physx::PxBroadPhaseType::eMBP;
         /* Create scene */
         m_pcScene = m_pcPhysics->createScene(*m_pcSceneDesc);
         /* Create default material */
         m_pcDefaultMaterial = m_pcPhysics->createMaterial(DEFAULT_STATIC_FRICTION,
                                                           DEFAULT_DYNAMIC_FRICTION,
                                                           DEFAULT_RESTITUTION_COEFFICIENT);
         /*
          * Add the ground
          */
         /* Create and add the plane body */
         m_pcGroundBody = PxCreatePlane(*m_pcPhysics,
                                        physx::PxPlane(physx::PxVec3(0.0f, 0.0f, 1.0f), 0.0f),
                                        *m_pcDefaultMaterial);
         m_pcScene->addActor(*m_pcGroundBody);
         /* Get a reference to the ground shape */
         physx::PxShape* ppcGroundShapes[1];
         m_pcGroundBody->getShapes(ppcGroundShapes, 1);
         m_pcGroundShape = ppcGroundShapes[0];
         /*
          * Create cylinder mesh
          */
         m_pcCylinderMesh = CreateCylinderMesh();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the PhysX engine \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::PostSpaceInit() {
      /*
       * Setup the broad-phase regions for the scene
       */
      /* Arena bounds */
      physx::PxBounds3 cArenaBounds;
      CVector3ToPxVec3(
         CSimulator::GetInstance().GetSpace().GetArenaCenter() -
         CSimulator::GetInstance().GetSpace().GetArenaSize() * 0.5f,
         cArenaBounds.minimum);
      CVector3ToPxVec3(
         CSimulator::GetInstance().GetSpace().GetArenaCenter() +
         CSimulator::GetInstance().GetSpace().GetArenaSize() * 0.5f,
         cArenaBounds.maximum);
      /* Create a regular grid subdivision of the arena volume */
      physx::PxBounds3* pcRegionBounds = new physx::PxBounds3[m_unSubdivBPRegions*m_unSubdivBPRegions];
      const physx::PxU32 unNumRegions =
         physx::PxBroadPhaseExt::createRegionsFromWorldBounds(pcRegionBounds,
                                                              cArenaBounds,
                                                              m_unSubdivBPRegions,
                                                              2 /* Up axis = Z axis */);
      LOG << "[INFO] PhysX engine \""
          << GetId()
          << "\" will use "
          << unNumRegions
          << " regions for broad-phase"
          << std::endl;
      /* Add the regions to the scene */
      physx::PxBroadPhaseRegion cRegion;
      cRegion.userData = NULL;
      for(physx::PxU32 i = 0; i < unNumRegions; ++i) {
         cRegion.bounds = pcRegionBounds[i];
         m_pcScene->addBroadPhaseRegion(cRegion, true);
      }
      /* Cleanup */
      delete[] pcRegionBounds;
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::Reset() {
      for(CPhysXModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::Destroy() {
      /* Empty the physics model map */
      for(CPhysXModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
      /* Release PhysX resources */
      m_pcScene->removeActor(*m_pcGroundBody);
      m_pcGroundBody->release();
      m_pcDefaultMaterial->release();
      m_pcScene->release();
      delete m_pcSceneDesc;
      delete m_pcCPUDispatcher;
      PxCloseExtensions();
      m_pcCooking->release();
      m_pcPhysics->release();
      m_pcFoundation->release();
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::Update() {
      /* Update the physics state from the entities */
      for(CPhysXModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      /* Perform the step */
      for(size_t i = 0; i < GetIterations(); ++i) {
         m_pcScene->simulate(GetPhysicsClockTick());
         m_pcScene->fetchResults(true);
      }
      /* Update the simulated space */
      for(CPhysXModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   bool CPhysXEngine::IsPointContained(const CVector3& c_point) {
      return true;
   }

   /****************************************/
   /****************************************/

   size_t CPhysXEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CPhysXEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome sOutcome =
         CallEntityOperation<CPhysXOperationAddEntity, CPhysXEngine, SOperationOutcome>
         (*this, c_entity);
      return sOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPhysXEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome sOutcome =
         CallEntityOperation<CPhysXOperationRemoveEntity, CPhysXEngine, SOperationOutcome>
         (*this, c_entity);
      return sOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPhysXEngine::IsEntityTransferNeeded() const {
      return false;
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::TransferEntities() {
   }

   /****************************************/
   /****************************************/

   CEmbodiedEntity* CPhysXEngine::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                           const CRay3& c_ray) const {
      /*
       * Set ray data
       */
      /* Ray start */
      physx::PxVec3 cRayStart;
      CVector3ToPxVec3(c_ray.GetStart(), cRayStart);
      /* Ray direction (normalized) */
      CVector3 cARGoSRayDir;
      c_ray.GetDirection(cARGoSRayDir);
      physx::PxVec3 cRayDir;
      CVector3ToPxVec3(cARGoSRayDir, cRayDir);
      /* Ray length */
      physx::PxReal fRange = c_ray.GetLength();
      /*
       * Perform the query
       */
      /* Buffer for the results */
      physx::PxRaycastHit cResult;
      if(m_pcScene->raycastSingle(cRayStart, cRayDir, fRange,
                                  physx::PxSceneQueryFlag::eDISTANCE,
                                  cResult)) {
         /* The ray hits something */
         f_t_on_ray = cResult.distance / fRange;
         return &(reinterpret_cast<CPhysXModel*>(cResult.shape->getActor()->userData)->GetEmbodiedEntity());
      }
      else {
         /* The ray does not hit anything */
         return NULL;
      }
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::AddPhysicsModel(const std::string& str_id,
                                      CPhysXModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::RemovePhysicsModel(const std::string& str_id) {
      CPhysXModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("PhysX model id \"" << str_id << "\" not found in physx engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   physx::PxConvexMesh* CPhysXEngine::CreateCylinderMesh() {
      /* Array of vertices */
      physx::PxVec3* pcVertices = new physx::PxVec3[CYLINDER_NUM_SLICES * 2];
      /* Description of the convex mesh */
      physx::PxConvexMeshDesc cDescription;
      /* Calculate the angle span of a slice */
      CRadians cSlice(CRadians::TWO_PI / CYLINDER_NUM_SLICES);
      /* Go through the slices */
      for(size_t i = 0; i < CYLINDER_NUM_SLICES; ++i) {
         /* Top face */
         pcVertices[i].x = Cos(cSlice * i);
         pcVertices[i].y = Sin(cSlice * i);
         pcVertices[i].z = 0.5f;
         /* Bottom face */
         pcVertices[i+CYLINDER_NUM_SLICES].x = pcVertices[i].x;
         pcVertices[i+CYLINDER_NUM_SLICES].y = pcVertices[i].y;
         pcVertices[i+CYLINDER_NUM_SLICES].z = -0.5f;
      }
      /* Set up the description */
      cDescription.points.count  = CYLINDER_NUM_SLICES * 2;
      cDescription.points.stride = sizeof(physx::PxVec3);
      cDescription.points.data   = pcVertices;
      cDescription.flags         = physx::PxConvexFlag::eCOMPUTE_CONVEX;
      /* Buffer to store the cooked mesh */
      physx::PxDefaultMemoryOutputStream cCookedMeshBuf;
      /* Cook mesh */
      if(! m_pcCooking->cookConvexMesh(cDescription,
                                       cCookedMeshBuf)) {
         THROW_ARGOSEXCEPTION("PhysX engine \"" <<
                              GetId() <<
                              "\": error creating the cylinder convex mesh");
      }
      /* Buffer to store the mesh data */
      physx::PxDefaultMemoryInputData cMeshData(cCookedMeshBuf.getData(),
                                                cCookedMeshBuf.getSize());
      /* Create mesh */
      physx::PxConvexMesh* pcMesh = m_pcPhysics->createConvexMesh(cMeshData);
      /* Dispose of vertex array */
      delete[] pcVertices;
      /* Return mesh */
      return pcMesh;
   }

   /****************************************/
   /****************************************/

   physx::PxConvexMeshGeometry* CreateCylinderGeometry(CPhysXEngine& c_physx_engine,
                                                       physx::PxReal f_radius,
                                                       physx::PxReal f_height) {
      return new physx::PxConvexMeshGeometry(
         &c_physx_engine.GetCylinderMesh(),
         physx::PxMeshScale(
            physx::PxVec3(f_radius,
                          f_radius,
                          f_height),
            physx::PxQuat::createIdentity()));
   }

   /****************************************/
   /****************************************/

   class CPhysXOperationAddBaseEntity : public CPhysXOperationAddEntity {
   public:
      virtual ~CPhysXOperationAddBaseEntity() {}
      SOperationOutcome ApplyTo(CPhysXEngine&, CEntity& c_entity) {
         LOGERR << "Entity type \""
                << c_entity.GetTypeDescription()
                << "\" not supported by the PhysX engine."
                << std::endl;
         return SOperationOutcome(false);
      }
   };

   class CPhysXOperationRemoveBaseEntity : public CPhysXOperationRemoveEntity {
   public:
      virtual ~CPhysXOperationRemoveBaseEntity() {}
      SOperationOutcome ApplyTo(CPhysXEngine&, CEntity& c_entity) {
         LOGERR << "Entity type \""
                << c_entity.GetTypeDescription()
                << "\" not supported by the PhysX engine."
                << std::endl;
         return SOperationOutcome(false);
      }
   };

   REGISTER_PHYSX_OPERATION(CPhysXOperationAddEntity,
                            CPhysXOperationAddBaseEntity,
                            CEntity);

   REGISTER_PHYSX_OPERATION(CPhysXOperationRemoveEntity,
                            CPhysXOperationRemoveBaseEntity,
                            CEntity);

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CPhysXEngine,
                           "physx",
                           "Carlo Pinciroli [ilpincy@gmail.com]",
                           "1.0",
                           "A 3D dynamics physics engine.",
                           "This physics engine is a 3D dynamics engine based on the NVIDIA PhysX library\n"
                           "(http://www.geforce.com/hardware/technology/physx).\n\n"
                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <physx id=\"px\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "If two engines share the same id, initialization aborts.\n\n"
                           "OPTIONAL XML CONFIGURATION\n\n"
                           "None.\n",
                           "Under development"
      );

}
