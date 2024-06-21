void myDelay(unsigned int delayTime){
  long int startTime = millis();
  while ((millis() - startTime) <= delayTime)
    ;
}