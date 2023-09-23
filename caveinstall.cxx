#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>

std::string targetDisk;
std::string mirrorLocations;
std::string timezonereal;
std::string locale;
std::string usernamereal;
std::string password;


void checkUser() {
    uid_t uid = geteuid();
    if (uid != 0) {
        std::cerr << "\033[1;31mFATAL: Non root account detected!\033[0m" << std::endl << std::endl;
        std::cerr << "You are not running the installer as the root user." << std::endl;
        std::cerr << "This can result in a partial installation, and an unstable system." << std::endl;
        std::cerr << "Please enter './cavemaninstaller' in the shell prompt, and try again." << std::endl << std::endl;
        std::cerr << "If that's not working, you might have a corrupted ISO." << std::endl;
        std::cerr << "Please redownload the ISO, and try again." << std::endl << std::endl;
        std::cerr << "If you've done everything, and you're still seeing this error," << std::endl;
        std::cerr << "Please report it to the Caveman Linux developers." << std::endl << std::endl;
        std::cerr << "https://github.com/cavemandevs/caveman-linux/" << std::endl;
        exit(1);
    }
}

void welcome() {
    system("clear");
    std::cout << "\e[1mCaveman Linux Installation Assistant" << std::endl;
    std::cout << "\e[0mWelcome to Caveman Linux!" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "This installer was made by the Caveman Linux contributors & Caveman Software" << std::endl;
    std::cout << "This software is released under the GPLv2 License" << std::endl << std::endl;
    std::cout << "This is pre-release software, so bugs might be present!" << std::endl;
    std::cout << "if you see any bugs, please report them to https://github.com/cavemandevs/caveman-linux/issues" << std::endl << std::endl;
    std::cout << "2023 - 2023 Caveman Software & Contributors" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "\033[1;31mWARNING: NETWORKING SETUP IS UNSTABLE. PLEASE SET UP NETWORKING BEFORE USING THE INSTALLER\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    while (1) {
        std::cout << "Please choose an option, then press ENTER. " << std::endl << std::endl;
        std::cout << "1: Start Installation" << std::endl;
        std::cout << "2: Enter Shell" << std::endl;
        std::cout << "3: Shutdown Computer" << std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            return;
                     }
        else if (choice == 2) {
            system("clear");
            std::cout << "\033[1;31m>>> ENTERED SHELL <<<\033[0m" << std::endl << std::endl;
            std::cout << "HERE BE DRAGONS!" << std::endl << std::endl;
            std::cout << "You can continue and do a custom installation or run some other commands here." << std::endl << std::endl;
            std::cout << "Do be warned that doing custom installations, and deviating from the standard" << std::endl;
            std::cout << "installation isn't officially supported and may result in instability." << std::endl << std::endl;
            std::cout << "If you entered the shell by mistake you may run the installer by running ./cavemaninstall" << std::endl;
            exit(0);
        }
        else if (choice == 3) {
             system("clear");
             std::cout << "Shutting down" << std::endl;
             sleep(5);
             // system("shutdown now");
             return;
         }
        else {
            std::cout << "Invalid choice. Select a valid option (1/2/3). " <<  std::endl;
        }
    }

    return;
}

void netCheck() {
    system("clear");
    std::cout << "\033[1mChecking Network Connection - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The Installation assistant is detecting active network connections. Please wait..." << std::endl;
    std::cout << "Connecting to: archlinux.org" <<std::endl <<std::endl;
    int netConnected = system("ping -c4 archlinux.org");
    if (netConnected == 0) {
        std::cout << "\033[1;32mNetwork Connection found! Resuming Installation..\033[0m" << std::endl;
        sleep(5);
        return;
    }
    else {
        system("clear");
        std::cout << "\033[1mChecking Network Connection - FAILED - Caveman Linux Installation Assistant\033[0m" << std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cout << "\033[1;31mA working network connection was NOT found!\033[0m" << std::endl << std::endl;
        std::cout << "As of now, a network assistant is not available in the installer." << std::endl;
        std::cout << "Please exit the installer, and connect to the internet, and try again." << std::endl;
        exit(0);
    }
    return;
}

void diskSetup() {
    system("clear");
    std::cout << "\033[1mDisk Configuration - Partitioning - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Disk Configuration" << std::endl;
    std::cout << "This screen will assist in setting up the target disk" << std::endl;
    std::cout << "\033[1;31mWARNING: AS OF NOW, SELECTING THE DISK WILL ***ERASE THE ENTIRE DISK***\033[0m" << std::endl;
    std::cout << "Here is a list of available disks" << std::endl << std::endl;
    system("lsblk");
    std::cout << "\n Make sure to select a disk and not a partition. The disk should not have a number at the end. " << std::endl << std::endl;
    std::cout << "Please enter the disk where Caveman Linux will be installed" << std::endl;
    std::cout << ">>> " ;
    std::cin >> targetDisk;

}

void mirrorSetup() {
    system("clear");
    std::cout << "\033[1mRegion Settings - Mirror Configuration - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Mirror Settings" << std::endl;
    std::cout << "What's a mirror?" << std::endl << std::endl;
    std::cout << "A mirror is a server near you which helps you download and install packages." << std::endl;
    std::cout << "Selecting the right country is important for fast downloads and installations" << std::endl;
    std::cout << "When selecting a mirror, it is vital that you format like so: " << std::endl << std::endl;
    std::cout << "Canada, France" << std::endl << std::endl;
    std::cout << "While it is possible to select two or more countries at once, it is not recommended." << std::endl << std::endl;
    std::cout << "\033[90mCaveman Software is committed to keeping your privacy safe and secure.\033[0m" << std::endl;
    std::cout << "\033[90mYour region details will be kept offline, and locally on your device.\033[0m" << std::endl;
    std::cout << "\033[90mThe packages you download will only be visible to the mirror, and will NOT be visible to outside attackers, and it's not visible to Caveman Software.\033[0m" << std::endl;
    std::cout << "\033[90mThe mirror system uses HTTP Secure encryption layers to keep your package choices private.\033[0m" <<std::endl << std::endl;
    std::cout << "Please enter the country for your mirrors and then press ENTER" << std::endl;
    std::cout << ">>> ";
    std::cin >> mirrorLocations;

    return;
}

