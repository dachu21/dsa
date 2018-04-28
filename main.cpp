#define NONE "NONE"
#define GENERATED "GENERATED"
#ifdef _WIN32
#define PAUSE "pause"
#define CLEAR "cls"
#else
#define CLEAR "clear"
#define PAUSE "read"
#endif

#include <iostream>

#include "DigitalSignature.hpp"

using namespace std;

int main() {

  int choice = -1;
  char saveChoice;
  string filename;

  AlgorithmParameters parameters;
  SecretKey secretKey;
  PublicKey publicKey;
  Message message;
  DigitalSignature signature;

  string parametersStatus = NONE;
  string secretKeyStatus = NONE;
  string publicKeyStatus = NONE;
  string messageStatus = NONE;
  string signatureStatus = NONE;

  while (choice != 0) {
    system(CLEAR);
    cout << "Algorithm parameters: " << parametersStatus << endl;
    cout << "Secret key: " << secretKeyStatus << endl;
    cout << "Public key: " << publicKeyStatus << endl;
    cout << "Message: " << messageStatus << endl;
    cout << "Signature: " << signatureStatus << endl;
    cout << "---------------------------------------------" << endl;
    cout << "1. Generate algorithm parameters" << endl;
    cout << "2. Load algorithm parameters" << endl;
    cout << "3. Generate secret key (algorithm parameters)" << endl;
    cout << "4. Load secret key" << endl;
    cout << "5. Generate public key (secret key)" << endl;
    cout << "6. Load public key" << endl;
    cout << "7. Load message" << endl;
    cout << "8. Sign message (secret key, message)" << endl;
    cout << "9. Load signature" << endl;
    cout << "10. Verify message (public key, message, signature)" << endl;
    cout << "0. Exit" << endl;

    cin >> choice;
    system(CLEAR);
    switch (choice) {

      case 1:
        parameters = AlgorithmParameters::generate();
        parametersStatus = GENERATED;
        cout << "Save? (y/n): ";
        cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
          cout << "Enter filename: ";
          cin >> filename;
          parameters.saveToFile(filename);
          parametersStatus = "\"" + filename + "\"";
        }
        break;

      case 2:
        cout << "Enter filename: ";
        cin >> filename;
        parameters = AlgorithmParameters::loadFromFile(filename);
        parametersStatus = "\"" + filename + "\"";
        break;

      case 3:
        secretKey = SecretKey::generate(parameters);
        secretKeyStatus = GENERATED;
        cout << "Save? (y/n): ";
        cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
          cout << "Enter filename: ";
          cin >> filename;
          secretKey.saveToFile(filename);
          secretKeyStatus = "\"" + filename + "\"";
        }
        break;

      case 4:
        cout << "Enter filename: ";
        cin >> filename;
        secretKey = SecretKey::loadFromFile(filename);
        secretKeyStatus = "\"" + filename + "\"";
        break;

      case 5:
        publicKey = PublicKey::generate(secretKey);
        publicKeyStatus = GENERATED;
        cout << "Save? (y/n): ";
        cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
          cout << "Enter filename: ";
          cin >> filename;
          publicKey.saveToFile(filename);
          publicKeyStatus = "\"" + filename + "\"";
        }
        break;

      case 6:
        cout << "Enter filename: ";
        cin >> filename;
        publicKey = PublicKey::loadFromFile(filename);
        publicKeyStatus = "\"" + filename + "\"";
        break;

      case 7:
        cout << "Enter filename: ";
        cin >> filename;
        message = Message::loadFromFile(filename);
        messageStatus = "\"" + filename + "\"";
        break;

      case 8:
        signature = DigitalSignature::sign(secretKey, message);
        signatureStatus = GENERATED;
        cout << "Save? (y/n): ";
        cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
          cout << "Enter filename: ";
          cin >> filename;
          signature.saveToFile(filename);
          signatureStatus = "\"" + filename + "\"";
        }
        break;

      case 9:
        cout << "Enter filename: ";
        cin >> filename;
        signature = DigitalSignature::loadFromFile(filename);
        signatureStatus = "\"" + filename + "\"";
        break;

      case 10:
        if (DigitalSignature::verify(publicKey, message, signature)) {
          cout << "SUCCESS" << endl;
        } else {
          cout << "FAILURE" << endl;
        }
        system(PAUSE);
        break;

      case 0:
      default:
        break;
    }
  }

  return 0;
}