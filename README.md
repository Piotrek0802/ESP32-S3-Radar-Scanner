# 📡 ESP32-S3 Radar Scanner

Projekt autonomicznego skanera 2D opartego na mikrokontrolerze **ESP32-S3**. Urządzenie mapuje otoczenie za pomocą czujnika laserowego i wizualizuje dane w czasie rzeczywistym na ekranie OLED.

## 🚀 Główne Funkcje
* **Skanowanie 180°:** Precyzyjne sterowanie serwem SG90.
* **Pomiar Laserowy:** Wykorzystanie czujnika VL53L0X (ToF) dla wysokiej dokładności.
* **Wizualizacja OLED:** Dynamiczny interfejs graficzny (radar sweep) na ekranie 128x64.
* **OOP Design:** Kod napisany w czystym C++ z wykorzystaniem klas i struktur (Point, Display).

## 🛠 Sprzęt (Hardware)
* **MCU:** ESP32-S3 DevKit
* **Sensor:** VL53L0X (Laser Time-of-Flight)
* **Display:** OLED SSD1306 0.96" (I2C)
* **Servo:** TowerPro SG90
* **Zasilanie:** Kondensator 1000uF dla stabilizacji serwa.

## 💻 Technologie
* **Język:** C++ (standard C++17)
* **Środowisko:** PlatformIO / VS Code
* **Biblioteki:** * Adafruit GFX & SSD1306
  * ESP32Servo
  * VL53L0X (Pololu)

## 📂 Struktura Projektu
* `src/main.cpp` - Główna pętla programu.
* `src/display.hpp` - Klasa obsługująca interfejs graficzny.
* `include/point.h` - Definicja struktur danych.
