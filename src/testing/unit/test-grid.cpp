#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/simulator/entity/led_entity.h>

using namespace argos;

const size_t NUM_LEDS = 10;

class CLEDEntityGridPrint : public CGrid<CLEDEntity>::COperation {

public:

   virtual bool operator()(CLEDEntity& c_entity) {
      fprintf(stderr, "%s\t%.3f\t%.3f\t%.3f\n",
              c_entity.GetId().c_str(),
              c_entity.GetPosition().GetX(),
              c_entity.GetPosition().GetY(),
              c_entity.GetPosition().GetZ());
      return true;
   }

};

void PrintGrid(CGrid<CLEDEntity>& g) {
   CLEDEntityGridPrint p;
   g.ForAllEntities(p);
   fprintf(stderr, "\n");
}

void PrintGridCells(CGrid<CLEDEntity>& g) {
   for(size_t k = 0; k < g.GetSizeK(); ++k) {
      for(size_t j = 0; j < g.GetSizeJ(); ++j) {
         for(size_t i = 0; i < g.GetSizeI(); ++i) {
            CGrid<CLEDEntity>::SCell& c = g.GetCellAt(i, j, k);
            fprintf(stderr, "[ %d, %d, %d ] %d entities, timestamp = %d\n", i, j, k, c.Entities.size(), c.Timestamp);
            if(!c.Entities.empty()) {
               for(CSet<CLEDEntity*>::iterator it = c.Entities.begin();
                   it != c.Entities.end();
                   ++it) {
                  fprintf(stderr, "   %s\t%.3f\t%.3f\t%.3f\n",
                          (**it).GetId().c_str(),
                          (**it).GetPosition().GetX(),
                          (**it).GetPosition().GetY(),
                          (**it).GetPosition().GetZ());
               }
            }
         }
      }
   }
}

int main() {
   CGrid<CLEDEntity> g(
      CVector3(0.0, 0.0, 0.0),
      CVector3(3.0, 2.0, 1.0),
      5, 2, 1);
   CLEDEntityGridUpdater u(g);
   g.SetUpdateEntityOperation(&u);

   CLEDEntity** leds = new CLEDEntity*[NUM_LEDS];
   for(size_t i = 0; i < NUM_LEDS/2; ++i) {
      leds[i] = new CLEDEntity(NULL,
                               "LED" + ToString(i),
                               CVector3(0.599+0.6*i, 0.5, 0.3),
                               CColor::RED);
      g.AddEntity(*(leds[i]));
   }
   for(size_t i = NUM_LEDS/2; i < NUM_LEDS; ++i) {
      leds[i] = new CLEDEntity(NULL,
                               "LED" + ToString(i),
                               CVector3(0.599+0.6*(i-NUM_LEDS/2), 0.5, 0.3),
                               CColor::RED);
      g.AddEntity(*(leds[i]));
   }

   PrintGrid(g);
   g.Update();
   PrintGridCells(g);

   for(size_t i = 0; i < NUM_LEDS/2; ++i) {
      leds[i]->SetPosition(CVector3(leds[i]->GetPosition().GetX(),
                                    1.5f,
                                    leds[i]->GetPosition().GetZ()));
   }

   g.Update();
   PrintGridCells(g);

   for(size_t i = 0; i < NUM_LEDS; ++i) {
      delete leds[i];
   }
   delete[] leds;

   return 0;
}
