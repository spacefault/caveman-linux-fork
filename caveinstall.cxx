#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <chrono>

std::string usernamereal;
std::string targetDisk;
std::string mirrorLocations;
std::string timezonereal;
std::string locale;
std::string password;
std::string confirmPassword;

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
    int netConnected = system("ping -c4 archlinux.org > /dev/null");
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
    bool acceptable = false;
    std::cout << ">>> "; std::cout << "Please enter your password now: ";
    std::cin >> password;
    int specialCharCount = 0;
    int capitalLetterCount = 0;
    do {
        std::cout << "Please enter your password again:" << std::endl;
        std::cout << ">>> ";
        std::cin >> confirmPassword;
        if (password == confirmPassword) {
            acceptable = true;
        } else {
            password = "";
            system("clear");
            std::cout << "\033[1;31mPasswords do not match. Please try again.\033[0m" << "\n" << std::endl;
            std::cout << ">>> "; std::cout << "Please enter your password now: ";
            std::cin >> password;
        }
    } while (!acceptable);
}

void summary() {
    system("clear");
    std::cout << "\033[1mSummary - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "These are the following settings that will be installed with your copy of Caveman Linux." << std::endl << std::endl;
    std::cout << "Partitioning" << std::endl;
    std::cout << "├─Target disk [**WILL BE ERASED**]: " << targetDisk << std::endl;
    std::cout << "├─Swap Size: 16G" << std::endl;
    std::cout << "└─Filesystem: btrfs" << std::endl;
    std::cout << "  ├─Subvolume: @" << std::endl;
    std::cout << "  ├─Subvolume: @home" << std::endl;
    std::cout << "  ├─Subvolume: @var_log" << std::endl;
    std::cout << "  ├─Subvolume: @pacman_pkg" << std::endl;
    std::cout << "  ├─Subvolume: @swap" << std::endl;
    std::cout << "  └─Subvolume: @snapshots" << std::endl << std::endl;
    std::cout << "Region Settings" << std::endl;
    std::cout << "├─Mirror Location: " << mirrorLocations << std::endl;
    std::cout << "├─Timezone: " << timezonereal << std::endl;
    std::cout << "└─Locale: " << locale << std::endl << std::endl;
    std::cout << "Users" << std::endl;
    std::cout << "├─Username: " << usernamereal << std::endl;
    std::cout << "└─Root Account: ENABLED" << std::endl << std::endl;
    std::cout << "Automatically Installed" << std::endl;
    std::cout << "├─Rok AUR Helper (coming soon)" << std::endl;
    std::cout << "└─System Drivers" << std::endl;
    std::cout << "  ├─Video Drivers (Automatically Detected by the Installation Assistant)" << std::endl;
    std::cout << "  ├─Audio Drivers (pipewire)" << std::endl;
    std::cout << "  ├─Disk Support software" << std::endl;
    std::cout << "  └─Extra Device Drivers" << std::endl << std::endl;
    std::cout << "Once you're ready, please press ENTER to begin installation." << std::endl;
    system("read");
}

