#include "TriangleIntersection.h"
namespace engine {

    bool trianglesIntersects(CollisionCheckTriangle* T1, CollisionCheckTriangle* T2) {
        T1->preComputeDistancesFrom(T2);
        T1->preComputeDFrom(T2);
        T2->preComputeDistancesFrom(T1);
        T2->preComputeDFrom(T1);

        if (T1->shouldRejectOverlap() || T2->shouldRejectOverlap()) return false;
        
        // should also check if triangles are coplanar before the next step
         
        // T1 interval/range (t1, t2)
        Range T1range  = T1->getTRange();
        
        // T2 interval/range (t1, t2)
        Range T2range  = T2->getTRange();

        // check if the intervals overlap, if they do, there is a collision
        if (T1range.overlaps(&T2range)) return true;

        return false;
    }
}
