using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ModuleType}")]
    public class TimeSlot
    {
        [XmlAttribute] public string ModuleType;
    }
}