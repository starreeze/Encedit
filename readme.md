# EncEdit

![](imgs/encedit.ico)

Encedit - a simple and elegant plain text editor: write down anything while secured by encryption!

Version 3.1, copyright (c) 2021~2023 by Starreeze, GPL v3 LICENSE.

## Features

- Primitive encryption;
- unicode and UTF-8 support;
- cross platform - tested on Windows and Linux KDE desktop;
- full keyboard support;
- automatic, customized index;
- customized font;
- customized background/foreground color;
- file history and custom settings memorized;
- simple window design;
- ultra-fast - faster than windows notepad in line wrapper mode on 10x scale;
- extremely low resource usage - usually about 20MB memory occupied;
- incremental encryption & save to avoid resource waste;
- automatic backup;
- and a lot more!

## Keyboard shortcuts

- Ctrl-E: about;
- Ctrl-\<NUM\>: open a recent file;
- Ctrl-H: show help on shortcuts;
- Ctrl-Alt-\<ARROW_KEY\>: adjust window position;
- Ctrl-A: select all;
- Ctrl-C: copy;
- Ctrl-X: cut;
- Ctrl-V: paste;
- Ctrl-Z: undo;
- Ctrl-Shift-Z/Ctrl-Y: redo;
- Ctrl-S: save current file;
- Ctrl-Shift-S: save as;
- Ctrl-N: new file;
- Ctrl-O: open file;
- Ctrl-F: select font family and size;
- Ctrl-L: select font color and background color;
- Ctrl-R: set index match rule (regular expression);
- Ctrl-T: toggle index (side widget) view;
- Ctrl-B: toggle frame-less mode;
- Ctrl-M: set autosave file size limit and interval;
- Ctrl-=: zoom in;
- Ctrl--: zoom out.

## Possible usage

### Keeping a diary

This function is what EncEdit is made for. The default title index match rule is designed for diary; see the left side of the image below:

![diary](imgs/diary.jpg)

Since texts are all encrypted, nobody can peek into you thoughts. But note that if you forget you password, all contents will be lost.

### 摸鱼

To enable this function, I further developed EncEdit from v2.0 to v2.3, while I was at a boring internship in an IT company.

In order to read novels when you're at work, follow the steps below:

1. open a .txt document without entering the password;
2. resizing and moving the window to a suitable position;
3. adjust font and background color so that it merges into the background;
4. press Ctrl-T and Ctrl-B to make it frameless.

![moyu](imgs/moyu.jpg)

Enjoy!

## Build

### Linux
In linux with fcitx input framework (e.g., Sougou Pinyin Input), Chinese input methods cannot be properly called. This is because `libfcitxplatforminputcontextplugin-qt6.so` is not shipped by default with the linux qt distribution. You need to compile beforehand.

A complete install script (including the required input plugin) for Debian-based distribution is provided in `./build-linux.sh`. Run it to build.

### Windows
You are recommended to install the qt distribution from its official website. Then use qtcreator to compile this project. No plugin is needed.

## Change log

- v1.0: 2021.7.24 - FIRST VERSION: basic encryption and UI
- v1.1: 2021.7.25 - **new feature**: dirty indication, keyboard shortcut and cursor history
- v1.2: 2021.7.28 - **new feature**: font size adjust; **optimization**: stronger encryption algorithm
- v1.3: 2021.8.4 - **optimization**: enhanced encryption (from 16bit to 64bit) and improve UI
- v2.0: 2022.1.9 - **optimization**: reorganize project with improved efficiency
- v2.1: 2022.8.8 - **new feature**: font & index title customize; **optimization**: change status file path to system default
- v2.2: 2022.8.12 - **new feature**: customize font color; **optimization**: automatic window layout adjust & remove icons and menus for full keyboard control
- v2.3: 2022.8.23 - **new feature**: direct open mode w/o encryption; **optimization**: improve docs & inline welcome message
- v2.4: 2022.9.9 - **bugfix**: 4+ bugs in v2.3; **new feature**: history list & manual window position minor adjust; **optimization**: improved custom config management
- v2.5: 2023.7.17 - **bugfix**: 3+ bugs in v2.4; **new feature**: zoom & autosave; **optimization**: sidebar adjustable & typos in doc
- v2.6: 2023.7.20 - **bugfix**: 2+ bugs in v2.5; **new feature**: statusbar; **optimization**: code reconstruction & typos in doc
- v3.0 [Unsafe version! Bug can cause data loss]: 2023.7.24 - **bugfix**: 1 bug in v2.6; **new feature**: about page; **optimization**: encryption updated to AES-128 for better security & updated to qt6
- v3.1: 2023.7.24 - **bugfix**: 2 fatal bugs in v3.0
