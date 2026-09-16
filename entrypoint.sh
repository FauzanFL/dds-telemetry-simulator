#!/bin/sh
# Setel tombol Backspace ke ^H
stty erase '^H' 2>/dev/null || true
exec "$@"
