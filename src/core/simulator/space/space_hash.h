/**
 * @file argos3/simulator/space/space_hash.h
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SPACE_HASH_H
#define SPACE_HASH_H

namespace argos {
   class CSpace;
   class CVector3;
   class CRay;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/ray.h>
#include <tr1/unordered_set>

namespace argos {

   /****************************************/
   /****************************************/

   template <class Element> class CAbstractSpaceHash {

   public:

      typedef std::tr1::unordered_set<Element*> TElementList;

   public:

      CAbstractSpaceHash() : m_unSize(0) {}

      virtual ~CAbstractSpaceHash() {}

      inline virtual void AddElement(Element& c_element) {
         m_tElements.insert(&c_element);
      }

      inline TElementList& GetElements() {
         return m_tElements;
      }

      inline void RemoveElement(Element& c_element) {
         typename TElementList::iterator it = m_tElements.find(&c_element);
         if(it != m_tElements.end()) {
            m_tElements.erase(it);
         }
         else {
            THROW_ARGOSEXCEPTION("Element not found when removing it from space hash.");
         }
      }

      inline size_t GetSize() {
         return m_unSize;
      }

      inline CVector3& GetCellSize() {
         return m_cCellSize;
      }

      inline CVector3& GetInvCellSize() {
         return m_cInvCellSize;
      }

      inline virtual void SetSize(size_t un_size) {
         m_unSize = un_size;
      }

      inline virtual void SetCellSize(const CVector3& c_cell_size) {
         m_cCellSize = c_cell_size;
         m_cInvCellSize.Set(1.0f / m_cCellSize.GetX(),
                            1.0f / m_cCellSize.GetY(),
                            1.0f / m_cCellSize.GetZ());
      }

      virtual void Update() = 0;

      virtual void UpdateCell(SInt32 n_x,
                              SInt32 n_y,
                              SInt32 n_z,
                              Element& c_element) = 0;

      inline virtual SInt32 SpaceToHashTable(Real c_coord,
                                             UInt32 un_axis) {
         return RoundClosestToZero(c_coord * GetInvCellSize()[un_axis]);
      }

      inline virtual Real HashTableToSpace(SInt32 n_coord,
                                           UInt32 un_axis) {
         return n_coord * m_cCellSize[un_axis];
      }

      inline virtual void SpaceToHashTable(SInt32& n_i,
                                           SInt32& n_j,
                                           SInt32& n_k,
                                           const CVector3& c_pos) {
         n_i = RoundClosestToZero(c_pos.GetX() * CAbstractSpaceHash::GetInvCellSize().GetX());
         n_j = RoundClosestToZero(c_pos.GetY() * CAbstractSpaceHash::GetInvCellSize().GetY());
         n_k = RoundClosestToZero(c_pos.GetZ() * CAbstractSpaceHash::GetInvCellSize().GetZ());
      }

      virtual bool CheckCell(SInt32 n_i,
                             SInt32 n_j,
                             SInt32 n_k,
                             TElementList& t_elements) = 0;

   protected:

      inline UInt32 CoordinateHash(SInt32 n_i,
                                   SInt32 n_j,
                                   SInt32 n_k) {
         return
            ((73856093u * n_i) ^
             (19349663u * n_j) ^
             (83492791u * n_k)) %
            m_unSize;
      }
      
   private:

      TElementList m_tElements;
      size_t m_unSize;
      CVector3 m_cCellSize;
      CVector3 m_cInvCellSize;

   };

   /****************************************/
   /****************************************/

   template <class Element> class CSpaceHashUpdater {

     public:

      virtual ~CSpaceHashUpdater() {}

      virtual void operator()(CAbstractSpaceHash<Element>& c_space_hash,
                              Element& c_element) = 0;

   };

   /****************************************/
   /****************************************/

   template <class Element, class Updater> class CSpaceHash : public CAbstractSpaceHash<Element> {

   public:

      inline virtual void Update() {
         /* Go through all the entities */
         for(typename CAbstractSpaceHash<Element>::TElementList::const_iterator el = CAbstractSpaceHash<Element>::GetElements().begin();
             el != CAbstractSpaceHash<Element>::GetElements().end(); ++el) {
            m_cUpdater(*this, **el);
         }
      }

   private:

      Updater m_cUpdater;

   };

   /****************************************/
   /****************************************/

}

#endif
