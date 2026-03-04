# ootmOS (One Of The Many OS)

`ootmOS` is a small, vibe‑coded hobby operating system.

- **Boot**: Legacy BIOS, GRUB, Multiboot
- **Arch**: i386 (32‑bit protected mode for now)
- **Kernel**: freestanding C + a little assembly

## Building (Ubuntu WSL)

Install dependencies:

```bash
sudo apt update
sudo apt install -y build-essential gcc-multilib \
  grub-pc-bin xorriso mtools qemu-system-x86
```

Then from the project root:

```bash
make iso
make run
```

This will:

- Build the kernel to `build/ootmOS.elf`
- Package a bootable ISO as `ootmOS.iso`
- Boot it with QEMU (`make run`)

## Current boot experience

Right now `ootmOS`:

- Boots via GRUB in legacy BIOS mode
- Drops into a deep‑blue text screen
- Shows a small series of boot log lines at the top
- Renders a centered `ootmOS` banner and tagline

## Roadmap (rough)

- Parse Multiboot info (memory map, modules, cmdline)
- Set up GDT/IDT and basic exception handlers
- Keyboard input and a tiny shell / command line
- Eventually: 64‑bit and UEFI boot path

