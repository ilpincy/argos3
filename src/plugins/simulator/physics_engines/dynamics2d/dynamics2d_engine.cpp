/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_engine.h"
#include "dynamics2d_model.h"
#include "dynamics2d_gripping.h"

#include <argos3/core/simulator/simulator.h>

#include <cmath>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DEngine::CDynamics2DEngine() :
      m_fStaticHashCellSize(0.1f),
      m_fActiveHashCellSize(2.0f * 0.085036758f),
      m_nStaticHashCells(1000),
      m_nActiveHashCells(1000),
      m_ptSpace(NULL),
      m_ptGroundBody(NULL),
      m_fElevation(0.0f) {
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CPhysicsEngine::Init(t_tree);
         /* Parse XML */
         GetNodeAttributeOrDefault(t_tree, "static_cell_size", m_fStaticHashCellSize, m_fStaticHashCellSize);
         GetNodeAttributeOrDefault(t_tree, "active_cell_size", m_fActiveHashCellSize, m_fActiveHashCellSize);
         GetNodeAttributeOrDefault(t_tree, "static_cells",     m_nStaticHashCells,    m_nStaticHashCells);
         GetNodeAttributeOrDefault(t_tree, "active_cells",     m_nActiveHashCells,    m_nActiveHashCells);
         GetNodeAttributeOrDefault(t_tree, "elevation",        m_fElevation,          m_fElevation);
         /* Override volume top and bottom with the value of m_fElevation */
         if(!GetVolume().TopFace)    GetVolume().TopFace    = new SHorizontalFace;
         if(!GetVolume().BottomFace) GetVolume().BottomFace = new SHorizontalFace;
         GetVolume().TopFace->Height    = m_fElevation;
         GetVolume().BottomFace->Height = m_fElevation;
         /* Initialize physics */
         cpInitChipmunk();
         cpResetShapeIdCounter();
         /* Used to attach static geometries so that they won't move and to simulate friction */
         m_ptGroundBody = cpBodyNew(INFINITY, INFINITY);
         /* Create the space to contain the movable objects */
         m_ptSpace = cpSpaceNew();
         /* Subiterations to solve constraints.
            The more, the better for precision but the worse for speed
         */
         m_ptSpace->iterations = GetIterations();
         /* Resize the space hash.
            This has dramatic effects on performance.
            TODO: - find optimal parameters automatically (average entity size)
            cpSpaceReindexStaticHash(m_ptSpace, m_fStaticHashCellSize, m_nStaticHashCells);
            cpSpaceResizeActiveHash(m_ptSpace, m_fActiveHashCellSize, m_nActiveHashCells);
         */
         /* Gripper-Gripped callback functions */
         cpSpaceAddCollisionHandler(
            m_ptSpace,
            SHAPE_GRIPPER,
            SHAPE_GRIPPABLE,
            BeginCollisionBetweenGripperAndGrippable,
            ManageCollisionBetweenGripperAndGrippable,
            NULL,
            NULL,
            NULL);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the dynamics 2D engine \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Reset() {
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
      cpSpaceReindexStatic(m_ptSpace);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Update() {
      /* Update the physics state from the entities */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      /* Perform the step */
      for(size_t i = 0; i < GetIterations(); ++i) {
         cpSpaceStep(m_ptSpace, GetPhysicsClockTick());
      }
      /* Update the simulated space */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Destroy() {
      /* Empty the physics model map */
      for(CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
      /* Get rid of the physics space */
      cpSpaceFree(m_ptSpace);
      cpBodyFree(m_ptGroundBody);
   }

   /****************************************/
   /****************************************/

   size_t CDynamics2DEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics2DOperationAddEntity, CDynamics2DEngine, SOperationOutcome>
         (*this, c_entity);
      cpResetShapeIdCounter();
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics2DOperationRemoveEntity, CDynamics2DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   struct SDynamics2DSegmentHitData {
      const CRay3& Ray;
      Real T;
      CEmbodiedEntity* Body;

      SDynamics2DSegmentHitData(const CRay3& c_ray) :
         Ray(c_ray),
         T(2.0f),
         Body(NULL) {}
   };

   static void Dynamics2DSegmentQueryFunc(cpShape* pt_shape, cpFloat f_t, cpVect, void* pt_data) {
      /* Get the data associated to this query */
      SDynamics2DSegmentHitData& sData = *reinterpret_cast<SDynamics2DSegmentHitData*>(pt_data);
      /* Hit found, is it within the limits? */
      CDynamics2DModel& cModel = *reinterpret_cast<CDynamics2DModel*>(pt_shape->body->data);
      CVector3 cIntersectionPoint;
      sData.Ray.GetPoint(cIntersectionPoint, f_t);
      if((cIntersectionPoint.GetZ() >= cModel.GetBoundingBox().MinCorner.GetZ()) &&
         (cIntersectionPoint.GetZ() <= cModel.GetBoundingBox().MaxCorner.GetZ()) ) {
         /* Yes, a real hit */
         if(f_t < sData.T) {
            /* Also the closest so far */
            sData.T = f_t;
            sData.Body = &cModel.GetEmbodiedEntity();
         }
      }
   }
   
   CEmbodiedEntity* CDynamics2DEngine::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                                const CRay3& c_ray) const {
      /* Query all hits along the ray */
      SDynamics2DSegmentHitData sHitData(c_ray);
      cpSpaceSegmentQuery(
         m_ptSpace,
         cpv(c_ray.GetStart().GetX(), c_ray.GetStart().GetY()),
         cpv(c_ray.GetEnd().GetX()  , c_ray.GetEnd().GetY()  ),
         CP_ALL_LAYERS,
         CP_NO_GROUP,
         Dynamics2DSegmentQueryFunc,
         &sHitData);
      /* Check whether we have a hit */
      if(sHitData.Body != NULL) {
         f_t_on_ray = sHitData.T;
      }
      return sHitData.Body;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::PositionPhysicsToSpace(CVector3& c_new_pos,
                                                  const CVector3& c_original_pos,
                                                  const cpBody* pt_body) {
      c_new_pos.SetX(pt_body->p.x);
      c_new_pos.SetY(pt_body->p.y);
      c_new_pos.SetZ(c_original_pos.GetZ());
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::OrientationPhysicsToSpace(CQuaternion& c_new_orient,
                                                     cpBody* pt_body) {
      c_new_orient.FromAngleAxis(CRadians(pt_body->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddPhysicsModel(const std::string& str_id,
                                            CDynamics2DModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemovePhysicsModel(const std::string& str_id) {
      CDynamics2DModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("Dynamics2D model id \"" << str_id << "\" not found in dynamics 2D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CDynamics2DEngine,
                           "dynamics2d",
                           "Carlo Pinciroli [ilpincy@gmail.com]",
                           "1.0",
                           "A 2D dynamics physics engine.",
                           "This physics engine is a 2D dynamics engine based on the Chipmunk library\n"
                           "(http://code.google.com/p/chipmunk-physics).\n\n"
                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "It is used in the subsequent section <arena_physics> to assign entities to\n"
                           "physics engines. If two engines share the same id, initialization aborts.\n\n"
                           "OPTIONAL XML CONFIGURATION\n\n"
                           "The plane of the physics engine can be translated on the Z axis, to simulate\n"
                           "for example hovering objects, such as flying robots. To translate the plane\n"
                           "2m up the Z axis, use the 'elevation' attribute as follows:\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\"\n"
                           "                elevation=\"2.0\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "When not specified, the elevation is zero, which means that the plane\n"
                           "corresponds to the XY plane.\n",
                           "Under development"
      );

}
