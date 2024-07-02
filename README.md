# Temperature Sensor Project with Arduino and Google Sheets

This repository contains a small Arduino project that reads temperature data from a DS18B20 sensor and saves it to a Google Sheets spreadsheet, eliminating the need for additional infrastructure.

## Description

The project uses a NodeMCU to read temperature data from a DS18B20 sensor and sends this information to a Google Sheets spreadsheet via HTTP request. This allows you to store and visualize the data without the need for an additional server.

## Requirements

- NodeMCU (ESP8266)
- DS18B20 Sensor
- Appropriate wiring
- Google account with access to Google Sheets

**Set up Google Sheets:**
    - Create a new spreadsheet in Google Sheets.
    - Set up a script in Google Sheets to receive the data (see `GoogleScript.gs` in this repository).
    - Based on this [video](https://www.youtube.com/watch?v=Lf2f1HxzQI0), create the App Script to record temperatures read by the sensor.
## License

This project is licensed under the GNU General Public License. See the `LICENSE` file for details.
