/**
 * @file <argos3/testing/unit/test-list.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include <argos3/core/utility/datatypes/set.h>
#include <cstdio>

using namespace argos;

struct T {
   int x;

   T(int v) : x(v) {}

   bool operator<(const T& t) const { return x < t.x; }
   bool operator==(const T& t) const { return x == t.x; }
};

int main() {
   CSet<T*> L;

   T v1(1);
   T v2(2);
   T v3(3);
   T v4(4);
   T v5(5);

   L.insert(&v1);
   L.insert(&v2);
   L.insert(&v1);
   L.insert(&v3);
   L.insert(&v1);
   L.insert(&v4);
   L.insert(&v1);
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::const_iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   L.clear();
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::const_iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   CSet<T*>::iterator itF = L.find(&v3);
   if(itF != L.end()) {
      fprintf(stderr, "v3 found: x = %d\n", (*itF)->x);
   }
   else {
      fprintf(stderr, "v3 not found!\n");      
   }

   itF = L.find(&v5);
   if(itF != L.end()) {
      fprintf(stderr, "v5 found: x = %d\n", (*itF)->x);
   }
   else {
      fprintf(stderr, "v5 not found!\n");      
   }

   CSet<T*> M(L);
   fprintf(stderr, "%zu elements\n", M.size());
   for(CSet<T*>::iterator it = M.begin(); it != M.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   L.erase(&v4);
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   L.erase(&v3);
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   L.erase(&v2);
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   L.erase(&v1);
   fprintf(stderr, "%zu elements\n", L.size());
   for(CSet<T*>::iterator it = L.begin(); it != L.end(); ++it) {
      fprintf(stderr, "x = %d\n", (*it)->x);
   }
   fprintf(stderr, "\n");

   return 0;
}
