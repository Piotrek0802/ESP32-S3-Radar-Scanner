# 🛰️ ESP32-S3 Interactive Radar Scanner

ZAAWANSOWANY SYSTEM SKANOWANIA OTOCZENIA Z WYKORZYSTANIEM LASERA TOF (TIME-OF-FLIGHT) I INTERFEJSEM OLED.

Projekt zbudowany na potężnym mikrokontrolerze **ESP32-S3 (wersja N16R8)**. Urządzenie precyzyjnie mapuje odległość, korzystając z profesjonalnej architektury obiektowej (OOP) w języku C++. Całość oferuje ręczne sterowanie za pomocą filtrowanego sprzętowo i programowo joysticka analogowego.

---

## ✨ Główne Możliwości Systemu

* 🎯 **Precyzyjny Pomiar (VL53L0X):** Obsługa czujnika laserowego Time-of-Flight z systemem wykrywania błędów i przekroczenia czasu (Timeout).
* 🕹️ **Smart Joystick Controller:** Autorska klasa filtrująca sygnał z przetwornika ADC. Posiada wbudowaną logikę "Martwej Strefy" (Deadzone) oraz dociągania krawędzi (Saturation/Clamping) dla idealnie płynnego sterowania.
* 📺 **Dynamiczny Interfejs OLED:** System wizualny oparty o własną strukturę matematyczną `Point` i predefiniowane kotwice na ekranie (np. `DefaultPoints::Center`). Pozwala na szybkie i bezbłędne rysowanie radaru i danych.
* 🏗️ **Architektura OOP:** Kod w pełni zmodularyzowany. Logika poszczególnych komponentów sprzętowych (Ekran, Laser, Joystick) jest całkowicie odizolowana od głównej pętli programu.
* 🚀 **Optymalizacja Pamięci:** Projekt skonfigurowany pod architekturę OPI PSRAM (8MB) i 16MB Flash, gotowy na zaawansowane mapowanie 2D.

---

## 🛠️ Wykorzystany Sprzęt

* **MCU:** ESP32-S3 DevKitC-1 (N16R8)
* **Wizualizacja:** Ekran OLED 0.96" SSD1306 (I2C)
* **Sensor Odległości:** VL53L0X Time-of-Flight (I2C)
* **Sterowanie:** Moduł Joysticka Analogowego (X, Y, Button)
* **Sygnalizacja:** Wbudowana dioda RGB WS2812 (NeoPixel)

---

## 📂 Struktura Architektury (C++)

Projekt zbudowany jest w oparciu o obiektowy wzorzec projektowy:
* `mainController.hpp` - Mózg operacji. Zarządza testami diagnostycznymi i przepływem danych między modułami.
* `joystickController.hpp` - Klasa hermetyzująca surowe dane z ADC w czyste dane sterujące.
* `laserVL53.hpp` - Wrapper dla biblioteki Pololu, dostosowujący odczyty do logiki radaru.
* `display.hpp` & `point.h` - Zaawansowany silnik renderowania oparty o koordynaty $X,Y$.
* `status.h` - Podstawa pod przyszły, globalny system obsługi błędów i weryfikacji danych (Stale Data).

---

## ⚙️ Uruchomienie projektu (PlatformIO)

Projekt został stworzony w środowisku **PlatformIO** dla Visual Studio Code. Zarządzanie bibliotekami jest w pełni zautomatyzowane.

1. Sklonuj to repozytorium na swój dysk.
2. Otwórz folder projektu w **VS Code** (wymaga wtyczki PlatformIO).
3. Środowisko samo pobierze zależne biblioteki (zdefiniowane w `platformio.ini`):
   * *Adafruit NeoPixel*, *Adafruit GFX*, *Adafruit SSD1306*, *Pololu VL53L0X*.
4. Podłącz ESP32-S3 przez USB.
5. Kliknij **Build (✓)** a następnie **Upload (→)**.

---
*Projekt rozwijany pasjonacko w celu nauki zaawansowanego programowania systemów wbudowanych w C++.*
