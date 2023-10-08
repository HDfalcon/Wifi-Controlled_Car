# Wifi-Controlled_Car
Project Overview: Lezgin ALIMOGLU(O7) and I developed a WiFi-controlled car by using two ESP32 WROOM boards for WiFi communication and one Arduino Uno board for analog reading purposes.

Why Arduino Uno was Used: We employed an extra Arduino Uno board due to limitations with the internal WiFi of the ESP32. Specifically, the ADC2 peripheral of the ESP32 cannot be used in conjunction with internal WiFi, and signal interference can occur with ADC1 peripherals. Therefore, we used an additional Arduino Uno board in remote controller.

WiFi Communication: For WiFi communication, we utilized the esp-now library. Esp-now proved to be very easy to understand and implement, making it an excellent choice for wireless communication projects.

Motor Driving: To handle the car's powerful motor, we opted for the HW-039 motor driver, capable of managing a maximum current of 20A and a maximum voltage of 27V.

Development Environment: We used platformio(I think it is the best choise for using arduino ide).
