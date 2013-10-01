/**
 * @file <argos3/plugins/simulator/physx_engines/physx/physx_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_engine.h"
#include "physx_model.h"

#if defined(PX_LINUX)
#   include <malloc.h> // for memalign()
#else
#   include <cstdlib> // for malloc()
#endif

namespace argos {

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

   void	CPhysXEngine::CPhysXEngineAllocatorCallback::deallocate(void* ptr) {
   }

   /****************************************/
   /****************************************/

   CPhysXEngine::CPhysXEngineErrorCallback::CPhysXEngineErrorCallback(CPhysXEngine& c_engine) :
      m_cEngine(c_engine) {
   }

   /****************************************/
   /****************************************/

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

   CPhysXEngine::CPhysXEngine() :
      m_cErrorCallback(*this),
      m_pcFoundation(NULL),
      m_pcPhysics(NULL),
      m_pcCPUDispatcher(NULL),
      m_pcSceneDesc(NULL),
      m_pcScene(NULL) {
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
         if (!PxInitExtensions(*m_pcPhysics)) {
            THROW_ARGOSEXCEPTION("Error calling PxInitExtensions()");
         }
         /* Create CPU dispatcher */
         UInt32 nThreads = 1; // TODO: parse thread number
         m_pcCPUDispatcher = physx::PxDefaultCpuDispatcherCreate(nThreads);
         if (m_pcCPUDispatcher == NULL) {
            THROW_ARGOSEXCEPTION("Error calling PxDefaultCpuDispatcherCreate(" << nThreads << ")");
         }
         /* Create scene descriptor */
         m_pcSceneDesc = new physx::PxSceneDesc(m_pcPhysics->getTolerancesScale());
         m_pcSceneDesc->gravity = physx::PxVec3(0.0f, 9.81f, 0.0f);
         m_pcSceneDesc->cpuDispatcher = m_pcCPUDispatcher;
         m_pcSceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;
         /* Create scene */
         m_pcScene = m_pcPhysics->createScene(*m_pcSceneDesc);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the PhysX engine \"" << GetId() << "\"", ex);
      }
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
      m_pcScene->simulate(GetSimulationClockTick());
      m_pcScene->fetchResults(true);
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

   UInt32 CPhysXEngine::GetNumPhysicsEngineEntities() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::AddEntity(CEntity& c_entity) {
      CallEntityOperation<CPhysXOperationAddEntity, CPhysXEngine, void>(*this, c_entity);
   }

   /****************************************/
   /****************************************/

   void CPhysXEngine::RemoveEntity(CEntity& c_entity) {
      CallEntityOperation<CPhysXOperationRemoveEntity, CPhysXEngine, void>(*this, c_entity);
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

   class CPhysXOperationAddBaseEntity : public CPhysXOperationAddEntity {
   public:
      virtual ~CPhysXOperationAddBaseEntity() {}
      void ApplyTo(CPhysXEngine&, CEntity& c_entity) {
         LOGERR << "Entity type \""
                << c_entity.GetTypeDescription()
                << "\" not supported by the PhysX engine."
                << std::endl;
      }
   };

   class CPhysXOperationRemoveBaseEntity : public CPhysXOperationRemoveEntity {
   public:
      virtual ~CPhysXOperationRemoveBaseEntity() {}
      void ApplyTo(CPhysXEngine&, CEntity& c_entity) {
         LOGERR << "Entity type \""
                << c_entity.GetTypeDescription()
                << "\" not supported by the PhysX engine."
                << std::endl;
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
                           "A 3D dynamics physics engine",
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
