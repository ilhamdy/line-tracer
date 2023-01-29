int count;
int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;
int s[7],i,sensor,sen[7],error,x;
int en1=3; 
int en2=6;
int in1=2;
int in2=4;
int in3=5;
int in4=7;
int kp,p,pwmka,pwmki,flag_pid;

String dataIn;
String dt[10];
boolean parsing=false;


void parsingData(){ 
 int j=0;
  Serial.print("data masuk : ");
  Serial.print(dataIn);
  Serial.print("\n");
 
  dt[j]=""; 
  for(i=1;i<dataIn.length();i++){
    if ((dataIn[i] == '&') || (dataIn[i] == ',')){
      j++;
      dt[j]="";       
    }
    else{
      dt[j] = dt[j] + dataIn[i];
    }
  }
  Serial.println(dt[0].toInt());
}

void terimadata(){
  if(Serial.available()>0) {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '&') {
    parsing = true;
  }
}
 
if(parsing){
    parsingData();
    parsing=false;
    dataIn="";
  }

}
void maju(int ki, int ka)
{
  analogWrite(en1,ki);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(en2,ka);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void kiri(int ki, int ka)
{
  analogWrite(en1,ki);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  analogWrite(en2,ka);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void STOP(int ki, int ka)
{
  analogWrite(en1,0);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  analogWrite(en2,0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


void kanan(int ki, int ka)
{
  analogWrite(en1,ki);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(en2,ka);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void mundur(int ki, int ka)
{
  analogWrite(en1,ki);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(en2,ka);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void manual(){
if(Serial.available()>0) {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '&') {
    parsing = true;
  }
}
 
if(parsing){
    parsingData();
    parsing=false;
    dataIn="";
  }
if(dt[0].toInt()==1){
    maju(dt[1].toInt(),dt[1].toInt());
}
    
  
  else if(dt[0].toInt()==2){
    kiri(dt[1].toInt(),dt[1].toInt());
    
  }
  
  else if(dt[0].toInt()==3){

    kanan(dt[1].toInt(),dt[1].toInt());
    
  }
  
  else if(dt[0].toInt()==4){
    mundur(dt[1].toInt(),dt[1].toInt());
    
  }
  
  else if(dt[0].toInt()==5){
    STOP(0,0);
  }

}

void konvert_logic(){
   /*
   for(i=0;i<5;i++){
       if(s[i]<280)        sen[i]=1;
       else if(s[i]>280)   sen[i]=0;
    */
       
       if(s[0]>350)        sen[0]=1;
       else if(s[0]<350)   sen[0]=0;
       if(s[1]>350)        sen[1]=1;
       else if(s[1]<350)   sen[1]=0;
       if(s[2]>350)        sen[2]=1;
       else if(s[2]<350)   sen[2]=0;
       if(s[3]>350)        sen[3]=1;
       else if(s[3]<350)   sen[3]=0;
       if(s[4]>350)        sen[4]=1;
       else if(s[4]<350)   sen[4]=0;
//     }        
} 
void logika(){                       
        sensor=(sen[4]*16)+(sen[3]*8)+(sen[2]*4)+(sen[1]*2)+(sen[0]*1);
}             
 
void cek_sensor()
{
        bacasensor();
        konvert_logic();
        logika();
        for(i=0;i<5;i++){
          Serial.print(sen[i]);
          Serial.print(" ");
      
        }
        Serial.println();
        //delay(1000);
}  

void scan(int kecepatan)
{       sensor=sensor & 0b11111;
        switch(sensor)        
        {        
                case 0b00001: error=4;    x=1; flag_pid=0;     break;
                case 0b00011: error=3;    x=1; flag_pid=0;     break;
                case 0b00010: error=2;    x=1; flag_pid=0;     break;
                case 0b00110: error=1;    x=1; flag_pid=0;     break;
                case 0b00100: error=0;         flag_pid=0;     break;  
                case 0b01100: error=-1;   x=0; flag_pid=0;     break;   
                case 0b01000: error=-2;   x=0; flag_pid=0;     break;
                case 0b11000: error=-3;   x=0; flag_pid=0;     break;
                case 0b10000: error=-4;   x=0; flag_pid=0;     break;
          
                case 0b0000000: 
                
                 if(x==1) {
                    flag_pid=1;
                    break;
                  }
                 else  {
                    flag_pid=2;
                    break;
                  } 
         
        }

            if(flag_pid==0){
                p=kp*error;
                pwmka=kecepatan+p;        
                pwmki=kecepatan-p;
                if(pwmka>kecepatan)  pwmka=kecepatan;
                if(pwmki>kecepatan)  pwmki=kecepatan;
                if(pwmka<0)    pwmka=0;
                if(pwmki<0)    pwmki=0;

                maju(pwmki, pwmka);
    
            }
            else if(flag_pid==1){
             kiri(150, 150);
    
            }
            else if(flag_pid==2){
             kanan(150, 150);
     
            }
            
            
}
void bacasensor(){
  s[0] = analogRead(sensor1);
  s[1] = analogRead(sensor2);
  s[2] = analogRead(sensor3);
  s[3] = analogRead(sensor4);
  s[4] = analogRead(sensor5);

}

int hitung=0;
void scanX(int brpkali,int kec1){          
  while (hitung<brpkali) {
            while ((sensor & 0b001110)!=0b001110){
                  cek_sensor();
                  scan(kec1); 
            }
            while ((sensor & 0b001110)==0b001110){ 
                  cek_sensor();                      
                  scan(kec1);
                  if ((sensor & 0b001110)!=0b001110) { 
                        hitung++;  
                  }; 
            }   
        };     
        hitung=0;
} 
void belki(int ki, int ka){  
  cek_sensor();
      while ((sensor & 0b00001)!=0b00001){
    cek_sensor(); 
            kiri(ki,ka);  
        }
 }   
void belka(int ki ,int ka) {            
  cek_sensor();                       
  while ((sensor & 0b10000)!=0b10000){
                 cek_sensor();    
                 kanan(ki,ka);                   
        }      
 }  


void setup()
{  
  pinMode(en1,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(en2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  Serial.begin(9600);
  kp=50;
}



void otomatis(){
 //maju(225,225);
 //delay(400);
 scanX(1,150);
 
 STOP(0,0);
 //delay(200);
 //kanan(200,200);
 //delay(400);
 //STOP(0,0);
 
// scanX(1,150);
// STOP(0,0);
// delay(200);
// kanan(200,200);
// delay(2000);
// STOP(0,0);
// delay(200);
// scanX(1,150);
// STOP(0,0);
// delay(200);

// count=0;
// while(++count<900){
//    cek_sensor();
//    scan(250);
// }
 while(1){STOP(0,0);}   
}
void loop(void) {
      //otomatis();

//cek_sensor();
maju(150,150);
delay(300);
STOP(0,0);

scanX(1,255);
belki(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
belka(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
belka(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
belki(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
belka(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
belki(200,200);
STOP(0,0);
delay(300);

scanX(1,255);
STOP(0,0);
while(1) STOP(0,0);
}
