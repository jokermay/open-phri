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
 * @file vrep_driver.h
 * @author Benjamin Navarro
 * @brief Definition of the VREPDriver class and the ReferenceFrame enum.
 * @date April 2014
 * @ingroup VREP
 */


/** @defgroup VREP
 * Provides an easy interface to the V-REP simulator
 *
 * Usage: #include <vrep_driver/vrep_driver.h>
 *
 */

#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <utility>

#include <RSCL/definitions.h>

namespace vrep {

/** @enum vrep::ReferenceFrame
 *  @brief Specify an object's reference frame.
 */
enum class ReferenceFrame {
	TCP,    /**< Controlled frame. Tool control frame */
	Base,   /**< Frame fixed relative to the robot's base */
	World   /**< Frame fixed relative to the environment */
};

/** @brief Wrapper for V-REP low level C API.
 *  @details Can be used to control the simulation (start/pause/stop), to tack objects' position, read and send robot related data.
 *  Some objects need to be present in the scene: tcp (actual TCP pose), tcp_target (TCP reference pose), force_sensor (force sensor attached to the TCP),
 *  base_frame (frame attached to the base link) and world_frame (frame attached to the environment).
 *  All these objects can have a prefix and/or suffix specified in the constructor.
 */
class VREPDriver {
public:
	/**
	 * @brief Construct a driver using an IP & port. Prefix and suffix can be used to target a specific robot.
	 * @param sample_time The sample time set in V-REP (sorry, no way to get it remotely...).
	 * @param prefix [optional] Can be used to specify a prefix for all the basic objects.
	 * @param suffix [optional] Can be used to specify a suffix for all the basic objects.
	 * @param ip [optional] Can be used to specify a IP address different than the local network.
	 * @param port [optional] Can be used to specify a port different thant the default one.
	 */
	VREPDriver(
		double sample_time,
		const std::string& prefix = "",
		const std::string& suffix = "",
		const std::string& ip = "127.0.0.1",
		int port = 19997);

	/**
	 * @brief Construct a driver using an already open connection. Prefix and suffix can be used to target a specific robot.
	 * @param sample_time The sample time set in V-REP (sorry, no way to get it remotely...).
	 * @param client_id The client ID of the previously openned connection.
	 * @param prefix [optional] Can be used to specify a prefix for all the basic objects.
	 * @param suffix [optional] Can be used to specify a suffix for all the basic objects.
	 */
	VREPDriver(
		double sample_time,
		int client_id,
		const std::string& prefix = "",
		const std::string& suffix = "");

	~VREPDriver();

	/**
	 * @brief Retrieve the client ID of the current connection.
	 * @return The client ID. A negative value means that the connection has failed to open.
	 */
	int getClientID() const;

	/**
	 * @brief Check the state of the connection.
	 * @return True if the connection is still open, false otherwise.
	 */
	bool checkConnection() const;

	/**
	 * @brief Turn synchronous operation on or off. When on, VREPDriver::nextStep has to be called to trigger a simulation step.
	 * @return True if correctly set, false otherwise.
	 */
	bool enableSynchonous(bool state);

	/**
	 * @brief Trigger a simulation step.
	 * @return True if correctly set, false otherwise or if synchronous operation is disabled.
	 */
	bool nextStep() const;

	/**
	 * @brief Start the simulation.
	 */
	void startSimulation() const;

	/**
	 * @brief Stop the simulation.
	 */
	void stopSimulation() const;

	/**
	 * @brief Pause the simulation.
	 */
	void pauseSimulation() const;

	/**
	 * @brief Get the TCP pose in the given frame.
	 * @param pose [out] The current TCP pose.
	 * @param frame [in] The reference frame.
	 * @return True if correctly read, false otherwise.
	 */
	bool readTCPPose(RSCL::Vector6dPtr pose, ReferenceFrame frame) const;

	/**
	 * @brief Get the TCP velocity in the given frame.
	 * @param velocity [out] The current TCP velocity.
	 * @param frame [in] The reference frame.
	 * @return True if correctly read, false otherwise.
	 */
	bool readTCPVelocity(RSCL::Vector6dPtr velocity, ReferenceFrame frame) const;

	/**
	 * @brief Get the target TCP pose in the given frame.
	 * @param pose [out] The current target TCP pose.
	 * @param frame [in] The reference frame.
	 * @return True if correctly read, false otherwise.
	 */
	bool readTCPTargetPose(RSCL::Vector6dPtr pose, ReferenceFrame frame) const;

	/**
	 * @brief Send the target TCP velocity in the given frame.
	 * @param velocity [in] The target TCP velocity.
	 * @param frame [in] The reference frame.
	 * @return True if correctly read, false otherwise.
	 */
	bool sendTCPtargetVelocity(RSCL::Vector6dConstPtr velocity, ReferenceFrame frame) const;

	/**
	 * @brief Get the TCP wrench in the TCP frame.
	 * @param wrench [out] The current TCP wrench.
	 * @return True if correctly read, false otherwise.
	 */
	bool readTCPWrench(RSCL::Vector6dPtr wrench) const;

	/**
	 * @brief Start tracking a given object in the specified frame.
	 * @param name [in] The full name of the object to track.
	 * @param frame [in] The reference frame.
	 * @return A shared pointer to the object's postion. Updated on VREPDriver::updateTrackedObjectsPosition.
	 */
	RSCL::Vector6dConstPtr trackObjectPosition(const std::string& name, ReferenceFrame frame);

	/**
	 * @brief Update all tracked objects' position.
	 * @return True if correctly read, false otherwise.
	 */
	bool updateTrackedObjectsPosition();

private:
	void init(const std::string& ip, int port);
	void init(int client_id);
	bool getObjectHandles();
	void startStreaming() const;
	int getFrameHandle(ReferenceFrame frame) const;

	int client_id_;
	double sample_time_;
	bool sync_mode_;

	std::string prefix_;
	std::string suffix_;

	std::unordered_map<std::string, int> object_handles_;
	std::map<std::pair<int,int>, RSCL::Vector6dPtr> tracked_objects_;

};

}
