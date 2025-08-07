# Declare project information
project(
    'inotify_ipc',
    ['cpp', 'c'],
    version : files('VERSION'),
    default_options : [ 'cpp_std=c++17', 'c_std=c17' ],
)

# Project root and build directories
root_dir = meson.project_source_root()
build_dir = meson.project_build_root()
include_dir = root_dir / 'include'
src_dir = root_dir / 'src'
src_c_dir = src_dir / 'c'
tests_dir = root_dir / 'tests'
examples_dir = root_dir / 'examples'
examples_c_dir = examples_dir / 'c'

# Insert the project version into the version header file
conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
    configuration : conf_data,
    input : include_dir / 'version.h.in',
    output : 'version.h',
)

lib_inotify_ipc_headers = files(
    build_dir / 'version.h',
    include_dir / 'iipc.hpp',
    include_dir / 'iipc.h',
)
lib_inotify_ipc = library(
    'inotify_ipc',
    files(
        src_dir / 'version.cpp',
        src_dir / 'iipc.cpp',
        src_c_dir / 'string_c.cpp',
        src_c_dir / 'iipc_c.cpp',
    ),
    version : meson.project_version(),
    install : true,
)
install_headers(lib_inotify_ipc_headers, subdir : 'inotify_ipc')

dep_gtest_main = dependency(
    'gtest_main',
    required : false,
    method : 'auto',
)

tests = [
    'version',
    'get_channel',
    'channel_get_path',
    'channel_set_path',
    'channel_send_receive',
]

if dep_gtest_main.found()
    foreach test_name : tests
        test_exec = executable(
            'test_' + test_name,
            files(
                tests_dir / (test_name + '.test.cpp'),
            ),
            dependencies : dep_gtest_main,
            link_with : lib_inotify_ipc,
        )
        test(test_name, test_exec)
    endforeach
else
    warning('Skipping tests due to missing dependencies')
endif

examples = [
    'version',
    'sender',
    'receiver',
]

foreach example_name : examples
    executable(
        'example_' + example_name,
        files(
            examples_dir / (example_name + '.cpp'),
        ),
        link_with : lib_inotify_ipc,
    )
endforeach

examples_c = [
    'version',
    'sender',
    'receiver',
]

foreach example_name : examples_c
    executable(
        'example_' + example_name + '_c',
        files(
            examples_c_dir / (example_name + '.c'),
        ),
        link_with : lib_inotify_ipc,
    )
endforeach
