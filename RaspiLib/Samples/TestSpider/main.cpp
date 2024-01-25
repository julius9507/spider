#include <Spider.h>
#include <RaspiLib.h>
#include "Led.h"

#include <iostream>
#include "Ultrasonic.h"


using namespace PiLib;
using namespace std;
using namespace SpiderLib;

int main(int argc, char *argv[])
{
  unsigned mVersion, lVersion;
  RaspiLib::LibVersion(mVersion, lVersion);

  cout << "using PiLib Version " << mVersion << "." << lVersion << endl;

  bool isOk = RaspiLib::Startup();
  if (! isOk)
  {
    cerr << "Problems on initializing PiLib - aborting" << endl;
    return -1;
  }
  Ultrasonic ultra = Ultrasonic(PIN13_GPIO27_PCMD,PIN15_GPIO22);
  Spider mySpider;
  ErrorCode err = mySpider.Init();
  //mySpider.SetHP();



  //while(mySpider.run(10,10,100,0,1));
  mySpider.Raise(-80);

  while(1)
  {
  
    cout <<   ultra.getDistance()<<"\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    mySpider.run(0, -35,100,0, 1);
    while(ultra.getDistance() < 40){
      mySpider.Turn();
      //mySpider.run(0, 0,100,30, 0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

  }


  return 0;
}
