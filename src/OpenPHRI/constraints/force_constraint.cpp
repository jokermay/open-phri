/*      File: force_constraint.cpp
*       This file is part of the program open-phri
*       Program description : OpenPHRI: a generic framework to easily and safely control robots in interactions with humans
*       Copyright (C) 2017 -  Benjamin Navarro (LIRMM). All Right reserved.
*
*       This software is free software: you can redistribute it and/or modify
*       it under the terms of the LGPL license as published by
*       the Free Software Foundation, either version 3
*       of the License, or (at your option) any later version.
*       This software is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*       LGPL License for more details.
*
*       You should have received a copy of the GNU Lesser General Public License version 3 and the
*       General Public License version 3 along with this program.
*       If not, see <http://www.gnu.org/licenses/>.
*/

#include <OpenPHRI/constraints/force_constraint.h>
#include <limits>

using namespace phri;

using namespace Eigen;

class ForceLimitationVelocityGenerator : public VelocityGenerator {
public:
	ForceLimitationVelocityGenerator(Vector6dConstPtr external_force, doubleConstPtr maximum_force) :
		VelocityGenerator(ReferenceFrame::TCP),
		external_force_(external_force),
		maximum_force_(maximum_force)
	{

	}

	virtual Twist compute() override {
		velocity_ = Vector6d::Zero();
		double f_norm = external_force_->norm();

		if(f_norm > *maximum_force_) {
			velocity_ = *external_force_ * 1e12; // Just something huge
		}

		return VelocityGenerator::compute();
	}

private:
	Vector6dConstPtr external_force_;
	doubleConstPtr maximum_force_;
};

/***		Constructor & destructor		***/
ForceConstraint::ForceConstraint(
	VelocityConstraintPtr constraint,
	doubleConstPtr maximum_force) :
	constraint_(constraint),
	maximum_force_(maximum_force)
{
	velocity_generator_ = std::make_shared<ForceLimitationVelocityGenerator>(
		robot_->controlPointExternalForce(),
		maximum_force_);
}

/***		Algorithm		***/
double ForceConstraint::compute() {
	return constraint_->compute();
}

VelocityGeneratorPtr ForceConstraint::getVelocityGenerator() const {
	return velocity_generator_;
}
