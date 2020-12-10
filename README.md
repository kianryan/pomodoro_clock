## Introduction

Pomodoro Clock for AtTiny84 and MAX7219.

## Prerequisites

Arduino Cli

```
choco install arduino-cli
```

Install ATTinyCore Core

```
arduino-cli core install ATTinyCore:avr
```


## Building

* Arduino CLI
* USBAsp

Compiles, generates build asset in /build and uploads via ASP.

```
arduino-cli compile --fqbn ATTinyCore:avr:attinyx4; arduino-cli upload --fqbn ATTinyCore:avr:attinyx4 -P usbasp
```

## Board and Schematics

Eagle designs are available in /eagle