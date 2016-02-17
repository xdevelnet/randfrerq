# randfrerq
Byte frequency cout

Uses /dev/urandom device if file is not specified. Intercepts SIGTERM and SIGINT just for finalize current output frame and return terminal to default state.

NOTE: For some reason some modern terminals demonstrate very low performance, and that's NOT program fault (xterm and pipe works just great).
