/*      File: main.cpp
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

#include <iostream>

#include <OpenPHRI/OpenPHRI.h>

using namespace std;
using namespace phri;

constexpr double SAMPLE_TIME = 0.001;

int main(int argc, char const *argv[]) {
	Clock clock(SAMPLE_TIME);
	DataLogger logger(
		"/tmp",
		clock.getTime(),
		true
		);

#if 0
	std::cout << "Testing the Trajectory class on double\n";
	auto p1 = TrajectoryPoint<double>(0.,0.,0.);
	auto p2 = TrajectoryPoint<double>(1.,0.,0.);
	auto tg = TrajectoryGenerator<double>(p1, SAMPLE_TIME, TrajectorySynchronization::SynchronizeWaypoints);
	tg.addPathTo(p2, 0.5, 1.);
	tg.addPathTo(p1, 0.1, 0.5);
	tg.computeTimings();
	cout << "Trajectory: " << tg.getTrajectoryDuration() << "\n";
	for (size_t i = 0; i < 2; ++i) {
		cout << "\tSegment " << i+1 << ": " << tg.getSegmentDuration(i) << "\n";
		cout << "\t\tComponent " << 1 << ": " << tg.getSegmentDuration(i, 0) << "\n";
	}


	std::cout << "Testing the Trajectory class on Vector6d\n";
	Vector6d y, vmax, amax;
	y << 1., 0.9, 0.8, 0.7, 0.6, 0.5;
	vmax << 1., 2., 3., 4., 5., 6.;
	amax << 1., 2., 3., 4., 5., 6.;
	auto point_1 = TrajectoryPoint<Vector6d>(Vector6d::Zero(), Vector6d::Zero(), Vector6d::Zero());
	auto point_2 = TrajectoryPoint<Vector6d>(y, Vector6d::Zero(), Vector6d::Zero());
	auto trajectory_generator = TrajectoryGenerator<Vector6d>(point_1, SAMPLE_TIME, TrajectorySynchronization::SynchronizeTrajectory);
	trajectory_generator.addPathTo(point_2, vmax, amax);
	trajectory_generator.addPathTo(point_1, vmax * 0.5, amax * 0.25);
	// get<1>(trajectory_generator[0][3]) = 0.1; // get<1> => dy, [0] => 1st point, [3] => 4th component
	trajectory_generator.computeTimings();

	cout << "Trajectory: " << trajectory_generator.getTrajectoryDuration() << "\n";
	for (size_t i = 0; i < 2; ++i) {
		cout << "\tSegment " << i+1 << ": " << trajectory_generator.getSegmentDuration(i) << "\n";
		for (size_t j = 0; j < 6; ++j) {
			cout << "\t\tComponent " << j+1 << ": " << trajectory_generator.getSegmentDuration(i, j) << "\n";
		}
	}

	auto reference = std::make_shared<Vector6d>(Vector6d::Zero());
	Vector6d threshold;
	threshold.setConstant(0.01);
	trajectory_generator.enableErrorTracking(reference, threshold, true);

	logger.logExternalData("traj6d", trajectory_generator.getPositionOutput()->data(), 6);
	while(not trajectory_generator()) {
		clock();
		logger();

		if(not (*clock.getTime() > 1.5 and *clock.getTime() < 2.5)) {
			*reference = *trajectory_generator.getPositionOutput();
		}
		else {
			reference->block<5,1>(1,0) = trajectory_generator.getPositionOutput()->block<5,1>(1,0);
		}
	}

	std::cout << "Reset & recomputeTimings" << std::endl;

	trajectory_generator.reset();
	trajectory_generator.computeTimings();
	double start = *clock.getTime();
	while(not trajectory_generator()) {
		clock();
		logger();

		if(not (*clock.getTime()-start > 4. and *clock.getTime()-start < 5.)) {
			*reference = *trajectory_generator.getPositionOutput();
		}
		else {
			reference->block<5,1>(1,0) = trajectory_generator.getPositionOutput()->block<5,1>(1,0);
		}
	}
#endif
	std::cout << "Testing the Trajectory class on Pose\n";
	Pose target_pose(Vector3d(0.5, 1., 2.), Vector3d(0.1, 1., 2.));
	// target_pose.translation() = Vector3d(0.5, 1., 2.);
	// target_pose.orientation().integrate(Vector3d(0.1, 1., 2.));
	Twist max_twist(Vector3d(1., 2., 0.5), Vector3d(0.5, 0.5, 0.5));
	Acceleration max_acceleration(Vector3d(10., 1., 2.), Vector3d(1., 0.25, 0.75));
	auto pose_1 = TrajectoryPoint<Pose>();
	auto pose_2 = TrajectoryPoint<Pose>(target_pose, Twist(), Acceleration());
	auto pose_generator = TrajectoryGenerator<Pose>(pose_1, SAMPLE_TIME, TrajectorySynchronization::SynchronizeWaypoints);
	pose_generator.addPathTo(pose_2, max_twist, max_acceleration);
	pose_generator.addPathTo(pose_1, Twist(static_cast<Vector6d>(max_twist) * 0.5), Acceleration(static_cast<Vector6d>(max_acceleration) * 0.25));
	pose_generator.computeTimings();

	clock.reset();
	logger.reset();
	logger.logExternalData("traj_position", pose_generator.getPoseOutput()->translation().data(), 3);
	logger.logExternalData("traj_orientation", pose_generator.getPoseOutput()->orientation().coeffs().data(), 4);
	logger.logExternalData("traj_twist", pose_generator.getTwistOutput()->data(), 6);
	logger.logExternalData("traj_acceleration", pose_generator.getAccelerationOutput()->data(), 6);
	while(not pose_generator()) {
		clock();
		logger();
	}

#if 0
	auto x_point_1 = make_shared<TrajectoryPoint<double>>(0.,  0.,     0.);
	auto x_point_2 = make_shared<TrajectoryPoint<double>>(1.,  0.,     0.);
	auto x_point_3 = make_shared<TrajectoryPoint<double>>(-1,  0.,     0.);

	auto y_point_1 = make_shared<TrajectoryPoint<double>>(1.,  0.,  0.);
	auto y_point_2 = make_shared<TrajectoryPoint<double>>(-1., 0.,  0.);
	auto y_point_3 = make_shared<TrajectoryPoint<double>>(0.,  0.,  0.);

	auto target_position = Vector2d();

	auto x_traj = make_shared<Trajectory<double>>(TrajectoryOutputType::Position, x_point_1, &(target_position.x()), SAMPLE_TIME);
	auto y_traj = make_shared<Trajectory<double>>(TrajectoryOutputType::Position, y_point_1, &(target_position.y()), SAMPLE_TIME);


	x_traj->addPathTo(x_point_2, 0.5, 1.);
	x_traj->addPathTo(x_point_3, 0.5, 1.);

	y_traj->addPathTo(y_point_2, 0.5, 0.5);
	y_traj->addPathTo(y_point_3, 0.25, 0.5);

	auto trajectory_generator = TrajectoryGenerator(TrajectorySynchronization::NoSynchronization);
	trajectory_generator.add("x_traj", x_traj);
	trajectory_generator.add("y_traj", y_traj);

	auto print_durations = [&x_traj, &y_traj]() {
							   std::cout << "-------------------------------------\n";
							   std::cout << "x_traj: 0., " << x_traj->getPathDuration(0) << ", " << x_traj->getPathDuration(0)+x_traj->getPathDuration(1) << "\n";
							   std::cout << "y_traj: 0., " << y_traj->getPathDuration(0) << ", " << y_traj->getPathDuration(0)+y_traj->getPathDuration(1) << "\n";
						   };

	trajectory_generator.computeParameters();
	print_durations();

	Clock clock(SAMPLE_TIME);
	DataLogger logger(
		"/mnt/tmpfs/open-phri_logs",
		clock.getTime(),
		true, // create gnuplot files
		true);

	logger.logExternalData("trajectory_no_sync", target_position.data(), 2);
	do {
		clock();
		logger();
	} while(not trajectory_generator.compute());

	logger.reset();
	clock.reset();
	logger.logExternalData("trajectory_wp_sync", target_position.data(), 2);
	trajectory_generator.setSynchronizationMethod(TrajectorySynchronization::SynchronizeWaypoints);
	trajectory_generator.computeParameters();
	trajectory_generator.reset();
	print_durations();
	do {
		clock();
		logger();
	} while(not trajectory_generator.compute());

	logger.reset();
	clock.reset();
	logger.logExternalData("trajectory_traj_sync", target_position.data(), 2);
	trajectory_generator.setSynchronizationMethod(TrajectorySynchronization::SynchronizeTrajectory);
	trajectory_generator.computeParameters();
	trajectory_generator.reset();
	print_durations();
	do {
		clock();
		logger();
	} while(not trajectory_generator.compute());

#endif

	return 0;
}
