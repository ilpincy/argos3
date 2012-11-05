/**
 * @file core/utility/plugins/vtable.h
 *
 * @see http://www.artima.com/cppsource/cooperative_visitor.html
 *
 * @author Carlo Pinciroli
 */

#ifndef VTABLE_H
#define VTABLE_H

#include <vector>

namespace argos {

   /**
    * Holds the value of the last used tag
    */
   template <typename BASE>
   struct STagCounter {
      static size_t Count;
   };
   /**
    * Holds the value of the last used tag
    */
   template <typename BASE>
   size_t STagCounter<BASE>::Count(0);

   /**
    * Holds the value of the tag associated to <tt>DERIVED</tt>
    */
   template <typename DERIVED, typename BASE>
   struct STagHolder {
      static size_t Tag;
   };

   /**
    * Returns the value of the tag associated to <tt>DERIVED</tt>
    */
   template <typename DERIVED, typename BASE>
   size_t GetTag() {
      size_t& unTag = STagHolder<const DERIVED, const BASE>::Tag;
      if(unTag == 0) {
         /* First call of this function, generate non-zero tag */
         ++STagCounter<BASE>::Count;
         unTag = STagCounter<BASE>::Count;
      }
      return unTag;
   }

   /**
    * Force initialization of tag holder
    */
   template <typename DERIVED, typename BASE>
   size_t STagHolder<DERIVED, BASE>::Tag = GetTag<DERIVED, BASE>();

   /**
    * Helper to make a class hierarchy vtable-enabled
    */
   template <typename BASE>
   struct EnableVTableFor {
      template <typename DERIVED>
      size_t GetTagHelper(const DERIVED*) const {
         return GetTag<DERIVED, BASE>();
      }
   };

/**
 * This macro enables the vtable for a specific class
 */
#define ENABLE_VTABLE()                         \
   virtual size_t GetTag() {                    \
      return GetTagHelper(this);                \
   }

   /**
    * The basic operation to be stored in the vtable
    */
   template <typename CONTEXT, typename BASE, typename RETURN_TYPE>
   class COperation {
   public:
      template <typename DERIVED, typename OPERATION_IMPL>
      RETURN_TYPE Hook(BASE& t_base) {
         return Dispatch<DERIVED, OPERATION_IMPL>(t_base);
      }
   protected:
      template <typename DERIVED, typename OPERATION_IMPL>
      RETURN_TYPE Dispatch(BASE& t_base) {
         /* First dispatch: cast this operation into the specific operation */
         OPERATION_IMPL& tOperation = static_cast<OPERATION_IMPL&>(*this);
         /* Second dispatch: cast t_base to DERIVED */
         DERIVED& tDerived = static_cast<DERIVED&>(t_base);
         /* Perform visit */
         tOperation.ApplyTo(tDerived);
      }
   };

   template <typename CONTEXT, typename BASE, typename RETURN_TYPE>
   class COperationInstanceHolder {
   public:
      ~COperationInstanceHolder() {
         while(!m_vecOperationInstances.empty()) {
            if(m_vecOperationInstances.back() != NULL) {
               delete m_vecOperationInstances.back();
            }
            m_vecOperationInstances.pop_back();
         }
      }
      template <typename DERIVED>
      void Add(COperation<CONTEXT, BASE, RETURN_TYPE>* pc_operation) {
         /* Find the slot */
         size_t unIndex = GetTag<DERIVED, BASE>();
         /* Does the holder have a slot for this index? */
         if(unIndex >= m_vecOperationInstances.size()) {
            /* No, new slots must be created
             * Fill the slots with NULL
             */
            /* Create new slots up to index+1 and fill them with tDefaultFunction */
            m_vecOperationInstances.resize(unIndex+1, NULL);
         }
         m_vecOperationInstances[unIndex] = pc_operation;
      }
      COperation<CONTEXT, BASE, RETURN_TYPE>* operator[](size_t un_index) const {
         if(un_index >= m_vecOperationInstances.size()) {
            return NULL;
         }
         return m_vecOperationInstances[un_index];
      }
   private:
      std::vector<COperation<CONTEXT, BASE, RETURN_TYPE>*> m_vecOperationInstances;
   };

