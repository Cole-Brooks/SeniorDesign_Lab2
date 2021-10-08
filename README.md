# Team CASH: Laboratory #2 ECE:4880 - Senior Design
Contributors: Cole Brooks, Hongyu Zeng, Ann Thomas, Steve Wasiswa

### Design Documentation
#### Hardware
10/7/21: 
Tested two different sensor modules that seemed viable. The 5V laser receiver sensor from Dallas Sensors was the first module, it performed quite well aside from when in contact with very bright light (a flashlight for example could trigger this sensor). Unfortunately that was not going to work for our purposes. Instead we opted to use the HW-201 Module from HiLetgo off of Amazon. This sensor is originally intended to be for obstacle avoidance, but it works on the concept of sensing IR light. This will work quite well for our application because it already has a physical filter to keep out all visible light.

This module will require some modification, though. First of all, we have to do something about the IR transmitter on board. We'll likely just need to cut that off, but I don't want to commit to that until more information is acquired. 
#### Software (maybe this section should be broken down into components? Not sure)

### Design Process and Experimentation:
#### Hardware
#### Software (maybe this section should be broken down into components? Not sure)

### Requirements
format = '| num | Desc |'

|Req # | Description | 
| ---- | ----------- | 
| 01 | Receiver must work at least as well as prototype receiver. Sepecifically under low-light conditions. It should not have a measurably worse rate of false positives or false negatives compared to the prototype |
| 02 | Receiver should work reliably in the presence of a 100 W incandescent light bulb and/or direct sunlight. A slight performance degradation compared to low light is acceptable |
| 03 | Each time the transmitted beam is interrupted, an SMS alert shall be sent to a cell phone. |
| 04 | Message format shall be as follows: "Critical safety event at HH:MM XX", where HH is the hour in range 00 to 12, MM is the minute in range 0 to 59, and XX is the time of day AM or PM. The phone number for receiving alerts should be hard coded into the system. |
| 05 | Receiver performs all functionality with existing prototype |
| 06 | Receiver shall not have an 'abnormally large size or weight, or other physical characteristics that prevent it from being a reasonable replacement for the prototype receiver' |

### Test Report
(Test syntax: "| Test Name | Requirement | Expected Result | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |" )
#### Hardware
| Test Name | Process | Expected Result | Actual Result | Pass/Fail | Date | Issue Written |
| --------- | ------- | --------------- | --------------| --------- | ---- | ------------- |
| Hardware Reliably Connects to Internet | Load Arduino with 'test', view test report. Inspect result for 'Hardware: connection reliability  | > 90 | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |
| Sensor component works out of box | Connect Vin pin from the light sensor to 3.3 or 5 volts. Connect ground pin from the light sensor to ground. Connect Out pin from sensor to a digital input pin on the arduino. View that when you put an object (your hand) near the black receiver diode that the red LED on the sensor's circuit board turns on  | Light should turn on when object is present, and turn off when it moves away. | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |
| Case Fit Test | All components should fit into the case neatly.  | There should be no visible wires, and the case should close easily | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |
| Case Drop Test | With all components stored in the case, the system should not fail if dropped from a reasonable bench height. Drop the system from desk and view results  | System should be unaffected by drop | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |


#### Software (maybe this section should be broken down into components? Not sure)
| Test Name | Process | Expected Result | Actual Result | Pass/Fail | Date | Issue Written |
| --------- | ------- | --------------- | --------------| --------- | ---- | ------------- |
| Test Name | Process | Expected Result | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |

#### System
| Test Name | Requirement | Expected Result | Actual Result | Pass/Fail | Date | Issue Written |
| --------- | ----------- | --------------- | --------------| --------- | ---- | ------------- |
| Overall Usability |  | Expected Result | Actual Result | Pass/Fail | Date | Corrective Action (write an issue for this) |

### Project Retrospective
#### What Went Well
#### What could have gone better
#### Summary

### Appendix & References
