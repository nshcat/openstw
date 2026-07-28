# openstw

A project aiming to design a physical replica of a German Sp Dr S60 relais signal box panel (_Stelltisch_) with a software simulation of local railway traffic behind it, in order to offer a compelling simulation of the day-to-day work of a signal box operator (_Fahrdienstleiter_).

## Project goals
- **A depiction of a Sp Dr S60 Stelltisch that is as realistic as possible** - This is pretty much the main goal for me. Unlike many other projects on the internet that already saw the light of day, I am not planning on making any compromises when it comes to the visual and functional design of the Stelltisch and its tiles. Even though that is likely never going to happen, I'd want it to be useable as a training aid for train routing controllers, even if only in theory.

- **Easy extensibility when adapting to new Betriebsstellen** - The system should be very modular and easy to configure, such that building replicas of any of the real German Betriebsstellen that employ a Sp Dr S60 signal box is as easy and frictionless as possible. Of course, local oddities will almost always require some degree of customization, such as custom tiles that feature specific colours and signal placements, but my goal is to reduce this work as much as possible, and make it as easy as possible.

- **As little expensive tools needed as possible** - I want the whole project to be buildable with nothing more than a run-of-the-mill 3D printer, access to a PCB manufacturing and assembly service and a set of screwdrivers. This includes not having to pay tons of money for software licenses!

- And of course, **Be completely Open-Source!** - To me, this also implies that all the tools used to make this project a reality should be open-source as well.

## Non-goals
- **Interfacing with model train setups** - I personally am neither interested in nor experienced with model train setups, and thus this project will not be designed or implemented with that in mind. Although the already existing software and hardware architecture design does leave the possibility open, if anyone else dares to try :)

- **Have everything be hand-solderable** - I have decided against this right from the start. The small size of the Stelltisch tiles make it almost a neccasity to use very small SMT components, and since access to PCB assembly is easy and comparatively cheap nowadays, I felt that it would be silly to restrict my design to being hand solderable and risk compromising my goal of realism.

## Repository structure

Each sub folder contains its own README file with more detailed information about that specific component of the project. Right now, the project consists of the following components:

 - `hardware`: Contains design files for the various PCBs and 3d-printed components used in the project that make up the physical Stelltisch, and act as the simulator frontend.
 - `software`: The software that simulates the signal box, including simulated time tables and train traffic. This acts as the simulator backend.
 - `firmware`: Firmware for all PCBs that contain a programmable microcontroller. The firmware handles communication with the simulator backend via CAN bus, and controlling and supervising the LEDs and input elements of each Stelltisch tile.

 
## Glossary
- **Stellwerk** - German term for a signalling box.
- **Fahrdienstleiter** - German term for a signal box operator.
- **Betriebsstelle** - German term for a part of the railway network that often has a Stellwerk associated with it. Often contains a public train station, but doesn't have to.
- **Sp Dr S60** - A specific kind of Stellwerk implementation based on signalling relais technology, developed by Siemens Germany in the 1960's that found wide spread adoption within the German railway infrastructure, and is still in operation today at many Betriebsstellen.
- **Stelltisch** - A big panel made up of rectangular tiles featuring a schematic depiction of the local railway topology and infrastructure of a Betriebsstelle, allowing the Fahrdienstleiter to interact with the local signalling systems to safely setup routes for trains.
