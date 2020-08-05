using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("Value {ID}")]
    public class ValueItem
    {
        [XmlAttribute]
        public string ID;

        [XmlAttribute]
        public int Index;
    }
}