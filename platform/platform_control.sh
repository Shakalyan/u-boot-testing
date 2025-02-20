#!/bin/bash

IN_PIPE=platform_in.pipe
OUT_PIPE=platform_out.pipe

PTY_DEV=""
_QEMU_PID=-1



kill_qemu() {
    pkill -P $_QEMU_PID 2>/dev/null
}

turn_off() {
    kill_qemu
    exit
}

turn_on() {
    rm $IN_PIPE $OUT_PIPE
    mkfifo $IN_PIPE $OUT_PIPE
    eval "exec 3<>$OUT_PIPE"

    trap turn_off SIGTERM
    trap turn_off SIGINT
}

get_pty() {
    qemu_output="$1"
    str=$(grep "/dev/pts" <(echo "$qemu_output"))
    echo "$str" | python3 -c 'print(input().split(" ")[5])'
}

reset() {
    kill_qemu
    coproc QEMU { make run ; }
    _QEMU_PID=$QEMU_PID
    qemu_output=$(timeout 1 cat <&"${QEMU[0]}")
    PTY_DEV=$(get_pty "$qemu_output")
    #echo $PTY_DEV
}

send_output() {
    echo "$1"
    echo "$1" > $OUT_PIPE
}



turn_on

while [ 1 ]; do
    inp=$(cat $IN_PIPE)
    if [ "$inp" = "reset" ]; then
        reset
#        send_output "RESET"
        send_output "$PTY_DEV"
    else
        send_output "unknown cmd: $inp"
    fi
done

turn_off
