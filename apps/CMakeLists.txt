function(add_application app)
  declare_PID_Component(
    EXAMPLE_APPLICATION
    NAME ${app}
    DIRECTORY ${app}
  )

  declare_PID_Component_Dependency(
    COMPONENT ${app}
    NATIVE open-phri
  )

  declare_PID_Component_Dependency(
    COMPONENT ${app}
    NATIVE open-phri-vrep-driver
  )
endfunction()

add_application(stop_constraint_example)
add_application(velocity_constraint_example)
add_application(obstacle_avoidance_example)
add_application(separation_distance_example)
add_application(trajectory_generator_example)
add_application(trajectory_generator_benchmark)
add_application(path_follower_example)
add_application(force_control_example)
add_application(controller_benchmark)
add_application(demo)
add_application(test_app)
