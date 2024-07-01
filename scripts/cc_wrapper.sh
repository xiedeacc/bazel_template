#!/bin/bash
# shellcheck disable=SC1083

set -euo pipefail

if [[ -f %{toolchain_path_prefix}bin/%{gcc_name}]]; then
  execroot_path=""
elif [[ ${BASH_SOURCE[0]} == "/"* ]]; then
  execroot_path="${BASH_SOURCE[0]%/*/*/*/*}/"
else
  echo >&2 "ERROR: could not find clang; PWD=\"${PWD}\"; PATH=\"${PATH}\"."
  exit 5
fi

function sanitize_option() {
  local -r opt=$1
  if [[ ${opt} == */cc_wrapper.sh ]]; then
    printf "%s" "${execroot_path}%{toolchain_path_prefix}bin/${gcc_name}"
  elif [[ ${opt} =~ ^-fsanitize-(ignore|black)list=[^/] ]]; then
    # shellcheck disable=SC2206
    parts=(${opt/=/ }) # Split flag name and value into array.
    printf "%s" "${parts[0]}=${execroot_path}${parts[1]}"
  else
    printf "%s" "${opt}"
  fi
}

cmd=()
for ((i = 0; i <= $#; i++)); do
  if [[ ${!i} == @* ]]; then
    while IFS= read -r opt; do
      opt="$(
        set -e
        sanitize_option "${opt}"
      )"
      cmd+=("${opt}")
    done <"${!i:1}"
  else
    opt="$(
      set -e
      sanitize_option "${!i}"
    )"
    cmd+=("${opt}")
  fi
done

# Call the C++ compiler.
exec "${cmd[@]}"
