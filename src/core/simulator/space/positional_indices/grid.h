#ifndef GRID_H
#define GRID_H

#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>

namespace argos {

   template<class ENTITY>
   class CGrid : public CPositionalIndex<ENTITY> {

   public:

      typedef typename CPositionalIndex<ENTITY>::COperation CEntityOperation;

      struct SCell {
         CSet<ENTITY*,SEntityComparator> Entities;
         size_t Timestamp;

         SCell() : Timestamp(0) {}
         inline void Reset() {
            Entities.clear();
            Timestamp = 0;
         }
      };

      class CCellOperation {
      public:
         virtual ~CCellOperation() {}
         virtual bool operator()(SInt32 n_i,
                                 SInt32 n_j,
                                 SInt32 n_k,
                                 SCell& s_cell) = 0;
      };

   public:

      CGrid(const CVector3& c_area_min_corner,
            const CVector3& c_area_max_corner,
            SInt32 n_size_i,
            SInt32 n_size_j,
            SInt32 n_size_k);

      virtual ~CGrid();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void AddEntity(ENTITY& c_entity);

      virtual void RemoveEntity(ENTITY& c_entity);

      virtual void Update();

      virtual void GetEntitiesAt(CSet<ENTITY*,SEntityComparator>& c_entities,
                                 const CVector3& c_position) const;

      virtual void ForAllEntities(CEntityOperation& c_operation);

      virtual void ForEntitiesInSphereRange(const CVector3& c_center,
                                            Real f_radius,
                                            CEntityOperation& c_operation);

      virtual void ForEntitiesInBoxRange(const CVector3& c_center,
                                         const CVector3& c_half_size,
                                         CEntityOperation& c_operation);

      virtual void ForEntitiesInCircleRange(const CVector3& c_center,
                                            Real f_radius,
                                            CEntityOperation& c_operation);

      virtual void ForEntitiesInRectangleRange(const CVector3& c_center,
                                               const CVector2& c_half_size,
                                               CEntityOperation& c_operation);

      virtual void ForEntitiesAlongRay(const CRay3& c_ray,
                                       CEntityOperation& c_operation,
                                       bool b_stop_at_closest_match = false);

      virtual void ForAllCells(CCellOperation& c_operation);

      virtual void ForCellsInSphereRange(const CVector3& c_center,
                                         Real f_radius,
                                         CCellOperation& c_operation);

      virtual void ForCellsInBoxRange(const CVector3& c_center,
                                      const CVector3& c_half_size,
                                      CCellOperation& c_operation);

      virtual void ForCellsInCircleRange(const CVector3& c_center,
                                         Real f_radius,
                                         CCellOperation& c_operation);

      virtual void ForCellsInRectangleRange(const CVector3& c_center,
                                            const CVector2& c_half_size,
                                            CCellOperation& c_operation);

      virtual void ForCellsAlongRay(const CRay3& c_ray,
                                    CCellOperation& c_operation);

      inline SInt32 GetSizeI() const {
         return m_nSizeI;
      }

      inline SInt32 GetSizeJ() const {
         return m_nSizeJ;
      }

      inline SInt32 GetSizeK() const {
         return m_nSizeK;
      }

      inline void SetUpdateEntityOperation(CEntityOperation* pc_operation);

      void UpdateCell(SInt32 n_i,
                      SInt32 n_j,
                      SInt32 n_k,
                      ENTITY& c_entity);

      inline void PositionToCell(SInt32& n_i,
                                 SInt32& n_j,
                                 SInt32& n_k,
                                 const CVector3& c_position) const;

      inline void PositionToCellUnsafe(SInt32& n_i,
                                       SInt32& n_j,
                                       SInt32& n_k,
                                       const CVector3& c_position) const;

      inline void ClampCoordinates(SInt32& n_i,
                                   SInt32& n_j,
                                   SInt32& n_k) const;

      inline void ClampCoordinates(CVector3& c_pos) const;

      inline SCell& GetCellAt(SInt32 n_i,
                              SInt32 n_j,
                              SInt32 n_k);

      inline const SCell& GetCellAt(SInt32 n_i,
                                    SInt32 n_j,
                                    SInt32 n_k) const;

   protected:

      CVector3 m_cAreaMinCorner;
      CVector3 m_cAreaMaxCorner;
      SInt32 m_nSizeI;
      SInt32 m_nSizeJ;
      SInt32 m_nSizeK;
      CRange<Real> m_cRangeX;
      CRange<Real> m_cRangeY;
      CRange<Real> m_cRangeZ;
      CVector3 m_cCellSize;
      CVector3 m_cInvCellSize;
      SCell* m_psCells;
      size_t m_unCurTimestamp;
      CSet<ENTITY*,SEntityComparator> m_cEntities;
      CEntityOperation* m_pcUpdateEntityOperation;

   };

}

#include <argos3/core/simulator/space/positional_indices/grid_impl.h>

#endif
