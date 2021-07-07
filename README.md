~~~
                        //////////////((((((((((((((((((((((((((((((((((((
                        //////////////((((((((((((((((((((((((((((((((((((
                        //////////////((((((((((((((((((((((((((((((((((((
                        //////////////((((((((((((((((((((((((((((((((((((
                        ///////////////(((//((((((((((((((((((((((((((((((
                        //////////////(((((%@@@@((((((((((((((((((((((((((
                        //////////////(((((%@@@@((((((((((((((((((((((((((
                        //////////////((((,%@@@@((((((((((((((((((((((((((
                        //////////////((((.%@@@@((((((((((((((((((((((((/(
                        ((((//////////(((*.%@@@@(((((((((((((((((((((((((/
                        ((((((((//////(((..%@@@@((((((((((((((((((((((((//
                        ((((((((((((//(((..%@@@@((((((((((((((((((((((////
                        ((((((((((((((((...%@@@@@@@@@@@@@@&((((((((((/////
                        (((((((((((((((#...,............*/((((((((((//////
                        (((((((((((((((((((((((((((((((((((((((((((///////
                        (((((((((((((((((((((((((((((((((((((((((/////////
                        ((((((((((((((((((((((((((((((((((((((((//////////
                        (((((((((((((((((((((((((((((((((((((((///////////
                        ((((((((((((((((((((((((((((((((((((((((((////////
                        ((((((((((((((((((((((((((((((((((((((((((((((////
~~~  
# Vantage
Linux port of lenova vantage software but in terminal 😃
- This is a TUI for lenova laptops with option for conservation mode in linux
- Alternate for lenova vantage in linux
- Feel free to use and contribute
# Build instructions:
~~~bash
mkdir build
cd build
cmake ..
make -j3
./vantage
~~~

# Installation

## Debian 
~~~
sudo apt install wget
wget https://github.com/gokulmaxi/vantage/releases/download/0.1/vantage-1.0.0-Linux.deb
sudo dpkg -i vantage-1.0.0-Linux.deb
sudo vantage
~~~
## Manually
~~~bash
mkdir build
cd build
cmake ..
sudo make install 
sudo vantage
~~~

# TODO

- add binary package to AUR
- add control for keyboard backlight
- add control power mode
- add more widgets in tui
- add control for switching optimus

## not in scope of project

- Gui (we are linux users 🥰)
- support for other laptops (this is specific for lenova's ideapad , thinkpad and legion series laptops)

