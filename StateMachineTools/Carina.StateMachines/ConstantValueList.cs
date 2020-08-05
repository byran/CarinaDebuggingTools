using System.Collections.Generic;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ConstantValueList
    {
        [XmlAttribute] public string ID;

        [XmlElement("ValueGroup")] public List<ConstantValueGroup> ValueGroup;
    }
}