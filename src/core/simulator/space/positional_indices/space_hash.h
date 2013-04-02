/**
 * @file <argos3/core/simulator/space/space_hash.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPACE_HASH_H
#define SPACE_HASH_H

namespace argos {
   class CSpace;
   class CVector3;
   class CRay3;
}

#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>

namespace argos {

   /****************************************/
   /****************************************/

   /**
    * The abstract definition of a space hash.
    * This template class defines all the basic functionality of a space hash.
    * This class is used only internally. To define your own space hash, you have to
    * subclass CSpaceHash.
    * @param ENTITY The type of the entity held by the space hash
    * @see CSpaceHash
    */
   template <class ENTITY>
   class CAbstractSpaceHash : public CPositionalIndex<ENTITY> {

   public:

      /**
       * Type definition for the list of entities held by the space hash
       */
      typedef CSet<ENTITY*> TEntityList;

   public:

      /**
       * Class constructor.
       * The size of the space hash (the number of buckets) is set to zero.
       * To be able to use the space hash, you first must set the size to an
       * appropriate value.
       * @see SetSize()
       */
      CAbstractSpaceHash() : m_unSize(0) {}

      /**
       * Class destructor.
       */
      virtual ~CAbstractSpaceHash() {}

      /**
       * Adds an entity to the space hash.
       * @param c_entity The entity to add.
       */
      virtual void AddEntity(ENTITY& c_entity) {
         m_tEntities.insert(&c_entity);
      }

      /**
       * Returns the list of entities held by this space hash.
       * @return The list of entities held by this space hash.
       */
      inline TEntityList& GetEntities() {
         return m_tEntities;
      }

      /**
       * Remove an entity from the space hash.
       * @param c_entity The entity to remove.
       */
      virtual void RemoveEntity(ENTITY& c_entity) {
         typename TEntityList::iterator it = m_tEntities.find(&c_entity);
         if(it != m_tEntities.end()) {
            m_tEntities.erase(it);
         }
         else {
            THROW_ARGOSEXCEPTION("Entity not found when removing it from space hash.");
         }
      }

      /**
       * Returns the size of the space hash.
       * The size corresponds to the number of buckets.
       * @return The size of the space hash.
       */
      inline size_t GetSize() {
         return m_unSize;
      }

      /**
       * Sets the size of the space hash.
       * The size corresponds to the number of buckets.
       * @param un_size The size of the space hash.
       */
      virtual void SetSize(size_t un_size) {
         m_unSize = un_size;
      }

      /**
       * Returns the size of the cells of the space hash.
       * The size is returned in meters.
       * @return The size of the cells of the space hash.
       */
      inline CVector3& GetCellSize() {
         return m_cCellSize;
      }

      /**
       * Returns the inverse size of the cells of the space hash.
       * It is used to convert measures of the space into cells of the space hash.
       * @return The size of the cells of the space hash.
       * @see SpaceToHashTable
       */
      inline CVector3& GetInvCellSize() {
         return m_cInvCellSize;
      }

      /**
       * Sets the size of the cells of the space hash.
       * The size is measured in meters.
       * @param c_cell_size The new size for the cells of the space hash.
       */
      virtual void SetCellSize(const CVector3& c_cell_size) {
         m_cCellSize = c_cell_size;
         m_cInvCellSize.Set(1.0f / m_cCellSize.GetX(),
                            1.0f / m_cCellSize.GetY(),
                            1.0f / m_cCellSize.GetZ());
      }

      /**
       * Updates the entire space hash.
       * @see UpdateCell
       * @see CSpaceHash::Update
       */
      virtual void Update() = 0;

      /**
       * Adds an entity to a cell of the space hash.
       * @param n_x The x coordinate of the cell.
       * @param n_y The y coordinate of the cell.
       * @param n_z The z coordinate of the cell.
       * @param c_entity The entity to add.
       */
      virtual void UpdateCell(SInt32 n_x,
                              SInt32 n_y,
                              SInt32 n_z,
                              ENTITY& c_entity) = 0;

      /**
       * Converts a single space coordinate into a space hash cell coordinate.
       * @param f_coord The value of the X/Y/Z coordinate in meters.
       * @param un_axis the axis: 0 -> X, 1 -> Y, 2 -> Z.
       * @return The X/Y/Z coordinate of the corresponding space hash cell.
       */
      virtual SInt32 SpaceToHashTable(Real f_coord,
                                             UInt32 un_axis) {
         return RoundClosestToZero(f_coord * GetInvCellSize()[un_axis]);
      }

      /**
       * Converts a single space hash cell coordinate into a space coordinate.
       * @param n_coord The value of the X/Y/Z space hash coordinate.
       * @param un_axis the axis: 0 -> X, 1 -> Y, 2 -> Z.
       * @return The X/Y/Z coordinate in the space.
       */
      virtual Real HashTableToSpace(SInt32 n_coord,
                                           UInt32 un_axis) {
         return n_coord * m_cCellSize[un_axis];
      }

      /**
       * Converts a space position into a space hash cell
       * The values are written into n_i, n_j, and n_k.
       * @param n_i The X coordinate of the space hash cell.
       * @param n_j The Y coordinate of the space hash cell.
       * @param n_k The Z coordinate of the space hash cell.
       * @param c_pos The value position in meters.
       */
      virtual void SpaceToHashTable(SInt32& n_i,
                                           SInt32& n_j,
                                           SInt32& n_k,
                                           const CVector3& c_pos) {
         n_i = RoundClosestToZero(c_pos.GetX() * CAbstractSpaceHash::GetInvCellSize().GetX());
         n_j = RoundClosestToZero(c_pos.GetY() * CAbstractSpaceHash::GetInvCellSize().GetY());
         n_k = RoundClosestToZero(c_pos.GetZ() * CAbstractSpaceHash::GetInvCellSize().GetZ());
      }

      /**
       * Looks for entities to process in a cell.
       * @param n_i The X coordinate of the space hash cell.
       * @param n_j The Y coordinate of the space hash cell.
       * @param n_k The Z coordinate of the space hash cell.
       * @param t_entities The resulting list of entities to process.
       * @return <tt>true</tt> if new entities were added to t_entities.
       */
      virtual bool CheckCell(SInt32 n_i,
                             SInt32 n_j,
                             SInt32 n_k,
                             TEntityList& t_entities) = 0;

      virtual void Dump(CARGoSLog& c_os) = 0;

   protected:

      /**
       * Calculates the hash of a space hash coordinate.
       * @param n_i The X coordinate of the space hash cell.
       * @param n_j The Y coordinate of the space hash cell.
       * @param n_k The Z coordinate of the space hash cell.
       * @return The hash of a space hash coordinate.
       */
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

      /**
       * The list of entities held by this space hash
       */
      TEntityList m_tEntities;

      /**
       * The size (number of buckets) of the space hash.
       */
      size_t m_unSize;

      /**
       * The size of a cell in the space.
       * Measured in meters.
       */
      CVector3 m_cCellSize;

      /**
       * The inverse of the size of a cell in the space.
       * It is used to convert measures of the space into cells of the space hash.
       */
      CVector3 m_cInvCellSize;

   };

   /****************************************/
   /****************************************/

   /**
    * Defines the basic cell updater of the space hash.
    * If you want to store your entities in a space hash, you need to implement
    * this interface. The aim of this class is to call
    * CAbstractSpaceHash::UpdateCell() for each cell in which the entity must be
    * contained.
    * @param Entity The type of entity held by the corresponding space hash
    * @see CSpaceHash
    */
   template <class ENTITY>
   class CSpaceHashUpdater {

     public:

      /**
       * Class destructor.
       */
      virtual ~CSpaceHashUpdater() {}

      /**
       * Updates the necessary cells of a space hash.
       * @param c_space_hash The space hash to update
       * @param c_entity The entity indexed by the space hash.
       */
      virtual void operator()(CAbstractSpaceHash<ENTITY>& c_space_hash,
                              ENTITY& c_entity) = 0;

   };

   /****************************************/
   /****************************************/

   /**
    * Defines the basic space hash.
    * If you want to create a new space hash implementation, you need to extend this
    * class.
    * @param ENTITY The type of entity held by the corresponding space hash
    * @param UPDATER The cell updater for type ENTITY
    * @see CSpaceHashNative
    */
   template <class ENTITY, class UPDATER>
   class CSpaceHash : public CAbstractSpaceHash<ENTITY> {

   public:

      /**
       * Updates the entire space hash.
       * It calls the updater for all the entities held by the space hash.
       * @see CSpaceHashUpdater
       */
      virtual void Update() {
         /* Go through all the entities */
         for(typename CAbstractSpaceHash<ENTITY>::TEntityList::const_iterator el = CAbstractSpaceHash<ENTITY>::GetEntities().begin();
             el != CAbstractSpaceHash<ENTITY>::GetEntities().end(); ++el) {
            m_cUpdater(*this, **el);
         }
      }

   private:

      UPDATER m_cUpdater;

   };

   /****************************************/
   /****************************************/

}

#endif
