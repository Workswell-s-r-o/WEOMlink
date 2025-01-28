# WEOMlink demo for ESP32-S2-Kaluga-Kit
This project demonstrates usage of WEOMlink library on ESP32 platform.

## Project setup
- Install [Docker](https://www.docker.com/)
- On Windows you need to take few extra steps:
    - Open powershell
    - Instal [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
        ```powershell
        wsl --install
        wsl --update
        wsl --install --distribution　Ubuntu
        wsl -s Ubuntu
        ```
    - Install [USBIPD](https://github.com/dorssel/usbipd-win):
        ```powershell
        winget update
        winget install usbipd
        ```
    - Restart powershell
    - Pass USB device to WSL:
        - `usbipd list` and look for `USB Serial Converter A, USB Serial Converter B`
            ```
            BUSID  VID:PID    DEVICE                                          STATE
            1-3    0403:6010  USB Serial Converter A, USB Serial Converter B  Not shared
            1-9    046d:c31c  USB Input Device                                Not shared
            1-10   046d:c092  USB Input Device                                Not shared
            ```
        - `usbipd bind --busid <BUSID>`
        - `usbipd attach --wsl --busid <BUSID>` (needs to be redone after reboot/unplugging)
        - check in WSL/container terminal with `dmesg | tail`
            ```
            [  346.183365] usb 1-1: Product: Dual RS232-HS
            [  346.183365] usb 1-1: Manufacturer: FTDI
            [  346.193755] usbcore: registered new interface driver ftdi_sio
            [  346.193761] usbserial: USB Serial support registered for FTDI USB Serial Device
            [  346.193768] ftdi_sio 1-1:1.0: FTDI USB Serial Device converter detected
            [  346.193777] usb 1-1: Detected FT2232H
            [  346.194287] usb 1-1: FTDI USB Serial Device converter now attached to ttyUSB0
            [  346.194323] ftdi_sio 1-1:1.1: FTDI USB Serial Device converter detected
            [  346.194332] usb 1-1: Detected FT2232H
            [  346.194734] usb 1-1: FTDI USB Serial Device converter now attached to ttyUSB1
            ```
- Install [Visual Studio Code](https://code.visualstudio.com/) 
- Open Visutal Studio Code and install extension `Dev Containers`
- Open this project's folder in Visual Studio Code
- `Ctrl + Shift + P` -> `Dev Containers: Reopen in Container` and wait for container to build and attach
- Alternatively, you can use `Ctrl + Shift + P` -> `Dev Containers: Open folder in Container...`

#### Troubleshooting
[ESP docker container setup guide](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/using-docker-container.md)


## Usefull Visual Studio Code shortcuts
- ESP:
    - Build `Ctrl + E, B`
    - Flash `Ctrl + E, F`
    - Clean `Ctrl + E, X`
    - Monitor `Ctrl + E, M`
    - Build, Flash and Monitor `Ctrl + E, D`
    - Select flash port `Ctrl + E, P`
- Terminal
    - Stop `Ctrl + T, X`
