using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("Group {ID}")]
    public class ConstantValueGroup
    {
        [XmlAttribute] public string ID;

        [XmlElement("CConstantValueItem")] public List<ConstantValueItem> ValueItem;
    }
}