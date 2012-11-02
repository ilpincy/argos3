/**
 * @file argos3/simulator/space/space_hash_native.h
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SPACE_HASH_NATIVE_H
#define SPACE_HASH_NATIVE_H

#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   template <class Element, class Updater> class CSpaceHashNative : public CSpaceHash<Element,Updater> {

   private:

      struct SBucket {

         struct SBucketData {
            Element* Elem;
            SInt32 I,J,K;
            SBucketData* Next;
         
            SBucketData(Element& c_element,
                        SInt32 n_i,
                        SInt32 n_j,
                        SInt32 n_k,
                        SBucketData* ps_next = NULL) :
               Elem(&c_element),
               I(n_i),
               J(n_j),
               K(n_k),
               Next(ps_next) {}

         };

         UInt64 StoreTimestamp;
         SBucketData* ElementList;

         SBucket() :
            StoreTimestamp(0),
            ElementList(NULL) {}

         ~SBucket() {
            Clear();
         }

         inline bool Empty() const {
            return (ElementList == NULL);
         }

         inline void Clear() {
            if(!Empty()) {
               SBucketData* psCur = ElementList;
               SBucketData* psNext = psCur->Next;
               do {
                  delete psCur;
                  psCur = psNext;
                  if(psCur) psNext = psCur->Next;
               } while(psCur);
               ElementList = NULL;
            }
         }

         inline void Add(Element& c_element,
                         SInt32 n_i,
                         SInt32 n_j,
                         SInt32 n_k) {
            if(Empty()) ElementList = new SBucketData(c_element, n_i, n_j, n_k);
            else ElementList = new SBucketData(c_element, n_i, n_j, n_k, ElementList);
         }
         
         inline bool Exists(const Element& c_element,
                            SInt32 n_i,
                            SInt32 n_j,
                            SInt32 n_k) {
            SBucketData* psCur = ElementList;
            while(psCur) {
               if(psCur->Elem == &c_element &&
                  psCur->I == n_i &&
                  psCur->J == n_j &&
                  psCur->K == n_k) return true;
               psCur = psCur->Next;
            }
            return false;
         }

      };

   public:

      CSpaceHashNative() :
         m_psBuckets(NULL),
         m_unCurrentStoreTimestamp(0) {}

      ~CSpaceHashNative() {
         Clear();
         delete[] m_psBuckets;
      }

      inline void Clear() {
         for(size_t i = 0; i < CSpaceHash<Element,Updater>::GetSize(); ++i) {
            m_psBuckets[i].Clear();
         }
      }

      inline virtual void SetSize(size_t un_size) {
         CSpaceHash<Element,Updater>::SetSize(un_size);
         m_psBuckets = new SBucket[CSpaceHash<Element,Updater>::GetSize()];
         // TODO: rehash!
      }

      inline virtual void Update() {
         /* Set the current store time stamp */
         m_unCurrentStoreTimestamp++;
         /* Call base class method */
         CSpaceHash<Element,Updater>::Update();
      }

      inline virtual void UpdateCell(SInt32 n_i,
                                     SInt32 n_j,
                                     SInt32 n_k,
                                     Element& c_element) {
         /* Calculate the hash of the current position */
         SInt32 nHash = CSpaceHash<Element,Updater>::CoordinateHash(n_i, n_j, n_k);
         /* Get a reference to the bucket */
         SBucket& sBucket = m_psBuckets[nHash];
         /* Check if the bucket's content is obsolete */
         if(sBucket.StoreTimestamp == m_unCurrentStoreTimestamp) {
            /* Add the current element to the bucket */
            if(! sBucket.Exists(c_element, n_i, n_j, n_k)) {
               sBucket.Add(c_element, n_i, n_j, n_k);
            }
         }
         else {
            /* The bucket's content is obsolete, erase it */
            sBucket.Clear();
            /* Set the store timestamp to the current time */
            sBucket.StoreTimestamp = m_unCurrentStoreTimestamp;
            /* Add the current element to the bucket */
            sBucket.Add(c_element, n_i, n_j, n_k);
         }
      }

      inline virtual bool CheckCell(SInt32 n_i,
                                    SInt32 n_j,
                                    SInt32 n_k,
                                    typename CSpaceHash<Element,Updater>::TElementList& t_elements) {
         /* In the beginning, no new elements have been found */
         bool bNewElements = false;
         /* Calculate the hash of the current position */
         SInt32 nHash = CSpaceHash<Element,Updater>::CoordinateHash(n_i, n_j, n_k);
         /* Get a reference to the bucket */
         SBucket& sBucket = m_psBuckets[nHash];
         /* Visit the bucket IF:
            1. its data is up-to-date AND
            2. it is not empty
          */
         if((sBucket.StoreTimestamp == m_unCurrentStoreTimestamp) && /* 1. */
            !sBucket.Empty()) /* 2. */ {
            /* Check the bucket's elements */
            for(typename SBucket::SBucketData* psCur = sBucket.ElementList;
                psCur;
                psCur = psCur->Next) {
               /* Check that the element is in the wanted cell */
               if(n_i == psCur->I &&
                  n_j == psCur->J &&
                  n_k == psCur->K) {
                  /* We have a new element to add to the list */
                  bNewElements = true;
                  t_elements.insert(psCur->Elem);
               }
            }
         }
         return bNewElements;
      }

   private:

      SBucket* m_psBuckets;
      UInt64 m_unCurrentStoreTimestamp;

   };

}

#endif
