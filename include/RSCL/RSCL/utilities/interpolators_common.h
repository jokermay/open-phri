/*
 *  Copyright (C) 2017 Benjamin Navarro <contact@bnavarro.info>
 *
 *  This file is part of RSCL <https://gite.lirmm.fr/navarro/RSCL>.
 *
 *  RSCL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RSCL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with RSCL.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file interpolators_common.h
 * @author Benjamin Navarro
 * @brief Common definitions used by the interpolators/trajectory generators
 * @date April 2017
 * @ingroup RSCL
 */

#pragma once

#include <RSCL/definitions.h>

namespace RSCL {

/** @brief Description of a point used by the TrajectoryGenerator.
 *  @details A TrajectoryPoint is described by a 2D point (x,y) and its first and second derivatives
 */
struct TrajectoryPoint {
	TrajectoryPoint(
		doublePtr y,
		doublePtr dy,
		doublePtr d2y) :
		y(y), dy(dy), d2y(d2y)
	{
	}

	TrajectoryPoint(
		double y,
		double dy,
		double d2y) :
		y(std::make_shared<double>(y)),
		dy(std::make_shared<double>(dy)),
		d2y(std::make_shared<double>(d2y))
	{
	}

	doublePtr y;   // value
	doublePtr dy;  // first derivative
	doublePtr d2y; // second derivative
};

using TrajectoryPointPtr = std::shared_ptr<TrajectoryPoint>;
using TrajectoryPointConstPtr = std::shared_ptr<const TrajectoryPoint>;


/** @brief Description of a point used by the PolynomialInterpolator.
 *  @details A PolynomialPoint is described by a 2D point (x,y) and its first and second derivatives
 */
struct PolynomialPoint : public TrajectoryPoint {
	PolynomialPoint(
		doublePtr x,
		doublePtr y,
		doublePtr dy,
		doublePtr d2y) :
		TrajectoryPoint(y, dy, d2y),
		x(x)
	{
	}

	PolynomialPoint(
		double x,
		double y,
		double dy,
		double d2y) :
		TrajectoryPoint(y, dy, d2y),
		x(std::make_shared<double>(x))
	{
	}

	doublePtr x;   // x value
};

using PolynomialPointPtr = std::shared_ptr<PolynomialPoint>;
using PolynomialPointConstPtr = std::shared_ptr<const PolynomialPoint>;


/** @brief Description of a point used by the LinearInterpolator.
 *  @details A LinearPoint is described by a 2D point (x,y)
 */
struct LinearPoint {
	LinearPoint(
		doublePtr x,
		doublePtr y) :
		x(x), y(y)
	{
	}

	LinearPoint(
		double x,
		double y) :
		x(std::make_shared<double>(x)), y(std::make_shared<double>(y))
	{
	}

	doublePtr x;   // x value
	doublePtr y;   // y value
};

using LinearPointPtr = std::shared_ptr<LinearPoint>;
using LinearPointConstPtr = std::shared_ptr<const LinearPoint>;


}
