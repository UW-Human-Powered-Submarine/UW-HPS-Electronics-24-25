#ifndef ___PRESSURE_SENSOR_H___
#define ___PRESSURE_SENSOR_H___

//  +-------------------------------------------------------------------------+
//  |   This is the pressure sensor class, which is wrapper to MS5873 lib     |
//  |       from BlueRobotics, and implement methods from Scheduler class     |
//  |       as a FSM. In this way, the pressure sensor can be easily          |
//  |       integrated with the rest of electronics system.                   |
//  |                                                                         |
//  |   The pressure sensor is Bar02 from BlueRobotics.                       |
//  |                                                                         |
//  |   DepthReading must be initialized before use by                      |
//  |       DepthReading.begin()                                            |
//  |   DepthReading values are buffered in the internal field, calling     |
//  |       DepthReading.refresh() to pull data from DepthReading and     |
//  |       update the field                                                  |
//  |   Calling DepthReading.update() at high frequency will automatically  |
//  |       update internal field at set interval.                            |
//  |                                                                         |
//  |   Pinout:                                                               |
//  |       +---------------+    +-----------------+    +---------------+     |
//  |       |               |    |   3.3v <=> 5V   |    |               |     |
//  |       |           VCC +----+  -------------  +----+ 5V            |     |
//  |       |           GND +----+  -------------  +----+ GND           |     |
//  |       |           SCL +----+  -------------  +----+ A5            |     |
//  |       | Bar02     SDA +----+  -------------  +----+ A4            |     |
//  |       | (MS5873)      |    |   Logic-Level   |    |               |     |
//  |       | Pressure      |    |     Shifter     |    |       Arduino |     |
//  |       | Sensor        |    +-----------------+    |   Uno or Nano |     |
//  |       +---------------+                           +---------------+     |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    March 4, 2025                                         |
//  |                                                                         |
//  +-------------------------------------------------------------------------+

#include "Arduino.h"
#include "Wire.h"
#include "Scheduler.h"
#include "MS5837_FSM.h"

#define DENSITY_FRESH_WATER_KGPM 997
#define DENSITY_SEA_WATER_KGPM 1029

class DepthReading: public Scheduler {
    public:
        DepthReading();
        DepthReading(unsigned long refresh_period_ms);
        ~DepthReading(); 

        void register_ms5873(MS5837_FSM *ms5873);

        float get_pressure_mbar() const;
        float get_depth_m() const;
        float get_temperature_c() const;
        float get_depth_calibration() const;
    
        void calibrate_depth_zero(float zero_depth_m);
        void calibrate_depth_zero_using_current_reading();

        void in_fresh_water();
        void in_sea_water();
        void fluid_density(float density_kgpm3);

    private:
        MS5837_FSM *sensor;
        bool is_depth_calibrated;
        float depth_zero_calibration;

        float pressure_mbar;
        float depth_m;
        float temperature_C;

        void event() override;
        void pull_sensor_data();
    };


#endif  //  ___PRESSURE_SENSOR_H___