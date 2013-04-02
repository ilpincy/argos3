namespace argos {

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CGrid<ENTITY>::CGrid(const CVector3& c_area_min_corner,
                        const CVector3& c_area_max_corner,
                        size_t un_size_i,
                        size_t un_size_j,
                        size_t un_size_k) :
      m_cAreaMinCorner(c_area_min_corner),
      m_cAreaMaxCorner(c_area_max_corner),
      m_unSizeI(un_size_i),
      m_unSizeJ(un_size_j),
      m_unSizeK(un_size_k),
      m_cRangeX(m_cAreaMinCorner.GetX(), m_cAreaMaxCorner.GetX()),
      m_cRangeY(m_cAreaMinCorner.GetY(), m_cAreaMaxCorner.GetY()),
      m_cRangeZ(m_cAreaMinCorner.GetZ(), m_cAreaMaxCorner.GetZ()),
      m_unCurTimestamp(0),
      m_pcUpdateEntityOperation(NULL) {
      m_cCellSize.Set(m_cRangeX.GetSpan() / m_unSizeI,
                      m_cRangeY.GetSpan() / m_unSizeJ,
                      m_cRangeZ.GetSpan() / m_unSizeK);
      m_cInvCellSize.Set(1.0f / m_cCellSize.GetX(),
                         1.0f / m_cCellSize.GetY(),
                         1.0f / m_cCellSize.GetZ());
      m_psCells = new SCell[m_unSizeI * m_unSizeJ * m_unSizeK];
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CGrid<ENTITY>::~CGrid() {
      delete[] m_psCells;
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::Init(TConfigurationNode& t_tree) {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::Reset() {
      m_unCurTimestamp = 0;
      for(size_t i = 0; i < m_unSizeI; ++i) {
         for(size_t j = 0; j < m_unSizeJ; ++j) {
            for(size_t k = 0; k < m_unSizeK; ++k) {
               GetCellAt(i,j,k).Reset();
            }
         }
      }
      Update();
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::Destroy() {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::AddEntity(ENTITY& c_entity) {
      m_cEntities.insert(&c_entity);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::RemoveEntity(ENTITY& c_entity) {
      m_cEntities.erase(&c_entity);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::Update() {
      ++m_unCurTimestamp;
      ForAllEntities(*m_pcUpdateEntityOperation);
   }

   /****************************************/
   /****************************************/
      
   template<class ENTITY>
   void CGrid<ENTITY>::GetEntitiesAt(CSet<ENTITY*>& c_entities,
                                     const CVector3& c_position) const {
      if(m_cRangeX.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetX()) &&
         m_cRangeY.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetY()) &&
         m_cRangeZ.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetZ())) {
         size_t i, j, k;
         PositionToCell(i, j, k, c_position);
         const SCell& sCell = GetCellAt(i, j, k);
         if(sCell.Timestamp < m_unCurTimestamp) {
            c_entities.clear();
         }
         else {
            c_entities = sCell.Entities;
         }
      }
      else {
         THROW_ARGOSEXCEPTION("CGrid<ENTITY>::GetEntitiesAt() : Position (" << c_position << " out of bounds X -> " << m_cRangeX << " Y -> " << m_cRangeY << " Z -> " << m_cRangeZ);
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForAllEntities(COperation& c_operation) {
      for(typename CSet<ENTITY*>::iterator it = m_cEntities.begin();
          it != m_cEntities.end() && c_operation(**it);
          ++it);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInSphereRange(const CVector3& c_center,
                                                Real f_radius,
                                                COperation& c_operation) {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInBoxRange(const CVector3& c_center,
                                             const CVector3& c_half_size,
                                             COperation& c_operation) {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInCircleRange(const CVector3& c_center,
                                                Real f_radius,
                                                COperation& c_operation) {
   }
   
   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInRectangleRange(const CVector3& c_center,
                                                   const CVector2& c_half_size,
                                                   COperation& c_operation) {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesAlongRay(const CRay3& c_ray,
                                           COperation& c_operation) {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::UpdateCell(size_t un_i,
                                  size_t un_j,
                                  size_t un_k,
                                  ENTITY& c_entity) {
      if((un_i < m_unSizeI) &&
         (un_j < m_unSizeJ) &&
         (un_k < m_unSizeK)) {
         SCell& sCell = GetCellAt(un_i, un_j, un_k);
         if(sCell.Timestamp < m_unCurTimestamp) {
            sCell.Entities.clear();
            sCell.Timestamp = m_unCurTimestamp;
         }
         sCell.Entities.insert(&c_entity);
      }
      else {
         THROW_ARGOSEXCEPTION("CGrid<ENTITY>::UpdateCell() : index (" << un_i << "," << un_j << "," << un_k << ") out of bounds (" << m_unSizeI-1 << "," << m_unSizeJ-1 << "," << m_unSizeK-1 << ")");
      }
   }
   
   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::SetUpdateEntityOperation(COperation* pc_operation) {
      m_pcUpdateEntityOperation = pc_operation;
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::PositionToCell(size_t& un_i,
                                      size_t& un_j,
                                      size_t& un_k,
                                      const CVector3& c_position) const {
      un_i = Floor((c_position.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX());
      un_j = Floor((c_position.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY());
      un_k = Floor((c_position.GetZ() - m_cAreaMinCorner.GetZ()) * m_cInvCellSize.GetZ());
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   typename CGrid<ENTITY>::SCell& CGrid<ENTITY>::GetCellAt(size_t un_i,
                                                           size_t un_j,
                                                           size_t un_k) {
      return m_psCells[m_unSizeI * m_unSizeJ * un_k +
                       m_unSizeI * un_j +
                       un_i];
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   const typename CGrid<ENTITY>::SCell& CGrid<ENTITY>::GetCellAt(size_t un_i,
                                                                 size_t un_j,
                                                                 size_t un_k) const {
      return m_psCells[m_unSizeI * m_unSizeJ * un_k +
                       m_unSizeI * un_j +
                       un_i];
   }

   /****************************************/
   /****************************************/

}
