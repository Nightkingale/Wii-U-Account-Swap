# Wii U Account Swap

A tool to manage a user's account.dat, particularly for Network IDs.

Wii U Account Swap is an elaborate tool that allows the management of a Wii U user's `account.dat` file, located in `storage_mlc:/usr/save/system/act/[PERSISTENT_ID]/`. This allows you to create a backup of (and of course, swap between) both a Nintendo Network ID and Pretendo Network ID. An unlinking feature is also provided for convienence, which will *locally* remove the currently attached Network ID from a user, all without deleting the user or Network ID itself.

Theoretically, this swap would limit needing two user accounts to use both services. That being said, the cost to this is that you may temporarily lose Nintendo eShop purchases, as the system believes that the Nintendo Network ID was deleted. If you happen to run into this issue, you'll need [signature patches](https://github.com/marco-calautti/SigpatchesModuleWiiU) installed.

Note that if you're trying to use a Pretendo Network ID with the same username as an existing Nintendo Network ID, this tool makes it possible. You will not be able to do the same on a Nintendo 3DS, however, without a similar tool on there, such as [FriendsAccountSwitcher.gm9](https://github.com/MatthewL246/pretendo-docker/blob/main/console-files/FriendsAccountSwitcher.gm9) by [MatthewL246](https://github.com/MatthewL246).

**Please be aware that an `account.dat` that's linked to a Network ID will have sensitive personal information in it. This includes both e-mail addresses and encrypted cached passwords (if you chose to save them). These will all be stored on your SD Card if you choose to back them up, so please do not share them with anybody!**

## Installation
A Wii U homebrew file will be bundled with each release. It should be placed on your SD card, particularly in `wiiu/apps/`.
* It's important to have the Aroma environment installed for Wii U Account Swap to work. Please visit our [hacking guide](https://wiiu.hacks.guide/) and the [Aroma webpage](https://aroma.foryour.cafe/) if you would like to softmod your Wii U console.

## Usage
If the program is placed correctly on an SD card, Wii U Account Swap will appear on the Wii U Menu alongside any other homebrew applications. It can then be launched just as anything else.
* If Wii U Account Swap doesn't show up on the Wii U Menu, confirm you placed the WUHB file on your SD card correctly and restart your console.
* `Swap to Nintendo Network ID`: Restores the file located at `SD:/wiiu/accounts/[PERSISTENT_ID]/nnid_account.dat`.
    * If you use the [Inkay](https://github.com/PretendoNetwork/Inkay) plugin for the Aroma environment and its configuration is detected (the plugin icon next to your Mii name will have a check), then your configuration will be rewritten automatically to disable Pretendo Network, allowing you to connect to the Nintendo Network.
    * If a backup does not exist, yet you still try and restore one, you will be prompted with an error requesting you make one.
* `Swap to Pretendo Network ID`: Restores the file located at `SD:/wiiu/accounts/[PERSISTENT_ID]/pnid_account.dat`.
    * If you use the [Inkay](https://github.com/PretendoNetwork/Inkay) plugin for the Aroma environment and its configuration is detected (the plugin icon next to your Mii name will have a check), then your configuration will be rewritten automatically to enable Pretendo Network.
    * If a backup does not exist, yet you still try and restore one, you will be prompted with an error requesting you make one.
* `Backup the account.dat File`: Saves the user's current `account.dat` file to the SD card.
    * If the currently linked account is detected to be a Nintendo Network ID, it will save to `SD:/wiiu/accounts/[PERSISTENT_ID]/nnid_account.dat`.
    * Otherwise, if the currently linked account is detected as a Pretendo Network ID, it will save to `SD:/wiiu/accounts/[PERSISTENT_ID]/pnid_account.dat`.
    * At this time, you cannot backup local accounts (meaning ones without a Network ID attached to it) with this tool.
* `Unlink the account.dat File`: Unlinks your Network ID from a user, essentially making it a local-only user.
    * This is done by replacing any Network-related values with default settings. Your user Mii and any local content associated with the user will stay intact.
    * Unlinking will not unlink the account from your Wii U on the server. This means you can reattach the Network ID to any user on the system (including reattaching it to the same one), but the Network ID is still locked to the same console.

## Credits
I hope that I am able to express my thanks as much as possible to those who made this repository possible.
* [GaryOderNichts](https://github.com/GaryOderNichts), for assisting me with obtaining user information such as the Mii name and the persistent ID, allowing me to recreate his signature user interface in the v2.0.0 release, and especially for helping me reimplement it.
* [Lazr1026](https://github.com/Lazr1026), for helping me figure out all of the logistics for the tool, and also stress testing it.
* [Luni Moon](https://github.com/luni-moon), for their amazing support on behalf of the [Pretendo server](https://invite.gg/pretendo), and also for exchanging ideas with me on code and concepts.
* [Maschell](https://github.com/Maschell), for all of his work on the Aroma environment.
* [NinStar](https://github.com/ninstar), who discovered over on [GBAtemp](https://gbatemp.net/threads/deleting-wii-u-user-without-nid-password.493387/page-5) the default values for a local user.
* [Font Awesome](https://fontawesome.com/), for their font containing some amazing icons used throughout the user interface starting with the v2.0.0 release.
* [Pretendo Network](https://github.com/PretendoNetwork/), for their amazing open-source Nintendo Network replacement servers, as well as for the support I've received from their developers after pitching this idea. This release is dedicated to their hard work!
    * You can donate to them [here](https://pretendo.network/account/upgrade) and support their endeavors, especially since they're gaining more traction after the recent shutdown.