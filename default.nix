{ pkgs ? import <nixpkgs> {} }:
pkgs.stdenv.mkDerivation {
  pname = "pipewire-to-json";
  version = "0.0.1";

  src = ./.;

  mesonFlags = [
    "-Dpipewire-config-dir=${pkgs.pipewire}/etc/pipewire"
  ];

  doCheck = true;

  nativeBuildInputs = with pkgs; [
    pkg-config
    meson
    ninja
  ];

  buildInputs = with pkgs; [
    json_c
    pipewire
  ];
}
