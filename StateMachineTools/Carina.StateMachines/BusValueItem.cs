using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{Index} '{ModuleType}' {ID}")]
    public class BusValueItem
    {
        [XmlAttribute] public string ID;
        [XmlAttribute] public int Index;
        [XmlAttribute] public string ModuleType;
        [XmlAttribute] public bool IsReadOnly;
    }
}