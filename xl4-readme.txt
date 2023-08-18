Cloned from https://github.com/commaai/openpilot.git

Build instructions:

git clone git@gitlab.excelfore.com:external/comma.ai/openpilot.git
cd openpilot
git submodule update --init
tools/ubuntu_setup.sh
source ~/.bashrc
poetry shell
scons -u -j6

These instructions can also be found here:  https://github.com/commaai/openpilot/tree/master/tools
You can run the openpilot app with instructions here: https://github.com/commaai/openpilot/blob/master/tools/CTF.md

or do this:

In one terminal:
    cd openpilot
    tools/replay/replay --dcam --ecam --demo
In a 2nd terminal:
    cd openpilot
    selfdrive/ui/ui

Play captured can streams with
  cd openpilot
  tools/cabana/cabana --demo

The xl4 modified cabana will attempt to open vcan interfaces - one for each can bus.
The demo stream plays over 7 can buses
Setup vcan:
sudo modprobe vcan
for n in $(seq 0 6); do sudo ip link add dev vcan$n type vcan; sudo ip link set up vcan$n; done

Now run cabana and it should play the can streams through  vcan0 - vcan6
bus to vcan mapping is printed on the console
eg.
Opened canbus 2 on interface vcan0
Opened canbus 0 on interface vcan1
Opened canbus 1 on interface vcan2
Opened canbus 129 on interface vcan3
Opened canbus 128 on interface vcan4
Opened canbus 192 on interface vcan5
Opened canbus 130 on interface vcan6

Notes:
  to commit changes the Pre-commit hook requires pylint, flake8 and mypy to be installed
  tinygrad may still fail mypy ipynb_checks
  to disable the pre-commit hook, run: git commit --no-verify
