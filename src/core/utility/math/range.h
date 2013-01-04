
/**
 * @file <argos3/core/utility/math/range.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CRANGE_H
#define CRANGE_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <iostream>

namespace argos {

   template<typename T> class CRange {

   public:

      CRange(const T& t_min = T(),
             const T& t_max = T()) :
         m_tMin(t_min),
         m_tMax(t_max),
         m_tSpan(m_tMax - m_tMin) {
         ARGOS_ASSERT(t_min <= t_max,
                      "Error initializing CRange(" <<
                      t_min << ", " << t_max << "): " <<
                      t_min << " is not <= " << t_max);
      }

      inline T GetMin() const {
         return m_tMin;
      }

      inline void SetMin(const T& t_min) {
         ARGOS_ASSERT(t_min <= m_tMax,
                      "Error setting min CRange bound (" <<
                      t_min << "): " <<
                      t_min << " is not <= " << m_tMax);
         m_tMin = t_min;
         /* Same as, but faster than
            m_tSpan = m_tMax - m_tMin; */
         m_tSpan = m_tMax;
         m_tSpan -= m_tMin;
      }

      inline T GetMax() const {
         return m_tMax;
      }

      inline void SetMax(const T& t_max) {
         ARGOS_ASSERT(m_tMin <= t_max,
                      "Error setting max CRange bound (" <<
                      t_max << "): " <<
                      m_tMin << " is not <= " << t_max);
         m_tMax = t_max;
         /* Same as, but faster than
            m_tSpan = m_tMax - m_tMin; */
         m_tSpan = m_tMax;
         m_tSpan -= m_tMin;
      }

      inline T GetSpan() const {
         return m_tSpan;
      }

      inline void Set(const T& t_min, const T& t_max) {
         ARGOS_ASSERT(t_min <= t_max,
                      "Error setting CRange bounds (" <<
                      t_min << ", " << t_max << "): " <<
                      t_min << " is not <= " << t_max);
         m_tMin = t_min;
         m_tMax = t_max;
         /* Same as, but faster than
            m_tSpan = m_tMax - m_tMin; */
         m_tSpan = m_tMax;
         m_tSpan -= m_tMin;
      }

      inline bool WithinMinBoundIncludedMaxBoundIncluded(const T& t_value) const {
         return t_value >= m_tMin && t_value <= m_tMax;
      }

      inline bool WithinMinBoundIncludedMaxBoundExcluded(const T& t_value) const {
         return t_value >= m_tMin && t_value < m_tMax;
      }

      inline bool WithinMinBoundExcludedMaxBoundIncluded(const T& t_value) const {
         return t_value > m_tMin && t_value <= m_tMax;
      }

      inline bool WithinMinBoundExcludedMaxBoundExcluded(const T& t_value) const {
         return t_value > m_tMin && t_value < m_tMax;
      }

      inline void TruncValue(T& t_value) const {
         if (t_value > m_tMax) t_value = m_tMax;
         if (t_value < m_tMin) t_value = m_tMin;
      }

      inline Real NormalizeValue(const T& t_value) const {
         T tTmpValue(t_value);
         TruncValue(tTmpValue);
         return static_cast<Real>(tTmpValue - m_tMin) /
                static_cast<Real>(m_tSpan);
      }

      template <typename U> void MapValueIntoRange(U& t_output_value,
                                                   const T& t_input_value,
                                                   const CRange<U>& c_range) const {
         t_output_value = (NormalizeValue(t_input_value) *
                           c_range.GetSpan()) + c_range.GetMin();
      }

      inline void WrapValue(T& t_value) const {
         while(t_value > m_tMax) t_value -= m_tSpan;
         while(t_value < m_tMin) t_value += m_tSpan;
      }

      inline friend std::ostream& operator<<(std::ostream& os,
                                             const CRange& c_range) {
         os << c_range.m_tMin << ":"
            << c_range.m_tMax;
         return os;
      }

      inline friend std::istream& operator>>(std::istream& is,
                                             CRange& c_range) {
         T tValues[2];
         ParseValues<T> (is, 2, tValues, ':');
         c_range.Set(tValues[0], tValues[1]);
         return is;
      }

   private:

      T m_tMin;
      T m_tMax;
      T m_tSpan;

   };

}

#endif
