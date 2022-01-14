/*  Myriad is a collection of components for controlling sensor driven generative LED art.
    
    Development is currently split between the Teensy 4.1 driven wearable "Starshroud", and the
    cost effective ESP32 driven "Lampert" for stationary installations. Starshroud is controled by
    the associated bluetooth app "Starchart", and Lampert is controlled through a wifi portal.

    Goals/Roadmap
      1. Unify settings data storage techniques across platforms. The ESP32's Preferences.h library
      is preferred.
      2. Upgrade existing mDNS webserver with static IP (for easier discovery) and OTA

    Acknowledgements and credit:
      Marc Merlin, for some great help, inspiration, and LEDNeomatrix + FramebufferGFX
      Many thanks to Sutaburosu for some fantastic help exploring the neon wasteland
      Many thanks to Yves-Bazin for creating the ESP32 Virtual Driver
      Stefan Petrick's, for the informative noise demos
      djbog's, for the object-based darts demo
      All the fantastic support from the people who contribute to FastLED and its communities
*/