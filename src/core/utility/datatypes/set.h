/**
 * @file <argos3/core/utility/datatypes/set.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef SET_H
#define SET_H

#include <cstddef>
#include <iterator>

namespace argos {

   /**
    * The data container of CSet.
    * @see CSet
    */
   template <class T>
   struct SSetElement {
      T Data;
      SSetElement* Previous;
      SSetElement* Next;
      
      SSetElement(const T& t_data,
                  SSetElement* ps_prev = NULL,
                  SSetElement* ps_next = NULL) :
         Data(t_data),
         Previous(ps_prev),
         Next(ps_next) {}
   };

   /**
    * The CSet iterator.
    * @see CSet
    * @see SSetElement
    */
   template<class CONTAINED_TYPE, class REFERENCED_TYPE>
   class CSetIterator {

   public:

      typedef std::forward_iterator_tag iterator_category;
      typedef REFERENCED_TYPE value_type;
      typedef std::ptrdiff_t difference_type;
      typedef REFERENCED_TYPE& reference;
      typedef REFERENCED_TYPE* pointer;

   public:

      CSetIterator(SSetElement<CONTAINED_TYPE>* ps_elem = NULL) :
         m_psElem(ps_elem) {}

      CSetIterator(const CSetIterator& c_it) :
         m_psElem(c_it.m_psElem) {}

      CSetIterator& operator=(const CSetIterator& c_it) {
         if(this != &c_it) {
            m_psElem = c_it.m_psElem;
         }
         return *this;
      }

      reference operator*() {
         return m_psElem->Data;
      }

      pointer operator->() {
         return &(m_psElem->Data);
      }

      CSetIterator& operator++() {
         m_psElem = m_psElem->Next;
         return *this;
      }

      bool operator==(const CSetIterator& c_it) {
         return (m_psElem == c_it.m_psElem);
      }

      bool operator!=(const CSetIterator& c_it) {
         return (m_psElem != c_it.m_psElem);
      }

      SSetElement<CONTAINED_TYPE>* m_psElem;

   };

   /**
    * Defines a very simple double-linked list that stores unique elements.
    * The interface of this class is STL-compatible, but internally it behaves
    * differently from standard containers. In fact, it only stores pointers to
    * objects, and never copies them like STL containers do. The pointers are used
    * to decide whether an element is already present in the list or not. Internally,
    * The list is ordered by pointer.
    * @see SSetElement
    * @see CSetIterator
    */
   template <class T>
   class CSet {

   public:

      typedef CSetIterator<T, T> iterator;

      class const_iterator : public CSetIterator<T, const T> {
      public:
         const_iterator(const iterator& c_it) : CSetIterator<T, const T>(c_it.m_psElem) {}
         const_iterator& operator=(const iterator& c_it) {
            CSetIterator<T, const T>::m_psElem = c_it.m_psElem;
            return *this;
         }
         bool operator==(const iterator& c_it) { return (CSetIterator<T, const T>::m_psElem == c_it.m_psElem); }
         bool operator!=(const iterator& c_it) { return (CSetIterator<T, const T>::m_psElem != c_it.m_psElem); }
      };

   public:

      /**
       * Class constructor.
       * Creates an empty set.
       */
      CSet() :
         m_psFirst(NULL),
         m_psLast(NULL),
         m_unSize(0) {}

      /**
       * Class copy constructor.
       * Creates a new set copying from the given set.
       * @param c_set The set to copy from.
       */
      CSet(const CSet& c_set) :
         m_psFirst(NULL),
         m_psLast(NULL),
         m_unSize(0) {
         *this = c_set;
      }

      /**
       * Class destructor.
       */
      ~CSet() {
         clear();
      }

      /**
       * Assignment operator.
       * Substitutes the contents of this set with those of the passed one.
       * @param c_set The set to copy from.
       */
      CSet& operator=(const CSet& c_set) {
         /* Is the passed set a different set? */
         if(this != &c_set) {
            /* Yes, copy from it */
            /* First, erase this set's contents */
            clear();
            /* Is the other set empty? */
            if(! c_set.empty()) {
               /* Not empty, there's at least one element to copy */
               /* Start by copying the size of the other set into this one */
               m_unSize = c_set.m_unSize;
               /* Create the first element */
               m_psFirst = new SSetElement<T>(c_set.m_psFirst->Data);
               /* Is the size of the other set 1? */
               if(m_unSize == 1) {
                  /* Yes, the first element is also the last one */
                  m_psLast = m_psFirst;
               }
               else {
                  /* There's more than just one element */
                  /* Copy all the elements starting from the second */
                  /* Current element on other set to be copied */
                  SSetElement<T>* psCurElemOnOther = c_set.m_psFirst->Next;
                  /* Last copied element on this set */
                  SSetElement<T>* psLastElemOnThis = m_psFirst;
                  /* Current element on this set being created */
                  SSetElement<T>* psCurElemOnThis = NULL;
                  /* Go on until we hit the end of the list on the other set */
                  while(psCurElemOnOther != NULL) {
                     /* Create a new element for this set, setting as previous psLastElemOnThis */
                     psCurElemOnThis = new SSetElement<T>(psCurElemOnOther->Data, psLastElemOnThis);
                     /* Set the next of psLastElemOnThis to the element just created */
                     psLastElemOnThis->Next = psCurElemOnThis;
                     /* Advance with the last element on this */
                     psLastElemOnThis = psCurElemOnThis;
                     /* Advance with the element to copy on the other set */
                     psCurElemOnOther = psCurElemOnOther->Next;
                  }
                  /* At this point, psCurElemOnThis corresponds to the last element of the list */
                  m_psLast = psCurElemOnThis;
               }
            }
         }
         return *this;
      }

      /**
       * Returns <tt>true</tt> if the list is empty.
       * @return <tt>true</tt> if the list is empty.
       */
      inline bool empty() const {
         return m_unSize == 0;
      }

      /**
       * Returns the number of elements in the list.
       * @return The number of elements in the list.
       */
      inline size_t size() const {
         return m_unSize;
      }

      inline T& first() {
         return m_psFirst->Data;
      }

      inline const T& first() const {
         return m_psFirst->Data;
      }

      inline T& last() {
         return m_psLast->Data;
      }

      inline const T& last() const {
         return m_psLast->Data;
      }

      /**
       * Inserts an element to the list.
       * The element is inserted only if not already present in the list.
       * @param t_element The element to insert.
       */
      void insert(const T& t_element) {
         /* Is the list empty? */
         if(m_unSize == 0) {
            /* Yes, the first and last element coincide */
            m_psFirst = new SSetElement<T>(t_element);
            m_psLast = m_psFirst;
            m_unSize = 1;
         }
         else {
            /* No, we have at least 1 element */
            /* Search for the element in the list that will be the next
               of the element to add */
            SSetElement<T>* psNextElem = m_psFirst;
            while(psNextElem != NULL &&
                  psNextElem->Data < t_element) {
               psNextElem = psNextElem->Next;
            }
            /* Did we get to the end of the list? */
            if(psNextElem == NULL) {
               /* Yes, add the new element after the last one */
               SSetElement<T>* psNewElem = new SSetElement<T>(t_element, m_psLast);
               m_psLast->Next = psNewElem;
               m_psLast = psNewElem;
               ++m_unSize;
               return;
            }
            /* Is the element already present? */
            if(psNextElem->Data == t_element) {
               /* Yes, nothing to add */
               return;
            }
            /* Is the next element the first in the list? */
            if(psNextElem == m_psFirst) {
               /* Yes, we must add the new element as the new first */
               SSetElement<T>* psNewElem = new SSetElement<T>(t_element, NULL, m_psFirst);
               m_psFirst->Previous = psNewElem;
               m_psFirst = psNewElem;
               ++m_unSize;
               return;
            }
            /* If we get here, it's because we have to add the new element in the middle */
            SSetElement<T>* psNewElem = new SSetElement<T>(t_element, psNextElem->Previous, psNextElem);
            psNextElem->Previous->Next = psNewElem;
            psNextElem->Previous = psNewElem;
            ++m_unSize;
         }
      }

      /**
       * Removes the passed element from the list.
       * @param t_element The element to remove.
       */
      void erase(const T& t_element) {
         /* Is the list empty? */
         if(m_unSize == 0) {
            /* Yes, nothing to do */
            return;
         }
         /* Is the list composed of a single element? */
         if(m_unSize == 1) {
            /* Is that the element we want to eliminate? */
            if(m_psFirst->Data == t_element) {
               /* Yes, erase it! */
               delete m_psFirst;
               m_psFirst = NULL;
               m_psLast = NULL;
               m_unSize = 0;
            }
            return;
         }
         /* If we get here, it's because the trivial cases
            don't apply */
         /* Look for the passed element */
         SSetElement<T>* psElem = find_impl(t_element);
         /* Did we find it? */
         if(psElem != NULL) {
            /* Yes, let's erase it */
            /* Are we removing the first element? */
            if(psElem == m_psFirst) {
               /* Yes, we need to update m_psFirst */
               m_psFirst = m_psFirst->Next;
               m_psFirst->Previous = NULL;
               delete psElem;
               --m_unSize;
               return;
            }
            /* Are we removing the last element? */
            if(psElem == m_psLast) {
               /* Yes, we need to update m_psLast */
               m_psLast = m_psLast->Previous;
               m_psLast->Next = NULL;
               delete psElem;
               --m_unSize;
               return;
            }
            /* If we get here, it's because we need to remove
               an element in the middle */
            psElem->Previous->Next = psElem->Next;
            psElem->Next->Previous = psElem->Previous;
            delete psElem;
            --m_unSize;
         }
      }

      /**
       * Removes the passed element from the list.
       * @param t_it An iterator to the element to remove.
       */
      inline void erase(iterator& c_it) {
         erase(*c_it);
      }

      /**
       * Erases the contents of the list.
       */
      void clear() {
         if(m_unSize == 0) {
            return;
         }
         if(m_unSize == 1) {
            delete m_psFirst;
            m_psFirst = NULL;
            m_psLast = NULL;
            m_unSize = 0;
            return;
         }
         SSetElement<T>* psCurElem = m_psFirst;
         SSetElement<T>* psNextElem = psCurElem->Next;
         while(psCurElem != NULL) {
            delete psCurElem;
            psCurElem = psNextElem;
            if(psCurElem != NULL) {
               psNextElem = psNextElem->Next;
            }
         }
         m_psFirst = NULL;
         m_psLast = NULL;
         m_unSize = 0;
      }

      /**
       * Returns <tt>true</tt> if the given element is in the list.
       * @param t_element The element to search for.
       * @return <tt>true</tt> if the given element is in the list.
       */
      inline bool exists(const T& t_element) {
         return find_impl(t_element) != NULL;
      }

      /**
       * Returns an iterator to the first element.
       * @return An iterator to the first element.
       */
      inline iterator begin() const {
         return iterator(m_psFirst);
      }

      /**
       * Returns an invalid iterator.
       * @return An invalid iterator.
       */
      inline iterator end() const {
         return iterator();
      }

      /**
       * Searches for an element in the list.
       * @return An iterator to the element found.
       */
      inline iterator find(const T& t_element) {
         return iterator(find_impl(t_element));
      }

   private:

      SSetElement<T>* find_impl(const T& t_element) const {
         if(m_psFirst == NULL) {
            return NULL;
         }
         SSetElement<T>* psElem = m_psFirst;
         while(psElem != NULL &&
               psElem->Data < t_element) {
            psElem = psElem->Next;
         }
         if(psElem == NULL) {
            return NULL;
         }
         else {
            return (psElem->Data == t_element) ? psElem : NULL;
         }
      }

   private:

      SSetElement<T>* m_psFirst;
      SSetElement<T>* m_psLast;
      size_t m_unSize;

   };

}

#endif
