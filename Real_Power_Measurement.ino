int decimalPrecision = 2;

        /* 1- AC Voltage Measurement */
        
        int VoltageAnalogInputPin = A2;                  
        float voltageSampleRead  = 0;                     
        float voltageLastSample  = 0;                     
        float voltageSampleSum   = 0;                     
        float voltageSampleCount = 0;                     
        float voltageMean ;                               
        float RMSVoltageMean ;                           


        /* 2- AC Current Measurement */
        int CurrentAnalogInputPin = A3;
        float mVperAmpValue = 100;
        float currentSampleRead  = 0;
        float currentLastSample  = 0;
        float currentSampleSum   = 0;
        float currentSampleCount = 0;
        float currentMean ;
        float RMSCurrentMean =0 ;
        float FinalRMSCurrent ;


        /* 3- AC Power Measurement */
        float sampleCurrent1 ;
        float sampleCurrent2 ;
        float sampleCurrent3 ;
        float apparentPower;
        float realPower = 0;
        float powerSampleRead  = 0; 
        float powerLastSample   = 0;
        float powerSampleCount  = 0;
        float powerSampleSum    = 0;
        float powerFactor = 0;


void setup()

{
        
        Serial.begin(11580);

}


void loop()
{
        /* 1- AC Voltage Measurement */
 
        if(millis() >= voltageLastSample + 1 )   
          {
            voltageSampleRead = 2*(analogRead(VoltageAnalogInputPin)- 512);
            voltageSampleSum = voltageSampleSum + sq(voltageSampleRead);
            voltageSampleCount = voltageSampleCount + 1;
            voltageLastSample = millis() ; 
          }
        
        if(voltageSampleCount == 1000)
          {           
            voltageMean = voltageSampleSum/voltageSampleCount; 
            RMSVoltageMean = sqrt(voltageMean);
            Serial.print("Voltage RMS: ");
            Serial.print(RMSVoltageMean,decimalPrecision);
            Serial.println("V  ");
            voltageSampleSum =0;
            voltageSampleCount=0;
          }


        /* 2- AC Current Measurement */
        
        if(millis() >= currentLastSample + 1) 
          {
            currentSampleRead = analogRead(CurrentAnalogInputPin)-512;
            currentSampleSum = currentSampleSum + sq(currentSampleRead) ;
            currentSampleCount = currentSampleCount + 1;
            currentLastSample = millis();
          }
        
        if(currentSampleCount == 1000)
          {       
            currentMean = currentSampleSum/currentSampleCount;
            RMSCurrentMean = sqrt(currentMean);
            FinalRMSCurrent = (((RMSCurrentMean /1024) *5000) /mVperAmpValue);
            Serial.print("Current RMS: ");
            Serial.print(FinalRMSCurrent,decimalPrecision);
            Serial.println(" A  ");
            currentSampleSum =0;
            currentSampleCount=0;
          }

       
        /* 3- AC Power with Direction */
        if(millis() >= powerLastSample + 1)
          {
            sampleCurrent1 = analogRead(CurrentAnalogInputPin)-512;
            sampleCurrent2 = (sampleCurrent1/1024)*5000;
            sampleCurrent3 = sampleCurrent2/mVperAmpValue;
            voltageSampleRead = 2*(analogRead(VoltageAnalogInputPin)- 512);
            powerSampleRead = voltageSampleRead * sampleCurrent3 ;
            powerSampleSum = powerSampleSum + powerSampleRead ;
            powerSampleCount = powerSampleCount + 1;
            powerLastSample = millis();
          }
        
        if(powerSampleCount == 1000) 
          {
            realPower = powerSampleSum/powerSampleCount ;
            Serial.print("Real Power (W): ");
            Serial.print(realPower);
            Serial.println(" W  ");           
            apparentPower= FinalRMSCurrent*RMSVoltageMean;
            Serial.print("Apparent Power (VA): ");
            Serial.print(apparentPower,decimalPrecision);
            Serial.println(" VA ");
            powerFactor = realPower/apparentPower;    
            if(powerFactor > 1 || powerFactor < 0)
            {
              powerFactor = 0;
            }
            Serial.print("Power Factor: ");
            Serial.println(powerFactor,decimalPrecision);  
            Serial.println(" ");                                           
            powerSampleSum =0;
            powerSampleCount=0;
          }
  
}