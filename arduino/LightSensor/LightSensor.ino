int led_out_pin = 9;
int light_in_pin = 0;
int score = 0;

double mean_value=0;
double TOLERANCE =10.0;
int start_pass=0;
int MIN_PAUSE = 500;
int MAX_WAIT = 1000;

void setup(){
  pinMode(led_out_pin,OUTPUT);
  digitalWrite(led_out_pin,HIGH);
  Serial.begin(9600);
 Serial.println("Start setup");
}

boolean first_run = true;
boolean ball_has_passed = true;
void loop(){
  if(first_run){     
    calibrate();    
    Serial.println("Setup Complete");
    first_run = false;   
  }
  int lightLevel = analogRead(light_in_pin);  
  if(checkScore(lightLevel)){
    
    //Avoid registering same score many times. Give the ball time to pass            
    if(ball_has_passed){     
      ball_has_passed = false;    
      score++;    
      Serial.println(score);   
  }
     
   
}
else{
      //The coast is all clear/ no more barrier between the sensor and the light
      //allow the program to register scores again in the next round.     
        ball_has_passed = true;       
}
  delay(100); 
  
}

void calibrate(){ 
  long sum =0;
  int i=0;
  for (i=0;i<3000;i++){
   int read_value= analogRead(light_in_pin);
    if(read_value<0){     
      i--;          
    }
    else{
     sum+= read_value;
    }  
     delay(1);
  } 
   mean_value = sum/i;
  Serial.println(mean_value);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}


boolean checkScore(double lightValue){
  double variance = lightValue - mean_value;  
  if(variance<0){
      variance *=-1;
     if(checkVariance(variance)){
       calibrate();
       return false;
     }     
      return false;
     
  }else{
   
     return checkVariance(variance);
  }
  
}

//Calculate the percentage variance
//Return true if the variance has grown
boolean checkVariance(double value){
  double percentage_variance =  (value /mean_value)*100;
    if(percentage_variance>TOLERANCE){    
      return true;      
    }else{
    return false;
    }
}

