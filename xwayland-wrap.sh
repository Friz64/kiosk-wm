#!/bin/sh
set -e
new_display=":"$((1000 + $RANDOM % 9000))
echo "using display $new_display"
trap 'kill 0' EXIT
echo "running Xwayland in rootful mode"
Xwayland-libdecor -decorate $new_display &
sleep 0.25s
echo "running kiosk wm"
env DISPLAY=$new_display ./kiosk-wm &
echo "running program"
env DISPLAY=$new_display "$@"
