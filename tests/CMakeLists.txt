
set(TESTS_TO_BUILD
    average_aggregation
    Fahrenheit_to_Celsius
    hvz_query
    identity_operation
    IncrementOp_test
    temp_sensors
    vowel_counter
   )


message("Creating targets for tests:")
foreach(build_target ${TESTS_TO_BUILD})
  message(" test_target: [${build_target}]")
  add_executable(${build_target} EXCLUDE_FROM_ALL ${build_target}/driver.cpp)
  target_link_libraries(${build_target} spe)
  target_include_directories(${build_target} PRIVATE ${spe_INCLUDE_DIR})
  set_target_properties(${build_target}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${spe_BINARY_DIR}/tests")
endforeach(build_target)


add_custom_target(tests
    COMMAND ${spe_TESTS_DIR}/run_tests.sh
    WORKING_DIRECTORY ${spe_TESTS_DIR}
    )
add_dependencies(tests ${TESTS_TO_BUILD})

