using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("Value {Index}.{ID} '{Value}'")]
    public class ConstantValueItem
    {
        [XmlAttribute] public string ID;
        [XmlAttribute] public int Index;

        public int Value;
    }
}