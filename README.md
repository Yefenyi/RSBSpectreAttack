## System setup
Our attacks require modern Spectre protections to be disabled. This can be done using Linux's `no_spectreV2` kernel parameter at boot time.

The cross-process attacks require ASLR to be disabled by running `echo 0 | sudo tee /proc/sys/kernel/randomize_va_space`. Revert by rebooting or running `echo 2 | sudo tee /proc/sys/kernel/randomize_va_space`.

As the success of these attacks is heavily based on the behaviour of your particular machine, we can demonstrate on our own machine if requested.

## Compiling and running
Run `make` to build all files.

### Single-process
Run `./single-process`.

### Cross-process
Run `taskset -c 0 ./cross-process-attacker` to start the attacker on virtual core 0. Run `taskset -c 0 ./cross-process-victim` to start the victim on the same core.

### ASLR
With ASLR enabled, run `taskset -c 0 ./aslr-victim`.

Modify line 157 of aslr-attacker.cpp to contain the starting bits of the victim's reported virtual address. Run `make` to compile the attacker. Run ``setarch `uname -m` --addr-no-randomize /bin/bash`` to open a shell with ASLR disabled. Finally, run `taskset -c 0 ./aslr-attacker` to determine the remainder of the victim's virtual address.