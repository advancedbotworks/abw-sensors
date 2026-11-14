# Third-party notices

Advanced Bot Works Sensors is licensed under the **GNU General Public License v3.0 or later** (see [LICENSE](LICENSE)). It includes, or depends on, the third-party work listed below. Each keeps its own copyright and licence.

## MyOwnBricks (GPL-3.0-or-later): included, modified

- **Source:** https://github.com/ysard/MyOwnBricks
- **Copyright:** (C) 2021-2023 Ysard, based on the original work of Ahmed Jouirou
- **Files:** `src/core/BaseSensor.h`, `src/core/BaseSensor.cpp`, `src/core/ColourSensor.h`, `src/core/ColourSensor.cpp`
- **Changes:** Aparaj Bogahawatta modified these files for Advanced Bot Works (2024-2026). The changes adapt them to emulate a LEGO SPIKE Prime colour sensor on Advanced Bot Works hardware and rename the files and classes. Each file's header says so, and the git history has the details.

MyOwnBricks is licensed under the GPL-3.0-or-later. Because this library is built on it, this library is also distributed under the GPL-3.0-or-later.

## Pybricks (MIT): included, unmodified

- **Source:** https://github.com/pybricks/pybricks-micropython (via MyOwnBricks)
- **File:** `src/core/lego_uart.h`

```
MIT License

Copyright (c) 2019-2020 The Pybricks Authors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Pololu VL53L1X library (BSD-3-Clause): dependency, not included

- **Source:** https://github.com/pololu/vl53l1x-arduino
- **Copyright:** (c) 2017 STMicroelectronics; (c) 2018-2022 Pololu Corporation
- **Used by:** `examples/LaserArrayFirmware`

This library isn't copied into this repository. It's declared as a dependency in `library.properties`, so the Arduino Library Manager installs it alongside this library, together with its own licence.

**Firmware binaries:** the Laser Distance Sensor Array's firmware includes compiled code from this library. The BSD-3-Clause licence requires its copyright notice and licence text to be reproduced with any binary distribution. The full text is in that library's `LICENSE.txt`, and a copy is included below.

```
Copyright (c) 2017, STMicroelectronics
Copyright (c) 2018-2022, Pololu Corporation
All Rights Reserved

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
