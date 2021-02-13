project('pipewire-to-json', 'c',
  version : '0.0.1',
  license : 'MIT',
)

possible_cc_flags = [
  '-ffast-math',
  '-fno-common',
  '-fdiagnostics-show-option',
  '-fno-strict-aliasing',
  '-fvisibility=hidden',
  '-fcf-protection',
  '--param=ssp-buffer-size=4'
]

possible_link_flags = [
  '-Wl,-z,relro',
  '-Wl,-z,now',
  '-Wl,--gc-sections',
  '-fstack-protector'
]

cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments(possible_cc_flags), language : 'c')
add_project_link_arguments(cc.get_supported_link_arguments(possible_link_flags), language : 'c')

json_c = dependency('json-c', required: true)
libspa = dependency('libspa-0.2', required: true)
libpipewire = dependency('libpipewire-0.3', required: true)

bash_prog = find_program('bash')
pipewire_config_dir = get_option('pipewire-config-dir')

pipewire_to_json = executable('pipewire-to-json',
                     [ 'main.c' ],
                     install : true,
                     pie : true,
                     dependencies : [ json_c, libspa, libpipewire ])

test('integration-test', bash_prog, args: [
       '-x', join_paths(meson.current_source_dir(), 'test.sh'),
       pipewire_to_json,
       pipewire_config_dir
     ])
