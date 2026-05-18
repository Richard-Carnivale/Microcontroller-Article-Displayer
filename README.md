# Article Search & Displayer

> A handheld embedded news browser — no phone or computer required.

**Demo:** [youtu.be/z_X3cQ4DOZ4](https://youtu.be/z_X3cQ4DOZ4)

An embedded news browser built for EEL 4745C (Microprocessors 2). The system pairs a **TI TM4C123 (TIVA)** microcontroller with a **BeagleBone Black** to deliver real-time news headlines and article summaries directly to a small display — using only physical controls and a custom RTOS.

---

## How It Works

- The **BeagleBone Black** runs a Python script that listens over UART for a news category and optional keyword sent from the TIVA. It queries [NewsAPI.org](https://newsapi.org), fetches the top 5 matching articles, and streams headlines and descriptions back to the TIVA.
- The **TM4C123** handles everything on the user side — UI rendering, joystick navigation, button input, and article display — running on a custom preemptive RTOS (**G8RTOS**) with priority-based scheduling, semaphores, periodic/aperiodic threads, and FIFO-based IPC.

---

## Features

* Browse news by category from an on-device selection screen
* Refine results with a keyword entered over UART terminal
* Scroll through article titles and toggle to full descriptions via joystick press
* Hardware button support with interrupt-driven debouncing
* Full system reset back to the main screen (SW4)

---

## System Architecture

| Component | Role |
| ------------- |:-------------|
| TM4C123 | UI, scheduling, display, UART communication |
| BeagleBone Black | News API queries, article parsing, UART relay |
| G8RTOS | Custom RTOS — priority scheduler, semaphores, FIFO IPC |
| NewsAPI.org | Live article source |

### TIVA Thread Structure

**Background Threads**

| Thread | Description |
| ------------- |:-------------|
| `Idle_Thread` | Does nothing — lowest priority keepalive |
| `Beginning_Screen_Thread` | Welcome screen, joystick category selection, UART send |
| `Query_Screen_Thread` | Keyword entry screen, sends keyword to BeagleBone via UART4 |
| `Choose_Article_Thread` | Receives/stores article titles, handles scroll and description toggle |
| `Read_Buttons` | Debounces Multimod buttons, handles next/previous/reset |

**Periodic Threads**

| Thread | Period | Description |
| ------------- |:-------------:|:-------------|
| `Get_Joystick` | 50 ms | Samples joystick X-position, writes to FIFO |

**Aperiodic / Interrupt-Driven**

| Handler | Trigger |
| ------------- |:-------------|
| `GPIOE_Handler` | Multimod button press — signals debounce semaphore |
| `GPIOD_Handler` | Joystick press — signals joystick debounce semaphore |

---

## Built With

* **C** — TM4C123 firmware and G8RTOS
* **Python** — BeagleBone API handler
* TI TivaWare / DriverLib
* ST7789 display driver (via Multimod board)

---

## Usage

1. Flash the TM4C123 with the firmware in this repo.
2. Run the Python script on the BeagleBone Black and connect via UART.
3. Use the joystick to select a news category on the welcome screen.
4. Optionally enter a keyword via the UART terminal on the query screen.
5. Scroll through returned article titles with the buttons; press the joystick to toggle between title and description views.
6. Press **SW4** at any time to reset to the main screen.
