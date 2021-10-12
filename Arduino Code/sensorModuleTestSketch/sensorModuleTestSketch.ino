/* fs = 2000
  fn = 1000
  passband = 450 - 550
  stopbands = 400, 600

  b =

  Columns 1 through 8

     1.2653e-009     0.0000e+000   -13.9178e-009     0.0000e+000    69.5890e-009     0.0000e+000  -208.7671e-009     0.0000e+000

  Columns 9 through 16

   417.5341e-009     0.0000e+000  -584.5478e-009     0.0000e+000   584.5478e-009     0.0000e+000  -417.5341e-009     0.0000e+000

  Columns 17 through 23

   208.7671e-009     0.0000e+000   -69.5890e-009     0.0000e+000    13.9178e-009     0.0000e+000    -1.2653e-009

  a =

  Columns 1 through 8

     1.0000e+000    -2.5759e-015     8.5878e+000   -19.2768e-015    33.7389e+000   -64.0107e-015    80.0068e+000  -128.0069e-015

  Columns 9 through 16

   127.1912e+000  -169.2895e-015   142.2860e+000  -154.2044e-015   114.2574e+000   -97.7002e-015    65.8423e+000   -42.5592e-015

  Columns 17 through 23

    26.6775e+000   -12.2802e-015     7.2364e+000    -2.1250e-015     1.1825e+000  -167.8542e-018    88.1623e-003
*/
int sensorPin = A7;
int n = 23;
float x[23], y[23], yn;     // Space to hold previous samples and outputs
float a[] = {1, -2.57591457011099e-15, 8.58778445148720, -1.92768199348703e-1, 33.7388841955045, -6.40107369713551e-14, 80.0067850106190,
             -1.28006910885680e-13, 127.191241175059, -1.69289510101439e-13, 142.286046269950, -1.54204396607107e-13, 114.257368277778,
             -9.77001771404314e-14, 65.8423276980048, -4.25592194485789e-14, 26.6775146556656, -1.22801935675992e-14, 7.23635157824856,
             -2.12499926321021e-15, 1.18245158142118, -1.67854199488016e-16, 0.0881622858761985
            };
float b[] = {1.26525499974620e-09, 0, -1.39178049972082e-08, 0, 6.95890249860409e-08, 0, -2.08767074958123e-07, 0, 4.17534149916246e-07,
             0, -5.84547809882744e-07, 0, 5.84547809882744e-07, 0, -4.17534149916246e-07, 0, 2.08767074958123e-07, 0, -6.95890249860409e-08,
             0, 1.39178049972082e-08, 0, -1.26525499974620e-09
            };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  unsigned long t1;
  int i, count, val;
  float s;

  count = 0;
  while (1) {
    t1 = micros();
    
    // Apply a digital filter.
    for (i = 1; i < n; i++) {     // Shift samples
      x[n - i] = x[(n - 1) - i];
      y[n - i] = y[(n - 1) - i];
    }
    val = 1023 - analogRead(sensorPin);
    x[0] = val * (3.3 / 1023.0) - 1.65; // New input to filter
    yn = 0;
    for (i = 0; i < n; i++)
      yn = yn + b[i] * x[i];
    for (i = 1; i < n; i++)
      yn = yn - a[i] * y[i];
    y[0] = yn;                   // New output from filter

    s = abs(yn);               // Use filtered signal

    // Now we can use the signal for its intended purpose. Below we check and turn an
    // LED on if the beam has been interrupted for.

    if (s > 0.25)                 // Threshold voltage
      count++;
    else
      count--;
    if (count > 150) count = 150;
    if (count <= 0) count = 0;

    if (count > 135) // normal
      ;
    else // alerts
      ;

    // The filter was designed assuming a 2000 Hz sampling rate. This corresponds
    // to a sample every 500 us.

    while ((micros() - t1) < 500) // 1035 Was determined via experimentation
      ;
  }
}
