Secondary Map
=============
***This Software is in alpha phase! Don't expect anything fancy, instead make yourself ready for a bumpy ride!***

This is an extensions for the Free2Play MMOFPS (Mass Multiplayer Online First Person Shooter) [Firefall](http://www.firefallthegame.com/).
As the name suggests Secondary Map provides an additional map to the in-game one and is best used on a secondary monitor.
Firefall itself is using their in-house anti-cheat software called RedHanded, thus to not get in conflict with it, Secondary Map uses the very open Lua API of the game, which in turn requires the user not only to use the application, but also to install an addon for Firefall.

Firefall Addon
--------------
The addon currently only tracks the map markers via the given UI events and the player's position and orientation.

Install
-------
- Download the ZIP archive of Secondary Map
- Unzip the archive to your desired location
- Download the ZIP archive of the Firefall addon
- Use [Melder](http://forums.firefallthegame.com/community/threads/addon-manager-melder.52327/) to install the addon
- Run Secondary Map & Firefall
- Start syncing by using the in-game command `/mapsync`

Usage of Secondary Map
----------------------

Run and have patient!

Usage of the Firefall Addon
---------------------------
    /mapip <IP> - Change sending IP address to <IP>
    /mapsync    - Synchronize SIN data with Secondary Map
    /mapresync  - Resynchronize SIN data with Secondary Map
    /mapclear   - Clear SIN data on Secondary Map

Credits
-------
* [Trailape](http://forums.firefallthegame.com/community/members/trailape.2733841/) for [suggesting the idea](http://forums.firefallthegame.com/community/threads/live-map-feed-to-2nd-monitor.649841/)
* [Z1N](http://forums.firefallthegame.com/community/members/z1n.595719/) & [SyntaxTerror](http://forums.firefallthegame.com/community/members/syntaxterror.131833/) for creating the Firefall addon [AltMap](http://forums.firefallthegame.com/community/threads/addon-altmap.1858641/)
* [RadthorDax](http://forums.firefallthegame.com/community/members/radthordax.285797/) for creating [Melder](http://forums.firefallthegame.com/community/threads/addon-manager-melder.52327/)
* [Fox_exe](http://forums.firefallthegame.com/community/members/fox_exe.272341/) for providing the Firefall New Eden minimap
* [The-Killer](http://forums.firefallthegame.com/community/members/the-killer.423417/) for creating the [Firefall Melding Pocket maps](http://forums.firefallthegame.com/community/threads/high-quality-firefall-map-v2.416142/#post-15281441)
* [Hanachi](http://forums.firefallthegame.com/community/members/arkii.267799/) & [Arkii](http://forums.firefallthegame.com/community/members/hanachi.43465/) for all the help on [IRC](http://forums.firefallthegame.com/community/threads/fan-operated-firefall-irc-channel.26037/)

* [Laurent](https://github.com/LaurentGomila/) for creating the awesome [SFML](http://www.sfml-dev.org/)
* [Nexus](http://www.bromeon.ch/) for creating [Thor](https://github.com/Bromeon/Thor)
* [binary1248](https://github.com/binary1248/) for creating [SFNUL](https://github.com/binary1248/SFNUL)
* [Tank](http://stefan.boxbox.org/) for creating [SFGUI](http://sfgui.sfml-dev.de/)
* [Baptiste Lepilleur](blep@users.sourceforge.net) for creating [JSONCPP](http://jsoncpp.sourceforge.net/)

License
-------
See the additional LICENSE file.