   /**
    * Function that returns a reference to the static operation instance holder
    */
   template <typename CONTEXT, typename BASE, typename RETURN_VALUE>
   COperationInstanceHolder<CONTEXT, BASE, RETURN_VALUE>& GetOperationInstanceHolder() {
      static COperationInstanceHolder<CONTEXT, BASE, RETURN_VALUE> cOperationInstanceHolder;
      return cOperationInstanceHolder;
   }

/**
 * Convenience macro to register vtable operations
 */
#define REGISTER_OPERATION(CONTEXT, BASE, OPERATION, RETURN_VALUE, DERIVED)                                                                           \
   class C ## CONTEXT ## BASE ## OPERATION ## RETURN_VALUE ## DERIVED {                                                                               \
      typedef RETURN_VALUE (COperation<CONTEXT, BASE, RETURN_VALUE>::*T ## CONTEXT ## BASE ## OPERATION ## RETURN_VALUE ## DERIVED)(BASE&);           \
   public:                                                                                                                                            \
      C ## CONTEXT ## BASE ## OPERATION ## RETURN_VALUE ## DERIVED() {                                                                                \
         GetVTable<CONTEXT, BASE, T ## CONTEXT ## BASE ## OPERATION ## RETURN_VALUE ## DERIVED>().Add<DERIVED>(&OPERATION::Hook<DERIVED, OPERATION>); \
         GetOperationInstanceHolder<CONTEXT, BASE, RETURN_VALUE>().Add<DERIVED>(new OPERATION());                                                     \
      }                                                                                                                                               \
   } c ## CONTEXT ## BASE ## OPERATION ## RETURN_VALUE ## DERIVED;

   /**
    * The actual vtable
    */
   template <typename CONTEXT, typename BASE, typename FUNCTION>
   class CVTable {
   public:
      template <typename DERIVED>
      void Add(FUNCTION t_function) {
         /* Find the slot */
         size_t unIndex = GetTag<DERIVED, BASE>();
         /* Does the table have a slot for this index? */
         if(unIndex >= m_vecVTable.size()) {
            /* No, new slots must be created
             * Fill the slots with the default function that handles BASE
             * or NULL if such function does not exist
             */
            /* Get the tag associated to the base class */
            const size_t unBaseTag = GetTag<BASE, BASE>();
            FUNCTION tDefaultFunction = 0;
            if(unBaseTag < m_vecVTable.size()) {
               tDefaultFunction = m_vecVTable[unBaseTag];
            }
            /* Create new slots up to index+1 and fill them with tDefaultFunction */
            m_vecVTable.resize(unIndex+1, tDefaultFunction);
         }
         m_vecVTable[unIndex] = t_function;
      }

      FUNCTION operator[](size_t un_index) const {
         if(un_index >= m_vecVTable.size()) {
            un_index = GetTag<BASE, BASE>();
         }
         return m_vecVTable[un_index];
      }

   private:
      std::vector<FUNCTION> m_vecVTable;

   };

   /**
    * Function that returns a reference to the static vtable
    */
   template <typename CONTEXT, typename BASE, typename FUNCTION>
   CVTable<CONTEXT, BASE, FUNCTION>& GetVTable() {
      static CVTable<CONTEXT, BASE, FUNCTION> cVTable;
      return cVTable;
   }

/**
 * Calls the operation corresponding to the given context and operand
 * NOTE: the operand here must be a <em>reference<em>, not a pointer
 */
#define CALL_OPERATION(CONTEXT, BASE, RETURN_VALUE, OPERAND)            \
   {                                                                    \
      typedef void (COperation<CONTEXT, BASE, RETURN_VALUE>::*TFunction)(BASE&); \
      TFunction tFunction = GetVTable<CONTEXT, BASE, TFunction>()[(OPERAND).GetTag()]; \
      COperation<CONTEXT, BASE, RETURN_VALUE>* pcOperation = GetOperationInstanceHolder<CONTEXT, BASE, RETURN_VALUE>()[(OPERAND).GetTag()]; \
      (pcOperation->*tFunction)(OPERAND);                               \
   }

}

#endif
