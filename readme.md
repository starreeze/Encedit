# EncEdit

![](imgs/encedit.ico)

Encedit - a simple and elegant plain text editor: write down anything while secured by encryption!

version 2.5, copyright (c) 2021~2023 by xsy, GPL v3 LICENSE.

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

## Change log

- v1.0: 2021.7.24 - basic encryption and UI
- v1.1: 2021.7.25 - dirty indication, keyboard shortcut and cursor history
- v1.2: 2021.7.28 - font size adjust and stronger encryption algorithm
- v1.3: 2021.8.4 - enhanced encryption (from 16bit to 64bit) and improve UI
- v2.0: 2022.1.9 - reorganize project with improved efficiency
- v2.1: 2022.8.8 - font & index title customize; change status file path to system default
- v2.2: 2022.8.12 - new feature: customize font color; optimization: automatic window layout adjust & remove icons and menus for full keyboard control
- v2.3: 2022.8.23 - new feature: direct open mode w/o encryption; optimization: improve docs & inline welcome message
- v2.4: 2022.9.9 - bugfix: 4+ bugs in v2.3; new feature: history list & manual window position minor adjust; optimization: improved custom config management
- v2.5: 2023.7.17 - bugfix: 3+ bugs in v2.4; new feature: zoom & autosave; optimization: sidebar adjustable & typos in doc
