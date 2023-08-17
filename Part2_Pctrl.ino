//모터11 유성17 몸통 46

//백색 LOW 선
#define dirR 12 //모터A 방향
#define dirL 13 //모터B 방향
#define R 10    //모터A 속도
#define L 11    //모터B 속도
#define MX_Speed 110
#define Kp 140 //블루투스로 값 바꿀건지, 매번 연결해 바꿀지 결정

//모터 외부전원 시 OPT 점퍼캡 제거 필수

bool isOnLine[5];
int IR[5] = {2,3,4,5,6}; //기본 센서
int IR2[2] = {7,8}; //마크 체크용 센서
int count;
int sum;

long time;
int MK = 0;
bool StopMV = false;

float error = 0;
float P;

void setup() {
  pinMode(dirR, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirR, HIGH); // DC모터의 회전 방향을 정해줍니다.//회로 연결상태에 따라서 LOW가 전진일지 HIGH가 전진인지 정해집니다.
  
  pinMode(dirL, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirL, LOW); // DC모터의 회전 방향을 정해줍니다.

  Serial.begin(9600);
}

void loop() {
  time = millis();
  //ChkMK();
  chkIR();
  Move();
}

void ChkMK(){
  //시작후 10초 경과시에만 작동(시작 라인 무시)
  if(time>=10000){
    MK = 0;
    if(digitalRead(IR2[0])==LOW){
      MK++;
    }
    if(digitalRead(IR2[1])==LOW){
      MK++;
    }

    if(MK>=2){
    if(digitalRead(IR[0]) == LOW || digitalRead(IR[4]) == LOW){
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

  //1. 변수 초기화
  error = 0;
  count = 0;
  sum = 0;

  //2. 적외선 센서값 읽기, 흰 선이면 LOW
  for (int i = 0; i < 5; i++){

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

void Move(){
  if(!StopMV){
      digitalWrite(dirR, HIGH);
      digitalWrite(dirL, LOW);

    //직진이면 그냥 풀악셀
    if(error==0){

        analogWrite(R, MX_Speed);
        analogWrite(L, MX_Speed);
    }

    //error가 음수, 왼쪽에서 선 감지, 왼바퀴 감속
    else if(error < 0){
      P = -error * Kp;

      analogWrite(R, MX_Speed);

      if(P > MX_Speed){
        digitalWrite(dirL, HIGH);
        analogWrite(L, 200);
      }
      else{
        analogWrite(L, P - MX_Speed);
      }
    }

    //error가 양수, 오른쪽에서 선 감지, 오른바퀴 감속
    else if(error > 0){
      analogWrite(L, MX_Speed);

      P = error * Kp;
      if(P > MX_Speed){
        digitalWrite(dirR, LOW);
        analogWrite(R, P - MX_Speed);
      }
      else{
        analogWrite(R, MX_Speed - P);
      }
    }
  }
}
