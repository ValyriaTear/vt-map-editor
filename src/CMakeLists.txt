#vt-editor /src

FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

# Set the release mode if not told otherwise
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Handles Clang flags
IF(CMAKE_CXX_COMPILER MATCHES ".*clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
ENDIF()

# Handles gcc flags
IF(CMAKE_COMPILER_IS_GNUCXX)
    # Help getting compilation warnings and c++11 support
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
    IF(WIN32)
        # This includes enough debug information to get something useful
        # from Dr. Mingw while keeping binary size down. Almost useless
        # with gdb, though.
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ggdb0 -gstabs2")
    ENDIF()
ENDIF()

# in case a user overwrites these paths and specifies a relative one
FOREACH(dir PKG_DATADIR LOCALEDIR)
    IF(WIN32)
        # no change for win32
        SET(FULL_${dir} ${${dir}})
    ELSE(WIN32)
        IF(NOT IS_ABSOLUTE ${${dir}})
            SET(FULL_${dir} "${CMAKE_INSTALL_PREFIX}/${${dir}}")
        ELSE()
            SET(FULL_${dir} ${${dir}})
        ENDIF()
    ENDIF(WIN32)
ENDFOREACH()

SET(FLAGS "-DPACKAGE_VERSION=\\\"${VERSION}\\\"")
SET(FLAGS "${FLAGS} -DPKG_DATADIR=\\\"${FULL_PKG_DATADIR}/\\\"")
SET(FLAGS "${FLAGS} -DLOCALEDIR=\\\"${FULL_LOCALEDIR}/\\\"")
SET(FLAGS "${FLAGS} -DPKG_NAME=\\\"${PKG_NAME}/\\\"")

IF (CMAKE_BUILD_TYPE)
    STRING(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
    IF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug OR
       CMAKE_BUILD_TYPE_TOLOWER MATCHES relwithdebinfo)
        SET(FLAGS "${FLAGS} -DDEBUG")

        IF (CMAKE_COMPILER_IS_GNUCXX)
            # Help getting debug info
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")
        ENDIF()
    ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${QT_QTOPENGL_INCLUDE_DIR}
    )

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/luabind")
MESSAGE(STATUS "Using included luabind files")

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/luabind/build_information.hpp.cmake_in" "${CMAKE_CURRENT_SOURCE_DIR}/luabind/luabind/build_information.hpp")
MESSAGE(STATUS "Configured luabind information file...")

# Enable C++11 in luabind
SET(FLAGS "${FLAGS} -DLUABIND_USE_CXX11")

# enable rc-handling with mingw
# most likely this part can be kicked out with some later cmake version
IF (MINGW)
    FIND_PATH(MINGW_INCLUDE_DIR windows.h $ENV{INCLUDE})
    IF (MINGW_INCLUDE_DIR)
        MESSAGE(STATUS "Found mingw headers: ${MINGW_INCLUDE_DIR}")
        INCLUDE_DIRECTORIES(${MINGW_INCLUDE_DIR})
    ELSE()
        MESSAGE(FATAL_ERROR "Unable to find mingw headers. Required for windres")
    ENDIF()
    SET(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
      "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -O coff -o <OBJECT> <SOURCE>")
ENDIF()

