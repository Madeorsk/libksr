project('ksr', 'c', version : '0.1', default_options : ['warning_level=3'])
c = meson.get_compiler('c')

# declare the dependencies of the library.
deps = [ dependency('jemalloc') ]
# add pthread dependency
deps += c.find_library('pthread')

# includes
include_dirs = include_directories([ 'include' ])

# source files.
libsrc = [
	'src/arrays.c',
	'src/buffers.c',
	'src/promises.c',
	'src/regex.c',
	'src/files.c',
	'src/string.c',
]

# library target.
libksr = library('ksr', libsrc, dependencies : deps, include_directories : include_dirs, install : true)
# initialize library dependency.
libksr_dep = declare_dependency(link_with : libksr, include_directories : 'include')

# add the library to the dependencies of the executables.
deps += libksr_dep

# test executables.
test_ksrarrays = executable('test_ksrarrays', [ 'tests/ksrarrays.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrbuffers = executable('test_ksrbuffers', [ 'tests/ksrbuffers.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrpromises = executable('test_ksrpromises', [ 'tests/ksrpromises.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksregex = executable('test_ksregex', [ 'tests/ksregex.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrstring = executable('test_ksrstring', [ 'tests/ksrstring.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrfiles = executable('test_ksrfiles', [ 'tests/ksrfiles.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrlogging = executable('test_ksrlogging', [ 'tests/ksrlogging.c' ], dependencies : deps, include_directories : include_dirs, install : false)
test_ksrerrors = executable('test_ksrerrors', [ 'tests/ksrerrors.c' ], dependencies : deps, include_directories : include_dirs, install : false)

# tests.
test('ksrarrays', test_ksrarrays)
test('ksrbuffers', test_ksrbuffers)
test('ksrpromises', test_ksrpromises)
test('ksregex', test_ksregex)
test('ksrfiles', test_ksrfiles)
test('ksrstring', test_ksrstring)
test('ksrlogging', test_ksrlogging)
test('ksrerrors', test_ksrerrors)
