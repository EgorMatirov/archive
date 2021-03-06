cmake_minimum_required(VERSION 3.0)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

project(bacs_archive)

if(BunsanCMakeBuiltin)
    BunsanCMakeLoad()
else()
    find_package(BunsanCMake CONFIG REQUIRED)
endif()

option(ENABLE_WEB "Enable CppCMS-based frontend" ON)

if(ENABLE_WEB)
    if(BunsanWebBuiltin)
        BunsanWebLoad()
    else()
        find_package(BunsanWeb CONFIG REQUIRED)
    endif()
endif()

bunsan_find_bunsan_package(bacs_problems bacs_problems)

bunsan_add_protobuf_cxx_library(
    TARGET ${PROJECT_NAME}_pb
    PROTOS
        bacs/archive/archive.proto
        bacs/archive/problem/error.proto
        bacs/archive/problem/flag.proto
        bacs/archive/problem/id.proto
        bacs/archive/problem/import.proto
        bacs/archive/problem/status.proto
    LIBRARIES ${bacs_problems_LIBRARIES}
    GRPC
    INCLUDE_DIRECTORIES ${bacs_problems_INCLUDE_DIRS}
    INSTALL
)

bunsan_add_library(${PROJECT_NAME}
    src/lib/archive_client.cpp
    src/lib/archive_service.cpp
    src/lib/importer.cpp
    src/lib/repository.cpp
    src/lib/repository_impl.cpp
    src/lib/repository_post.cpp
    src/lib/rpc.cpp
    src/lib/problem.cpp
)
bunsan_use_target(${PROJECT_NAME} ${PROJECT_NAME}_pb)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_crypto bunsan_crypto)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_protobuf bunsan_protobuf)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_pm bunsan_pm)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_rpc bunsan_rpc)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

bunsan_add_executable(${PROJECT_NAME}_client
    src/bin/client.cpp
)
bunsan_use_target(${PROJECT_NAME}_client ${PROJECT_NAME})

if(ENABLE_WEB)
    set(web_srcs
        src/web/repository.cpp
        src/web/content/form/base.cpp
        src/web/content/form/list_query.cpp
        src/web/content/form/upload.cpp
        src/web/content/form/download.cpp
        src/web/content/form/rename.cpp
        src/web/content/form/existing.cpp
        src/web/content/form/status.cpp
        src/web/content/form/with_flag.cpp
        src/web/content/form/set_flags.cpp
        src/web/content/form/unset_flags.cpp
        src/web/content/form/import.cpp
        src/web/content/form.cpp
        src/web/content/form/widgets/archiver_config.cpp
        src/web/content/form/widgets/problem/id.cpp
        src/web/content/form/widgets/problem/id_set.cpp
        src/web/content/form/widgets/problem/optional_id_set.cpp
        src/web/content/form/widgets/problem/flag_set.cpp
    )
endif()

bunsan_add_executable(${PROJECT_NAME}_server
    ${web_srcs}
    src/bin/server.cpp
)
bunsan_use_target(${PROJECT_NAME}_server ${PROJECT_NAME})
bunsan_use_bunsan_package(${PROJECT_NAME}_server bunsan_protobuf bunsan_protobuf)
if(WIN32)
    target_link_libraries(${PROJECT_NAME}_server PRIVATE ws2_32)
endif()
if(ENABLE_WEB)
    target_compile_definitions(${PROJECT_NAME}_server PRIVATE
        BACS_ARCHIVE_USE_WEB=1
    )
    bunsan_use_bunsan_package(${PROJECT_NAME}_server bunsan_web bunsan_web)
    bunsan_use_package(${PROJECT_NAME} CppCMS)

    bunsan_web_add_skin(${PROJECT_NAME}_skin_plain plain
        view/plain/base.tmpl
        view/plain/base_form.tmpl

        view/plain/response/id_set.tmpl
        view/plain/response/import_map.tmpl
        view/plain/response/status.tmpl
        view/plain/response/status_map.tmpl

        view/plain/error.tmpl
        view/plain/upload.tmpl
        view/plain/download.tmpl
        view/plain/rename.tmpl
        view/plain/existing.tmpl
        view/plain/status.tmpl
        view/plain/with_flag.tmpl
        view/plain/set_flags.tmpl
        view/plain/unset_flags.tmpl
        view/plain/clear_flags.tmpl
        view/plain/import_result.tmpl
        view/plain/import.tmpl
    )
    bunsan_use_target(${PROJECT_NAME}_skin_plain ${PROJECT_NAME})
endif()

bunsan_install_headers()
bunsan_install_targets(
    ${PROJECT_NAME}
    ${PROJECT_NAME}_client
    ${PROJECT_NAME}_server
)
if(ENABLE_WEB)
    bunsan_web_install_skins(${PROJECT_NAME}_skin_plain)
    bunsan_web_install_static()
endif()
bunsan_install_project()

bunsan_include_tests()
