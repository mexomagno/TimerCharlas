void setup(){
  initDisplay(2,3,4);
  writeDisc(B11101110);
}
long number=1;
void loop(){
  writeWord(number++);
  delay(1);
  rotateDisc(1);
}
