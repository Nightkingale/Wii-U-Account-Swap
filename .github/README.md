# Wii U Account Swap

A tool to manage a user's account.dat, particularly for Network IDs.

<p align="left">
  <a href="https://discord.nightkingale.com/">
    <img src="https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white" alt="Join us!" width="10%" height="10%">
  </a>
  <a href="https://donate.nightkingale.com/">
    <img src="https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white" alt="Thank you!" width="10%" height="10%">
  </a>
  <a href="https://nightkingale.com/">
    <img src="https://img.shields.io/badge/website-000000?style=for-the-badge&logo=About.me&logoColor=white" alt="Visit us!" width="10%" height="10%">
  </a>
</p>


Wii U Account Swap is an elaborate Aroma tool that allows the management of a user's `account.dat` file, located in `storage_mlc:/usr/save/system/act/[PERSISTENT_ID]/`.

> ⚠️ **Disclaimer:** Please be aware that an `account.dat` that's linked to a Network ID will have sensitive personal information in it. This includes both e-mail addresses and encrypted cached passwords (if you chose to save them). These will all be stored on your SD Card if you choose to back them up, so please do not share them with anybody!

## Installation
For convenience, it is recommended that you download Wii U Account Swap from the [Homebrew App Store](https://hb-app.store/wiiu/Wii-U-Account-Swap). Both are maintained by me, and I ensure that the mirror is updated instantaneously after a new release.

<p align="center">
  <a href="https://hb-app.store/wiiu/Wii-U-Account-Swap">
    <img src="appstore.png" alt="Get it on the Homebrew App Store!" width="50%" height="50%">
  </a>
</p>

A Wii U homebrew file will be bundled with each release. It should be placed on your SD card, particularly in `wiiu/apps/`.
* It's important to have the Aroma environment installed for Wii U Account Swap to work. Please visit our [hacking guide](https://wiiu.hacks.guide/) and the [Aroma webpage](https://aroma.foryour.cafe/) if you would like to softmod your Wii U console.

## Usage
If the program is placed correctly on an SD card, Wii U Account Swap will appear on the Wii U Menu alongside any other homebrew applications. It can then be launched just as anything else.
* If Wii U Account Swap doesn't show up on the Wii U Menu, confirm you placed the WUHB file on your SD card correctly and restart your console.

### Configuration
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

## Assistance
If you encounter bugs, the best place to report them would be the [Issues](https://github.com/Nightkingale/Wii-U-Account-Swap/issues) tab. This allows for easy tracking and reference, though please check for duplicates first and comment there if possible!

For assistance or other inquiries, the best place to reach out would be the [Nightkingale Studios](https://discord.nightkingale.com/) Discord server ([#chat-hangout](https://discord.com/channels/450846070025748480/1127657272315740260) is okay). I am active in many other Wii U homebrew Discord servers as well.

### Frequently Asked Questions

**1. Why should I use this instead of separate users like Pretendo Network recommends?**

Realistically, Pretendo Network's developers are much smarter than I am. That's why I believe that as long as the [Pretendo Network installation guide](https://pretendo.network/docs/install/wiiu) recommends what it does, that should be the primary method. However, should you still want to use this (either for convienence or using the same name for both Network IDs), it does serve as an alternative method. Please do your research beforehand.

**2. Wii U Account Swap is very confusing! How do I set it up?**

I described the best method for initial setup [here](https://github.com/Nightkingale/Wii-U-Account-Swap/issues/3#issuecomment-2075667254), which you will only need to do once per user. I am open to suggestions on how things could be streamlined, so if you have any ideas, leave an issue with feedback or give me a shout!

**3. Help! All the games I purchased from Nintendo eShop no longer work!**

Since you are swapping between accounts on one user (meaning the Nintendo Network ID is effectively taken off your console), the system believes that the Nintendo Network ID was deleted. If you happen to run into this issue, you'll need [signature patches](https://github.com/marco-calautti/SigpatchesModuleWiiU) installed as long as you are using your Pretendo Network ID. Swapping back to your Nintendo Network ID will rectify the issue as well.

## Compilation
You should have [wut](https://github.com/devkitPro/wut) installed. Following the [devkitPro Getting Started guide](https://devkitpro.org/wiki/Getting_Started) will set you up with it and all other basic dependancies. Additionally, you will need the following dependancies:

* [libmocha](https://github.com/wiiu-env/libmocha)
* [wiiu-sdl2](https://github.com/yawut/SDL)
* [wiiu-sdl2_mixer](https://github.com/yawut/SDL)
* [wiiu-sdl2_ttf](https://github.com/yawut/SDL)

After all dependancies have been installed, you can just run `make`.

* In addition, you can pass `DEBUG=1` as an argument, which will include the commit hash as part of the version string inside the binary. All products produced by [workflows](https://github.com/Nightkingale/Wii-U-Account-Swap/actions) utilize this.
* The resulting binaries will be a `.rpx` file (not typically distributed for releases) and a `.wuhb` file. You can use these the same way as a release binary.

## Credits
I hope that I am able to express my thanks as much as possible to those who made this repository possible.
* [GaryOderNichts](https://github.com/GaryOderNichts), for assisting me with obtaining user information such as the Mii name and the persistent ID, allowing me to recreate his signature user interface in the v2.0.0 release, and especially for helping me reimplement it.
* [Lazr1026](https://github.com/Lazr1026), for helping me figure out all of the logistics for the tool, and also stress testing it.
* [luni-moon](https://github.com/luni-moon), for their amazing support on behalf of the [Pretendo server](https://invite.gg/pretendo), and also for exchanging ideas with me on code and concepts.
* [Maschell](https://github.com/Maschell), for all of his work on the Aroma environment.
* [NinStar](https://github.com/ninstar), who discovered over on [GBAtemp](https://gbatemp.net/threads/deleting-wii-u-user-without-nid-password.493387/page-5) the default values for a local user.
* [Font Awesome](https://fontawesome.com/), for their font containing some amazing icons used throughout the user interface starting with the v2.0.0 release.
* [Pretendo Network](https://github.com/PretendoNetwork/), for their amazing open-source Nintendo Network replacement servers, as well as for the support I've received from their developers after pitching this idea. This release is dedicated to their hard work!
    * You can donate to them [here](https://pretendo.network/account/upgrade) and support their endeavors, especially since they're gaining more traction after the recent shutdown.