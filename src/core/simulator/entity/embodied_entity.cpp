#include "embodied_entity.h"
#include "composable_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Init(TConfigurationNode& t_tree) {
      CPositionalEntity::Init(t_tree);
      m_bBoundingBoxRecalculationNeeded = true;
      m_bMovable = true;
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntity::Reset() {
      CPositionalEntity::Reset();

      /* Reset collision data */
      ClearCollisionDetected();
      m_unNumCollisions = 0;
      m_bBoundingBoxRecalculationNeeded = true;
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::CheckIntersectionWithRay(Real& f_distance,
                                                  const CRay& c_ray) const {
      for(UInt32 i = 0; i < m_tPhysicsEngineEntityVector.size(); ++i)
         if(m_tPhysicsEngineEntityVector[i]->CheckIntersectionWithRay(f_distance, c_ray)) {
            return true;
      }
      return false;
   }

   /****************************************/
   /****************************************/

   bool CEmbodiedEntity::MoveTo(const CVector3& c_position,
                                const CQuaternion& c_orientation,
                                bool b_check_only) {
      bool bNoCollision = true;
      for(CPhysicsEngineEntity::TVector::const_iterator it = m_tPhysicsEngineEntityVector.begin();
          it != m_tPhysicsEngineEntityVector.end() && bNoCollision; ++it) {
         if(! (*it)->MoveTo(c_position, c_orientation, b_check_only)) {
            bNoCollision = false;
         }
      }
      if(bNoCollision && !b_check_only) {
         /* Update space position */
         SetPosition(c_position);
         SetOrientation(c_orientation);
	 if( HasParent() ) {
	    CComposableEntity* pcEntity = dynamic_cast<CComposableEntity*>(&GetParent());
	    if( pcEntity != NULL ) {
	       pcEntity->UpdateComponents();
	    }
	 }
         return true;
      }
      else {
         /* No Collision or check only, undo changes */
         for(CPhysicsEngineEntity::TVector::const_iterator it = m_tPhysicsEngineEntityVector.begin();
             it != m_tPhysicsEngineEntityVector.end(); ++it) {
            (*it)->MoveTo(GetPosition(), GetOrientation());
         }
         if(!bNoCollision) {
            /* Collision */
            return false;
         }
         else {
            /* No collision, it was a check-only */
            return true;
         }
      }
   }

   /****************************************/
   /****************************************/

   void CalculateBoundingBoxFromHalfSize(CEmbodiedEntity::SBoundingBox& s_bounding_box,
                                         const CVector3& c_half_size,
                                         const CVector3& c_center_pos,
                                         const CRotationMatrix3& c_orientation) {
      CVector3 cExtent(Abs(c_orientation(0) * c_half_size.GetX()) +
                       Abs(c_orientation(1) * c_half_size.GetY()) +
                       Abs(c_orientation(2) * c_half_size.GetZ()),
                       Abs(c_orientation(3) * c_half_size.GetX()) +
                       Abs(c_orientation(4) * c_half_size.GetY()) +
                       Abs(c_orientation(5) * c_half_size.GetZ()),
                       Abs(c_orientation(6) * c_half_size.GetX()) +
                       Abs(c_orientation(7) * c_half_size.GetY()) +
                       Abs(c_orientation(8) * c_half_size.GetZ()));
      s_bounding_box.MinCorner =  c_center_pos;
      s_bounding_box.MinCorner -= cExtent;
      s_bounding_box.MaxCorner =  c_center_pos;
      s_bounding_box.MaxCorner += cExtent;
   }

   /****************************************/
   /****************************************/

   void CEmbodiedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CEmbodiedEntity>& c_space_hash,
                                                    CEmbodiedEntity& c_element) {
      /* Update the bounding box */
      c_element.UpdateBoundingBox();
      /* Translate the min corner of the bounding box into the map's coordinate */
      c_space_hash.SpaceToHashTable(m_nMinX, m_nMinY, m_nMinZ, c_element.GetBoundingBox().MinCorner);
      /* Translate the max corner of the bounding box into the map's coordinate */                                     
      c_space_hash.SpaceToHashTable(m_nMaxX, m_nMaxY, m_nMaxZ, c_element.GetBoundingBox().MaxCorner);
      /* Finally, go through the affected cells and update them */
      for(SInt32 nK = m_nMinZ; nK <= m_nMaxZ; ++nK) {
         for(SInt32 nJ = m_nMinY; nJ <= m_nMaxY; ++nJ) {
            for(SInt32 nI = m_nMinX; nI <= m_nMaxX; ++nI) {
               c_space_hash.UpdateCell(nI, nJ, nK, c_element);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

}
