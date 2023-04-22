#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <fstream>
#include <openssl/sha.h>

using namespace std;

class User {
private:
    string username;
    string password;
public:
    User(string username, string password) {
        this->username = username;
        this->password = password;
    }
    string getUsername() {
        return username;
    }
    string getPassword() {
        return password;
    }
};

class AccountSystem {
private:
    vector<User> users;
    string filename = "users.txt";

    bool checkPasswordStrength(string password) {
        // Password must contain at least one uppercase letter, one lowercase letter, one number and one special character
        regex pattern("(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!@#$%^&*()_+\\-=[\\]{};':\"\\\\|,.<>\\/?]).{8,}");
        return regex_match(password, pattern);
    }

    string hashPassword(string password) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, password.c_str(), password.length());
        SHA256_Final(hash, &sha256);
        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    bool checkUsernameAndPasswordLength(string username, string password) {
        // Username and password must be between 6 and 20 characters
        return username.length() >= 6 && username.length() <= 20 && password.length() >= 6 && password.length() <= 20;
    }

    bool checkUsernameAndPasswordFormat(string username, string password) {
        // Username and password must only contain letters, numbers, and underscore
        regex pattern("[a-zA-Z0-9_]+");
        return regex_match(username, pattern) && regex_match(password, pattern);
    }

    bool checkUsernameExists(string username) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                return true;
            }
        }
        return false;
    }

    void loadUsersFromFile() {
        ifstream file(filename);
        if (file) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find(":");
                if (pos != string::npos) {
                    string username = line.substr(0, pos);
                    string password = line.substr(pos + 1);
                    User user(username, password);
                    users.push_back(user);
                }
            }
            file.close();
        }
    }

    void saveUsersToFile() {
        ofstream file(filename);
        if (file) {
            for (int i = 0; i < users.size(); i++) {
                file << users[i].getUsername() << ":" << users[i].getPassword() << endl;
            }
            file.close();
        }
    }
public:
    AccountSystem() {
        loadUsersFromFile();
    }

    void registerUser(string username, string password) {
        if (!checkUsernameAndPasswordLength(username, password)) {
            cout << "Error: Username and password must be between 6 and 20 characters." << endl;
            return;
        }
        if (!checkUsernameAndPasswordFormat(username, password)) {
            cout << "Error: Username and password can only contain letters, numbers, and underscore." << endl;
            return;
        }
        if (checkUsernameExists(username)) {
            cout << "Error: Username already exists." << endl;
            return;
        }
        if (!checkPasswordStrength(password)) {
            cout << "Error: Password must contain at least one uppercase letter, one lowercase letter, one number and one special character." << endl;
            return;
        }
        string hashedPassword = hashPassword(password);
        User user(username, hashedPassword);
        users.push_back(user);
        saveUsersToFile();
        cout << "User registered successfully." << endl;
    }

    void loginUser(string username, string password) {
        if (!checkUsernameAndPasswordLength(username, password)) {
            cout << "Error: Username and password must be between 6 and 20 characters." << endl;
            return;
        }
        if (!checkUsernameAndPasswordFormat(username, password)) {
            cout << "Error: Username and password can only contain letters, numbers, and underscore." << endl;
            return;
        }
        string hashedPassword = hashPassword(password);
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username && users[i].getPassword() == hashedPassword) {
                cout << "User logged in successfully." << endl;
                return;
            }
        }
        cout << "Error: Invalid username or password." << endl;
    }

    void resetPassword(string username, string oldPassword, string newPassword) {
        if (!checkUsernameAndPasswordLength(username, oldPassword) || !checkUsernameAndPasswordLength(username, newPassword)) {
            cout << "Error: Username and password must be between 6 and 20 characters." << endl;
            return;
        }
        if (!checkUsernameAndPasswordFormat(username, oldPassword) || !checkUsernameAndPasswordFormat(username, newPassword)) {
            cout << "Error: Username and password can only contain letters, numbers, and underscore." << endl;
            return;
        }
        if (!checkPasswordStrength(newPassword)) {
            cout << "Error: Password must contain at least one uppercase letter, one lowercase letter, one number and one special character." << endl;
            return;
        }
        string hashedOldPassword = hashPassword(oldPassword);
        string hashedNewPassword = hashPassword(newPassword);
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username && users[i].getPassword() == hashedOldPassword) {
                users[i] = User(username, hashedNewPassword);
                saveUsersToFile();
                cout << "Password reset successfully." << endl;
                return;
            }
        }
        cout << "Error: Invalid username or password." << endl;
    }
};

int main() {
    AccountSystem accountSystem;
    int choice;
    string username, password, oldPassword, newPassword;

    do {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Reset Password" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                accountSystem.registerUser(username, password);
                break;
            case 2:
                cout << "在此键入用户名称: ";
                cin >> username;
                cout << "在此键入用户密码: ";
                cin >> password;
                accountSystem.loginUser(username, password);
                break;
            case 3:
                cout << "Enter username: ";
                cin >> username;
                cout << "在此键入旧的密码: ";
                cin >> oldPassword;
                cout << "在此键入新的密码: ";
                cin >> newPassword;
                accountSystem.resetPassword(username, oldPassword, newPassword);
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
