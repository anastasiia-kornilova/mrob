/* Copyright (c) 2018, Skolkovo Institute of Science and Technology (Skoltech)
 * All rights reserved.
 *
 * See LICENSE file in the root of the mrob library.
 *
 *
 * nodeLandmark3d.hpp
 *
 *  Created on: March 17, 2020
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech 
 */

#ifndef NODELANDMARK3D_HPP_
#define NODELANDMARK_HPP_

#include "mrob/matrix_base.hpp"
#include "mrob/SE3.hpp" //requires including and linking SE3 library
#include "mrob/node.hpp"

namespace mrob{

class NodeLandmark3d : public Node
{
  public:
    /**
     * For initialization, requires an initial estimation of the state.
     */
    NodeLandmark3d(const Mat31 &initial_x);
    //NodePose3d(const SE3 &initial_T);
    virtual ~NodeLandmark3d();

    virtual void update(const Eigen::Ref<const MatX1> &dx);
    virtual void update_from_auxiliary(const Eigen::Ref<const MatX1> &dx);
    virtual void set_state(const Eigen::Ref<const MatX> &x);
    virtual void set_auxiliary_state(const Eigen::Ref<const MatX> &x);
    virtual const Eigen::Ref<const MatX> get_state() const {return state_;};
    virtual const Eigen::Ref<const MatX> get_auxiliary_state() const {return auxiliaryState_;};
    void print() const;

  protected:
    Mat31 state_;
    Mat31 auxiliaryState_;

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW // as proposed by Eigen


};


}


#endif /* NODEPOSE3D_HPP_ */
