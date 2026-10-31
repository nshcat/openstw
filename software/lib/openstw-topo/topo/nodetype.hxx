#pragma once

#include <cstdint>

namespace Openstw
{
    namespace Topology
    {
        // Enumeration describing all the possible node types within
        // the topology graph.
        enum class NodeType : std::uint32_t
        {
            // Simple node that connects two edges (which symbolize tracks).
            // This is required if two sections of track have different properties
            // (such as allowed speed, curvature etc) but theres no signal or other
            // type of node present that can act as a border node between them.
            // A connector has two ports, Front and Back.
            // The pathfinding algorithm completely ignores this type of node.
            Connector,

            // A generic signal node. The topology layer does not know (and also does not
            // care) which type of signal this is or what kinds of aspects it can show,
            // it only cares about whether the signal shows a passing aspect or not.
            // The signal has two ports, Front and Back, and is directional -
            // the front port is behind the signal and connects to the track behind it,
            // and the back port is in front of the signal and connects to the incoming track.
            // A train coming from an edge connected to the back can see the signal,
            // while a train coming from an edge connected to the front cant see the signal
            // and will thus ignore it.
            //
            //  Track --><B> Signal <F>--> Track
            //  Visible-^              ^-Not visible
            //
            Signal,

            // An axle counter element that is used in the definition of track sections
            // (Gleisfreimeldeabschnitte). It is dumb and counts any axle going through it,
            // but it does know in which direction the axle is traveling (front-to-back vs
            // back-to-front). An axle link instance within the interlocking layer uses
            // this information to correctly update the occupancy state of a track section.
            // It has two ports, front and back.
            AxleCounter,

            // A bumper ends a section of track. It only has a single port, Center, since
            // there is no track leading away from it.
            Bumper,

            // A special meta node that is used to mark the outer boundary of the topology
            // network modeled by the simulation.
            // Simulated trains enter and leave here.
            // It has a single port, Center.
            Boundary,

            // Special type of virtual signal that is used to set Zugfahrstrassen that lead from
            // a position within the train station into the freie Strecke (which is often managed
            // by a Zentralblock or Selbstblock). In reality, Sp Dr S60 interlocking systems
            // have special relais sections for this called "Ausfahrbloecke", which take care of
            // evaluating whether the automatic block corresponding to these virtual signals
            // can currently allow a train entering it, and this influences whether a Fahrstrasse
            // can be set with the corresponding Zugstrassentaste as a destination.
            // These virtual signals usually have a special Zugstrassentaste on the Stelltisch, but
            // without any signal indicators associated with them.
            // Route destination nodes have two ports just like signal nodes, Front and Back,
            // with the same interpretation as with signals. The Front port points into the
            // automatic block, and the track going into the train station connects to the Back port.
            // But trains will ignore nodes of this type since it doesnt actually exists in the
            // real world.
            RouteDestination,

            // A simple, two-way switch. Has three ports, Wurzel, Gerade and Abzweig.
            // A train coming from a track connected to Wurzel can go either to Gerade or Abzweig,
            // while trains coming from tracks connected to either Gerade or Abzweig can only
            // go to Wurzel.
            SimpleSwitch,
        };
    }
}
