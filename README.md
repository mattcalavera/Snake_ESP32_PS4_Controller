# Snake Game ESP32 PS4 Controller
(re)porting of snake on ESP32 (Heltec wifi lora) controlled by PS4 dualshock via bluetooth

Library to connect PS4 Controller via bluetooth from : https://github.com/aed3/PS4-esp32

--> To pair controller with ESP32 follow the instruction there ^^^^^^, and insert MAC BT address in code <--

Code for snake adapted from arduino version found in : https://wokwi.com/projects/296135008348799496

*) When you power up Heltec board, while waiting PS4 dualshock to connect :

![WaitConnect - dimensioni grandi - dimensioni medie](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/7e6dc6d2-9d98-411d-b291-5be8f4ca7325)

*) As soon as controller connects, led of controller become green and snake appears :

![Start_snake - dimensioni grandi - dimensioni medie](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/36e8dde8-ce18-434f-8251-e7ae84c99421)

*) While playing ... 

![PlayingGreen - dimensioni grandi - dimensioni medie](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/a44fd73c-730a-41ae-9d1a-de2b815bb465)

*) when you hit a wall, controlled vibrates and led become red :

![Red_Wall - dimensioni grandi - dimensioni medie](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/01c3563d-6629-45ae-a8cd-1b3f62eee348)

*) ... and an opportune icon appear :-)

![Dead - dimensioni grandi - dimensioni medie](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/72e0aa79-c975-42d1-b0ce-f854d4731c38)

Just press a directional button to restart (for NOW they are the only abilitated, I plan to try use the jogs)

![Keys](https://github.com/mattcalavera/Snake_ESP32_PS4_Controller/assets/3481862/1e222a11-fd3b-4138-8bac-155730044217)