void timezoneSetup() {

    system("clear");
    std::cout << "\033[1mRegion Settings - Timezone Configuration - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Timezone" << std::endl << std::endl;
    std::cout << "This screen will assist you in setting up a timezone" << std::endl;
    std::cout << "A better implementation for this is coming soon." << std::endl << std::endl;
    std::cout << "An example of a timezone code looks something like this:" << std::endl << std::endl;
    std::cout << "Canada/Mountain" << std::endl << std::endl;
    std::cout << "Enter the timezone code and press ENTER: " << std::endl;
    std::cout << ">>> ";
    std::cin >> timezonereal;
    return;
}

void localeSetup() {
    system("clear");
    std::cout << "\033[1mRegion Settings - Locale Configuration - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Locale Configuration" << std::endl;
    std::cout << "This screen will help you set up locales." << std::endl;
    std::cout << "Once again, this section is very primitive, and it will be improved soon" << std::endl;
    std::cout << "Here are some examples of locale codes:" << std::endl << std::endl;
    std::cout << "English [US]       -->  en_US.UTF-8 UTF-8" << std::endl;
    std::cout << "French [Canadian]  -->  fr_CA.UTF-8 UTF-8" << std::endl << std::endl;
    std::cout << "Please enter the locale code, and press ENTER." << std::endl;
    std::cout << ">>> ";
    std::cin >> locale;
    return;
}

void makeUser() {
    std::vector<std::string> reservedUsernames;
    std::string usernamereal;
    std::ifstream file("reserved_usernames.txt");
    std::string line;

    while (std::getline(file, line)) {
        reservedUsernames.push_back(line);
    }

    system("clear");

    std::cout << "\033[1mUser Configration - Creating Users - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "This screen will assist you in setting up a user account." << std::endl << std::endl;
    std::cout << "Usernames may NOT begin with special characters or numbers." << std::endl;
    std::cout << "Usernames may contain underscores, dashes, numbers, or mixed case letters." << std::endl << std::endl;
    std::cout << "Please enter a username, and we will check for any errors." << std::endl;

    do {
        std::cout << ">>> ";
        std::cin >> usernamereal;
        if (std::find(reservedUsernames.begin(), reservedUsernames.end(), usernamereal) != reservedUsernames.end()) {
            std::cout << "\033[1;31mThis username is reserved. Please choose a different one.\033[0m" << std::endl;
            continue;
        }
        if (!isalpha(usernamereal[0])) {
            std::cout << "\033[1;31mUsername must start with a letter (a-z or A-Z). Please choose a different one.\033[0m" << std::endl;
            continue;
        }
        bool valid = true;
        for (char c : usernamereal) {
            if (!isalnum(c) && c != '-' && c != '_') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mUsername can only contain letters, digits, hyphens, or underscores. Please choose a different one.\033[0m" << std::endl;
            continue;
        }
        if (usernamereal.length() > 30) {
            std::cout << "\033[1;31mUsername cannot exceed 30 characters. Please choose a shorter one.\033[0m" << std::endl;
            continue;
        }
        std::cout << "\033[1;32mUsername \"" << usernamereal << "\" is valid and can be used! Resuming installation...\033[0m" << std::endl;
        sleep(5);
        break;
    } while (true);
}

void makeUserPassword() {
    system("clear");

    std::cout << "\033[1mUser Configuration - Configuring Passwords - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "This screen will assist you in setting up a password for your system." << std::endl;
    std::cout << "The password you choose will also be used for the root account on the computer." << std::endl << std::endl;
    std::cout << "Your password must be at least 10 characters long, must contain 2 special characters, and must have 3 uppercase letters." << std::endl;
    std::cout << "Make sure this password is secure and complicated, write it down, and DO NOT SHARE IT TO ANYONE." << std::endl << std::endl;
    std::cout << "Please enter your password now:" << std::endl;

    while (true) {
        std::cout << ">>> ";
        std::cin >> password;
        int specialCharCount = 0;
        int capitalLetterCount = 0;
        if (password.length() < 10) {
            std::cout << "\033[1;31mPassword does not meet the requirements. Please try again.\033[0m" << std::endl;
            continue;
        }
        for (char c : password) {
            if (!isalnum(c) && c != ' ') {
                specialCharCount++;
            }
            if (isupper(c)) {
                capitalLetterCount++;
            }
        }
        if (specialCharCount >= 2 && capitalLetterCount >= 3) {
            std::cout << "\033[1;32mPassword is valid! Resuming installation...\033[0m" << std::endl;
            sleep(5);
            break; // Break out of the loop if the password is valid
        } else {
            std::cout << "\033[1;31mPassword does not meet the requirements. Please try again.\033[0m" << std::endl;
        }
    }
}

int main() {
//    checkUser(); TO BE UNCOMMENTED ON RELEASE, it's uncommented for now to make testing easier
    welcome();
    netCheck();
    diskSetup();
    mirrorSetup();
    timezoneSetup();
    localeSetup();
    makeUser();
    makeUserPassword();
    return 0;
}
