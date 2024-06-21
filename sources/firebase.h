#include <Update.h>
#include <Firebase_ESP_Client.h>  // Firebase
#include "addons/TokenHelper.h"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

// #include "NotoSansBold15.h"
#define API_KEY "AIzaSyCnKR1F1quWY5pSKHapDZZmC9Nl_v4okCU"  // Firebase: Define the API Key
#define USER_EMAIL "ayoolagabriel2712@gmail.com"           // Firebase: Define the user Email
#define USER_PASSWORD "Newness01#"                         // Firebase: Define password
#define STORAGE_BUCKET_ID "vtrackfirmware.appspot.com"     // Firebase: Define the Firebase storage bucket ID e.g bucket-name.appspot.com
#define FIRMWARE_PATH "test/vitalsfirmware.bin"            // Firebase: Define the firmware path on Firebase

String swversion = __FILE__;

