using System.Collections.Generic;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class BusValueList
    {
        [XmlAttribute] public string ID;

        [XmlElement("ValueGroup")] public List<BusValueGroup> ValueGroup;
    }
}