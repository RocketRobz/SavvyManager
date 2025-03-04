<p align="center">
 <img src="https://github.com/RocketRobz/SavvyManager/blob/master/resources/title.png"><br>
  <a href="https://gbatemp.net/threads/release-savvy-manager-girls-mode-style-savvy-style-boutique-save-editor.553576/">
   <img src="https://img.shields.io/badge/GBATemp-Thread-blue.svg">
  </a>
</p>
For Style Savvy's 10th Anniversary, a save editor is here!

## Features

* Edit save data from all four Girls Mode/Style Savvy/Style Boutique games.
* As of currently, only games 2-4 are supported.
   * Only the 2nd game is supported for KOR regions.
* High-resolution *Horizon HD* (800px wide mode), exclusive to 3DS, New 3DS, and New 2DS consoles!
* Pre-rendered character previews.
* Originally composed menu music!
* Sound effects from ChuChu Rocket.
* Play as most characters from all Girls Mode/Style Savvy/Style Boutique games, in all (except first) games!     (Shop customers and brand reps not included.)
* When importing a character from those games, you can press L or R to select the seasonal outfit.     (Note that not all characters will have seasonal outfits.)
* Add characters from the first game, GM2/Trendsetters/NSB, and Star Stylist/Styling Star, as extra contacts in Kirakira Code/Fashion Forward!
* Import any character from the Kirakira Code/Fashion Forward save data into Star Stylist/Styling Star, or vice-versa.
* You can also export your current character, in case if you want to restore/import it later.
* (3rd and 4th games) Import/Export brand emblems for custom designed outfits.
* Apply music packs to Girls Mode 2/Style Savvy: Trendsetters/New Style Boutique. (Requires game patching enabled in Luma.)
* (GM4/SS4/NSB3): Add, remove, and/or change characters in Mewtube videos!
* Add all fashion to your wardrobe and shop in the 3rd and 4th games!
* Change attributes of your character (such as gender (currently only for the 4th game), pose set, height, etc.).
* Coming soon (GM4/SS4/NSB3): Change bow placement on your character, if a bow is being worn.

Before using Savvy Manager, it is recommended to back up your Girls Mode/Style Savvy/Style Boutique game save(s) using Checkpoint, in case something goes wrong.

## Compilation

### Setting up your environment

To build Savvy Manager from source, you will need to install devkitARM and some libraries. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run the following command to install everything you need:
```
(sudo dkp-)pacman -S 3ds-dev
```
(The `sudo dkp-` may not be needed depending on your OS)

You will also need [bannertool](https://github.com/Steveice10/bannertool/releases/latest) and [makerom](https://github.com/profi200/Project_CTR/releases/latest) in your PATH.

### Cloning the repository

To download the source you will need to clone the repository with submodules, this can be done by running:
```
git clone --recursive https://github.com/RocketRobz/SavvyManager.git
```

If you've already cloned it, you can use the following command to update all of the submodules:
```
git submodule update --init --recursive
```

### Building

Once you've cloned the repository (with submodules), simply run `make` in the root of the repository.

## Credits
* devkitPro: libctru, citro2d/3d, nds-hb-menu's file/folder browsing code
* Universal Team: Universal Core
* Pixel Perfect: [Music icon](https://www.flaticon.com/free-icon/musical-note_727218)
* rog9001: His New Style Boutique 3 Save Editor tool has motivated me to work on this further.
