/* 
 fc = 400
 fs = 2000
 [b,a] = butter(3,w,'high)
*/
int sensorPin = A7;
int n = 4;
float x[4], y[4], ynew;     // Space to hold previous samples and outputs
//float a[] = {1.0000,   -1.5906,    2.0838,   -1.5326,    0.8694,   -0.3192,   0.0821,   -0.0122,    0.0009};
//float b[] = {0.0293,   -0.2341,    0.8193,   -1.6386,    2.0483,   -1.6386, 0.8193,   -0.2341,    0.0293};
float a[] = {1.0000,   -0.5772,    0.4218,   -0.0563};
float b[] = {0.2569,   -0.7707,    0.7707,   -0.2569};
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

    s = abs(ynew);               // Use filtered signal
    Serial.println(ynew);

    //Serial.print("delay: ");
    //Serial.println(micros() - t1);
    //    Serial.println(val);
    //    delay(1000);
    //    // Now we can use the signal for its intended purpose. Below we check and turn an
    //    // LED on if the beam has been interrupted for.
    //
    if (s > 1.2)                 // Threshold voltage
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
