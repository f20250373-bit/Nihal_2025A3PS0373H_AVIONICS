Name: K Nihal Reddy
ID  : 2025A3PS0373H

TASK 2:
EXPLANATION:

In my code, I have taken the initial state to be ANCHOR_DROPPED, this way I can be assured that I am protected if my initial conditions(state) is a hazard, and can launch open my sails immediately after I press the button. The code follows all the instructions
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
