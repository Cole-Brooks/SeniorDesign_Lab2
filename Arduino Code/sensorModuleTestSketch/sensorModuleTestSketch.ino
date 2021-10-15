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
//int n = 23;
//float x[23], y[23], ynew;     // Space to hold previous samples and outputs
//float a[] = {1, -2.57591457011099e-15, 8.58778445148720, -1.92768199348703e-1, 33.7388841955045, -6.40107369713551e-14, 80.0067850106190,
//             -1.28006910885680e-13, 127.191241175059, -1.69289510101439e-13, 142.286046269950, -1.54204396607107e-13, 114.257368277778,
//             -9.77001771404314e-14, 65.8423276980048, -4.25592194485789e-14, 26.6775146556656, -1.22801935675992e-14, 7.23635157824856,
//             -2.12499926321021e-15, 1.18245158142118, -1.67854199488016e-16, 0.0881622858761985
//            };
//float b[] = {1.26525499974620e-09, 0, -1.39178049972082e-08, 0, 6.95890249860409e-08, 0, -2.08767074958123e-07, 0, 4.17534149916246e-07,
//             0, -5.84547809882744e-07, 0, 5.84547809882744e-07, 0, -4.17534149916246e-07, 0, 2.08767074958123e-07, 0, -6.95890249860409e-08,
//             0, 1.39178049972082e-08, 0, -1.26525499974620e-09
//            };
int n = 3;
float x[3], y[3], ynew;     // Space to hold previous samples and outputs
float a[] = {1.0000e+000,   -1.3272e+000, 876.9765e-003};
float b[] = {938.4882e-003, -1.3272e+000, 938.4882e-003};
void setup() {
  pinMode(4, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(4, HIGH);
  analogReference(AR_DEFAULT);
  Serial.begin(115200);
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV64 |    // Divide Clock by 512.
                   ADC_CTRLB_RESSEL_10BIT;         // 10 bits resolution as default
  for (int i = 0; i < n; i++)   //  Clear x and y arrays
    x[i] = y[i] = 0.0;
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
    x[0] = val * (3.3 / 1023.0); // New input to filter
    ynew = 0.0;
    for (i = 0; i < n; i++)
      ynew = ynew + b[i] * x[i];
    for (i = 1; i < n; i++)
      ynew = ynew - a[i] * y[i];
    y[0] = ynew;                   // New output from filter

    s = abs(x[0]);               // Use filtered signal
    Serial.println(ynew);

    //Serial.print("delay: ");
    //Serial.println(micros() - t1);
    //    Serial.println(val);
    //    delay(1000);
    //    // Now we can use the signal for its intended purpose. Below we check and turn an
    //    // LED on if the beam has been interrupted for.
    //
    if (s > 0.25)                 // Threshold voltage
      count++;
    else
      count--;
    if (count > 150) count = 150;
    if (count <= 0) count = 0;
    //
    if (count > 135) // normal
      digitalWrite(4, HIGH);
    else // alerts
      digitalWrite(4, LOW);
    //
    //    // The filter was designed assuming a 2000 Hz sampling rate. This corresponds
    //    // to a sample every 500 us.
    //
    while ((micros() - t1) < 489) // 1035 Was determined via experimentation
      ;
    //Serial.println(micros() - t1);
    //delay(100);
  }
}
