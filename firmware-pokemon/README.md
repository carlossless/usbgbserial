# usbgbserial-pokemon

This is firmware for [usbgbserial](http://github.com/carlossless/usbgbserial) to simulate Pokemon Gen2 JP (Silver, Gold, Crystal) trades with a real GameBoy acting as player 1 (internal clock) and usbgbserial acting as player 2 (external clock).

The main parts of the protocol have been figured out thanks to [Bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_II), the [Pokemon Crystal Disassembly](https://github.com/pret/pokecrystal) and some trial and error with a logic analyzer. However, there are still quite a few nuances of the pokemon trade center protocol that are not handled by this software.

All of the communication and trade-simulation is done on the atmega32u2 and does not require use of a connected USB host (PC) to usbgbserial.

# Video

**TBD**

# TODOs

- [] Use AVR SPI peripheral for the link protocol instead of single I/O pin interrupts. (requires board change)
- [] Implement a timeout for when a player exits the trade room and reset back to the `NOT_CONNECTED` state.

# References

* [SameBoy](https://sameboy.github.io/) - a cross-platform DMG/GBC emulator that works great on macOS.
* [Bulbapedia](https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_II) - full documentation on the 48byte data structure that is used to store pokemon data in Gen2 games.
* [github.com/pret/pokecrystal](https://github.com/pret/pokecrystal) - ongoing project to create a readable disassembly of Pokemon Crystal.
* [Spoofing Pokémon trades with the Stellaris LaunchPad](http://www.adanscotney.com/2014/01/spoofing-pokemon-trades-with-stellaris.html) - another project that uses a TI Stellaris LaunchPad to spoof Pokemon Gen1 Trades.
