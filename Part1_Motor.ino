#define dirR 12 //모터A 방향
#define dirL 13 //모터B 방향
#define R 10    //모터A 속도
#define L 11    //모터B 속도

//모터 외부전원 시 OPT 점퍼캡 제거 필수

void setup() {
  pinMode(dirR, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirR, LOW); // DC모터의 회전 방향을 정해줍니다.//회로 연결상태에 따라서 LOW가 전진일지 HIGH가 전진인지 정해집니다.
  
  pinMode(dirL, OUTPUT); // DC모터의 방향제어 pinMode를 설정합니다.
  digitalWrite(dirL, HIGH); // DC모터의 회전 방향을 정해줍니다.
}

void loop() {
    analogWrite(R, 255);
    analogWrite(L, 255);
}