void confirm() {
    system("clear");
    std::cout << "\033[1mConfirmation Required - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "\033[1;31m>>> CONFIRMATION REQUIRED <<<\033[0m" << std::endl << std::endl;
    std::cout << "The installer requires your confirmation to continue." << std::endl;
    std::cout << "Please enter the following (without the quotes): " << std::endl << std::endl;
    std::cout << "'InstallCavemanLinux'" << std::endl << std::endl;
    std::cout << "then press ENTER to begin installation." << std::endl << std::endl;
    std::string verifyString = "InstallCavemanLinux";
    std::string userInput;

    while (true) {
        std::cout << ">>> ";
        std::cin >> userInput;

        if (userInput == verifyString) {
            std::cout << "\033[1;32mVerified! Resuming installation...\033[0m" << std::endl;
            break;
        } else {
            std::cout << "\033[1;31mVerification failed, please try again.\033[0m" << std::endl;
        }
    }
}

void* installThread(void* arg) {
    std::cout << "Performing installation steps..." << std::endl << std::endl;

    // Check network connection
    system("clear");
    std::cout << "\033[1mInstalling - Caveman Linux Installation" << targetDisk << "Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes, and don't touch your computer." << std::endl << std::endl;
    std::cout << "======== Installation Stages ========" << std::endl << std::endl;
    std::cout << "\033[1m>> Preparing for Installation <<\033[0m" << std::endl;
    std::cout << "Formatting & Partitioning Disks" << std::endl;
    std::cout << "Installing the base system" << std::endl;
    std::cout << "Configuring Mirrors" << std::endl;
    std::cout << "Setting up Locales & Timezones" << std::endl;
    std::cout << "Making users" << std::endl;
    std::cout << "Installing Bootloader" << std::endl;
    std::cout << "Finalizing Installation" << std::endl;
    std::cout << "" << std::endl;
    system("ping -c 3 www.google.com > /dev/null");
    system("sed -i '/^#.*ParallelDownloads =.*/s/^#//' /etc/pacman.conf; sed -i '/^#.*ParallelDownloads =.*/s/[[:digit:]]*/10/' /etc/pacman.conf");
    system("rankmirrors -n 10 /etc/pacman/.d/mirrorlist");
    system("pacman -Sy");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Disk Setup
    system("clear");
    std::cout << "\033[1mInstalling - Caveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes, and don't touch your computer." << std::endl << std::endl;
    std::cout << "======== Installation Stages ========" << std::endl << std::endl;
    std::cout << "Preparing for Installation" << std::endl;
    std::cout << "\033[1m>> Formatting & Partitioning Disks <<\033[0m" << std::endl;
    std::cout << "Installing the base system" << std::endl;
    std::cout << "Configuring Mirrors" << std::endl;
    std::cout << "Setting up Locales & Timezones" << std::endl;
    std::cout << "Making users" << std::endl;
    std::cout << "Installing Bootloader" << std::endl;
    std::cout << "Finalizing Installation" << std::endl;
    std::cout << "" << std::endl;
    std::string diskMakeGpt = "parted -s " + targetDisk + " mklabel gpt";
    std::string diskSetUnit = "parted -s " + targetDisk + " unit MiB";
    std::string diskMakeEfiPartition = "parted -s " + targetDisk + " mkpart primary fat32 1MiB 700M";
    std::string diskSetEfiFlags = "parted -s " + targetDisk + " set 1 esp on";
    std::string diskMakeEfiLabel = "parted -s " + targetDisk + " name 1 EFI";
    std::string diskMakeRootPartition = "parted -s " + targetDisk + " mkpart primary ext4 700M -1s";
    std::string diskMakeRootLabel = "parted -s " + targetDisk + " name 2 root";
    std::string diskMountEfi = "mount" + targetDisk + " name 2 root";
    system(diskMakeGpt.c_str());
    system(diskSetUnit.c_str());
    system(diskMakeEfiPartition.c_str());
    system(diskSetEfiFlags.c_str());
    system(diskMakeEfiLabel.c_str());
    system(diskMakeRootPartition.c_str());
    system(diskMakeRootLabel.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Installing base system
    system("clear");
    std::cout << "\033[1mInstalling - Caveman Linux Installation" << targetDisk << "Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes, and don't touch your computer." << std::endl << std::endl;
    std::cout << "======== Installation Stages ========" << std::endl << std::endl;
    std::cout << "Preparing for Installation" << std::endl;
    std::cout << "Formatting & Partitioning Disks" << std::endl;
    std::cout << "\033[1m>> Installing the base system <<\033[0m" << std::endl;
    std::cout << "Configuring Mirrors" << std::endl;
    std::cout << "Setting up Locales & Timezones" << std::endl;
    std::cout << "Making users" << std::endl;
    std::cout << "Installing Bootloader" << std::endl;
    std::cout << "Finalizing Installation" << std::endl;
    std::cout << "" << std::endl;
    system("pacstrap /mnt base base-devel linux linux-headers linux-firmware sudo nano vim git neofetch networkmanager dhcpcd pipewire wpa_supplicant gnome gdm reflector"); // system ucode and drivers will be added soon
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Setting Up Mirrors
    system("clear");
    std::cout << "\033[1mInstalling - Caveman Linux Installation" << targetDisk << "Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes, and don't touch your computer." << std::endl << std::endl;
    std::cout << "======== Installation Stages ========" << std::endl << std::endl;
    std::cout << "Preparing for Installation" << std::endl;
    std::cout << "Formatting & Partitioning Disks" << std::endl;
    std::cout << "Installing the base system" << std::endl;
    std::cout << "\033[1m>> Configuring Mirrors <<\033[0m" << std::endl;
    std::cout << "Setting up Locales & Timezones" << std::endl;
    std::cout << "Making users" << std::endl;
    std::cout << "Installing Bootloader" << std::endl;
    std::cout << "Finalizing Installation" << std::endl;
    std::cout << "" << std::endl;
    std::string mirrorConfig = "reflector --country" + mirrorLocations + " --latest 10 --protocol https --sort rate --save /etc/pacman.d/mirrorlist --verbose";
    std::string mirrorXDGLocation = "echo --country" + mirrorLocations + " >> /etc/xdg/reflector/reflector.conf";
    system("cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak");
    system(mirrorConfig.c_str());
    system("echo --save /etc/pacman.d/mirrorlist >> /etc/xdg/reflector/reflector.conf");
    system(mirrorXDGLocation.c_str());
    system("echo --protocol https >> /etc/xdg/reflector/reflector.conf");
    system("echo --latest 10 >> /etc/xdg/reflector/reflector.conf");
    system("echo --sort-rate >> /etc/xdg/reflector/reflector.conf")
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Locales & region (to be refactored)
    // to be added
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // Create user account
    system("clear");
    std::cout << "\033[1mInstalling - Caveman Linux Installation" << targetDisk << "Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes, and don't touch your computer." << std::endl << std::endl;
    std::cout << "======== Installation Stages ========" << std::endl << std::endl;
    std::cout << "Preparing for Installation" << std::endl;
    std::cout << "Formatting & Partitioning Disks" << std::endl;
    std::cout << "Installing the base system" << std::endl;
    std::cout << "Configuring Mirrors" << std::endl;
    std::cout << "Setting up Locales & Timezones" << std::endl;
    std::cout << "\033[1m>> Making users <<\033[0m" << std::endl;
    std::cout << "Installing Bootloader" << std::endl;
    std::cout << "Finalizing Installation" << std::endl;
    std::cout << "" << std::endl;
    std::string makeUser = "useradd -m -s /bin/bash" + usernamereal;
    std::string makeUserGroups = "usermod -aG wheel,storage,power" + usernamereal;
    std::string makeUserPassword = "echo" + password + "| passwd --stdin" + usernamereal;
    system(makeUser.c_str());
    system(makeUserGroups.c_str());
    system(makeUserPassword.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Installing Bootloader
    // [to be added]
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // Finalizing installation
    // [to be added]
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // std::cout << std::endl;
    // std::cout << "\033[1;32mInstallation complete!\033[0m" << std::endl;

    // // Wait for 10 seconds before rebooting
    // std::cout << "Rebooting in 10 seconds..." << std::endl;
    // for (int i = 10; i >= 1; i--) {
    //     std::cout << "\r" << i << " seconds remaining...";
    //     std::cout.flush();
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // }

    // // Reboot the computer
    // std::cout << std::endl;
    // std::cout << "Rebooting..." << std::endl;
    // sleep(1);
    // reboot(RB_AUTOBOOT);

    return NULL;
}

void installCavemanLinux() {
    std::cout << "\033[1mCaveman Linux Installation Assistant\033[0m" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "The installer will now install Caveman Linux." << std::endl;
    std::cout << "Please wait while the installation completes." << std::endl << std::endl;

    pthread_t thread;
    pthread_create(&thread, NULL, installThread, NULL);
    std::cout << std::endl;

    const char* spinner = "|/-\\";
    int i = 0;
    while (true) {
        std::cout << "\rInstalling Caveman Linux [" << spinner[i % 4] << "]";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        i++;
    }

    pthread_join(thread, NULL);
}
int main() {
//    checkUser(); TO BE UNCOMMENTED ON RELEASE, it's uncommented for now to make testing easier
    // welcome();
    // netCheck();
    // diskSetup();
    // mirrorSetup();
    // timezoneSetup();
    // localeSetup();
    // makeUser();
    // makeUserPassword();
    // summary();
    // confirm();
    installCavemanLinux();
    return 0;
}
