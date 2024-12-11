#!/bin/sh

generate_password() {
  length=$1
  tr -dc 'A-Za-z0-9!@#$%^&*()_+<>?' </dev/urandom | head -c "$length" ; echo ''
}

if [ "$1" = "--generate-password" ]; then
  if [ -z "$2" ]; then
    echo "Error: Please specify the password length."
    echo "Usage: $0 --generate-password <length>"
    exit 1
  fi
  PASSWORD=$(generate_password "$2")
  echo "Generated password: $PASSWORD"

  HASHED_PASSWORD=$(echo -n "$PASSWORD" | sha256sum | awk '{print $1}')
  echo "Hashed password: $HASHED_PASSWORD"

  export GENERATED_PASSWORD="$PASSWORD"
fi

if [ ! -d "build" ]; then
  mkdir build
fi
cd build || exit 1

cmake ..
make
./main
