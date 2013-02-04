/**
 * @file <argos3/core/simulator/space/space_hash_native.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPACE_HASH_NATIVE_H
#define SPACE_HASH_NATIVE_H

#include <argos3/core/simulator/space/space_hash.h>

namespace argos {

   /**
    * A space hash implementation that does not rely on std::map or std::tr1:unordered_map.
    * It is much more performant than the standard hash table implementations.
    */
   template <class Element, class Updater>
   class CSpaceHashNative : public CSpaceHash<Element,Updater> {

   private:

      /**
       * A bucket of the native space hash.
       */
      struct SBucket {

         /**
          * An item of data held by each bucket.
          * These data items are organized into a simple linked list.
          */
         struct SBucketData {
            /**
             * The element indexed by this data item.
             */
            Element* Elem;
            /**
             * The space hash cell coordinate corresponding to this data item.
             */
            SInt32 I,J,K;
            /**
             * Pointer to the next data item in the list, or NULL if this is the last
             */
            SBucketData* Next;

            /**
             * Struct constructor.
             * @param c_element The element to reference.
             * @param n_i The X coordinate of the space hash cell.
             * @param n_j The Y coordinate of the space hash cell.
             * @param n_k The Z coordinate of the space hash cell.
             */
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

         /**
          * The step of the last storage operation in this bucket.
          */
         UInt64 StoreTimestamp;

         /**
          * The list of data items held in this bucket.
          */
         SBucketData* ElementList;

         /**
          * Struct constructor.
          * The bucket is created empty.
          */
         SBucket() :
            StoreTimestamp(0),
            ElementList(NULL) {}

         /**
          * Struct destructor.
          * It clears all the data items.
          * @see SBucketData
          */
         ~SBucket() {
            Clear();
         }

         /**
          * Returns <tt>true</tt> if the bucket is empty.
          * @return <tt>true</tt> if the bucket is empty.
          */
         inline bool Empty() const {
            return (ElementList == NULL);
         }

         /**
          * Deletes all the data items of this bucket.
          * @see SBucketData
          */
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

         /**
          * Adds an element to this bucket.
          * @param c_element The element to add.
          * @param n_i The X coordinate of the space hash cell.
          * @param n_j The Y coordinate of the space hash cell.
          * @param n_k The Z coordinate of the space hash cell.
          */
         inline void Add(Element& c_element,
                         SInt32 n_i,
                         SInt32 n_j,
                         SInt32 n_k) {
            if(Empty()) ElementList = new SBucketData(c_element, n_i, n_j, n_k);
            else ElementList = new SBucketData(c_element, n_i, n_j, n_k, ElementList);
         }

         /**
          * Returns <tt>true</tt> if a certain data item is already in the bucket.
          * @param c_element The element to search for.
          * @param n_i The corresponding X coordinate of the space hash cell.
          * @param n_j The corresponding Y coordinate of the space hash cell.
          * @param n_k The corresponding Z coordinate of the space hash cell.
          */
         bool Exists(const Element& c_element,
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

      /**
       * Class constructor.
       * The size of the space hash (the number of buckets) is set to zero.
       * To be able to use the space hash, you first must set the size to an
       * appropriate value.
       * @see SetSize()
       */
      CSpaceHashNative() :
         m_psBuckets(NULL),
         m_unCurrentStoreTimestamp(0) {}

      /**
       * Class destructor.
       */
      ~CSpaceHashNative() {
         Clear();
         delete[] m_psBuckets;
      }

      /**
       * Empties all the buckets in the space hash.
       */
      inline void Clear() {
         for(size_t i = 0; i < CSpaceHash<Element,Updater>::GetSize(); ++i) {
            m_psBuckets[i].Clear();
         }
      }

      /**
       * Sets the size of the space hash.
       * The size corresponds to the number of buckets.
       * @param un_size The size of the space hash.
       */
      inline virtual void SetSize(size_t un_size) {
         CSpaceHash<Element,Updater>::SetSize(un_size);
         m_psBuckets = new SBucket[CSpaceHash<Element,Updater>::GetSize()];
         /** @todo rehash */
      }

      /**
       * Updates the entire space hash.
       * It calls the updater for all the elements held by the space hash.
       * @see CSpaceHashUpdater
       */
      inline virtual void Update() {
         /* Set the current store time stamp */
         m_unCurrentStoreTimestamp++;
         /* Call base class method */
         CSpaceHash<Element,Updater>::Update();
      }

      /**
       * Adds an element to a cell of the space hash.
       * @param n_x The x coordinate of the cell.
       * @param n_y The y coordinate of the cell.
       * @param n_z The z coordinate of the cell.
       * @param c_element The element to add.
       */
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

      /**
       * Looks for elements to process in a cell
       * @param n_i The X coordinate of the space hash cell.
       * @param n_j The Y coordinate of the space hash cell.
       * @param n_k The Z coordinate of the space hash cell.
       * @param The list of elements to process.
       */
      virtual bool CheckCell(SInt32 n_i,
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

      virtual void Dump(CARGoSLog& c_os) {
         for(size_t i = 0; i < CSpaceHash<Element,Updater>::GetSize(); ++i) {
            if((m_psBuckets[i].StoreTimestamp == m_unCurrentStoreTimestamp) &&
               !m_psBuckets[i].Empty()) {
               c_os << "BUCKET " << i << std::endl;
               for(typename SBucket::SBucketData* psCur = m_psBuckets[i].ElementList;
                   psCur;
                   psCur = psCur->Next) {
                  c_os << "   "
                       << psCur->Elem->GetId()
                       << " ("
                       << psCur->I
                       << ","
                       << psCur->J
                       << ","
                       << psCur->K
                       << ")"
                       << std::endl;
               }
            }
         }
      }
      
   private:

      /**
       * The list of buckets in this space hash.
       */
      SBucket* m_psBuckets;

      /**
       * The current timestamp.
       * Internally used to check whether the content of a bucket is up-to-date.
       * @see Update
       * @see UpdateCell
       * @see CheckCell
       */
      UInt64 m_unCurrentStoreTimestamp;

   };

}

#endif
