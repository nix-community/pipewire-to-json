# pipewire-to-json

DEPRECATED: Pipewire now comes with a similar tool: https://gitlab.freedesktop.org/pipewire/pipewire/-/commit/3c9996aa781c3cf3623547dbddad4772196ae391

Convert pipewire configuration files to json

## USAGE

```console
$ pipewire-to-json pipewire-config json-output
```

## Build

Depends on json-c and pipewire

```console
$ mkdir build
$ cd build
$ meson ..
$ ninja
```
