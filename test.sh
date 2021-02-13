#!/usr/bin/env bash
set -euo pipefail

pwtojson=$1
pipewire_config_dir=$2

tempfile=$(mktemp)
trap "rm -f $tempfile" EXIT

configs=$(find "$pipewire_config_dir" -type f -iname '*.conf')

for from in $configs; do
    echo "$pwtojson" "$from" "$tempfile"
    "$pwtojson" "$from" "$tempfile"
done
