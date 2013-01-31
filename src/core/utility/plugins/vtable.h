/**
 * @file <argos3/core/utility/plugins/vtable.h>
 *
 * @brief This file provides the facility to map operations to operands in plugins.
 *
 * <p>This file contains a set of facilities to map operations to operands in
 * plugins. These facilities are heavily used in physics engines and
 * visualizations to call operations on entities that are defined outside
 * the plugin scope. In this way, new entities can be added without touching
 * the plugin code. These facilities can be used also in other cases (it is
 * not necessary to refer to entitie).
 *
 * <p>From the user perspective, if you want to add support to decoupled
 * operations on a class hierarchy, you need to add the ENABLE_VTABLE() macro
 * to each class in the hierarchy:
 * <pre>
 *    class MyBase {
 *    public:
 *       ENABLE_VTABLE();
 *    };
 *
 *    class MyDerived1 : public MyBase {
 *    public:
 *       ENABLE_VTABLE();
 *    };
 *
 *    class MyDerived2 : public MyBase {
 *    public:
 *       ENABLE_VTABLE();
 *    };
 * </pre>
 * This is all you need to do to the class hierarchy.
 *
 * <p>Then, you define somewhere the operations. Before that, you need to
 * create a symbol of any sort, that will be used as context of the operation.
 * The context is used by the system to differentiate among operations with the
 * same signature. For instance, in the case of a physics engine, the class
 * name of the physics engine is a good choice. Here we define an empty struct
 * as context:
 * <pre>
 *    struct MyContext {};
 * </pre>
 * <p>It is time to define the operations. An operation MUST have a method
 * ApplyTo(), because that is what the system wants to call. Not having such a
 * method generates nasty compile errors. Also, an operation MUST have its
 * parameter passed by reference (not by pointer).
 * <pre>
 *    class MyOperationOnDerived1 : public COperation<MyContext, MyBase, void> {
 *    public:
 *       void ApplyTo(MyDerived1& d) {
 *          std::cout << "MyOperationOnDerived1" << std::endl;
 *       }
 *    };
 *    REGISTER_OPERATION(MyContext, MyBase, MyOperationOnDerived1, void, MyDerived1)
 *    class MyOperationOnDerived2 : public COperation<MyContext, MyBase, void> {
 *    public:
 *       void ApplyTo(MyDerived2& d) {
 *          std::cout << "MyOperationOnDerived2" << std::endl;
 *       }
 *    };
 *    REGISTER_OPERATION(MyContext, MyBase, MyOperationOnDerived2, void, MyDerived2)
 * </pre>
 * The template arguments to provide to COperations are the context, the base
 * class of the hierarchy, and the return value of the function ApplyTo(). The
 * registration macros inform the system of the presence of the operations.
 *
 * <p>The operations are now ready to be called from your code. To call them,
 * use this syntax:
 * <pre>
 *    MyBase* b1 = new MyDerived1();
 *    MyBase* b2 = new MyDerived2();
 *    CallOperation<MyContext, MyBase, void>(*b1);
 *    CallOperation<MyContext, MyBase, void>(*b2);
 * </pre>
 *
 * @see http://www.artima.com/cppsource/cooperative_visitor.html
 *
 * @author Carlo Pinciroli
 */

#ifndef VTABLE_H
#define VTABLE_H

#include <vector>
#include <cstddef>

#include <argos3/core/utility/logging/argos_log.h>

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
         ++STagCounter<const BASE>::Count;
         unTag = STagCounter<const BASE>::Count;
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

      inline size_t Size() const {
         return m_vecVTable.size();
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
   
#define INIT_VTABLE_FOR(BASE)                   \
   struct SVTableInitializerFor ## BASE {       \
      SVTableInitializerFor ## BASE() {         \
         GetTag<BASE, BASE>();                  \
      }                                         \
   } sVTableInitializerFor ## BASE;

}

#endif
