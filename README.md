# PSP Platform — Signal Collector Demo v0.2

Small native PSP homebrew test game. **Source code, not a prebuilt EBOOT**. It demonstrates PSP input, screen output, game loop, timing, and a score counter. It does **not** install or replace firmware.

## Game
- `X`: start/restart
- D-pad: move `@` to collect `*`
- `O`: return to menu
- `START`: quit
- Collect as many signals as possible in 45 seconds.

## Easiest path on Windows 11 (GitHub Desktop)
1. Extract the ZIP to Desktop. Open GitHub Desktop -> File -> Add local repository -> Choose the extracted `psp-platform-demo` folder. If prompted, click **Create a repository here**.
2. Commit all files and **Publish repository** (choose **Keep this code private** if desired).
3. On GitHub website open **Actions** -> **Build PSP EBOOT**. A push to `main`/`master` should start the workflow automatically; otherwise choose **Run workflow**.
4. After a successful run, download artifact **PSP-Platform-Demo-EBOOT**. Unzip it. You will get `PSP/GAME/PSPPLATFORM/EBOOT.PBP`.
5. Copy the **PSP** folder into the **root of the Memory Stick**, merging with any existing `PSP` folder. Never format the card without backing it up.
6. In PPSSPP choose **File -> Load**, and select the resulting `EBOOT.PBP`. On a real PSP, use a compatible homebrew-capable firmware and launch it from Game > Memory Stick.

## Local build (optional)
With PSPDEV/PSPSDK installed and on PATH, run `make`. Output is `EBOOT.PBP`.

## Important limitations
- This demo uses the PSPSDK debug-screen text renderer, not a polished graphical launcher.
- GitHub Actions has **not been executed or verified** as part of this package; if the PSPDEV container image or SDK changes, the workflow may need adjustment.
- PPSSPP and physical PSP behavior have **not been tested** in this environment.
- PSP-1000 currently failing to boot must be repaired before testing on hardware. Copying files to Memory Stick will not fix a boot fault.
- This project includes no Sony firmware or copyrighted game assets.
