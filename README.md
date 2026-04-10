# Zentop

A fast, lightweight terminal-based system monitor for Linux.

Zentop displays real-time CPU, memory, uptime/load, network, disk, and top processes in a clean, continuously refreshing dashboard — built from scratch in C++ using `/proc`.

![zentop demo](assets/demo.gif) -->

---

## Features

* Real-time terminal dashboard with in-place updates
* CPU and memory usage visualization
* Uptime and load average tracking
* Network and disk I/O stats (rates + totals)
* Process table with sorting (CPU / memory)
* Interactive keyboard controls
* Optional colorized output

---

## Requirements

* Linux (uses `/proc`)
* `g++` (C++20)
* `make`

---

## Quick Start

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y g++ make
make
./zentop
```

### Fedora

```bash
sudo dnf install -y gcc-c++ make
make
./zentop
```

### Arch Linux

```bash
sudo pacman -S --needed gcc make
make
./zentop
```

---

## Build

```bash
make
```
## Run

```bash
./zentop
```
## Install

### User (recommended)

```bash
make install-user
```

If needed:

```bash
export PATH="$HOME/.local/bin:$PATH"
```
### System-wide

```bash
sudo make install
```
## Uninstall

```bash
make uninstall-user
# or
sudo make uninstall
```

---

## Usage

```bash
zentop [options]
```

### Options

* `--interval <sec>` — refresh interval (default: 1)
* `--top <n>` — max process rows
* `--sort <cpu|mem>` — initial sort (default: cpu)
* `--reverse` — reverse sort order
* `--no-color` — disable colors
* `--help` — show help

---

## Keyboard Controls

* `q` — quit
* `c` — sort by CPU
* `m` — sort by memory
* `r` — reverse sort
* `+` — faster refresh
* `-` — slower refresh

---

## Notes

* Output adapts to terminal size
* Designed to be minimal, fast, and readable
* No external dependencies

---

## Troubleshooting

* `g++: command not found` → install compiler
* `make: command not found` → install make
* `Permission denied` → use `sudo make install`
* `zentop not found` → ensure `~/.local/bin` is in PATH
* Broken terminal → run `stty sane`
