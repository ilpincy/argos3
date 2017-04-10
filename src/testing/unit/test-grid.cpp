#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/plugins/simulator/entities/led_entity.h>

using namespace argos;

/****************************************/
/****************************************/

const size_t NUM_LEDS = 10;

/****************************************/
/****************************************/

class CLEDEntityGridPrint : public CGrid<CLEDEntity>::COperation {

public:

   virtual bool operator()(CLEDEntity& c_entity) {
      fprintf(stdout, "%s\t%.3f\t%.3f\t%.3f\n",
              c_entity.GetId().c_str(),
              c_entity.GetPosition().GetX(),
              c_entity.GetPosition().GetY(),
              c_entity.GetPosition().GetZ());
      return true;
   }

};
CLEDEntityGridPrint p;

void PrintGrid(CGrid<CLEDEntity>& g) {
   g.ForAllEntities(p);
   fprintf(stdout, "\n");
}

/****************************************/
/****************************************/

void PrintGridCells(CGrid<CLEDEntity>& g) {
   for(SInt32 k = g.GetSizeK()-1; k >= 0; --k) {
      for(SInt32 j = g.GetSizeJ()-1; j >= 0; --j) {
         for(SInt32 i = 0; i < g.GetSizeI(); ++i) {
            CGrid<CLEDEntity>::SCell& c = g.GetCellAt(i, j, k);
            fprintf(stdout, "[ %d, %d, %d ] %zu entities, timestamp = %zu\n", i, j, k, c.Entities.size(), c.Timestamp);
            if(!c.Entities.empty()) {
               for(CSet<CLEDEntity*>::iterator it = c.Entities.begin();
                   it != c.Entities.end();
                   ++it) {
                  fprintf(stdout, "   %s\t%.3f\t%.3f\t%.3f\n",
                          (**it).GetId().c_str(),
                          (**it).GetPosition().GetX(),
                          (**it).GetPosition().GetY(),
                          (**it).GetPosition().GetZ());
               }
            }
         }
      }
   }
   fprintf(stdout, "\n");
}

/****************************************/
/****************************************/

// void PrintVisitedCells(CGrid<CLEDEntity>& g) {
//    for(SInt32 k = g.GetSizeK()-1; k >= 0; --k) {
//       for(SInt32 j = g.GetSizeJ()-1; j >= 0; --j) {
//          for(SInt32 i = 0; i < g.GetSizeI(); ++i) {
//             CGrid<CLEDEntity>::SCell& c = g.GetCellAt(i, j, k);
//             if(c.Visited > 0) {
//                fprintf(stdout, "%d ", c.Visited);
//             }
//             else {
//                fprintf(stdout, ". ");
//             }
//          }
//          fprintf(stdout, "\n");
//       }
//       fprintf(stdout, "\n");
//    }
//    fprintf(stdout, "\n");
// }

/****************************************/
/****************************************/

int main() {
   // Create stuff
   // CGrid<CLEDEntity> g(
   //    CVector3(0.0, 0.0, 0.0),
   //    CVector3(3.0, 2.0, 1.0),
   //    50, 20, 5);
   // CLEDEntityGridUpdater u(g);
   // g.SetUpdateEntityOperation(&u);

   // CLEDEntity** leds = new CLEDEntity*[NUM_LEDS];
   // for(size_t i = 0; i < NUM_LEDS/2; ++i) {
   //    leds[i] = new CLEDEntity(NULL,
   //                             "LED" + ToString(i),
   //                             CVector3(0.599+0.6*i, 0.5, 0.3),
   //                             CColor::RED);
   //    g.AddEntity(*(leds[i]));
   // }
   // for(size_t i = NUM_LEDS/2; i < NUM_LEDS; ++i) {
   //    leds[i] = new CLEDEntity(NULL,
   //                             "LED" + ToString(i),
   //                             CVector3(0.3+0.6*(i-NUM_LEDS/2), 1.5, 0.3),
   //                             CColor::RED);
   //    g.AddEntity(*(leds[i]));
   // }
   // fprintf(stdout, "START\n");
   // PrintGrid(g);
   // g.Update();
   // PrintGridCells(g);

   // for(size_t i = 0; i < NUM_LEDS/2; ++i) {
   //    leds[i]->SetPosition(CVector3(leds[i]->GetPosition().GetX(),
   //                                  1.5f,
   //                                  leds[i]->GetPosition().GetZ()));
   // }
   // g.Update();
   // fprintf(stdout, "MOVE EVERYTHING TO 1.5Y\n");
   // PrintGridCells(g);
   // fprintf(stdout, "RAY QUERY\n");
   // g.ForEntitiesAlongRay(CRay3(CVector3(),
   //                             CVector3(5,2,1)),
   //                       p);
   // fprintf(stdout, "\n");

   // fprintf(stdout, "MOVE EVERYTHING TO 0.5Y\n");
   // for(size_t i = 0; i < NUM_LEDS; ++i) {
   //    leds[i]->SetPosition(CVector3(leds[i]->GetPosition().GetX(),
   //                                  0.5f,
   //                                  leds[i]->GetPosition().GetZ()));
   // }
   // g.Update();
   // PrintGridCells(g);
   // fprintf(stdout, "RAY QUERY\n");
   // g.ForEntitiesAlongRay(CRay3(CVector3(1,2,1), CVector3(1.5,0,1)), p, false);
   // PrintVisitedCells(g);
   
   // fprintf(stdout, "BOX RANGE QUERY\n");
   // g.ForEntitiesInBoxRange(CVector3(2.5,1,0), CVector3(1,0.5,0), p);
   // PrintVisitedCells(g);

   // fprintf(stdout, "CIRCLE QUERY\n");
   // g.ForEntitiesInCircleRange(CVector3(1.5, 1, 0.5), 0.5, p);
   // PrintVisitedCells(g);

   // fprintf(stdout, "SPHERE QUERY\n");
   // g.ForEntitiesInSphereRange(CVector3(0.0, 0.0, 0.5), 0.5, p);
   // PrintVisitedCells(g);

   // for(size_t i = 0; i < NUM_LEDS; ++i) {
   //    delete leds[i];
   // }
   // delete[] leds;

   return 0;
}
