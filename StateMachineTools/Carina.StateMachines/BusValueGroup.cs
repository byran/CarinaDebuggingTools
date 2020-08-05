using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("Group {ID}")]
    public class BusValueGroup
    {
        [XmlAttribute] public string ID;

        [XmlElement("CValueItemBus")] public List<BusValueItem> ValueItem;
    }
}