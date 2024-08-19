# Puzzle Box

This puzzle box was created for my brother's bachelor party. The box displays a basic map on the LEDs on top, using a Teensy 4.1 microcontroller and an Arduino-compatible GPS receiver. 
The map updates in real-time, and helped navigate to different challenges that my brother had to complete to earn the codes to each of the different locks on the side of the box. The box cannot open until all 8 locks have been removed.
This project took about a month to develop, and it was made entirely from scratch aside from using some Arduino libraries to control the LEDs and interface with the GPS unit.

## Materials Used
- [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)
- [NEO-6M GPS Receiver](https://a.co/d/7m8FWfx)
- [WS2812B Individually Addressable LED Rings](https://a.co/d/0RZDFQk)
- 4x [Brinks Luggage Padlock (2-Pack)](https://www.walmart.com/ip/Brinks-1-4in-32mm-3-Dial-Resettable-TSA-Luggage-Padlock-with-a-Steel-Cable-Shackle-2-Pack/5020233202?wmlspartner=wlpa&selectedSellerId=0&wl13=2325&gclsrc=aw.ds&adid=222222222775020233202_117755028669_12420145346&wl0=&wl1=g&wl2=c&wl3=501107745824&wl4=pla-394283752452&wl5=9033456&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=5020233202&veh=sem_LIA&gclsrc=aw.ds&gad_source=1&gclid=CjwKCAjwzIK1BhAuEiwAHQmU3i-WarAhwdahmAcczhaaN_05SU1wwxQ0LPyhY8sm47aON8RbduxusxoCHnMQAvD_BwE)
- All of the 3D Printed parts were in [Bambu Labs PLA Matte](https://us.store.bambulab.com/collections/bambu-lab-3d-printer-filament/products/pla-matte-filament?variant=40489681944712)

## Video Demo

https://github.com/user-attachments/assets/07382008-efad-4c62-b222-95e3310d10c2

## Example Usage

1. First, the device is powered on. Since the power switch is on the inside, this needs to be done before the locks are reinstalled.

2. Eight locks are installed on the exterior of the box, keeping it closed.

3. The GPS unit will look for a GPS signal. While the GPS is searching, the LEDs display a sweeping animation.

4. The map will update with the GPS data, showing the user's location in the center and a single point for the first objective the user must navigate to. For example, if the user was located in Seattle, WA and the first point was in San Francisco, CA the map would show the next point at the bottom of the display, indicating that the user needs to travel south.

5. The user then navigates to the point using the map. The map will continue to update as the user's location changes, displaying the relative location of the current objective, and getting closer to the center as the user approaches the destination.

6. Once the user successfully navigates to the objective, the LEDs will display a pulsing animation indicating that the user has arrived. This animation will last 3 minutes before updating again. This gives the user time to complete a task to find a code for a lock.

7. The display will then show the next objective. The user must repeat steps 4-6 for each objective.

8. Once the last objective is reached, a final rainbow animation will play indefinitely, indicating there are no more objectives to find. The locks may be removed from the puzzle box and the device can be shut down.

## Features

- Live location updates
- Custom animations
- Customizable amount of locations
- Battery Powered

## Technology Used

![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white) 
![Teensy 4.1](https://img.shields.io/badge/Teensy_4.1-000080?style=for-the-badge)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Bambu Labs](https://img.shields.io/badge/Bambu_Labs-00AE42?style=for-the-badge&logo=bambulab&logoColor=white)
![Fusion 360](https://img.shields.io/badge/Fusion_360-FF7008?style=for-the-badge)

