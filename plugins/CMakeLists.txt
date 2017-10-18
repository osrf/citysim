include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


set (plugins
)

foreach (src ${plugins})
  add_library(${src} SHARED ${src}.cc)
  target_link_libraries(${src} ${GAZEBO_LIBRARIES})
  install (TARGETS ${src} DESTINATION ${CITYSIM_PLUGIN_INSTALL_DIR})
endforeach (src ${plugins})
