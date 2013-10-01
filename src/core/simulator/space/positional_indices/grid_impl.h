namespace argos {

   /****************************************/
   /****************************************/

   static const Real EPSILON = 1e-6;

   /****************************************/
   /****************************************/

#define APPLY_ENTITY_OPERATION_TO_CELL(nI,nJ,nK)                        \
   {                                                                    \
      SCell& sCell = GetCellAt((nI), (nJ), (nK));                       \
      if((sCell.Timestamp == m_unCurTimestamp) &&                       \
         (! sCell.Entities.empty())) {                                  \
         for(typename CSet<ENTITY*>::iterator it = sCell.Entities.begin(); \
             it != sCell.Entities.end();                                \
             ++it) {                                                    \
            if(!c_operation(**it)) return;                              \
         }                                                              \
      }                                                                 \
   }

#define APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI,nJ,nK)              \
   {                                                                    \
      SCell& sCell = GetCellAt(nI, nJ, nK);                             \
      if((sCell.Timestamp == m_unCurTimestamp) &&                       \
         (! sCell.Entities.empty())) {                                  \
         for(typename CSet<ENTITY*>::iterator it = sCell.Entities.begin(); \
             it != sCell.Entities.end();                                \
             ++it) {                                                    \
            if(!c_operation(**it)) return;                              \
         }                                                              \
         if(b_stop_at_closest_match) return;                            \
      }                                                                 \
   }

#define APPLY_CELL_OPERATION_TO_CELL(nI,nJ,nK)          \
   {                                                    \
      SCell& sCell = GetCellAt((nI), (nJ), (nK));       \
      if(!c_operation((nI), (nJ), (nK), sCell)) return; \
   }

/****************************************/
/****************************************/

template<class ENTITY>
CGrid<ENTITY>::CGrid(const CVector3& c_area_min_corner,
                     const CVector3& c_area_max_corner,
                     SInt32 n_size_i,
                     SInt32 n_size_j,
                     SInt32 n_size_k) :
   m_cAreaMinCorner(c_area_min_corner),
   m_cAreaMaxCorner(c_area_max_corner),
   m_nSizeI(n_size_i),
   m_nSizeJ(n_size_j),
   m_nSizeK(n_size_k),
   m_cRangeX(m_cAreaMinCorner.GetX(), m_cAreaMaxCorner.GetX()),
   m_cRangeY(m_cAreaMinCorner.GetY(), m_cAreaMaxCorner.GetY()),
   m_cRangeZ(m_cAreaMinCorner.GetZ(), m_cAreaMaxCorner.GetZ()),
   m_unCurTimestamp(0),
   m_pcUpdateEntityOperation(NULL) {
   m_cCellSize.Set(m_cRangeX.GetSpan() / m_nSizeI,
                   m_cRangeY.GetSpan() / m_nSizeJ,
                   m_cRangeZ.GetSpan() / m_nSizeK);
   m_cInvCellSize.Set(1.0f / m_cCellSize.GetX(),
                      1.0f / m_cCellSize.GetY(),
                      1.0f / m_cCellSize.GetZ());
   m_psCells = new SCell[m_nSizeI * m_nSizeJ * m_nSizeK];
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
      for(SInt32 i = 0; i < m_nSizeI; ++i) {
         for(SInt32 j = 0; j < m_nSizeJ; ++j) {
            for(SInt32 k = 0; k < m_nSizeK; ++k) {
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
      try {
         SInt32 i, j, k;
         PositionToCell(i, j, k, c_position);
         const SCell& sCell = GetCellAt(i, j, k);
         if(sCell.Timestamp < m_unCurTimestamp) {
            c_entities.clear();
         }
         else {
            c_entities = sCell.Entities;
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("CGrid<ENTITY>::GetEntitiesAt() : Position <" << c_position << "> out of bounds X -> " << m_cRangeX << " Y -> " << m_cRangeY << " Z -> " << m_cRangeZ, ex);
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForAllEntities(CEntityOperation& c_operation) {
      for(typename CSet<ENTITY*>::iterator it = m_cEntities.begin();
          it != m_cEntities.end() && c_operation(**it);
          ++it);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInSphereRange(const CVector3& c_center,
                                                Real f_radius,
                                                CEntityOperation& c_operation) {
      /* Calculate cells for center */
      SInt32 nIC, nJC, nKC, nIR, nJR, nKR;
      PositionToCellUnsafe(nIC, nJC, nKC, c_center);
      if(nKC >= 0 && nKC < m_nSizeK) {
         /* Check circle center */
         if((nIC >= 0 && nIC < m_nSizeI) && (nJC >= 0 && nJC < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC, nKC);
         /* Calculate radia of circle */
         nIR = Floor(f_radius * m_cInvCellSize.GetX() + 0.5f);
         nJR = Floor(f_radius * m_cInvCellSize.GetY() + 0.5f);
         /* Go through diameter on j at i = 0 */
         if(nIC >= 0 && nIC < m_nSizeI) {
            for(SInt32 j = nJR; j > 0; --j) {
               if(nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC + j, nKC);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC - j, nKC);
            }
         }
         /* Go through diameter on i at j = 0 */
         if(nJC >= 0 && nJC < m_nSizeJ) {
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC, nKC);
            }
         }
         /* Go through cells with k = nKC */
         for(SInt32 j = nJR; j > 0; --j) {
            nIR = Floor(Sqrt(Max<Real>(0.0f, f_radius * f_radius - j * m_cCellSize.GetY() * j * m_cCellSize.GetY())) * m_cInvCellSize.GetX() + 0.5f);
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC + j, nKC);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC - j, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC + j, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC - j, nKC);
            }
         }
      }
      /* Go through other cells */
      nKR = Floor(f_radius * m_cInvCellSize.GetZ() + 0.5f);
      Real fCircleRadius2;
      for(SInt32 k = nKR; k > 0; --k) {
         /* Check center of circle at k and -k */
         if((nIC >= 0 && nIC < m_nSizeI) && (nJC >= 0 && nJC < m_nSizeJ)) {
            if(nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC, nKC + k);
            if(nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC, nKC - k);
         }
         /* Calculate radius of circle at k and -k */
         fCircleRadius2 = Max<Real>(0.0f, f_radius * f_radius - k * m_cCellSize.GetZ() * k * m_cCellSize.GetZ());
         /* Calculate radius of circle at j,k */
         nIR = Floor(Sqrt(fCircleRadius2) * m_cInvCellSize.GetX() + 0.5f);
         /* Go through diameter on i at j = 0 */
         if(nJC >= 0 && nJC < m_nSizeJ) {
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC, nKC - k);
            }
         }
         /* Calculate circle radius in cells on j */
         nJR = Floor(Sqrt(fCircleRadius2) * m_cInvCellSize.GetY() + 0.5f);
         for(SInt32 j = nJR; j > 0; --j) {
            /* Go through diameter on j at i = 0 */
            if(nIC >= 0 && nIC < m_nSizeI) {
               if(nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC + j, nKC + k);
               if(nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC + j, nKC - k);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC - j, nKC + k);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC, nJC - j, nKC - k);
            }
            /* Calculate radius of circle at j,k */
            nIR = Floor(Sqrt(Max<Real>(0.0f, fCircleRadius2 - j * m_cCellSize.GetY() * j * m_cCellSize.GetY())) * m_cInvCellSize.GetX() + 0.5f);
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC + j, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC + j, nKC - k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC - j, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC + i, nJC - j, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC + j, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC + j, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC - j, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_ENTITY_OPERATION_TO_CELL(nIC - i, nJC - j, nKC - k);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInBoxRange(const CVector3& c_center,
                                             const CVector3& c_half_size,
                                             CEntityOperation& c_operation) {
      /* Calculate cell range */
      SInt32 nI1, nJ1, nK1, nI2, nJ2, nK2;
      PositionToCellUnsafe(nI1, nJ1, nK1, c_center - c_half_size);
      ClampCoordinates(nI1, nJ1, nK1);
      PositionToCellUnsafe(nI2, nJ2, nK2, c_center + c_half_size);
      ClampCoordinates(nI2, nJ2, nK2);
      /* Go through cells */
      for(SInt32 k = nK1; k <= nK2; ++k) {
         for(SInt32 j = nJ1; j <= nJ2; ++j) {
            for(SInt32 i = nI1; i <= nI2; ++i) {
               APPLY_ENTITY_OPERATION_TO_CELL(i, j, k);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInCircleRange(const CVector3& c_center,
                                                Real f_radius,
                                                CEntityOperation& c_operation) {
      /* Make sure the Z coordinate is inside the range */
      if(! m_cRangeZ.WithinMinBoundIncludedMaxBoundIncluded(c_center.GetZ())) return;
      /* Calculate cells for center */
      SInt32 nI, nJ, nK;
      PositionToCellUnsafe(nI, nJ, nK, c_center);
      /* Check circle center */
      if((nI >= 0 && nI < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI, nJ, nK);
      /* Check circle diameter on I */
      SInt32 nID = Floor(f_radius * m_cInvCellSize.GetX() + 0.5f);
      for(SInt32 h = nID; h > 0; --h) {
         if((nI + h >= 0 && nI + h < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI + h, nJ, nK);
         if((nI - h >= 0 && nI - h < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI - h, nJ, nK);
      }
      /* Check circle diameter on J */
      SInt32 nJD = Floor(f_radius * m_cInvCellSize.GetY() + 0.5f);
      for(SInt32 h = nJD; h > 0; --h) {
         if((nI >= 0 && nI < m_nSizeI) && (nJ + h >= 0 && nJ + h < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI, nJ + h, nK);
         if((nI >= 0 && nI < m_nSizeI) && (nJ - h >= 0 && nJ - h < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI, nJ - h, nK);
      }
      /* Check rest of the circle */
      for(SInt32 i = nID; i > 0; --i) {
         nJD = Floor(Sqrt(f_radius * f_radius - i * m_cCellSize.GetX() * i * m_cCellSize.GetX()) * m_cInvCellSize.GetY() + 0.5f);
         for(SInt32 j = nJD; j > 0; --j) {
            if((nI + i >= 0 && nI + i < m_nSizeI) && (nJ + j >= 0 && nJ + j < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI + i, nJ + j, nK);
            if((nI + i >= 0 && nI + i < m_nSizeI) && (nJ - j >= 0 && nJ - j < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI + i, nJ - j, nK);
            if((nI - i >= 0 && nI - i < m_nSizeI) && (nJ + j >= 0 && nJ + j < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI - i, nJ + j, nK);
            if((nI - i >= 0 && nI - i < m_nSizeI) && (nJ - j >= 0 && nJ - j < m_nSizeJ)) APPLY_ENTITY_OPERATION_TO_CELL(nI - i, nJ - j, nK);
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesInRectangleRange(const CVector3& c_center,
                                                   const CVector2& c_half_size,
                                                   CEntityOperation& c_operation) {
      /* Calculate cell range */
      SInt32 nI1 = Min<SInt32>(m_nSizeI-1, Max<SInt32>(0, Floor((c_center.GetX() - c_half_size.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX())));
      SInt32 nJ1 = Min<SInt32>(m_nSizeJ-1, Max<SInt32>(0, Floor((c_center.GetY() - c_half_size.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY())));
      SInt32 nI2 = Min<SInt32>(m_nSizeI-1, Max<SInt32>(0, Floor((c_center.GetX() + c_half_size.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX())));
      SInt32 nJ2 = Min<SInt32>(m_nSizeJ-1, Max<SInt32>(0, Floor((c_center.GetY() + c_half_size.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY())));
      SInt32 nK  = Min<SInt32>(m_nSizeK-1, Max<SInt32>(0, Floor((c_center.GetZ()                      - m_cAreaMinCorner.GetZ()) * m_cInvCellSize.GetZ())));
      /* Go through cells */
      for(SInt32 j = nJ1; j <= nJ2; ++j) {
         for(SInt32 i = nI1; i <= nI2; ++i) {
            APPLY_ENTITY_OPERATION_TO_CELL(i, j, nK);
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForEntitiesAlongRay(const CRay3& c_ray,
                                           CEntityOperation& c_operation,
                                           bool b_stop_at_closest_match) {
      /* Transform ray start and end position into cell coordinates */
      SInt32 nI1, nJ1, nK1, nI2, nJ2, nK2;
      PositionToCellUnsafe(nI1, nJ1, nK1, c_ray.GetStart());
      ClampCoordinates(nI1, nJ1, nK1);
      PositionToCellUnsafe(nI2, nJ2, nK2, c_ray.GetEnd());
      ClampCoordinates(nI2, nJ2, nK2);
      /* Go through cells one by one, from start to end.
         Stop as soon as an entity is found.
         If the loop is completed, it means no entities were found -> no intersection.
      */
      /* Calculate deltas for later use */
      SInt32 nDI(Abs(nI2 - nI1));
      SInt32 nDJ(Abs(nJ2 - nJ1));
      SInt32 nDK(Abs(nK2 - nK1));
      /* Calculate the increment for each direction */
      SInt32 nSI(nI2 >= nI1 ? 1 : -1);
      SInt32 nSJ(nJ2 >= nJ1 ? 1 : -1);
      SInt32 nSK(nK2 >= nK1 ? 1 : -1);
      /* Set the starting cell */
      SInt32 nI(nI1), nJ(nJ1), nK(nK1);
      if(nDI >= nDJ && nDI >= nDK) {
         /* I is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEJ(3 * nDJ - nDI);
         SInt32 nEK(3 * nDK - nDI);
         APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDI; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nI += nSI;
            APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEJ > 0 && nEK > 0) {
               /* Advance on both the other axes */
               if(nEJ * nDK > nEK * nDJ) {
                  nJ += nSJ;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               else {
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nJ += nSJ;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               nEJ += 2 * (nDJ - nDI);
               nEK += 2 * (nDK - nDI);
            }
            else if(nEJ > 0) {
               /* Advance only on J */
               nJ += nSJ;
               APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               nEJ += 2 * (nDJ - nDI);
               nEK += 2 * nDK;
            }
            else {
               nEJ += 2 * nDJ;
               if(nEK > 0) {
                  /* Advance only on K */
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nEK += 2 * (nDK - nDI);
               }
               else {
                  nEK += 2 * nDK;
               }
            }
         }
      }
      else if(nDJ >= nDI && nDJ >= nDK) {
         /* J is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEI(3 * nDI - nDJ);
         SInt32 nEK(3 * nDK - nDJ);
         APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDJ; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nJ += nSJ;
            APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEI > 0 && nEK > 0) {
               /* Advance on both the other axes */
               if(nEI * nDK > nEK * nDI) {
                  nI += nSI;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               else {
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nI += nSI;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               nEI += 2 * (nDI - nDJ);
               nEK += 2 * (nDK - nDJ);
            }
            else if(nEI > 0) {
               /* Advance only on I */
               nI += nSI;
               APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               nEI += 2 * (nDI - nDJ);
               nEK += 2 * nDK;
            }
            else {
               nEI += 2 * nDI;
               if(nEK > 0) {
                  /* Advance only on K */
                  nK += nSK;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nEK += 2 * (nDK - nDJ);
               }
               else {
                  nEK += 2 * nDK;
               }
            }
         }
      }
      else {
         /* K is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEI(3 * nDI - nDK);
         SInt32 nEJ(3 * nDJ - nDK);
         APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDK; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nK += nSK;
            APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEI > 0 && nEJ > 0) {
               /* Advance on both the other axes */
               if(nEI * nDJ > nEJ * nDI) {
                  nI += nSI;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nJ += nSJ;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               else {
                  nJ += nSJ;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nI += nSI;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               }
               nEI += 2 * (nDI - nDK);
               nEJ += 2 * (nDJ - nDK);
            }
            else if(nEI > 0) {
               /* Advance only on I */
               nI += nSI;
               APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
               nEI += 2 * (nDI - nDK);
               nEJ += 2 * nDJ;
            }
            else {
               nEI += 2 * nDI;
               if(nEJ > 0) {
                  /* Advance only on J */
                  nJ += nSJ;
                  APPLY_ENTITY_OPERATION_TO_CELL_ALONG_RAY(nI, nJ, nK);
                  nEJ += 2 * (nDJ - nDK);
               }
               else {
                  nEJ += 2 * nDJ;
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForAllCells(CCellOperation& c_operation) {
      for(SInt32 k = 0; k < m_nSizeK; ++k) {
         for(SInt32 j = 0; j < m_nSizeJ; ++j) {
            for(SInt32 i = 0; i < m_nSizeI; ++i) {
               APPLY_CELL_OPERATION_TO_CELL(i, j, k);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForCellsInSphereRange(const CVector3& c_center,
                                             Real f_radius,
                                             CCellOperation& c_operation) {
      /* Calculate cells for center */
      SInt32 nIC, nJC, nKC, nIR, nJR, nKR;
      PositionToCellUnsafe(nIC, nJC, nKC, c_center);
      if(nKC >= 0 && nKC < m_nSizeK) {
         /* Check circle center */
         if((nIC >= 0 && nIC < m_nSizeI) && (nJC >= 0 && nJC < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC, nKC);
         /* Calculate radia of circle */
         nIR = Floor(f_radius * m_cInvCellSize.GetX() + 0.5f);
         nJR = Floor(f_radius * m_cInvCellSize.GetY() + 0.5f);
         /* Go through diameter on j at i = 0 */
         if(nIC >= 0 && nIC < m_nSizeI) {
            for(SInt32 j = nJR; j > 0; --j) {
               if(nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC + j, nKC);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC - j, nKC);
            }
         }
         /* Go through diameter on i at j = 0 */
         if(nJC >= 0 && nJC < m_nSizeJ) {
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC, nKC);
            }
         }
         /* Go through cells with k = nKC */
         for(SInt32 j = nJR; j > 0; --j) {
            nIR = Floor(Sqrt(Max<Real>(0.0f, f_radius * f_radius - j * m_cCellSize.GetY() * j * m_cCellSize.GetY())) * m_cInvCellSize.GetX() + 0.5f);
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC + j, nKC);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC - j, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC + j, nKC);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC - j, nKC);
            }
         }
      }
      /* Go through other cells */
      nKR = Floor(f_radius * m_cInvCellSize.GetZ() + 0.5f);
      Real fCircleRadius2;
      for(SInt32 k = nKR; k > 0; --k) {
         /* Check center of circle at k and -k */
         if((nIC >= 0 && nIC < m_nSizeI) && (nJC >= 0 && nJC < m_nSizeJ)) {
            if(nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC, nKC + k);
            if(nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC, nKC - k);
         }
         /* Calculate radius of circle at k and -k */
         fCircleRadius2 = Max<Real>(0.0f, f_radius * f_radius - k * m_cCellSize.GetZ() * k * m_cCellSize.GetZ());
         /* Calculate radius of circle at j,k */
         nIR = Floor(Sqrt(fCircleRadius2) * m_cInvCellSize.GetX() + 0.5f);
         /* Go through diameter on i at j = 0 */
         if(nJC >= 0 && nJC < m_nSizeJ) {
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC, nKC - k);
            }
         }
         /* Calculate circle radius in cells on j */
         nJR = Floor(Sqrt(fCircleRadius2) * m_cInvCellSize.GetY() + 0.5f);
         for(SInt32 j = nJR; j > 0; --j) {
            /* Go through diameter on j at i = 0 */
            if(nIC >= 0 && nIC < m_nSizeI) {
               if(nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC + j, nKC + k);
               if(nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC + j, nKC - k);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC - j, nKC + k);
               if(nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC, nJC - j, nKC - k);
            }
            /* Calculate radius of circle at j,k */
            nIR = Floor(Sqrt(Max<Real>(0.0f, fCircleRadius2 - j * m_cCellSize.GetY() * j * m_cCellSize.GetY())) * m_cInvCellSize.GetX() + 0.5f);
            for(SInt32 i = nIR; i > 0; --i) {
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC + j, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC + j, nKC - k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC - j, nKC + k);
               if(nIC + i >= 0 && nIC + i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC + i, nJC - j, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC + j, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC + j >= 0 && nJC + j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC + j, nKC - k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC + k >= 0 && nKC + k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC - j, nKC + k);
               if(nIC - i >= 0 && nIC - i < m_nSizeI && nJC - j >= 0 && nJC - j < m_nSizeJ && nKC - k >= 0 && nKC - k < m_nSizeK) APPLY_CELL_OPERATION_TO_CELL(nIC - i, nJC - j, nKC - k);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForCellsInBoxRange(const CVector3& c_center,
                                          const CVector3& c_half_size,
                                          CCellOperation& c_operation) {
      /* Calculate cell range */
      SInt32 nI1, nJ1, nK1, nI2, nJ2, nK2;
      PositionToCellUnsafe(nI1, nJ1, nK1, c_center - c_half_size);
      ClampCoordinates(nI1, nJ1, nK1);
      PositionToCellUnsafe(nI2, nJ2, nK2, c_center + c_half_size);
      ClampCoordinates(nI2, nJ2, nK2);
      /* Go through cells */
      for(SInt32 k = nK1; k <= nK2; ++k) {
         for(SInt32 j = nJ1; j <= nJ2; ++j) {
            for(SInt32 i = nI1; i <= nI2; ++i) {
               APPLY_CELL_OPERATION_TO_CELL(i, j, k);
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForCellsInCircleRange(const CVector3& c_center,
                                             Real f_radius,
                                             CCellOperation& c_operation) {
      /* Make sure the Z coordinate is inside the range */
      if(! m_cRangeZ.WithinMinBoundIncludedMaxBoundIncluded(c_center.GetZ())) return;
      /* Calculate cells for center */
      SInt32 nI, nJ, nK;
      PositionToCellUnsafe(nI, nJ, nK, c_center);
      /* Check circle center */
      if((nI >= 0 && nI < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
      /* Check circle diameter on I */
      SInt32 nID = Floor(f_radius * m_cInvCellSize.GetX() + 0.5f);
      for(SInt32 h = nID; h > 0; --h) {
         if((nI + h >= 0 && nI + h < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI + h, nJ, nK);
         if((nI - h >= 0 && nI - h < m_nSizeI) && (nJ >= 0 && nJ < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI - h, nJ, nK);
      }
      /* Check circle diameter on J */
      SInt32 nJD = Floor(f_radius * m_cInvCellSize.GetY() + 0.5f);
      for(SInt32 h = nJD; h > 0; --h) {
         if((nI >= 0 && nI < m_nSizeI) && (nJ + h >= 0 && nJ + h < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI, nJ + h, nK);
         if((nI >= 0 && nI < m_nSizeI) && (nJ - h >= 0 && nJ - h < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI, nJ - h, nK);
      }
      /* Check rest of the circle */
      for(SInt32 i = nID; i > 0; --i) {
         nJD = Floor(Sqrt(f_radius * f_radius - i * m_cCellSize.GetX() * i * m_cCellSize.GetX()) * m_cInvCellSize.GetY() + 0.5f);
         for(SInt32 j = nJD; j > 0; --j) {
            if((nI + i >= 0 && nI + i < m_nSizeI) && (nJ + j >= 0 && nJ + j < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI + i, nJ + j, nK);
            if((nI + i >= 0 && nI + i < m_nSizeI) && (nJ - j >= 0 && nJ - j < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI + i, nJ - j, nK);
            if((nI - i >= 0 && nI - i < m_nSizeI) && (nJ + j >= 0 && nJ + j < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI - i, nJ + j, nK);
            if((nI - i >= 0 && nI - i < m_nSizeI) && (nJ - j >= 0 && nJ - j < m_nSizeJ)) APPLY_CELL_OPERATION_TO_CELL(nI - i, nJ - j, nK);
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForCellsInRectangleRange(const CVector3& c_center,
                                                const CVector2& c_half_size,
                                                CCellOperation& c_operation) {
      /* Calculate cell range */
      SInt32 nI1 = Min<SInt32>(m_nSizeI-1, Max<SInt32>(0, Floor((c_center.GetX() - c_half_size.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX())));
      SInt32 nJ1 = Min<SInt32>(m_nSizeJ-1, Max<SInt32>(0, Floor((c_center.GetY() - c_half_size.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY())));
      SInt32 nI2 = Min<SInt32>(m_nSizeI-1, Max<SInt32>(0, Floor((c_center.GetX() + c_half_size.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX())));
      SInt32 nJ2 = Min<SInt32>(m_nSizeJ-1, Max<SInt32>(0, Floor((c_center.GetY() + c_half_size.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY())));
      SInt32 nK  = Min<SInt32>(m_nSizeK-1, Max<SInt32>(0, Floor((c_center.GetZ()                      - m_cAreaMinCorner.GetZ()) * m_cInvCellSize.GetZ())));
      /* Go through cells */
      for(SInt32 j = nJ1; j <= nJ2; ++j) {
         for(SInt32 i = nI1; i <= nI2; ++i) {
            APPLY_CELL_OPERATION_TO_CELL(i, j, nK);
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ForCellsAlongRay(const CRay3& c_ray,
                                        CCellOperation& c_operation) {
      /* Transform ray start and end position into cell coordinates */
      SInt32 nI1, nJ1, nK1, nI2, nJ2, nK2;
      PositionToCellUnsafe(nI1, nJ1, nK1, c_ray.GetStart());
      ClampCoordinates(nI1, nJ1, nK1);
      PositionToCellUnsafe(nI2, nJ2, nK2, c_ray.GetEnd());
      ClampCoordinates(nI2, nJ2, nK2);
      /* Go through cells one by one, from start to end.
         Stop as soon as an entity is found.
         If the loop is completed, it means no entities were found -> no intersection.
      */
      /* Calculate deltas for later use */
      SInt32 nDI(Abs(nI2 - nI1));
      SInt32 nDJ(Abs(nJ2 - nJ1));
      SInt32 nDK(Abs(nK2 - nK1));
      /* Calculate the increment for each direction */
      SInt32 nSI(nI2 >= nI1 ? 1 : -1);
      SInt32 nSJ(nJ2 >= nJ1 ? 1 : -1);
      SInt32 nSK(nK2 >= nK1 ? 1 : -1);
      /* Set the starting cell */
      SInt32 nI(nI1), nJ(nJ1), nK(nK1);
      if(nDI >= nDJ && nDI >= nDK) {
         /* I is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEJ(3 * nDJ - nDI);
         SInt32 nEK(3 * nDK - nDI);
         APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDI; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nI += nSI;
            APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEJ > 0 && nEK > 0) {
               /* Advance on both the other axes */
               if(nEJ * nDK > nEK * nDJ) {
                  nJ += nSJ;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               else {
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nJ += nSJ;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               nEJ += 2 * (nDJ - nDI);
               nEK += 2 * (nDK - nDI);
            }
            else if(nEJ > 0) {
               /* Advance only on J */
               nJ += nSJ;
               APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               nEJ += 2 * (nDJ - nDI);
               nEK += 2 * nDK;
            }
            else {
               nEJ += 2 * nDJ;
               if(nEK > 0) {
                  /* Advance only on K */
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nEK += 2 * (nDK - nDI);
               }
               else {
                  nEK += 2 * nDK;
               }
            }
         }
      }
      else if(nDJ >= nDI && nDJ >= nDK) {
         /* J is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEI(3 * nDI - nDJ);
         SInt32 nEK(3 * nDK - nDJ);
         APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDJ; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nJ += nSJ;
            APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEI > 0 && nEK > 0) {
               /* Advance on both the other axes */
               if(nEI * nDK > nEK * nDI) {
                  nI += nSI;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               else {
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nI += nSI;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               nEI += 2 * (nDI - nDJ);
               nEK += 2 * (nDK - nDJ);
            }
            else if(nEI > 0) {
               /* Advance only on I */
               nI += nSI;
               APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               nEI += 2 * (nDI - nDJ);
               nEK += 2 * nDK;
            }
            else {
               nEI += 2 * nDI;
               if(nEK > 0) {
                  /* Advance only on K */
                  nK += nSK;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nEK += 2 * (nDK - nDJ);
               }
               else {
                  nEK += 2 * nDK;
               }
            }
         }
      }
      else {
         /* K is the driving axis */
         /* Calculate error used to know when to move on other axes */
         SInt32 nEI(3 * nDI - nDK);
         SInt32 nEJ(3 * nDJ - nDK);
         APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
         /* Cycle through cells */
         for(SInt32 nCell = nDK; nCell > 0; --nCell) {
            /* Advance on driving axis */
            nK += nSK;
            APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
            /* Advance on other axes, if necessary */
            if(nEI > 0 && nEJ > 0) {
               /* Advance on both the other axes */
               if(nEI * nDJ > nEJ * nDI) {
                  nI += nSI;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nJ += nSJ;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               else {
                  nJ += nSJ;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nI += nSI;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               }
               nEI += 2 * (nDI - nDK);
               nEJ += 2 * (nDJ - nDK);
            }
            else if(nEI > 0) {
               /* Advance only on I */
               nI += nSI;
               APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
               nEI += 2 * (nDI - nDK);
               nEJ += 2 * nDJ;
            }
            else {
               nEI += 2 * nDI;
               if(nEJ > 0) {
                  /* Advance only on J */
                  nJ += nSJ;
                  APPLY_CELL_OPERATION_TO_CELL(nI, nJ, nK);
                  nEJ += 2 * (nDJ - nDK);
               }
               else {
                  nEJ += 2 * nDJ;
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::UpdateCell(SInt32 n_i,
                                  SInt32 n_j,
                                  SInt32 n_k,
                                  ENTITY& c_entity) {
      if((n_i >= 0) && (n_i < m_nSizeI) &&
         (n_j >= 0) && (n_j < m_nSizeJ) &&
         (n_k >= 0) && (n_k < m_nSizeK)) {
         SCell& sCell = GetCellAt(n_i, n_j, n_k);
         if(sCell.Timestamp < m_unCurTimestamp) {
            sCell.Entities.clear();
            sCell.Timestamp = m_unCurTimestamp;
         }
         sCell.Entities.insert(&c_entity);
      }
      else {
         THROW_ARGOSEXCEPTION("CGrid<ENTITY>::UpdateCell() : index (" << n_i << "," << n_j << "," << n_k << ") out of bounds (" << m_nSizeI-1 << "," << m_nSizeJ-1 << "," << m_nSizeK-1 << ")");
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::SetUpdateEntityOperation(CEntityOperation* pc_operation) {
      m_pcUpdateEntityOperation = pc_operation;
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::PositionToCell(SInt32& n_i,
                                      SInt32& n_j,
                                      SInt32& n_k,
                                      const CVector3& c_position) const {
      if(m_cRangeX.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetX()) &&
         m_cRangeY.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetY()) &&
         m_cRangeZ.WithinMinBoundIncludedMaxBoundIncluded(c_position.GetZ())) {
         n_i = Floor((c_position.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX());
         n_j = Floor((c_position.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY());
         n_k = Floor((c_position.GetZ() - m_cAreaMinCorner.GetZ()) * m_cInvCellSize.GetZ());
      }
      else {
         THROW_ARGOSEXCEPTION("CGrid<ENTITY>::PositionToCell() : Position <" << c_position << "> out of bounds X -> " << m_cRangeX << " Y -> " << m_cRangeY << " Z -> " << m_cRangeZ);
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::PositionToCellUnsafe(SInt32& n_i,
                                            SInt32& n_j,
                                            SInt32& n_k,
                                            const CVector3& c_position) const {
      n_i = Floor((c_position.GetX() - m_cAreaMinCorner.GetX()) * m_cInvCellSize.GetX());
      n_j = Floor((c_position.GetY() - m_cAreaMinCorner.GetY()) * m_cInvCellSize.GetY());
      n_k = Floor((c_position.GetZ() - m_cAreaMinCorner.GetZ()) * m_cInvCellSize.GetZ());
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CGrid<ENTITY>::ClampCoordinates(SInt32& n_i,
                                        SInt32& n_j,
                                        SInt32& n_k) const {
      if(n_i < 0) n_i = 0;
      else if(n_i >= m_nSizeI) n_i = m_nSizeI - 1;
      if(n_j < 0) n_j = 0;
      else if(n_j >= m_nSizeJ) n_j = m_nSizeJ - 1;
      if(n_k < 0) n_k = 0;
      else if(n_k >= m_nSizeK) n_k = m_nSizeK - 1;
   }

   /****************************************/
   /****************************************/
   
   template<class ENTITY>
   void CGrid<ENTITY>::ClampCoordinates(CVector3& c_pos) const {
      if(c_pos.GetX() < m_cRangeX.GetMin()) c_pos.SetX(m_cRangeX.GetMin() + EPSILON);
      else if(c_pos.GetX() > m_cRangeX.GetMax()) c_pos.SetX(m_cRangeX.GetMax() - EPSILON);
      if(c_pos.GetY() < m_cRangeY.GetMin()) c_pos.SetY(m_cRangeY.GetMin() + EPSILON);
      else if(c_pos.GetY() > m_cRangeY.GetMax()) c_pos.SetY(m_cRangeY.GetMax() - EPSILON);
      if(c_pos.GetZ() < m_cRangeZ.GetMin()) c_pos.SetZ(m_cRangeZ.GetMin() + EPSILON);
      else if(c_pos.GetZ() > m_cRangeZ.GetMax()) c_pos.SetZ(m_cRangeZ.GetMax() - EPSILON);
   }

   /****************************************/
   /****************************************/
   
   template<class ENTITY>
   typename CGrid<ENTITY>::SCell& CGrid<ENTITY>::GetCellAt(SInt32 n_i,
                                                           SInt32 n_j,
                                                           SInt32 n_k) {
      return m_psCells[m_nSizeI * m_nSizeJ * n_k +
                       m_nSizeI * n_j +
                       n_i];
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   const typename CGrid<ENTITY>::SCell& CGrid<ENTITY>::GetCellAt(SInt32 n_i,
                                                                 SInt32 n_j,
                                                                 SInt32 n_k) const {
      return m_psCells[m_nSizeI * m_nSizeJ * n_k +
                       m_nSizeI * n_j +
                       n_i];
   }

   /****************************************/
   /****************************************/
   
}
