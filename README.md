# WaveSketch

Embedded software for an arbitrary function generator.

## Overview

The purpose of this project is to provide embedded software for the 
EtchaSound hardware. It implements audio output of an arbitrary 
function using an ISR to achieve a ~96kHz sample rate. It also 
provides non-volatile memory storage of up to four unique waveforms. 
Lastly, the software dynamically captures arbitrary analog signals to
generate dynamic waveforms in real-time.