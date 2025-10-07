# Double Pulse Generator
## General
Method for Measuring Switching Parameters The standard test method for measuring switching parameters of Si, SiC, and GaN MOSFETs and IGBTs is the double pulse Test (DPT). Double pulse testing can be used to measure energy loss during device turn-on and turn-off, as well as reverse recovery parameters. A good explanation can be found here: https://www.tek.com/en/documents/application-note/double-pulse-test-tektronix-afg31000-arbitrary-function-generator?anv=2
Program runs on an Raspberry Pi Zero, Zero 2 or 4B as bare metal program (tested on a Pi ZeroW V1.1). Minimum pulsewith = 0,3 micro second (using value 1 for width), resolution 70nsec. For example: sending JSON {"pulseWidth1": 10} via serial port gives a delay of 0,3 usec + (10\*0,07usec) = ~1usec. For the delay between pulstrains (pulseInterval) the starting delay is 1,3usec. So sending JSON {"pulseInterval": 10} gives 1,3 usec + (10\*0,07usec) = ~2usec.
Connect 
Sending a Json like: {"pulseInterval": 500, "pulseWidth1": 10, "interPulseDelay": 200, "pulseWidth2": 10} via the serial port (115200 baud, n, 8, 1) generates: <br />
>_______________&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ______________ <br />
>| pulseWidth1 | interPulseDelay | pulseWith2 | pulseInterval <br />
>&nbsp; &nbsp; &nbsp; &nbsp; 10&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ______ 200 ______ &nbsp; &nbsp; &nbsp; &nbsp; 10 &nbsp; &nbsp; &nbsp; &nbsp; _____ 500 _____ <br />
## Connections
Connect TXD to GPIO 14, 4th pin from the top, most right row. RXD to GPIO 15, 5th pin from the top, most right row. GND to pin 6, 3rd pin from the top, most right row. Output pulses to GPIO 18, , 6th pin from the top, most right row.  
![Raspberry Pi Zero Connector](Information/raspizero.jpg)

## Thanks to
Software is partly used from Teensy 4.0 Signal Generator, Electronics Workshop, Robin O'Reilly.
For JSON extraction thanks to https://github.com/zserge/jsmn/. 
For starting with the bare metal programming https://github.com/dwelch67/raspberrypi-zero.

## Generating code
Made on a Windows11 machine using:
make: The makefile is made to be used with https://www.gnu.org/software/make/.
compiler: The Compiler used is 'arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-arm-none-eabi.exe' downloaded from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
I had to include the install directories from both in my system variables PATH by hand. Do not know why, and did not spent the time to find out why.
After running 'make' copy kernel.img to an SD. Don't forget to copy bootcode.bin and start.elf. All in the root of the SD card.

## Modifying values via JSON
Included is a .exe file to run on a windows pc to generate a JSON (only to use with this DoublePulse_Tester). Source code can be found on https://github.com/ErikBakker100/JSONGenerator

## Building using CMAKE
To generate the build files via Cmake follow the following commands.
Run the following command from the root directory to configure the specific system:
`cmake -B <NAME> -G "Unix Makefiles" -DARCH=<VERSION> -DRPI_MODEL=<MODEL>`
Whereby:
\<NAME\> can be anything you want. F.e. “build_Z1_32bit”. This wil be used as the output directory where all of your build files will be located. The actual output (kernel.img or kernel8.img) can be found in the \<NAME\>/bin.
\<VERSION\> must be 'arm32' or 'arm64' depending if you want to use 32bit or 64bit version of the program.
\<MODEL\> must be 'Z1', 'Z2' or '4B' depending on the version raspberry pi you are using.

For example if you want to build for a raspberry pi Zero1, first use:
`cmake -B build_Z1_arm32 -G "Unix Makefiles" -DARCH=arm32 -DRPI_MODEL=Z1`
This will configure the build system, and generates the MAKE files in the <NAME> directory.
Now we can compile, do:
`cmake --build <NAME>`.
As an alternative you could also enter the \<NAME\> directory via `cd <NAME>` and run `make`.

If you want to see what is going on under the hood add `-- VERBOSE=1` to the command.
When the source file(s) have changed you need to run `cmake --build <NAME> again.
If a configuration has changed (f.e. New version compiler has been installed), you need to rerun the `cmake -B ….' command. But then first do 'rm -r <NAME>/*`
If you want debugging information add `-DCMAKE_BUILD_TYPE=Debug` to the `cmake -B …` command.

Now copy all files in the /sdcard directory to your SD_Card and insert in your Raspberry

## Updating Arm toolchain
Make sure your toolchain directory is in your 'PATH' systemvariable.