SET(SRCS_LUABIND
luabind/src/class_registry.cpp
luabind/src/class_rep.cpp
luabind/src/object_rep.cpp
luabind/src/pcall.cpp
luabind/src/scope.cpp
luabind/src/stack_content_by_name.cpp
luabind/src/inheritance.cpp
luabind/src/class.cpp
luabind/src/error.cpp
luabind/src/weak_ref.cpp
luabind/src/class_info.cpp
luabind/src/create_class.cpp
luabind/src/function.cpp
luabind/src/link_compatibility.cpp
luabind/src/exception_handler.cpp
luabind/src/wrapper_base.cpp
luabind/src/open.cpp
luabind/src/lua51compat.cpp
luabind/luabind/prefix.hpp
luabind/luabind/operator.hpp
luabind/luabind/back_reference_fwd.hpp
luabind/luabind/config.hpp
luabind/luabind/out_value_policy.hpp
luabind/luabind/return_reference_to_policy.hpp
luabind/luabind/back_reference.hpp
luabind/luabind/function.hpp
luabind/luabind/object.hpp
luabind/luabind/scope.hpp
luabind/luabind/copy_policy.hpp
luabind/luabind/version.hpp
luabind/luabind/container_policy.hpp
luabind/luabind/class.hpp
luabind/luabind/lua_include.hpp
luabind/luabind/open.hpp
luabind/luabind/get_pointer.hpp
luabind/luabind/raw_policy.hpp
luabind/luabind/class_info.hpp
luabind/luabind/get_main_thread.hpp
luabind/luabind/luabind.hpp
luabind/luabind/from_stack.hpp
luabind/luabind/dependency_policy.hpp
luabind/luabind/typeid.hpp
luabind/luabind/adopt_policy.hpp
luabind/luabind/weak_ref.hpp
luabind/luabind/iterator_policy.hpp
luabind/luabind/shared_ptr_converter.hpp
luabind/luabind/discard_result_policy.hpp
luabind/luabind/exception_handler.hpp
luabind/luabind/value_wrapper.hpp
luabind/luabind/make_function.hpp
luabind/luabind/tag_function.hpp
luabind/luabind/nil.hpp
luabind/luabind/handle.hpp
luabind/luabind/detail/object_rep.hpp
luabind/luabind/detail/enum_maker.hpp
luabind/luabind/detail/call_function.hpp
luabind/luabind/detail/format_signature.hpp
luabind/luabind/detail/constructor.hpp
luabind/luabind/detail/object_call.hpp
luabind/luabind/detail/inheritance.hpp
luabind/luabind/detail/decorate_type.hpp
luabind/luabind/detail/primitives.hpp
luabind/luabind/detail/convert_to_lua.hpp
luabind/luabind/detail/most_derived.hpp
luabind/luabind/detail/stack_utils.hpp
luabind/luabind/detail/open.hpp
luabind/luabind/detail/property.hpp
luabind/luabind/detail/other.hpp
luabind/luabind/detail/link_compatibility.hpp
luabind/luabind/detail/typetraits.hpp
luabind/luabind/detail/has_get_pointer.hpp
luabind/luabind/detail/debug.hpp
luabind/luabind/detail/garbage_collector.hpp
luabind/luabind/detail/instance_holder.hpp
luabind/luabind/detail/class_rep.hpp
luabind/luabind/detail/signature_match.hpp
luabind/luabind/detail/call_member.hpp
luabind/luabind/detail/pointee_sizeof.hpp
luabind/luabind/detail/policy.hpp
luabind/luabind/detail/deduce_signature.hpp
luabind/luabind/detail/class_registry.hpp
luabind/luabind/detail/yes_no.hpp
luabind/luabind/detail/operator_id.hpp
luabind/luabind/detail/make_instance.hpp
luabind/luabind/detail/pcall.hpp
luabind/luabind/detail/call.hpp
luabind/luabind/detail/call_operator_iterate.hpp
luabind/luabind/yield_policy.hpp
luabind/luabind/error.hpp
luabind/luabind/wrapper_base.hpp
)

# Files used by both the game and the editor
SET(SRCS_COMMON
script/script.h
script/script.cpp
script/script_read.h
script/script_read.cpp
script/script_write.h
script/script_write.cpp
utils/utils_pch.h
utils/utils_pch.cpp
utils/singleton.h
utils/utils_random.h
utils/utils_random.cpp
utils/utils_files.h
utils/utils_files.cpp
utils/utils_numeric.h
utils/utils_numeric.cpp
utils/utils_strings.h
utils/utils_strings.cpp
)

SET(EDITOR_QT_HEADERS
dialog_boxes.h
editor.h
grid.h
tileset_editor.h
)

SET(SRCS_EDITOR
dialog_boxes.cpp
editor.cpp
editor_main.cpp
grid.cpp
tileset.cpp
tileset.h
tileset_editor.cpp
)

QT5_WRAP_CPP(EDITOR_QT_HEADERS_MOC ${EDITOR_QT_HEADERS})

QT5_ADD_RESOURCES(EDITOR_QT_RES "${PROJECT_SOURCE_DIR}/data/data.qrc")

SET (PROGRAMS vt-editor)

ADD_EXECUTABLE(vt-editor
    ${SRCS_EDITOR}
    ${SRCS_LUABIND}
    ${EDITOR_QT_HEADERS_MOC}
    ${EDITOR_QT_RES}
    ${SRCS_COMMON}
)
qt5_use_modules(vt-editor Widgets OpenGL)

TARGET_LINK_LIBRARIES(vt-editor
    ${INTERNAL_LIBRARIES}
    ${QT_LIBRARIES}
    ${QT_QTOPENGL_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${X11_LIBRARIES}
    ${EXTRA_LIBRARIES}
)

INSTALL(TARGETS vt-editor RUNTIME DESTINATION ${PKG_BINDIR})
SET_TARGET_PROPERTIES(vt-editor PROPERTIES COMPILE_FLAGS "${FLAGS}")

IF (UNIX)
    # uninstall target
    ADD_CUSTOM_TARGET(
        uninstall-vt-editor
        COMMAND rm -f "${PKG_BINDIR}/vt-editor"
        VERBATIM
    )
ENDIF()
