#define SensorPin 0
#define TempPin 1
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
int Temperature=0;
 
void setup()
{
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
}
void loop()
{
  
  //temperature = readTemperature();
  //Temperature=analogRead(TempPin);
  
  
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=2.9*phValue;                      //convert the millivolt into pH value
  
  
  
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  Serial.print("    Temp:");  
  //Serial.print(Temperature);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 
 
}