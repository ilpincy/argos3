/**
 * @file <argos3/core/simulator/physics_engine/physics_engine.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <cstdlib>
#include "physics_engine.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/entity.h>

namespace argos {

   /* The default value of the simulation clock tick */
   Real CPhysicsEngine::m_fSimulationClockTick = 0.1f;
   Real CPhysicsEngine::m_fInverseSimulationClockTick = 1.0f / CPhysicsEngine::m_fSimulationClockTick;

   /****************************************/
   /****************************************/

   CPhysicsEngine::SVolume::SVolume() :
      TopFace(NULL),
      BottomFace(NULL) {
   }

   /****************************************/
   /****************************************/
   
   void CPhysicsEngine::SVolume::Init(TConfigurationNode& t_node) {
      try {
         /* Parse top face, if specified */
         if(NodeExists(t_node, "top")) {
            TConfigurationNode& tNode = GetNode(t_node, "top");
            TopFace = new SHorizontalFace;
            GetNodeAttribute(tNode, "height", TopFace->Height);
         }
         /* Parse bottom face, if specified */
         if(NodeExists(t_node, "bottom")) {
            TConfigurationNode& tNode = GetNode(t_node, "bottom");
            BottomFace = new SHorizontalFace;
            GetNodeAttribute(tNode, "height", BottomFace->Height);
         }
         /* Parse side faces, if specified */
         if(NodeExists(t_node, "sides")) {
            CVector2 cFirstPoint, cLastPoint, cCurPoint;
            std::string strConnectWith;
            TConfigurationNode& tNode = GetNode(t_node, "sides");
            TConfigurationNodeIterator tVertexIt("vertex");
            /* Get the first vertex */
            tVertexIt = tVertexIt.begin(&tNode);
            if(tVertexIt == tVertexIt.end()) {
               THROW_ARGOSEXCEPTION("No <vertex> specified within <sides> section");
            }
            GetNodeAttribute(*tVertexIt, "point", cFirstPoint);
            cLastPoint = cFirstPoint;
            /* Go through the other vertices */
            ++tVertexIt;
            while(tVertexIt != tVertexIt.end()) {
               /* Read vertex data and fill in segment struct */
               GetNodeAttribute(*tVertexIt, "point", cCurPoint);
               SVerticalFace* psFace = new SVerticalFace;
               psFace->BaseSegment.SetStart(cLastPoint);
               psFace->BaseSegment.SetEnd(cCurPoint);
               SideFaces.push_back(psFace);
               /* Next vertex */
               cLastPoint = cCurPoint;
               ++tVertexIt;
            }
            /* Make sure that the boundary is a closed path */
            if(SideFaces.size() < 3) {
               THROW_ARGOSEXCEPTION("The <sides> path is not closed; at least 3 segments must be specified");
            }
            if(cLastPoint != cFirstPoint) {
               SVerticalFace* psFace = new SVerticalFace;
               psFace->BaseSegment.SetStart(cLastPoint);
               psFace->BaseSegment.SetEnd(cFirstPoint);
               SideFaces.push_back(psFace);
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error parsing physics engine <boundaries> information", ex);
      }
   }

   /****************************************/
   /****************************************/

   CPhysicsEngine::SVolume::~SVolume() {
      if(TopFace)    delete TopFace;
      if(BottomFace) delete BottomFace;
      while(!SideFaces.empty()) {
         delete SideFaces.back();
         SideFaces.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   bool CPhysicsEngine::SVolume::IsActive() const {
      return TopFace || BottomFace || (!SideFaces.empty());
   }

   /****************************************/
   /****************************************/

   CPhysicsEngine::CPhysicsEngine() :
      m_unIterations(10),
      m_fPhysicsClockTick(m_fSimulationClockTick) {}

   /****************************************/
   /****************************************/

   void CPhysicsEngine::Init(TConfigurationNode& t_tree) {
      try {
         /* Get id from the XML */
         GetNodeAttribute(t_tree, "id", m_strId);
         /* Get iterations per time step */
         GetNodeAttributeOrDefault(t_tree, "iterations", m_unIterations, m_unIterations);
         m_fPhysicsClockTick = GetSimulationClockTick() / static_cast<Real>(m_unIterations);
         LOG << "[INFO] The physics engine \""
             << GetId()
             << "\" will perform "
             << m_unIterations
             << " iterations per tick (dt = "
             << GetPhysicsClockTick() << " sec)"
             << std::endl;
         /* Parse the boundary definition, if necessary */
         if(NodeExists(t_tree, "boundaries")) {
            m_sVolume.Init(GetNode(t_tree, "boundaries"));
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION("Error initializing a physics engine");
      }
   }

   /****************************************/
   /****************************************/

   bool CPhysicsEngine::IsPointContained(const CVector3& c_point) {
      if(! IsEntityTransferActive()) {
         /*
          * The engine has no boundaries, so the wanted point is in for sure
          */
         return true;
      }
      else {
         /*
          * Check the boundaries
          */
         /* Check top/bottom boundaries */
         if((m_sVolume.TopFace    && c_point.GetZ() > m_sVolume.TopFace->Height) ||
            (m_sVolume.BottomFace && c_point.GetZ() < m_sVolume.BottomFace->Height)) {
            return false;
         }
         /* Check side boundaries */
         for(size_t i = 0; i < GetVolume().SideFaces.size(); ++i) {
            const CVector2& cP0 = GetVolume().SideFaces[i]->BaseSegment.GetStart();
            const CVector2& cP1 = GetVolume().SideFaces[i]->BaseSegment.GetEnd();
            Real fCriterion =
               (c_point.GetY() - cP0.GetY()) * (cP1.GetX() - cP0.GetX()) -
               (c_point.GetX() - cP0.GetX()) * (cP1.GetY() - cP0.GetY());
            if(fCriterion < 0.0f) {
               return false;
            }
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   void CPhysicsEngine::ScheduleEntityForTransfer(CEmbodiedEntity& c_entity) {
      m_vecTransferData.push_back(&c_entity);
   }

   /****************************************/
   /****************************************/

   void CPhysicsEngine::TransferEntities() {
      for(size_t i = 0; i < m_vecTransferData.size(); ++i) {
         RemoveEntity(m_vecTransferData[i]->GetRootEntity());
         CSimulator::GetInstance().GetSpace().AddEntityToPhysicsEngine(*m_vecTransferData[i]);
      }
      m_vecTransferData.clear();
   }

   /****************************************/
   /****************************************/

   Real CPhysicsEngine::GetSimulationClockTick() {
      return m_fSimulationClockTick;
   }

   /****************************************/
   /****************************************/

   Real CPhysicsEngine::GetInverseSimulationClockTick() {
      return m_fInverseSimulationClockTick;
   }

   /****************************************/
   /****************************************/

   void CPhysicsEngine::SetSimulationClockTick(Real f_simulation_clock_tick) {
      LOG << "[INFO] Using simulation clock tick = " << f_simulation_clock_tick << std::endl;
      m_fSimulationClockTick = f_simulation_clock_tick;
      m_fInverseSimulationClockTick = 1.0f / f_simulation_clock_tick;
   }

   /****************************************/
   /****************************************/

}
