/* $COPYRIGHT_SKOLTECH
 * $LICENSE_LGPL
 *
 * weight_point.cpp
 *
 *  Created on: April 5, 2019
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech 
 */

#include <Eigen/LU> // for inverse and determinant

#include <memory>
#include <iostream>
#include "mrob/pc_registration.hpp"


using namespace mrob;

int PCRegistration::Weighted_point(const Eigen::Ref<const MatX> X, const Eigen::Ref<const MatX> Y,
           const Eigen::Ref<const MatX1> weight,  SE3 &T, double tol)
{
    assert(X.rows() == 3  && "PCRegistration::Weighted_point: Incorrect sizing, we expect 3xN");
    assert(X.cols() >= 3  && "PCRegistration::Weighted_point: Incorrect sizing, we expect at least 3 correspondences (not aligned)");
    assert(Y.cols() == X.cols()  && "PCRegistration::Weighted_point: Same number of correspondences");
    uint_t N = X.cols();
    // TODO precalculation of T by reduced Arun

    // Initialize Jacobian and Hessian
    Mat61 J = Mat61::Zero();
    Mat6 H = Mat6::Zero();
    uint_t iters = 0;
    double deltaUpdate = 1e3;
    do
    {
        J.setZero();
        H.setZero();
        // not vectoried operations (due to Jacobian)
        for ( uint_t i = 0; i < N ; ++i)
        {
            // 1) Calculate residual r = y - Tx and the inverse of joint covariance
            Mat31 Txi = T.transform(X.col(i));
            Mat31 r = Y.col(i) - Txi;

            // 2) Calculate Jacobian for residual Jf = df/d xi = w * r' Jr, where Jr = [(Tx)^ ; -I])
            Mat<3,6> Jr;
            Jr << hat3(Txi) , -Mat3::Identity();
            Mat<1,6> Ji = weight(i) * r.transpose() * Jr;
            J += Ji;//Eigen manages this for us

            // 3) Hessian Hi ~ w * Jr' * Jr
            Mat6 Hi = weight(i) * Jr.transpose() * Jr;
            H += Hi;
        }
        // 4) Update Solution
        Mat61 dxi = -H.inverse()*J;
        T.updateLhs(dxi); //Left side update
        deltaUpdate = dxi.norm();
        iters++;

    }while(deltaUpdate > tol && iters < 20);

    return iters; // number of iterations
}
