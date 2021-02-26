#pragma once

#include <algorithm>
#include <glm/glm.hpp>

// Moller method implementation (from the paper: A Fast Triangle-Triangle Intersection Test)
// http://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf

namespace engine {

    struct Range {

        const float min;
        const float max;
        const float length = max - min;
        
        bool overlaps(Range* other) {
            auto smallerRange = this->length <= other->length ? this : other;
            auto biggerRange  = this->length > other->length ? this : other;

            return   ( smallerRange->max > biggerRange->min && smallerRange->max < biggerRange->max ) ||
            ( smallerRange->min < biggerRange->max && smallerRange->min > smallerRange->min );
        }
    };

    struct Triangle {
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;

        //signed distance from other triangle plane
        float dv0;
        float dv1;
        float dv2;

        glm::vec3 D;

        void preComputeDistancesFrom(Triangle* other) {
            // compute signed distance from other s plane 
            const glm::vec3 n = other->n(); 
            const float d = other->d(); 
            dv0 = glm::dot(n, v0) + d;
            dv1 = glm::dot(n, v1) + d;
            dv2 = glm::dot(n, v2) + d;
        }

        void preComputeDFrom(Triangle* other) {
            D = glm::cross(n(), other->n());
        }

        glm::vec3 n() {
            return glm::cross((v1 - v0), (v2 - v0));
        }

        float d() {
            return glm::dot(-n(), v0);
        }

        bool shouldRejectOverlap() {
            //if all distances are != 0 and have the same sign the overlap is rejected for t1 on PI2
            if (
                (dv0 > 0 && dv1 > 0 && dv2 > 0) ||
                (dv0 < 0 && dv1 < 0 && dv2 < 0)
            ) {
                return true;
            }

            return false;

        }

        bool isCoplanar() { 
            // if all distances == 0 is coplanar
            if (dv0 == 0 && dv1 == 0 && dv2 == 0) {
                return true;
            }

            return false;
        }

        float t1() {
            float pv0 = glm::dot(D, v0); 
            float pv1 = glm::dot(D, v1);

            float t1 =  pv0 + (pv1 - pv0) * ( dv0 / (dv0 - dv1) );

            return t1;
        }

        float t2() {
            float pv0 = glm::dot(D, v1); 
            float pv1 = glm::dot(D, v2);

            float t2 =  pv0 + (pv1 - pv0) * ( dv0 / (dv0 - dv1) );

            return t2;
        }

        Range getTRange() {
            float t1Result = t1();
            float t2Result = t2();
            
            return {
                std::min(t1Result, t2Result),
                std::max(t1Result, t2Result)
            };
        }

    };


    bool trianglesIntersects(Triangle* T1, Triangle* T2);

}

