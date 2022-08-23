# EncEdit

Encedit - a simple and elegant plain text editor: write down anything while secured by encrytion!

version 2.3, copywrite (c) 2021~2022 by xsy, GPL v3 LICENSE.

## Features

- Primitive encryption;
- unicode and UTF-8 support;
- cross platform - tested on Windows and Linux KDE desktop;
- keyboard friendly;
- automatic, customized index;
- customized font;
- customized background/foreground color;
- file history and custom settings memorized;
- simple window design - you can even hide window border;
- ultra-fast - faster than windows notepad in line wrapper mode on 10x scale;
- extremely low resource usage - usually about 20MB memory and less than 0.5% cpu time occupied;
- incremental encryption & save;
- and a lot more!

## Keyboard shotcuts

- Ctrl-H: show help on shotcuts;
- Ctrl-A: select all;
- Ctrl-C: copy;
- Ctrl-X: cut;
- Ctrl-V: paste;
- Ctrl-Z: undo;
- Ctrl-Shift-Z: redo;
- Ctrl-S: save current file;
- Ctrl-Shift-S: save as;
- Ctrl-N: new file;
- Ctrl-O: open file;
- Ctrl-F: select font family and size;
- Ctrl-L: select font color and background color;
- Ctrl-R: set index match rule (regular expression);
- Ctrl-T: toggle index (side widget) view;
- Ctrl-B: toggle frameless mode;
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

## Known issues

- On linux (and possibly also MacOS) when press Ctrl-B to make EncEdit frameless, it will disappear and reappear at the center of screen. This makes 摸鱼 difficult. It is hard to fix because it relies on different window management systems. However, personally it won't appear to be a big problem, since at work usually all of us use the Windows OS where it functions perfectly.
- When press Ctrl-L to choose colors for the text editor, if 'cancel' is pressed instead of 'ok', all colors will be set to black rather than kept as they are. **To be fixed in the next version**.
- Fonts in the sidebar (index widget) changes abnormally when changing fonts for the text editor. **To be fixed in the next version**.

## Change log

- v1.0: 2021.7.24 - basic encryption and UI
- v1.1: 2021.7.25 - dirty indication, keyboard shutcut and cursor history
- v1.2: 2021.7.28 - font size adjust and stronger encryption algorithm
- v1.3: 2021.8.4 - enhanced encryption (from 16bit to 64bit) and improve UI
- v2.0: 2022.1.9 - reorganize project with improved efficiency
- v2.1: 2022.8.8 - font & index title customize; change status file path to system default
- v2.2: 2022.8.12 - automatic window layout adjust; remove icons and menus for full keyboard control; customize font color
- v2.3: 2022.8.23 - improve docs; direct open mode w/o encryption; inline welcome message
