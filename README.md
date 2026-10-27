# VitaSurf experimental build for PS Vita

This repository contains a GitHub Actions workflow that builds the upstream VitaSurf source into a PS Vita VPK using the official VitaSDK container.

## Build it

1. Open the **Actions** tab.
2. Select **Build VitaSurf experimental VPK**.
3. Press **Run workflow** on the `main` branch.
4. Wait for the job to finish. If it succeeds, open the run and download the `VitaSurf-experimental-VPK` artifact.
5. Extract the downloaded ZIP; it contains `VitaSurf.vpk`.

The workflow checks out the public upstream project from https://github.com/Breezyslasher/VitaSurf and builds its current source. It does **not** currently add WebGL support or guarantee Pony Town compatibility.

## Turbo profile

The `turbo-kit/config/Choices` file is an experimental, conservative cache profile. Back up your existing `ux0:data/VitaSurf/Choices` before replacing it. Larger caches may help repeat visits but can also increase memory pressure.

The compatibility test at `turbo-kit/tests/pony-town-compatibility.html` reports which browser APIs are exposed. It does not contact Pony Town or send personal data.

## Important limitations

- This is a build of upstream VitaSurf plus a separate optional configuration/test kit, not a fully modern Chromium port.
- The current source inspected for this project does not implement WebGL. Pony Town requires WebGL for gameplay, so a successful build does not mean Pony Town will run.
- The workflow builds on GitHub's hosted runner; no VitaSDK installation is required on your PC.
- This repository is public. Do not upload private source, game assets, credentials, tokens, or other confidential material here.
