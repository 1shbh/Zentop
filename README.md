# zentop

A lightweight terminal system monitor for Linux.

`zentop` shows live CPU, memory, uptime/load, network, disk, and top processes in a single refreshing dashboard.

## Features

- Live terminal dashboard with in-place refresh
- CPU and memory usage bars
- Uptime and load average
- Network and disk read/write rates with totals
- Process table with sorting controls
- Keyboard controls for sorting and refresh speed
- Optional colorized thresholds

## Requirements

- Linux (uses `/proc`)
- `g++` with C++20 support
- `make`

## Quick Start (Common Distros)

Install build tools, then build and run.

Ubuntu / Debian:

```bash
sudo apt update
sudo apt install -y g++ make
make
./zentop
```

Fedora:

```bash
sudo dnf install -y gcc-c++ make
make
./zentop
```

Arch Linux:

```bash
sudo pacman -S --needed gcc make
make
./zentop
```

## Build

```bash
make
```

This builds the `zentop` binary in the project root.

## Run

```bash
make run
```

Or directly:

```bash
./zentop
```

## Install

### User install (recommended)

```bash
make install-user
```

This installs `zentop` to `~/.local/bin/zentop`.

If `zentop` is not found after install, add this to your shell config:

```bash
export PATH="$HOME/.local/bin:$PATH"
```

### System-wide install

```bash
sudo make install
```

This installs `zentop` to `/usr/local/bin/zentop`.

### One-command setup

```bash
make setup
```

Alias for `make install-user`.

## Uninstall

```bash
make uninstall-user
# or
sudo make uninstall
```

## Usage

```bash
zentop [options]
```

### CLI options

- `--interval <sec>`: refresh interval in seconds (default: `1`)
- `--top <n>`: max process rows (default: auto-fit terminal)
- `--sort <cpu|mem>`: initial process sort (default: `cpu`)
- `--reverse`: reverse process sort order
- `--no-color`: disable ANSI colors
- `--help`: print help

## Keyboard Controls

- `q`: quit
- `c`: sort processes by CPU
- `m`: sort processes by memory
- `r`: reverse sort order
- `+`: refresh faster
- `-`: refresh slower

## Project Structure

```text
main.cpp
Makefile
core/
models/
monitors/
ui/
utils/
```

## Notes

- Terminal height affects how many process rows are displayed.
- Designed to stay simple and readable rather than overly complex.

## Troubleshooting

- `g++: command not found`: install your distro's C++ toolchain (see Quick Start).
- `make: command not found`: install `make`.
- `Permission denied` when installing globally: use `sudo make install`, or use `make install-user`.
- `zentop: command not found` after `install-user`: ensure `~/.local/bin` is in your `PATH`.
- Garbled terminal after crash/forced stop: run `stty sane`.

## Release Checklist

Before publishing to GitHub:

1. Build from clean state: `make clean && make`.
2. Verify CLI help: `./zentop --help`.
3. Smoke test runtime: `timeout 2s ./zentop --no-color`.
4. Confirm `.gitignore` excludes build artifacts.
5. Ensure `LICENSE` and `README.md` are present and up to date.
