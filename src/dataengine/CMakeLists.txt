PROJECT(todo_engine)

include_directories(${KDE4_INCLUDE_DIR} ../../)

set(todo_engine_SRCS
    todoengine.cpp
    korganizerutil.cpp
)

kde4_add_plugin(plasma_engine_todo ${todo_engine_SRCS})
target_link_libraries(plasma_engine_todo ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} 
                                         ${KDE4_KDEUI_LIBS} ${KDE4_KCAL_LIBS} ${KDE4_KPIM_LIBS})

install(TARGETS plasma_engine_todo DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-todo.desktop DESTINATION ${SERVICES_INSTALL_DIR})
