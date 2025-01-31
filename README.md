# Breakout_C

# For Windows:
Note: May need to use WSL or other terminals that can execute the command "make" and "sudo"
1. Pull the code repo
2. Using your preferred terminal, change your current directory to Breakout_C, which is the main folder of this repo.
3. In your terminal, key in make windows-release and press the Enter button. To prevent typo error, copy paste the command below:
```
make windows-release
```
4. The command is considered successful if the terminal returns ONLY your directory as the output
5. Once successful, change your directory to win_release. Again, to prevent typo error, copy paste the command below:
```
cd win_release
```
6. Copy paste the command below to download mingw-w64-x86-64-dev:
```
sudo apt-get install mingw-w64-x86-64-dev
```
Fill in your password if requested.

7. To run the game, copy paste the command:
```
./wwr.exe
```

# For Linux:
1. Pull the code repo
2. Using your preferred terminal, change your current directory to Breakout_C, which is the main folder of this repo.
4. Once successful, change your directory to linux_release. Again, to prevent typo error, copy paste the command :
```
cd linux_release
```
5. To run the game, copy paste the command below:
```
./wwr
```

Note: linux does have many differing distribution and its hard to keep track of all the libraries involved. So Just in case the current build does not work:

Ubuntu:
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libfreetype6-dev libcrypto-dev libwavpack-dev libssl-dev
```

Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel freetype-devel openssl-devel wavpack-devel
```

1. build the game:
```
make dynamic_release
```
2. navigate to linux_release and run the game:
```
```
./wwr
```


