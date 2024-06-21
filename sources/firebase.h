#include <Update.h>
#include <Firebase_ESP_Client.h>  // Firebase
#include "addons/TokenHelper.h"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

// #include "NotoSansBold15.h"
#define API_KEY "YOUR-FIREBASE-API-KEY"  // Firebase: Define the API Key
#define USER_EMAIL "YOUR-EMAIL"           // Firebase: Define the user Email
#define USER_PASSWORD "YOUR-LOGIN-PASSWORD"                         // Firebase: Define password
#define STORAGE_BUCKET_ID "YOUR-STORAGE-BUCKET-ID"     // Firebase: Define the Firebase storage bucket ID e.g bucket-name.appspot.com
#define FIRMWARE_PATH "YOUR-FIRMWARE-PATH"            // Firebase: Define the firmware path on Firebase

String swversion = __FILE__;

