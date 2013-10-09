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
      m_fElevation(0.0f),
      m_bEntityTransferActive(false) {
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CPhysicsEngine::Init(t_tree);
      /* Parse XML */
      GetNodeAttributeOrDefault(t_tree, "static_cell_size", m_fStaticHashCellSize, m_fStaticHashCellSize);
      GetNodeAttributeOrDefault(t_tree, "active_cell_size", m_fActiveHashCellSize, m_fActiveHashCellSize);
      GetNodeAttributeOrDefault(t_tree, "static_cells",     m_nStaticHashCells,    m_nStaticHashCells);
      GetNodeAttributeOrDefault(t_tree, "active_cells",     m_nActiveHashCells,    m_nActiveHashCells);
      GetNodeAttributeOrDefault(t_tree, "elevation",        m_fElevation,          m_fElevation);
      if(NodeExists(t_tree, "boundaries")) {
         /* Parse the boundary definition */
         TConfigurationNode& tBoundaries = GetNode(t_tree, "boundaries");
         SBoundarySegment sBoundSegment;
         CVector2 cLastPoint, cCurPoint;
         std::string strConnectWith;
         TConfigurationNodeIterator tVertexIt("vertex");
         /* Get the first vertex */
         tVertexIt = tVertexIt.begin(&tBoundaries);
         if(tVertexIt == tVertexIt.end()) {
            THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": you didn't specify any <vertex>!");
         }
         GetNodeAttribute(*tVertexIt, "point", cLastPoint);
         m_vecVertices.push_back(cLastPoint);
         /* Go through the other vertices */
         ++tVertexIt;
         while(tVertexIt != tVertexIt.end()) {
            /* Read vertex data and fill in segment struct */
            GetNodeAttribute(*tVertexIt, "point", cCurPoint);
            m_vecVertices.push_back(cCurPoint);
            sBoundSegment.Segment.SetStart(cLastPoint);
            sBoundSegment.Segment.SetEnd(cCurPoint);
            GetNodeAttribute(*tVertexIt, "connect_with", strConnectWith);
            if(strConnectWith == "gate") {
               /* Connect to previous vertex with a gate */
               sBoundSegment.Type = SBoundarySegment::SEGMENT_TYPE_GATE;
               GetNodeAttribute(*tVertexIt, "to_engine", sBoundSegment.EngineId);
            }
            else if(strConnectWith == "wall") {
               /* Connect to previous vertex with a wall */
               sBoundSegment.Type = SBoundarySegment::SEGMENT_TYPE_WALL;
               sBoundSegment.EngineId = "";
            }
            else {
               /* Parse error */
               THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": unknown vertex connection method \"" << strConnectWith << "\". Allowed methods are \"wall\" and \"gate\".");
            }
            m_vecSegments.push_back(sBoundSegment);
            /* Next vertex */
            cLastPoint = cCurPoint;
            ++tVertexIt;
         }
         /* Check that the boundary is a closed path */
         if(m_vecVertices.front() != m_vecVertices.back()) {
            THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": the specified path is not closed. The first and last points of the boundaries MUST be the same.");
         }
      }
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
      /* Add boundaries, if specified */
      if(! m_vecSegments.empty()) {
         cpShape* ptSegment;
         for(size_t i = 0; i < m_vecSegments.size(); ++i) {
            if(m_vecSegments[i].Type == SBoundarySegment::SEGMENT_TYPE_WALL) {
               ptSegment =
                  cpSpaceAddShape(
                     m_ptSpace,
                     cpSegmentShapeNew(
                        m_ptGroundBody,
                        cpv(m_vecSegments[i].Segment.GetStart().GetX(),
                            m_vecSegments[i].Segment.GetStart().GetY()),
                        cpv(m_vecSegments[i].Segment.GetEnd().GetX(),
                            m_vecSegments[i].Segment.GetEnd().GetY()),
                        0.0f));
               ptSegment->e = 0.0f; // no elasticity
               ptSegment->u = 1.0f; // max friction
            }
            else {
               /* There is at least a gate, transfer is activated */
               m_bEntityTransferActive = true;
            }
         }
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
      cpSpaceStep(m_ptSpace, GetSimulationClockTick());
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

   bool CDynamics2DEngine::IsPointContained(const CVector3& c_point) {
      /* Check that Z-coordinate is within elevation */
      if(c_point.GetZ() > m_fElevation || c_point.GetZ() < m_fElevation) {
         return false;
      }
      if(! IsEntityTransferActive()) {
         /* The engine has no boundaries on XY, so the wanted point is in for sure */
         return true;
      }
      else {
         /* Check the boundaries */
         for(size_t i = 0; i < m_vecSegments.size(); ++i) {
            const CVector2& cP0 = m_vecSegments[i].Segment.GetStart();
            const CVector2& cP1 = m_vecSegments[i].Segment.GetEnd();
            Real fCriterion =
               (c_point.GetY() - cP0.GetY()) * (cP1.GetX() - cP0.GetX()) -
               (c_point.GetX() - cP0.GetX()) * (cP1.GetY() - cP0.GetY());
            if(fCriterion > 0.0f) {
               return false;
            }
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   UInt32 CDynamics2DEngine::GetNumPhysicsEngineEntities() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddEntity(CEntity& c_entity) {
      CallEntityOperation<CDynamics2DOperationAddEntity, CDynamics2DEngine, void>(*this, c_entity);
      cpResetShapeIdCounter();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemoveEntity(CEntity& c_entity) {
      CallEntityOperation<CDynamics2DOperationRemoveEntity, CDynamics2DEngine, void>(*this, c_entity);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::TransferEntities() {
      for(size_t i = 0; i < m_vecTransferData.size(); ++i) {
         CPhysicsEngine& cToEngine = CSimulator::GetInstance().GetPhysicsEngine(m_vecTransferData[i].EngineId);
         cToEngine.AddEntity(*m_vecTransferData[i].Entity);
         RemoveEntity(*m_vecTransferData[i].Entity);
      }
      m_vecTransferData.clear();
   }

   /****************************************/
   /****************************************/
   
   CEmbodiedEntity* CDynamics2DEngine::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                                const CRay3& c_ray) const {
      /* Structure to store the query data */
      cpSegmentQueryInfo tInfo;
      /* Query the first hit along the ray */
      cpShape* ptHit = cpSpaceSegmentQueryFirst(
         m_ptSpace,
         cpv(c_ray.GetStart().GetX(), c_ray.GetStart().GetY()),
         cpv(c_ray.GetEnd().GetX()  , c_ray.GetEnd().GetY()  ),
         CP_ALL_LAYERS,
         CP_NO_GROUP,
         &tInfo);
      /* Check whether we have a hit */
      if(ptHit) {
         /* Hit found, is it within the limits? */
         CDynamics2DModel& cModel = *reinterpret_cast<CDynamics2DModel*>(ptHit->body->data);
      	 CVector3 cIntersectionPoint;
      	 c_ray.GetPoint(cIntersectionPoint, tInfo.t);
      	 if((cIntersectionPoint.GetZ() >= cModel.GetBoundingBox().MinCorner.GetZ()) &&
      			(cIntersectionPoint.GetZ() <= cModel.GetBoundingBox().MaxCorner.GetZ()) ) {
            /* Yes, a real hit */
            f_t_on_ray = tInfo.t;
            return &cModel.GetEmbodiedEntity();
      	 }
      	 else {
            return NULL;
      	 }
      }
      else {
         /* Hit not found */
         return NULL;
      }
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::CalculateTransfer(Real f_x, Real f_y,
                                             std::string& str_engine_id) {
      /*
       * TODO: this method makes the assumption that only one gate is trespassed at any time.
       * This assumption may be false in some ill-shaped polygons or when the gate isn't just a
       * segment, but is a sequence of segments.
       */
      for(size_t i = 0; i < m_vecSegments.size(); ++i) {
         if(m_vecSegments[i].Type == SBoundarySegment::SEGMENT_TYPE_GATE) {
            const CVector2& cP0 = m_vecSegments[i].Segment.GetStart();
            const CVector2& cP1 = m_vecSegments[i].Segment.GetEnd();
            Real fCriterion =
               (f_y - cP0.GetY()) * (cP1.GetX() - cP0.GetX()) -
               (f_x - cP0.GetX()) * (cP1.GetY() - cP0.GetY());
            if(fCriterion < 0.0f) {
               str_engine_id = m_vecSegments[i].EngineId;
               return true;
            }
         }
      }
      return false;
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
