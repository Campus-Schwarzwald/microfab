# Configuring the screen of the raspberry

# Setup

We want to pair our headless Raspberry Pi 4 with a 5" capacitive touchscreen (Waveshare; 800x480).

To display our GUI we need a browser. In our case we choose Chromium.

Steps:

- Generate Raspberry Pi OS Lite on a micro sd

- Change config.txt on the boot drive


    sudo nano /boot/config.txt

- Insert the following code


    disable_overscan=1
    hdmi_group=2
    hdmi_mode=87
    hdmi_cvt 800 480 60 6 0 0 0
    hdmi_drive=1
    dtoverlay=ads7846,cs=1,penirq=25,penirq_pull=2,speed=50000,keep_vref_on=0,swapxy=0,pmax=255,xohms=150,xmin=200,xmax=3900,ymin=200,ymax=3900

- Connect to your raspberry via ssh

    
    ssh pi@raspberrypi.local


## Browser configuration

- Update and install packages


    sudo apt-get update -qq

    sudo apt-get install --no-install-recommends xserver-xorg-video-all \
      xserver-xorg-input-all xserver-xorg-core xinit x11-xserver-utils \
      chromium-browser unclutter

- Enable console Autologin

    
    sudo raspi-config
        - System options --> Boot / Auto login --> Console Autologin

- Configure automatic GUI startup


    sudo nano /home/pi/.bash_profile

- Insert code


    if [ -z $DISPLAY ] && [ $(tty) = /dev/tty1 ]
    then
      startx
    fi

- Configure chromium to run when startx runs


    sudo nano /home/pi/.xinitrc

- Insert code

    
    #!/usr/bin/env sh
    xset -dpms
    xset s off
    xset s noblank
    
    unclutter &
    chromium-browser https://campus-schwarzwald.de \
      --window-size=840,400 \
      --window-position=0,0 \
      --start-fullscreen \
      --kiosk \
      --incognito \
      --noerrdialogs \
      --disable-translate \
      --no-first-run \
      --fast \
      --fast-start \
      --disable-infobars \
      --disable-features=TranslateUI \
      --disk-cache-dir=/dev/null \
      --overscroll-history-navigation=0 \
      --disable-pinch

- to start chromium type 


    startx
    (This only works locally on the Pi and not via SSH)

- To stop chromium press


    Alt + F4

    
    

## Touch screen configuration

- Load calibrator software

    
    sudo apt-get install xserver-xorg-input-evdev xinput-calibrator


- Copy the config and create a new .conf file


    sudo cp -rf /usr/share/X11/xorg.conf.d/10-evdev.conf /usr/share/X11/xorg.conf.d/45-evdev.conf
    sudo nano /usr/share/X11/xorg.conf.d/99-calibration.conf

- Insert the following code


    Section "InputClass"
        Identifier      "calibration"
        MatchProduct    "ADS7846 Touchscreen"
        Option  "Calibration"   "208 3905 288 3910"
        Option  "SwapAxes"      "0"
        Option "EmulateThirdButton" "1"
        Option "EmulateThirdButtonTimeout" "1000"
        Option "EmulateThirdButtonMoveThreshold" "300"
    EndSection

- reboot

    
    sudo reboot


Sources:

- https://blog.r0b.io/post/minimal-rpi-kiosk/
- https://www.waveshare.com/wiki/5inch_HDMI_LCD#Hardware_Connection