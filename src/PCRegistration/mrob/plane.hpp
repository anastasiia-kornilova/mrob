/* $COPYRIGHT SKOLTECH
 * $LICENSE_LGPL
 *
 * plane.hpp
 *
 *  Created on: Feb 1, 2019
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech
 */

#ifndef PLANE_HPP_
#define PLANE_HPP_


#include <vector>
#include <memory>
#include "mrob/SE3.hpp"
#include <Eigen/StdVector>



namespace mrob{

/**
 * class Plane stores a vector of point clouds from different observations
 * and  provides the gradients with respect to those poses.
 *
 */
class Plane{
  public:
    Plane(uint_t timeLength);
    ~Plane();

    void reserve(uint_t d, uint_t t);
    Mat41 get_plane(void) {return planeEstimation_;};
    /**
     * This function is intended to add a point that belongs to the plane at time t
     * Internally, we will save a copy of them
     */
    void push_back_point(Mat31 &point, uint_t t);
    std::vector<Mat31>& get_points(uint_t t);
    uint_t get_number_points(uint_t t) {return allPlanePoints_[t].size();};
    void clear_points();
    void set_trajectory(const std::shared_ptr<const std::vector<SE3>> &trajectory) {trajectory_ = trajectory;};

    /**
     * Estimates the plane parameters: v = [n', d]'_{4x1}, where v is unitary, (due to the SVD solution)
     * although for standard plane estimation we could enforce unitary on the normal vector n.
     */
    double estimate_plane();
    /**
     * get error: returns the error as the min eigenvalue
     */
    double get_error() const {return lambda_;};

    /**
     *  calculates the matrix S = sum(p*p'), where p = [x,y,z,1]
     * for all planes, as an aggregation of the outer product of all
     * homogeneous points
     */
    void calculate_all_matrices_S();
    /**
     *  calculates the matrix Qi = 1^T_i * Si * (1^T_i)^transp
     *  for all planes. Since this is an iterative process on T's,
     *  we separate the calculation of the S matrix,
     *  and the Q matrix which rotates S
     */
    void calculate_all_matrices_Q();
    /**
     * calculate Jacobian at time t, returns the Jacobian over the SVD od Q  = U D V'
     * as d lamda = v' * dQ * v
     *
     */
    Mat61 calculate_jacobian(uint_t t);


    void print() const;


  protected:
    // max index of time, or trajectory length
    uint_t timeLength_;
    //std::vector<uint_t> timeIndex_;// not used now, but it would be useful for non-consecutive obs

    // Overparametrized plane, as a transformation in SE3 TODO needed?
    //SE3 plane_;
    Mat41 planeEstimation_;
    double lambda_;
    bool isPlaneEstimated_;

    // subset of pointcloud for the given plane
    std::vector< std::vector<Mat31> > allPlanePoints_;

    // Transformations. We shared it across all planes that need it.
    std::shared_ptr<const std::vector<SE3>> trajectory_;

    // gradient calculation Q
    std::vector<Mat4> matrixS_, matrixQ_;

  public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}


#endif /* PLANE_HPP_ */