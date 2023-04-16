# Lettermorph

Lettermorph is a challenging word-themed game inspired by the likes of Wordle. The goal of Lettermorph is to connect one word that is given to another by changing only one letter at a time, with each change resulting in a valid English word. For example, if the target is BOT and the start is CAR, the player could go CAR to CAT to COT to BOT.

### Installation

The game can be downloaded in one of prepackaged releases, or alternatively built from source. Note that the packaged builds are only available for macOS. The build-system generator used is [premake5](https://github.com/premake/premake-core), which can be installed using a package manager such as [homebrew](https://brew.sh) or directly from the premake website for other platforms. This would enable the game to be built for Windows, Linux, or nearly any other platform supported by SDL. on macOS, or by downloading prebuilt binaries on other platforms. On macOS, static binaries for each of are dependencies are included in the repo. The repo also includes a macOS bundle in the bundle folder. To use, simply compile the executable (preferably in release or dist config), and move it to Lettermorph.app/Contents/macOS. The app can then be used as any other macOS app.

### Licensing

The project is licensed under the MIT license, which is permissive of most open-source usage, modification, or redistribution of the game, as long as credit is given to this project.

### Libraries

* **SDL3** - z-lib license (https://github.com/libsdl/SDL)
* **SDL3_image** - z-lib license (https://github.com/libsdl/SDL_image)
* **SDL3_ttf** - z-lib license (https://github.com/libsdl/SDL_ttf)
* **SDL3_mixer** - z-lib license (https://github.com/libsdl/SDL_mixer)

### Authors

* **James Dadabo** - Programming, Design

### Credits

* **Pixabay** - License free sounds and music
