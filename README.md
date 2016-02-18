# UE4Cardboard Plugin

#### Description:

Simple plugin based on SimpleHMD to allow head mounted display with lens distortion. 

#### How to install:

Close UE4 and extract archive content to a folder named Plugins inside your project directory. 
**I.e.: /MyProject/Plugins/UE4Cardboard/UE4Cardboard.uplugin.** UE4 will recognize the plugin upon startup and will ask you to compile it before launching the editor. Once the plugin is compiled and ready it must be
activated (if not already) in the UE4 under Edit->Plugins->Project->Virtual Reality. Simply tick the Enabled checkbox in UE4Cardboard plugin and restart UE4 
(make sure other SimpelHMD-like plugins are disabled when UE4Cardboard plugin is active).

#### If plugin is not visible:
You need to extract plugin content to MyProject/Plugins/UE4Cardboard (MyProject being a C++ project) so that the UE4Cardboard.uplugin is inside that folder. Upon launch UE4 will ask you to compile the project. Please note that the plugin must be compiled in order to be used. If your project is not a C++ project it would be theoretically enough to add an empty C++ class to your project and let UE4 do the rest. In any case for c++ UE4 project on Windows Visual Studio 2015 is required.

#### Building for Android:

Whenever the UE4Cardboard plugin is active it will be built into the Android package and used automatically. Just deploy your app on the device.

#### Additional Notes:

Tested on UE 4.10.2 with Nexus5 and Galaxy Note4.

Lenses might be stretched slightly more than necessary on some devices due to the lack of detecting certain device features from UE plugin system. If done correctly those parameters should scale the distortion mesh accordingly (*FSimpleHMD::XformDistortionMesh(DetectedX, DetectedY)*).

![Example](http://i.imgur.com/hufwLqR.jpg "Example")
