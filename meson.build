project(
  'sociodex',
  'c',
  version: '0.1.0'
)

release_date = ''

version = '-DVERSION="@0@"'
version = version.format(meson.project_version())
add_global_arguments(version, language : 'c')

version_conf = configuration_data()

version_conf.set(
  'SOCIODEX_VERSION',
  meson.project_version()
)

version_conf.set(
  'SOCIODEX_RELEASE_DATE',
  release_date
)

deps = [
  dependency('ncurses'),
  dependency('menu'),
  dependency('panel'),
  dependency('libpq'),
]

sources = []
headers = []

subdir('src')
# subdir('doc')

exe = executable(meson.project_name(),
  sources + headers,
  dependencies: deps,
  install: true
)

# this is just a quick hack to let us link our unit tests
lib = shared_library('lib' + meson.project_name(),
  sources + headers,
  dependencies: deps,
)

subdir('tests')
