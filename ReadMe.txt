Name: K Nihal Reddy
ID  : 2025A3PS0373H

TASK 2:
EXPLANATION:
The system was built in Tinkercad using an Arduino, ultrasonic distance sensor, light sensor, LCD, push button, LED, and buzzer.
A state machine was implemented with the five required states: OPEN SEA, ANCHOR DROPPED, STORM, CHARYBDIS, and WRECKED.
The light sensor and ultrasonic sensor were used to detect STORM and CHARYBDIS conditions using the given thresholds.
millis() was used to track the required continuous five-second danger period without blocking the system, while the LED and buzzer provide the required warnings.
The current state is displayed on the LCD, with the anchor protecting the ship and resetting the danger timer when dropped.
The code follows all the instructions
given in the task pdf.

Link to Tinker Cad:

    
https://www.tinkercad.com/things/akPzZE1kZop-fantabulous-vihelmo/editel?returnTo=%2Fthings%2FakPzZE1kZop-fantabulous-vihelmo&sharecode=hE-4oPxnXZQ51xf8Z7B31_jGYgBqVteno3oWM_askLk

TASK 1:
The depth data was extracted from the provided CSV file using Pandas, with each sample assigned a time value based on the one-second sampling interval.
Invalid and corrupted sensor readings were identified and converted to missing values, while extreme outliers were detected using a rolling median approach.
The corrupted/outlier values were replaced using interpolation based on neighboring valid readings.
A moving average filter was then applied to reduce random sensor noise and produce smoother depth data.
Finally, Matplotlib was used to plot the depth-time graph and animate it by adding one data point every second.

(Honestly, all the code for task 1 was generated using AI with my inputs regarding the methods, since  I'm still at a learning stage of the pandas library)
