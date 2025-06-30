# Dead by Daylight External Overlay  |  [English](#english-introduction)

> Kernel‑mode physical memory read/write based external overlay
> **This project does NOT include the kernel driver**—only the external rendering component.

---

## 🔍 Project Overview

Dead by Daylight External Overlay is an external rendering tool for *Dead by Daylight*.
It uses a custom kernel driver to perform direct physical memory reads and writes to obtain in‑game entity data for rendering.
Rendering is built on the [noahware/external-overlay](https://github.com/noahware/external-overlay) framework.
For the latest offsets, please refer to [Encryqed/Dumper-7](https://github.com/Encryqed/Dumper-7).

---

## ⚙️ Features

| Feature          | Description                                         |
| ---------------- | --------------------------------------------------- |
| Match Overview   | Display real‑time game state and player information |
| Skill Icons      | Show equipped Survivor/Killer perks                 |
| Killer ESP       | Highlight and track Killer location                 |
| Position Markers | Mark Survivors and Killer positions on the map      |
| Distance Meter   | Calculate and display distance (meters)             |
| Exit Gates       | Mark exit gate locations                            |
| Totems           | Tag in‑game totem objects                           |
| Pallets          | Mark interactable pallet locations                  |
| Generators       | Track generator progress and locations              |

---

## 🚀 Quick Start

1. **Clone the repository**

   ```bash
   git clone https://github.com/YourUser/DbD-External-Overlay.git
   cd DbD-External-Overlay
   ```

2. **Prepare the kernel driver**

   > *This project provides only the external overlay code, no driver included.*
   > Please set up or use your own driver template, ensure it's properly signed and loaded:

   * Example driver: `YourKernelDriver.sys`
   * Driver API: custom `ReadPhysicalMemory` / `WritePhysicalMemory`

3. **Configure offsets**
   Keep offsets up to date by downloading and parsing:

   ```bash
   git clone https://github.com/Encryqed/Dumper-7.git
   # Generate latest offsets.json with Dumper-7 tool
   cp Dumper-7/offsets.json config/offsets.json
   ```




---


## ⚖️ Disclaimer

* This project is for educational and research purposes only.
* Using it may violate the game’s terms of service; use at your own risk.
* Please evaluate and assume all potential consequences.

---

## ❤️ Acknowledgements

* [noahware/external-overlay](https://github.com/noahware/external-overlay) — Rendering framework
* [Encryqed/Dumper-7](https://github.com/Encryqed/Dumper-7) — Real‑time offsets generator

---

