Emoticons plugin
----------------

CAUTION: THIS IS A BETA STAGE PLUGIN. IT CAN DO VERY BAD THINGS. USE AT YOUR OWN RISK.

This is a plugin to replace emoticons in message windows. It support replacement in the log and in message entry area. Animated gifs are supported in both areas. For that AniSmiley plugin is supported and advised.

It is based in two concepts:

1. The emoticons configuration is per protocol, and comes bundled with the plugin. They reference emoticons by a set of pre-configured names (they can't create new names - if someone needs new names, please tell me) and set the shortcut and names for the emoticons the protocol support.
They are *.emo files and must be inside <Miranda>\Plugins\Emoticons
Currently I created a pack for MSN, ICQ, JGMAIL, YAHOO and a Default. If someone knows where I can find a list of default emoticons for other protocols, please tell me.
The format of the emo file, it is like this:
  "smile" = "Small smile", "/wx" "/:)" "/small_smile"
field 1 -> "smile" -> The unique name of the emoticon (as the Tango pack has it)
field 2 -> "Small smile" -> A text description for the emoticon, as the protocol uses it. This should be in english (so all users can 'understand' it), and is translatable via language pack
field 3 -> "/wx" -> Default emoticon text (used when using the selection window). It also can be a service call, in the format "<Service:/ServiceName:hContact:0>"
fields 4 ... -> "/:)" "/small_smile" -> Other possible texts (only if not a service call). Please, try get all possible texts. Also, note that this is case sensitive, so if the protocol is not you have to type all possible combinations.

2. Emoticon packs, containing the emoticons to show, only contain the images and a .mep file describing the creator. 
Each one must be a folder inside Customize\Emoticons, with the images beeing the pre-configured name of the smiley. 
If you want to change images for a protocol, you can create a sub-folder with the protocol name and put the images there (with the images beeing the pre-configured name of the smiley). Currently gif (included animated ones), png and jpeg images are supported.
The name of the pack is the name of the folder, and a file <Pack name>.mep can exist inside the folder, containing this entries (all of then are optional):
Name: <Full name of the pack>
Creator: <Artist name>
Updater URL: <URL for updating this pack - not yet implemented>

Also, the mep file can expecify the filename for an emoticon, in a line like:
"<The unique name of the emoticon>" = "<Relative path from pack foler>", <Frame number, starting in 1 - optional>
For example:
"smile" = "01.gif"
This will affect all protocols. If you want to change the image for a protocol only, you can write:
"ICQ\smile" = "ICQ_smile01.gif"
or
"ICQ\smile" = "ICQ\smile01.gif"


A default pack, containing Tango Emoticons (made by Hylke) is packed and can be used as an example.

For custom smileys to work, you need MSN protocol from miranda 0.8 #10.

Currently there is a lot more work to be done on this plugin, but I wanted to show an initial version, so I could get some feedback.

Thanks to FYR for the AniSmiley plugin and to Hylke for the nice emoticons.

To report bugs/make suggestions, go to the forum thread: http://forums.miranda-im.org/showthread.php?t=17210


TODO:
- Per protocol pack
- Updater support for Emoticon Packs

