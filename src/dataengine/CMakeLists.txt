PROJECT(todo_engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
include_directories(${KDE4_INCLUDE_DIR} ../../)

set(todo_engine_SRCS
    todoengine.cpp
    korganizerutil.cpp
)

kde4_add_plugin(plasma_engine_todo ${todo_engine_SRCS})
target_link_libraries(plasma_engine_todo ${KDE4_PLASMA_LIBS}
                                         ${KDE4_AKONADI_LIBS}
                                         ${KDE4_KIO_LIBS}
                                         ${KDE4_KMIME_LIBS}
                                         ${KDE4_AKONADI_KMIME_LIBS}
                                         ${KDE4_KPIMUTILS_LIBS}
                                         ${KDE4_KCAL_LIBS}
                                         ${KDE4_KABC_LIBS})

install(TARGETS plasma_engine_todo DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-todo.desktop DESTINATION ${SERVICES_INSTALL_DIR})
