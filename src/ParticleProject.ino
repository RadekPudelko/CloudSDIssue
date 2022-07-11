#include "Particle.h"
#include "Utils.h"
#include "SdFat.h"

SYSTEM_MODE(SEMI_AUTOMATIC); // Toggle to disable cloud on startup
SYSTEM_THREAD(ENABLED);
#define SD_CS D8
SerialLogHandler logHandler;
SdFat SD;

int myCloudFunc(String data){
    int time = atoi(data);
    Log.info("cloud func %d", time);
    delay(time);
    return 0;
}

bool writeChunk(String filename, void *buffer, unsigned long offset, unsigned long chunkSize)
{
    File myFile;
    myFile.open(filename, FILE_WRITE);
    if (!myFile)
        return false;
    else if(offset > myFile.fileSize()) {
        myFile.close();
        return false;
    }
    myFile.seek(offset);
    myFile.write(buffer, chunkSize);
    myFile.close();
    return true;
}

void handle_all_the_events(system_event_t event, int param)
{
    Log.info("got event %d with value %d", event, param);
}

void setup() {
    Serial.begin(115200);
    while (!Serial.isConnected())
        delay(1000);
    reconfigureSpi();
    if (!SD.begin(SD_CS))
        Log.printf("⚠️  SD init failed.");
    Particle.function("radek-test", myCloudFunc);
    System.on(all_events, handle_all_the_events);
}
const char buffer[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Venenatis tellus in metus vulputate eu scelerisque felis imperdiet proin. Purus sit amet volutpat consequat mauris nunc. Eu nisl nunc mi ipsum faucibus vitae aliquet nec ullamcorper. Ante in nibh mauris cursus. Nisi vitae suscipit tellus mauris a diam maecenas sed. Mi quis hendrerit dolor magna eget est. Bibendum est ultricies integer quis auctor elit sed. Vitae justo eget magna fermentum iaculis eu. Augue mauris augue neque gravida in fermentum et sollicitudin. Sagittis orci a scelerisque purus. Ut diam quam nulla porttitor. Etiam dignissim diam quis enim lobortis scelerisque. Tellus rutrum tellus pellentesque eu tincidunt tortor aliquam nulla. Aenean vel elit scelerisque mauris pellentesque pulvinar pellentesque habitant.\nVelit ut tortor pretium viverra suspendisse potenti nullam ac tortor. Nunc non blandit massa enim nec dui nunc mattis enim. Molestie at elementum eu facilisis sed. Elementum curabitur vitae nunc sed velit dignissim sodales. Quam lacus suspendisse faucibus interdum posuere. Euismod elementum nisi quis eleifend quam adipiscing vitae. Non diam phasellus vestibulum lorem sed risus ultricies tristique nulla. Aenean sed adipiscing diam donec adipiscing tristique. Non odio euismod lacinia at quis. Faucibus et molestie ac feugiat sed lectus vestibulum mattis ullamcorper. Lectus mauris ultrices eros in cursus. Lacus vel facilisis volutpat est. Risus ultricies tristique nulla aliquet enim. Turpis egestas sed tempus urna et pharetra. Ultrices in iaculis nunc sed augue. Vitae auctor eu augue ut. Turpis egestas sed tempus urna et pharetra. A lacus vestibulum sed arcu. Sit amet mauris commodo quis. Amet consectetur adipiscing elit duis tristique.\nSuspendisse faucibus interdum posuere lorem ipsum dolor sit. Tempus egestas sed sed risus. Pretium quam vulputate dignissim suspendisse in est. In eu mi bibendum neque. Venenatis urna cursus eget nunc scelerisque viverra mauris. Nisi est sit amet facilisis. Dictum at tempor commodo ullamcorper. Est velit egestas dui id ornare arcu odio. Id diam vel quam elementum pulvinar etiam non. Eget nullam non nisi est. Tellus id interdum velit laoreet id donec. Nunc congue nisi vitae suscipit tellus mauris a. Tincidunt augue interdum velit euismod in pellentesque massa placerat. Vehicula ipsum a arcu cursus vitae congue mauris rhoncus. Cursus sit amet dictum sit amet justo donec enim. Dictum varius duis at consectetur. Vulputate dignissim suspendisse in est ante in nibh. Semper quis lectus nulla at./nConsequat mauris nunc congue nisi. Aenean et tortor at risus viverra. Morbi tincidunt augue interdum velit euismod in pellentesque. Sem integer vitae justo eget magna fermentum iaculis. Fringilla est ullamcorper eget nulla facilisi etiam. Nulla aliquet porttitor lacus luctus accumsan tortor posuere ac ut. Et molestie ac feugiat sed lectus vestibulum mattis ullamcorper velit. In metus vulputate eu scelerisque felis imperdiet proin fermentum leo. Eget aliquet nibh praesent tristique magna. Nec tincidunt praesent semper feugiat. Pulvinar pellentesque habitant morbi tristique senectus. Viverra mauris in aliquam sem fringilla ut morbi tincidunt augue. Id venenatis a condimentum vitae. Diam vel quam elementum pulvinar. Arcu non sodales neque sodales./nConsequat semper viverra nam libero justo laoreet sit amet cursus. In est ante in nibh mauris cursus mattis molestie a. Libero volutpat sed cras ornare arcu. Mauris sit amet massa vitae tortor condimentum lacinia quis vel. Rutrum quisque non tellus orci ac auctor augue mauris. Leo vel orci porta non pulvinar neque. Scelerisque fermentum dui faucibus in ornare quam. Lectus sit amet est placerat. In hac habitasse platea dictumst quisque sagittis purus sit amet. Quam vulputate dignissim suspendisse in est. Vel pharetra vel turpis nunc eget lorem dolor sed viverra. Hendrerit dolor magna eget est lorem ipsum. Adipiscing vitae proin sagittis nisl rhoncus mattis rhoncus. Ac placerat vestibulum lectus mauris. Sit amet venenatis urna cursus. Orci eu lobortis elementum nibh tellus molestie nunc. Semper auctor neque vitae tempus quam pellentesque. Turpis in eu mi bibendum neque egestas congue quisque. Quam id leo in vitae turpis massa.";

int myCount = 0;
int myCount2 = 0;
void loop() {
    if(!writeChunk("myfile.txt", (char *)buffer, 0, 2000)) {
        Log.info("Failed");
        myCount++;
        // SD.begin(SD_CS);
        SD.printSdError(&Serial);
        while (myCount > 1000) {
            delay(100);
        }
    } 
    // Playing with this delay time doesnt seem to have an effect
    delay(10);
    myCount2++;
    if(myCount2 % 100 == 0)
        Log.info("Loop");
}
