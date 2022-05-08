#include "vectorAlg.h"

sf::Vector3f normalize(sf::Vector3f& A) {
    double l = sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
    A.x /= l;
    A.y /= l;
    A.z /= l;
    return A;
}


sf::Vector3f vec_mult(const sf::Vector3f& A, const sf::Vector3f& B) {
    sf::Vector3f norm;
    norm.x = (A.y * B.z - A.z * B.y);
    norm.y = (A.z * B.x - A.x * B.z);
    norm.z = (A.x * B.y - A.y * B.x);
    return norm;
}