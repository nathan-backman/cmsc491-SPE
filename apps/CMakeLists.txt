
set(APPS_TO_BUILD
    sample_driver
    increment
    )


message("Creating targets for apps:")
foreach(build_target ${APPS_TO_BUILD})
  message(" app_target: [${build_target}]")
  add_executable(${build_target} EXCLUDE_FROM_ALL ${build_target}.cpp)
  target_link_libraries(${build_target} spe)
  target_include_directories(${build_target} PRIVATE ${spe_INCLUDE_DIR})
  set_target_properties(${build_target}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${spe_BINARY_DIR}/apps")
endforeach(build_target)


add_custom_target(apps)
add_dependencies(apps ${APPS_TO_BUILD})


add_custom_target(run
    COMMAND sample_driver
    DEPENDS sample_driver
    )
