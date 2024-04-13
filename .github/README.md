# Wii U Account Swap

A tool to manage a user's account.dat, particularly for Network IDs.

Wii U Account Swap is an elaborate tool that allows the management of a Wii U user's `account.dat` file, located in `storage_mlc:/usr/save/system/act/[PERSISTENT_ID]`. This allows you to create a backup of (and of course, swap between) both a Nintendo Network ID and Pretendo Network ID. An unlinking feature is also provided for convienence, which will *locally* remove the currently attached Network ID from a user, all without deleting the user or Network ID itself.

Theoretically, this swap would limit needing two user accounts to use both services, especially if you want to use the same Network ID username on both. That being said, there are likely side effects of utilizing this (such as possibly losing access to purchased Nintendo eShop content while on Pretendo Network). This still needs to be investigated further. 

In the meanwhile, I'd still suggest using two seperate users if possible, per Pretendo Network's recommendation.

**Please be aware that an `account.dat` that's linked to a Network ID will have sensitive personal information on it. This includes both e-mail addresses and encrypted cached passwords (if you chose to save them). These will all be stored on your SD Card if you choose to back them up, so please do not share them with anybody!**

## Installation
A Wii U homebrew file will be bundled with each release. It should be placed on your SD card, particularly in `wiiu/apps/`.
* It's important to have the Aroma environment installed for Wii U Account Swap to work. Please visit our [hacking guide](https://wiiu.hacks.guide/) and the [Aroma webpage](https://aroma.foryour.cafe/) if you would like to softmod your Wii U console.

## Usage
If the program is placed correctly on an SD card, Wii U Account Swap will appear on the Wii U Menu alongside any other homebrew applications. It can then be launched just as anything else.
* If Wii U Account Swap doesn't show up on the Wii U Menu, confirm you placed the WUHB file on your SD card correctly and restart your console.
* `Switch to Nintendo Network ID`: Restores the file located at `SD:/wiiu/accounts/[PERSISTENT_ID]/nnid_account.dat`.
    * If you use the [Inkay](https://github.com/PretendoNetwork/Inkay) plugin for the Aroma environment, your configuration will be rewritten automatically to disable Pretendo Network, allowing you to connect to the Nintendo Network.
    * If a backup does not exist, yet you still try and restore one, you will be prompted with an error requesting you make one.
* `Switch to Pretendo Network ID`: Restores the file located at `SD:/wiiu/accounts/[PERSISTENT_ID]/pnid_account.dat`.
    * If you use the [Inkay](https://github.com/PretendoNetwork/Inkay) plugin for the Aroma environment, your configuration will be rewritten automatically to enable Pretendo Network.
    * If a backup does not exist, yet you still try and restore one, you will be prompted with an error requesting you make one.
* `Backup your current account`: Saves the user's current `account.dat` file to the SD card.
    * If the currently linked account is detected to be a Nintendo Network ID, it will save to `SD:/wiiu/accounts/[PERSISTENT_ID]/nnid_account.dat`.
    * Otherwise, if the currently linked account is detected as a Pretendo Network ID, it will save to `SD:/wiiu/accounts/[PERSISTENT_ID]/pnid_account.dat`.
    * At this time, you cannot backup local accounts (meaning ones without a Network ID attached to it) with this tool.
* `Unlink your account locally`: Unlinks your Network ID from a user, essentially making it a local-only user.
    * This is done by replacing any Network-related values with default settings. Your user Mii and any local content associated with the user will stay intact.
    * Unlinking will not unlink the account from your Wii U on the server. This means you can reattach the Network ID to any user on the system (including reattaching it to the same one), but the Network ID is still locked to the same console.

## Credits
I hope that I am able to express my thanks as much as possible to those who made this repository possible.
* [GaryOderNichts](https://github.com/GaryOderNichts), for assisting me with obtaining user information such as the Mii name and the persistent ID.
* [Lazr1026](https://github.com/Lazr1026), for helping me figure out all of the logistics for the tool, and also stress testing it.
* [Maschell](https://github.com/Maschell), for all of his work on the Aroma environment.
* [Pretendo Network](https://github.com/PretendoNetwork/), for their amazing open-source Nintendo Network replacement servers. This release is dedicated to their hard work, especially now that Nintendo Network is ultimately discontinued.