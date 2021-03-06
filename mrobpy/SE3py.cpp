/* Copyright (c) 2018, Skolkovo Institute of Science and Technology (Skoltech)
 * All rights reserved.
 *
 * See LICENSE file in the root of the mrob library.
 *
 *
 * SE3py.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab, Skoltech
 */

/**
 * This programm generates the Pyhton binding for the SE3 library
 * using the pybind11 library, here included as an external dependency,
 */

#include "mrob/SE3.hpp"
#include "mrob/SO3.hpp"
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
namespace py = pybind11;
using namespace mrob;


void init_geometry(py::module &m) {
    py::class_<SE3>(m, "SE3")
		.def(py::init<>(),
				"Default contructor, creates the identity transformation",
				py::return_value_policy::copy)
        .def(py::init<const Mat4 &>(),
        		"Matrix contructor, requires a 4x4 RBT matrix",
				py::return_value_policy::copy)
        .def(py::init<const Mat61 &>(), py::return_value_policy::copy)
        .def(py::init<const SE3 &>(), py::return_value_policy::copy)
        .def(py::init<const SO3 &, const Mat31>(), py::return_value_policy::copy)
        .def("T", &SE3::T, py::return_value_policy::copy) // makes a copy of the 4x4 Transformation
        .def("R", &SE3::R, py::return_value_policy::copy)
        .def("t", &SE3::t, py::return_value_policy::copy)
        .def("mul", &SE3::mul, "multiplies a SE3 on the right hand side of the current object", py::return_value_policy::copy)
        .def("update", &SE3::update_lhs)
        .def("update_lhs", &SE3::update_lhs)
        .def("update_rhs", &SE3::update_rhs)
        .def("Ln", &SE3::ln_vee, "Logarithm + vee operator, returns 6 coordinates in the manifold", py::return_value_policy::copy)
        .def("transform", &SE3::transform, py::return_value_policy::copy)
        .def("transform_array", &SE3::transform_array, py::return_value_policy::copy,
              "Input is a an array Nx3 and output is Nx3") // makes a copy of the array. TODO, pass by Ref and avoid copying, look at ownership
        .def("inv", &SE3::inv, py::return_value_policy::copy)
        .def("adj", &SE3::adj, py::return_value_policy::copy)
        .def("distance", &SE3::distance)
        .def("distance_rotation", &SE3::distance_rotation)
        .def("distance_trans", &SE3::distance_trans)
        .def("print", &SE3::print)
        ;
    m.def("isSE3", &mrob::isSE3, "Returns True is the matrix is a valid transformation and False if not");

    py::class_<SO3>(m, "SO3")
		.def(py::init<>(), py::return_value_policy::copy)
		.def(py::init<const Mat31 &>(), py::return_value_policy::copy)
        .def(py::init<const Mat3 &>(), py::return_value_policy::copy)
        .def(py::init<const SO3 &>(), py::return_value_policy::copy)
        .def("R", &SO3::R, py::return_value_policy::copy)
        .def("mul", &SO3::mul, "multiplies a SO3 on the right hand side of the current object", py::return_value_policy::copy)
        .def("update", &SO3::update_lhs )
        .def("update_lhs", &SO3::update_lhs )
        .def("update_rhs", &SO3::update_rhs )
        .def("Ln", &SO3::ln_vee, py::return_value_policy::copy)
        .def("inv", &SO3::inv, py::return_value_policy::copy)
        .def("adj", &SO3::adj, py::return_value_policy::copy)
        .def("distance", &SO3::distance)
        .def("print", &SO3::print)
        ;
    m.def("hat3", &mrob::hat3, "Returns a skew symetric matrix 3x3 from a 3-vector", py::return_value_policy::copy);
    m.def("hat6", &mrob::hat6, "Returns a Lie algebra matrix 4x4 from a 6-vector", py::return_value_policy::copy);

    // AUxiliary functions to support other conventions (TORO, g2o)
    m.def("quat_to_so3", &quat_to_so3,"Suport function from quaternion to a rotation");
    m.def("so3_to_quat", &so3_to_quat,"Suport function from rotation matrix to quaternion");
    m.def("rpy_to_so3",  &rpy_to_so3,"Suport function from roll pitch yaw to a rotation");
}

