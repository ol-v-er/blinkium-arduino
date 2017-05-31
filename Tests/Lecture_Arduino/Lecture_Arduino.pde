import processing.serial.*;

Serial myPort;
String val[];
String lastStringRead;

void setup(){
  for(String s : Serial.list())
    println(s);
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  val = new String[1];
}

void draw(){
  if ( myPort.available() > 0) 
  {  // If data is available
    String s = myPort.readStringUntil('\n');
    
    if(s != null){
      if(val[0] == null)
        val[0] = s;
      else
        val[0] = val[0] + s;
      //println(s);
      saveStrings("received.txt", val);
    }
     
  }
  
}