# Tetris

A console remake of the legendary Russian game! I used pretty basic, yet robust tools of C++ to create it.

### Features

This project is not just a bare Tetris game, but also it includes several other features, important for any C++ developer: writing leaderboard into file, masterful work with console and ticks manipulation.

## Deployment

Unfortunately, this game isn't cross-platform and uses resources of windows.h. Anyway, for Windows user the install procedure will be a gravy train!

All you've got to do is to write the following command in your git console:

    $ git clone https://github.com/Danilx8/Tetris.git
    
## Game overview

You can manipulate the figures, using WASD. The goal is extremely simple and, in case you would like to customize your gaming experience, the constants "FIELD_HEIGHT" and "FIELD_WIDTH" are easily adjustable.

The game uses ticks to count the time figures go down, increasing it every line you close, whic makes it harder to play with every your success. All the results are shown in the lose screen or in the main menu.

In addition to basic Tetris figures the following ones were added:

![Tetris figures](https://user-images.githubusercontent.com/108181060/232781621-0edf5ddb-4893-45f3-859f-b4bfcf934dea.png)
