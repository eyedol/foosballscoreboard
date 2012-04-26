/*
Test code for motion detection.

Made up of two photo resistors connected to Analog pin A1, A0

Works in a way that when the light breaks, a score is recorded.
*/
  
/*
===================================
Light sensor pins for team A and B
===================================*/

int light_in_pin_A = 1; //Analog pin A1
int light_in_pin_B = 0; //Analog pin A0

/*
=============================
 Track scores for both teams 
=============================*/
int score_A = 0;
int score_B = 0;

/*
==============================
Light sensor calibration values
==============================*/
double mean_value_A=0;
double mean_value_B=0;


double TOLERANCE =10.0;
int start_pass=0;

int MIN_PAUSE = 500;
int MAX_WAIT = 1000;

/*

*/
void setup(){
  pinMode(light_in_pin_A,INPUT);
   pinMode(light_in_pin_B,INPUT);
  Serial.begin(9600);
  Serial.println("Start setup");
}

boolean first_run = true;

//Track the ball between the time the light was broken and the light restored.
boolean ball_has_passed_A = true;
boolean ball_has_passed_B = true;

void loop(){
  if(first_run){     
    calibrate();    
    Serial.println("Setup Complete");
    first_run = false;   
  }
  monitor_team_a();
  monitor_team_b();
}

/*
* Monitor if the ball has been detected on team A goal.
*/
void monitor_team_a(){
  int lightLevel_A = analogRead(light_in_pin_A);  

  if(checkScore(lightLevel_A,mean_value_B)){

    //Avoid registering same score many times. Give the ball time to pass            
    if(ball_has_passed_A){     
      ball_has_passed_A = false;    
      score_A++;    
      Serial.println(score_A);   
    }


  }
  else{
    //The coast is all clear/ no more barrier between the sensor and the light
    //allow the program to register scores again in the next round.     
    ball_has_passed_A = true;       
  }
  delay(100); 

}

/*
* Monitor if the ball has been detected on team B goal.
*/
void monitor_team_b(){
  int lightLevel_B = analogRead(light_in_pin_B);  
  if(checkScore(lightLevel_B,mean_value_B)){

    //Avoid registering same score many times. Give the ball time to pass            
    if(ball_has_passed_B){     
      ball_has_passed_B = false;    
      score_B ++;    
      Serial.println(score_B );   
    }


  }
  else{
    //The coast is all clear/ no more barrier between the sensor and the light
    //allow the program to register scores again in the next round.     
    ball_has_passed_B = true;       
  }
  delay(100); 

}



/*
* Calibrate the sensors to determine normal light levels in the room.
*/
void calibrate(){ 
  long sum_A =0;
  long sum_B =0;
  int i=0;

  //Calibrate the first Light sensor
  for (i=0;i<3000;i++){
    int read_value= analogRead(light_in_pin_A);   
    if(read_value<0){     
      i--;          
    }
    else{
      sum_A+= read_value;
    }  
    delay(1);
  } 
  mean_value_A = sum_A/i;
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("Sensor A:");
  Serial.println(mean_value_A);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

  //Calibrate the next Light sensor
  for (i=0;i<3000;i++){
    int read_value= analogRead(light_in_pin_A);   
    if(read_value<0){     
      i--;          
    }
    else{
      sum_B+= read_value;
    }  
    delay(1);
  } 


  mean_value_B = sum_B/i;
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("Sensor B:");
  Serial.println(mean_value_B);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

/*
* detemine if the change in light intensity is enough to declare score.
*/
boolean checkScore(double lightValue,double mean){
  double variance = lightValue - mean;  
  if(variance<0){
    variance *=-1;
    if(checkVariance(variance,mean)){
      calibrate();
      return false;
    }     
    return false;

  }
  else{

    return checkVariance(variance,mean);
  }

}

//Calculate the percentage variance
//Return true if the variance has grown
boolean checkVariance(double value,double mean){
  double percentage_variance =  (value /mean)*100;
  if(percentage_variance>TOLERANCE){    
    return true;      
  }
  else{
    return false;
  }
}