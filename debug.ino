void debugPrint(String val) {
  #ifdef DEBUG
  Serial.print(val);
  #endif
}

void debugPrintln(String val) {
  #ifdef DEBUG
  Serial.println(val);
  #endif
}