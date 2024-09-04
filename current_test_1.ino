
  #define RefVal 5.0
 

//An OLED Display is required here
//use pin A0
#define Pin A0

// Take the average of 500 times

const int averageValue = 500;

long int sensorValue = 0;

float sensitivity = 1000.0 / 200.0; //1000mA per 200mV 


//float Vref = 244;
float Vref = 2275.39;

void setup() 
{
  Serial.begin(9600);
}
static float tempval;
void loop() 
{

  // Read the value 500 times:
  for(int i=0;i<20;i++)
  {

  for (int i = 0; i < averageValue; i++)
  {
    int temp;
    temp= analogRead(Pin);
    if(temp>sensorValue)
    {
        sensorValue=temp;
    }
    delayMicroseconds(40);
  }
  tempval+=sensorValue;
  }

  sensorValue=tempval/20.0;
  tempval=0;
  // The on-board ADC is 10-bits 
  // Different power supply will lead to different reference sources
  // example: 2^10 = 1024 -> 5V / 1024 ~= 4.88mV
  //          unitValue= 5.0 / 1024.0*1000 ;
  float unitValue= RefVal / 1024.0*1000 ;
  float voltage = unitValue * sensorValue; 

  //When no load,Vref=initialValue
  Serial.print("initialValue: ");
  Serial.print(voltage);
  Serial.println("mV"); 
  
  // Calculate the corresponding current
  float current = ((voltage - Vref) * sensitivity)*0.707;

  // Print display voltage (mV)
  // This voltage is the pin voltage corresponding to the current
  
  voltage = unitValue * sensorValue-Vref;
  Serial.print(voltage);
  Serial.println("mV");
  

  // Print display current (mA)
  Serial.print("current: ");
  Serial.print(current);
  Serial.println("mA");
   
  Serial.print("\n");


  // Reset the sensorValue for the next reading
  sensorValue = 0;

  // Read it once per second
  delay(1000);
}
