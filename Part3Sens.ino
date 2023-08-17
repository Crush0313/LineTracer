
bool isOnLine[5];
int IR[5] = {2,3,4,5,6}; //기본 센서
int IR2[2] = {7,8}; //마크 체크용 센서
int count;
int sum;

long time;
int MK = 0;
bool StopMV = false;

float error = 0;
#define Kp 40 //블루투스로 값 바꿀건지, 매번 연결해 바꿀지 결정
int P;

void setup() {
  Serial.begin(9600);
}

void ChkMK(){
  //시작후 10초 경과시에만 작동(시작 라인 무시)
  if(time>=10000){
    MK = 0;
    
    if(digitalRead(IR2[0])==HIGH){
      MK++;
    }
    if(digitalRead(IR2[1])==HIGH){
      MK++;
    }

    if(MK>=2){
      if(digitalRead(IR[0]) == HIGH || digitalRead(IR[4]) == HIGH){
          return;
      } 
      else{
            StopMV = true;
      }
    }
  }
}


// 위치에 따른 각 IR의 값
// -2 -1 0 1 2
// (2-i) 로 표현

// Error 표 (IR 센서값에 따른)
// +2.0 : 00001
// +1.5 : 00011
// +1.0 : 00010 = 00111
// +0.5 : 00110
//  0.0 : 00100 = 01110 = 11111(교차선)
// -0.5 : 01100
// -1.0 : 01000 = 11100
// -1.5 : 11000
// -2.0 : 10000
//마킹을 감지할지 안 할지는 모르겠음...

//for문 돌면서 적외선 센서값 측정해서, error 산출
void chkIR(){
/*
  //2. 적외선 센서값 읽기, 흰 선이면 HIGH
  for (int i = 0; i < 5; i++){

    //Serial.print(analogRead(IR[i]));
    Serial.print(digitalRead(IR[i]));
    Serial.print(" ");
  }
  Serial.println(" ");
  */


  
  //1. 변수 초기화
  error = 0;
  count = 0;
  sum = 0;

  //2. 적외선 센서값 읽기, 흰 선이면 HIGH
  for (int i = 0; i < 5; i++){

    Serial.print(digitalRead(IR[i]));
    Serial.print(" ");

    if(digitalRead(IR[i]) == LOW){
      sum += (i-2); //-2 -1 0 1 2
      count ++;
    }
  }
  
  //3. error 값 생성
  //직진이 아니라면, 반응 센서 갯수로 나눔
  if(sum != 0){
    error = (float)sum / count; //소수 결과 보존하도록 분자 명시적 형변환 
  }
  //직진이라면 그냥 sum 사용
  else{
    error = sum;
  }
  Serial.println(error);
}

void loop() {
  time = millis();
  //ChkMK();
  chkIR();
  //Serial.println(digitalRead(4));
}
