#define dirR 12 //모터A 방향
#define dirL 13 //모터B 방향
#define R 10    //모터A 속도
#define L 11    //모터B 속도

//모터 외부전원 시 OPT 점퍼캡 제거 필수

bool isOnLine[5];
int[] IR = {2,3,4,5,6}
int count;
int sum;

float error = 0;
#define Kp 1 //블루투스로 값 바꿀건지, 매번 연결해 바꿀지 결정
int P;

void setup() {
  pinMode(dirR, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirR, LOW); // DC모터의 회전 방향을 정해줍니다.//회로 연결상태에 따라서 LOW가 전진일지 HIGH가 전진인지 정해집니다.
  
  pinMode(dirL, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirL, LOW); // DC모터의 회전 방향을 정해줍니다.
}

void loop() {
  chkIR();
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
  cum = 0;

  //2. 적외선 센서값 읽기, 
  for (int i = 0; i <= 5; i++){

    if(digitalRead(IR[i]) == HIGH){
      sum += (2-i) //-2 -1 0 1 2
      count ++;
    }; 
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
}

void Move(){

  //직진이면 그냥 풀악셀
  if(error==0){
      analogWrite(R, 255);
      analogWrite(L, 255);
  }

  //error가 음수, 왼쪽에서 선 감지, 왼바퀴 감속
  else if(error < 0){
    P = -error * Kp
    analogWrite(R, 255);
    analogWrite(L, 255 - P);
  }

  //error가 양수, 오른쪽에서 선 감지, 오른바퀴 감속
  else if(error < 0){
    P = error * Kp
    analogWrite(R, 255 - P);
    analogWrite(L, 255);
  }
}